/* ObjectMgr.h */

#ifndef _OBJECTMGR_H_
#define _OBJECTMGR_H_

#include "Types.h"

typedef struct ObjectMgr ObjectMgr;

PUBLIC void ObjectMgr_delete(ObjectMgr * this);
PUBLIC ObjectMgr * ObjectMgr_copy(ObjectMgr * this);

#endif /* _OBJECTMGR_H_ */