#ifndef _GZ_CRASH_SYMBOLS_H
#define _GZ_CRASH_SYMBOLS_H

#include <types.h>

struct JUTConsole;

namespace gz {

/// Resolve a code/data address to a symbol using the precomputed ``/pikmin2UP.sym``
/// table (built by tools/gen_crash_symbols.py) and print it to `console`.
/// @return  1 = address resolved and printed
///          0 = .sym is available but no symbol contains `address` (fast miss)
///         -1 = .sym unavailable (caller should fall back to the legacy .map scan)
int crashSymbolsTryPrint(JUTConsole* console, u32 address, bool beginWithNewline);

} // namespace gz

#endif
