#ifndef _GZ_MACROS_H
#define _GZ_MACROS_H

#include <JSystem/JUtility/JUTException.h>

#define GZASSERTLINE(cond) JUT_ASSERTLINE(__LINE__, cond, "GZAssert")
#define GZEXPECT(cond, ...) JUT_ASSERTLINE(__LINE__, cond, __VA_ARGS__)

#endif
