/* SdbMgr.h */

#ifndef _SDBBMGR_H_
#define _SDBMGR_H_

#include "Types.h"
#include "String2.h"

typedef struct SdbMgr SdbMgr;

PUBLIC SdbMgr * SdbMgr_new(String * name);
PUBLIC void SdbMgr_delete(SdbMgr* this);
PUBLIC SdbMgr * SdbMgr_copy(SdbMgr* this);
PUBLIC SdbMgr * SdbMgr_getRef();
PUBLIC unsigned int SdbMgr_execute(SdbMgr* this, const char* statement, String *** result);

#endif /* _SDBMGR_H_ */