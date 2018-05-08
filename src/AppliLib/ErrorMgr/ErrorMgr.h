/* ErrorMgr.h */

#ifndef _ERRORMGR_H_
#define _ERRORMGR_H_

#include "Types.h"

typedef struct ErrorMgr ErrorMgr;

PUBLIC void ErrorMgr_delete(ErrorMgr * this);
PUBLIC ErrorMgr * ErrorMgr_copy(ErrorMgr * this);

#endif /* _ERRORMGR_H_ */
