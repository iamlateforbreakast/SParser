/* Debug.h */

#include <stdarg.h>

#define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)

void dbg_printf(const char *fmt, ...);
