#ifndef _GZ_CRASH_SYMBOLS_H
#define _GZ_CRASH_SYMBOLS_H

#include <types.h>

struct JUTConsole;

namespace gz {

// resolve a code/data address to a symbol using the precomputed `/pikmin2UP.sym` table + print to console
int crashSymbolsTryPrint(JUTConsole* console, u32 address, bool beginWithNewline);

} // namespace gz

#endif
