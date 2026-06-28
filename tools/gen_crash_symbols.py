#!/usr/bin/env python3
"""Generate a compact binary symbol table for the in-game crash handler.

The exception handler (JUTException) used to resolve every stack-frame / register
address by re-opening the 4.2 MB ``pikmin2UP.map`` and linearly scanning it from
disc *for each address* - dozens of full-file scans per crash. This tool turns the
map into ``pikmin2UP.sym``: a sorted, binary, big-endian table that the runtime can
binary-search with a handful of tiny disc reads (see src/p2gz/crashSymbols.cpp).

Format:

  header block, padded to HEADER_BLOCK (512 bytes), 32-byte-aligned start of file:
    0x00 u32 magic            = 'P2GZ'
    0x04 u32 version          = 1
    0x08 u32 count            = number of symbol entries
    0x0C u32 index_offset     = file offset of the entry array (== HEADER_BLOCK)
    0x10 u32 strings_offset   = file offset of the string blob
    0x14 u32 section_count
    0x18 u32 reserved (0)
    0x1C u32 reserved (0)
    0x20 ...   section-name table: section_count null-terminated names
  index array (at index_offset, 32-aligned): `count` entries, 16 bytes each,
    sorted ascending by vaddr (16 divides 512 and 32 -> every entry sits inside a
    single aligned disc block, so the runtime can read one block per probe):
      0x00 u32 vaddr
      0x04 u32 size
      0x08 u32 name_off     (offset from strings_offset)
      0x0C u32 section_id   (index into the section-name table)
  string blob (at strings_offset): deduplicated null-terminated symbol names.
"""

import argparse
import os
import re
import struct
import sys

MAGIC = 0x5032475A  # 'P2GZ'
VERSION = 1
HEADER_BLOCK = 512  # header + section table live in this first (aligned) block
ENTRY_SIZE = 16

# Matches a real symbol line, e.g.
#   "  00000030 000024 80003130  4 TRK_memcpy \tmem_TRK.o"
# groups: file-offset, size, vaddr, alignment, rest(symbol [+ \tobject])
_SYM_RE = re.compile(r"^\s+([0-9a-fA-F]{8})\s+([0-9a-fA-F]{6})\s+([0-9a-fA-F]{8})\s+\d+\s+(.*)$")
_SECTION_RE = re.compile(r"^(\S[\w.$]*) section layout")


def parse_map(map_path):
    """Return (entries, section_names).

    entries: list of (vaddr, size, name, section_id), unsorted.
    section_names: list of section names indexed by section_id.
    """
    section_names = []
    section_id_of = {}
    entries = []
    current_section = None

    with open(map_path, "r", encoding="latin-1") as f:
        for line in f:
            line = line.rstrip("\n")

            # The post-layout summary ("Memory map:", "Linker generated symbols:")
            # is not symbol data - stop before we start mis-parsing it.
            if line.startswith("Memory map") or line.startswith("Linker generated"):
                break

            m = _SECTION_RE.match(line)
            if m:
                current_section = m.group(1).strip()
                continue

            if current_section is None:
                continue

            m = _SYM_RE.match(line)
            if not m:
                continue

            size_hex, vaddr_hex, rest = m.group(2), m.group(3), m.group(4)
            size = int(size_hex, 16)
            if size == 0:
                continue  # entry-of-section labels and other zero-size markers

            # The symbol token is everything up to the tab/object-file or first space.
            name = rest.split("\t")[0].strip().split(" ")[0]
            if not name:
                continue
            # Section-contribution lines repeat the section name as the "symbol";
            # skip them so they don't shadow the real per-symbol entries.
            if name == current_section:
                continue

            vaddr = int(vaddr_hex, 16)

            sid = section_id_of.get(current_section)
            if sid is None:
                sid = len(section_names)
                section_id_of[current_section] = sid
                section_names.append(current_section)

            entries.append((vaddr, size, name, sid))

    return entries, section_names


def dedup_by_vaddr(entries):
    """Keep one entry per vaddr (the smallest size = most specific symbol)."""
    best = {}
    for vaddr, size, name, sid in entries:
        prev = best.get(vaddr)
        if prev is None or size < prev[0]:
            best[vaddr] = (size, name, sid)
    out = [(v, s, n, sid) for v, (s, n, sid) in best.items()]
    out.sort(key=lambda e: e[0])
    return out


def build_blob(entries):
    """Build the deduplicated string blob; return (blob_bytes, name_off_of)."""
    blob = bytearray()
    name_off_of = {}
    for _, _, name, _ in entries:
        if name not in name_off_of:
            name_off_of[name] = len(blob)
            blob += name.encode("latin-1") + b"\x00"
    return bytes(blob), name_off_of


def build_section_table(section_names):
    """Pack section names as null-terminated strings + assert they fit the header."""
    table = bytearray()
    for s in section_names:
        table += s.encode("latin-1") + b"\x00"
    if 0x20 + len(table) > HEADER_BLOCK:
        raise SystemExit(
            f"ERROR: section table ({len(table)}B) overflows header block; bump HEADER_BLOCK"
        )
    return bytes(table)


def write_sym(out_path, entries, section_names):
    blob, name_off_of = build_blob(entries)
    section_table = build_section_table(section_names)

    count = len(entries)
    index_offset = HEADER_BLOCK
    strings_offset = index_offset + count * ENTRY_SIZE
    # 32-align the string blob so the runtime's aligned name reads stay simple.
    strings_offset = (strings_offset + 31) & ~31

    with open(out_path, "wb") as f:
        header = struct.pack(
            ">8I",
            MAGIC,
            VERSION,
            count,
            index_offset,
            strings_offset,
            len(section_names),
            0,
            0,
        )
        f.write(header)
        f.write(section_table)
        # pad header block
        f.write(b"\x00" * (HEADER_BLOCK - 0x20 - len(section_table)))

        for vaddr, size, name, sid in entries:
            f.write(struct.pack(">4I", vaddr, size, name_off_of[name], sid))

        # pad to strings_offset
        pos = index_offset + count * ENTRY_SIZE
        f.write(b"\x00" * (strings_offset - pos))
        f.write(blob)

    return count, strings_offset, len(blob)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("map", nargs="?", default=os.path.join("root", "files", "pikmin2UP.map"),
                    help="input linker .map (default: root/files/pikmin2UP.map)")
    ap.add_argument("-o", "--out", default=os.path.join("root", "files", "pikmin2UP.sym"),
                    help="output .sym path (default: root/files/pikmin2UP.sym)")
    args = ap.parse_args()

    if not os.path.exists(args.map):
        print(f"ERROR: map file not found: {args.map}", file=sys.stderr)
        return 1

    entries, section_names = parse_map(args.map)
    if not entries:
        print(f"ERROR: no symbols parsed from {args.map}", file=sys.stderr)
        return 1

    entries = dedup_by_vaddr(entries)
    count, strings_offset, blob_len = write_sym(args.out, entries, section_names)

    print(f"Crash symbol lookup file created: {count} symbols, {len(section_names)} sections ")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
