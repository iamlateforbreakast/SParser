/* SdbMgr.h */

#ifndef _SDBBMGR_H_
#define _SDBMGR_H_

#include "Types.h"
#include "String2.h"

typedef struct SdbMgr SdbMgr;

PRIVATE SdbMgr* SdbMgr_new(String * name);
PUBLIC void SdbMgr_delete(SdbMgr* this);
PUBLIC SdbMgr* SdbMgr_getSdbMgr();
PUBLIC unsigned int SdbMgr_execute(SdbMgr* this, const char* statement);

#endif /* _SDBMGR_H_ */