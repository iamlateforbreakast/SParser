/* ObjectMgr.h */

#ifndef _OBJECTMGR_H_
#define _OBJECTMGR_H_

#include "Object.h"
#include "Types.h"

typedef struct ObjectMgr ObjectMgr;

PUBLIC void ObjectMgr_delete(ObjectMgr * this);
PUBLIC ObjectMgr * ObjectMgr_copy(ObjectMgr * this);
PUBLIC ObjectMgr * ObjectMgr_getRef();
PUBLIC Object * ObjectMgr_allocate(ObjectMgr * this, unsigned int size);
PUBLIC void ObjectMgr_deallocate(ObjectMgr * this, Object * object);

#endif /* _OBJECTMGR_H_ */