#include <p2gz/crashSymbols.h>
#include <types.h>
#include <Dolphin/dvd.h>
#include <Dolphin/os.h>
#include <JSystem/JUtility/JUTConsole.h>

// Fast crash log symbol lookup (see tools/gen_crash_symbols.py)

namespace gz {

namespace {

// .sym layout (to match what tools/gen_crash_symbols.py generates)
const u32 SYM_MAGIC    = 'P2GZ';
const u32 SYM_VERSION  = 1;
const s32 HEADER_BLOCK = 512;
const u32 ENTRY_SIZE   = 16;
const s32 INDEX_BLOCK  = 512;
const int MAX_SECTIONS = 16;
const int MAX_NAME     = 80;
const s32 NAME_BUF     = 192;

enum State { STATE_UNINIT, STATE_READY, STATE_FAILED };

State sState = STATE_UNINIT;
DVDFileInfo sFile;
u32 sCount;
u32 sIndexOffset;
u32 sStringsOffset;
u32 sFileLength;
int sSectionCount;
char* sSectionName[MAX_SECTIONS];

u8 sHeaderRaw[HEADER_BLOCK + 32];
u8 sBlockRaw[INDEX_BLOCK + 32];
u8 sNameRaw[NAME_BUF + 32];
u8* sHeaderBuf;
u8* sBlockBuf;
u8* sNameBuf;
u32 sCachedBlockOff;

inline u8* align32(u8* p)
{
	return (u8*)ALIGN_NEXT((u32)p, 32);
}

inline u32 read32(const u8* p)
{
	return *(const u32*)p;
}

// synchronous aligned DVD read (NB: offset, length and dst need to be 32-aligned)
bool readAligned(u32 offset, void* dst, s32 length)
{
	BOOL enabled = OSEnableInterrupts();
	BOOL ok      = DVDReadAsync(&sFile, dst, length, (s32)offset, nullptr);
	if (ok) {
		while (DVDGetCommandBlockStatus(&sFile.cBlock)) {
			;
		}
	}
	OSRestoreInterrupts(enabled);
	return ok != 0;
}

bool readEntry(u32 i, u32* vaddr, u32* size, u32* nameOff, u32* sectionId)
{
	u32 byteOff  = sIndexOffset + i * ENTRY_SIZE;
	u32 blockOff = ALIGN_PREV(byteOff, INDEX_BLOCK);
	if (blockOff != sCachedBlockOff) {
		if (!readAligned(blockOff, sBlockBuf, INDEX_BLOCK)) {
			return false;
		}
		sCachedBlockOff = blockOff;
	}
	const u8* e = sBlockBuf + (byteOff - blockOff);
	*vaddr      = read32(e + 0);
	*size       = read32(e + 4);
	*nameOff    = read32(e + 8);
	*sectionId  = read32(e + 12);
	return true;
}

// read one symbol name into `out`
void readName(u32 nameOff, char* out, int outCap)
{
	out[0]      = '\0';
	u32 fileOff = sStringsOffset + nameOff;
	if (fileOff >= sFileLength) {
		return;
	}

	u32 alignOff = ALIGN_PREV(fileOff, 32);
	u32 delta    = fileOff - alignOff;
	s32 readLen  = (s32)ALIGN_NEXT(delta + (u32)outCap, 32);
	if (readLen > NAME_BUF) {
		readLen = NAME_BUF;
	}

	u32 fileEnd = ALIGN_NEXT(sFileLength, 32);
	if (alignOff + (u32)readLen > fileEnd) {
		readLen = (s32)(fileEnd - alignOff);
	}
	if (readLen <= (s32)delta) {
		return;
	}
	if (!readAligned(alignOff, sNameBuf, readLen)) {
		return;
	}

	const char* src = (const char*)sNameBuf + delta;
	int max         = readLen - (int)delta;
	if (max > outCap - 1) {
		max = outCap - 1;
	}
	int i = 0;
	for (; i < max && src[i] != '\0'; i++) {
		out[i] = src[i];
	}
	out[i] = '\0';
}

bool init()
{
	BOOL enabled = OSEnableInterrupts();
	BOOL opened  = DVDOpen((char*)"/pikmin2UP.sym", &sFile);
	OSRestoreInterrupts(enabled);
	if (!opened) {
		return false;
	}
	sFileLength = sFile.length;

	sHeaderBuf      = align32(sHeaderRaw);
	sBlockBuf       = align32(sBlockRaw);
	sNameBuf        = align32(sNameRaw);
	sCachedBlockOff = 0xFFFFFFFF;

	if (!readAligned(0, sHeaderBuf, HEADER_BLOCK)) {
		return false;
	}
	if (read32(sHeaderBuf + 0) != SYM_MAGIC || read32(sHeaderBuf + 4) != SYM_VERSION) {
		return false;
	}
	sCount         = read32(sHeaderBuf + 8);
	sIndexOffset   = read32(sHeaderBuf + 12);
	sStringsOffset = read32(sHeaderBuf + 16);
	sSectionCount  = (int)read32(sHeaderBuf + 20);
	if (sCount == 0 || sSectionCount <= 0 || sSectionCount > MAX_SECTIONS) {
		return false;
	}

	// section-name table: sSectionCount amount of strings starting at 0x20
	char* p   = (char*)sHeaderBuf + 0x20;
	char* end = (char*)sHeaderBuf + HEADER_BLOCK;
	for (int s = 0; s < sSectionCount; s++) {
		sSectionName[s] = p;
		while (p < end && *p != '\0') {
			p++;
		}
		if (p >= end) {
			return false; // bad table
		}
		p++;
	}
	return true;
}

} // namespace

int crashSymbolsTryPrint(JUTConsole* console, u32 address, bool beginWithNewline)
{
	if (sState == STATE_UNINIT) {
		sState = init() ? STATE_READY : STATE_FAILED;
	}
	if (sState != STATE_READY) {
		return -1;
	}

	// binary search for the entry with the greatest start address <= `address`
	s32 lo  = 0;
	s32 hi  = (s32)sCount - 1;
	s32 res = -1;
	u32 vaddr;
	u32 size;
	u32 nameOff;
	u32 sectionId;
	while (lo <= hi) {
		s32 mid = (lo + hi) >> 1;
		if (!readEntry((u32)mid, &vaddr, &size, &nameOff, &sectionId)) {
			return -1;
		}
		if (vaddr <= address) {
			res = mid;
			lo  = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	if (res < 0) {
		return 0;
	}
	if (!readEntry((u32)res, &vaddr, &size, &nameOff, &sectionId)) {
		return -1;
	}
	if (address >= vaddr + size) {
		return 0;
	}

	char name[MAX_NAME + 1];
	readName(nameOff, name, sizeof(name));
	const char* section = (sectionId < (u32)sSectionCount) ? sSectionName[sectionId] : "?";

	if (beginWithNewline) {
		console->print("\n");
	}
	console->print_f("  [%08X]: %s [%08X: %XH]\n  %s +%XH\n", address, section, vaddr, size, name, address - vaddr);
	return 1;
}

} // namespace gz
