/* TimeMgr.h */

#ifndef _TIMEMGR_H_
#define _TIMEMGR_H_

#include "Types.h"

typedef struct TimeMgr TimeMgr;

PUBLIC void TimeMgr_delete(TimeMgr * this);
PUBLIC TimeMgr * TimeMgr_copy(TimeMgr * this);
PUBLIC TimeMgr * TimeMgr_getRef();

#endif /* _TIMEMGR_H_ */