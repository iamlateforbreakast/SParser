/* TimeMgr.h */

#ifndef _TIMEMGR_H_
#define _TIMEMGR_H_

#include "Types.h"
#include "String2.h"

typedef struct TimeMgr TimeMgr;

PUBLIC void TimeMgr_delete(TimeMgr * this);
PUBLIC TimeMgr * TimeMgr_copy(TimeMgr * this);
PUBLIC TimeMgr * TimeMgr_getRef();
PUBLIC void TimeMgr_latchTime(TimeMgr * this, String * s);

#endif /* _TIMEMGR_H_ */
