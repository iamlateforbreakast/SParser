/* ErrorMgr.h */

#ifndef _ERRORMGR_H_
#define _ERRORMGR_H_

#include "Types.h"
#include <stdarg.h>

typedef struct ErrorMgr ErrorMgr;

PUBLIC void ErrorMgr_delete(ErrorMgr * this);
PUBLIC ErrorMgr * ErrorMgr_copy(ErrorMgr * this);
PUBLIC void ErrorMgr_exit(ErrorMgr * this, const char *fmt, ...);

#endif /* _ERRORMGR_H_ */
