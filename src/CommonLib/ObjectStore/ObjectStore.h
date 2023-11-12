/* ObjectStore.h */

#ifndef _OBJECTSTORE_H_
#define _OBJECTSTORE_H_

#include "Types.h"
#include "Object.h"
#include "Pool.h"

typedef struct ObjectStore ObjectStore;

PUBLIC void ObjectStore_delete(ObjectStore * this);
PUBLIC ObjectStore * ObjectStore_getRef();
PUBLIC Pool * ObjectStore_createPool(ObjectStore * this);
PUBLIC void ObjectStore_deletePool(ObjectStore * this, Pool * pool);
PUBLIC Object ObjectStore_createObject(ObjectStore * this, Class * class);
PUBLIC void ObjectStore_deleteObject(ObjectStore * this, Object object);
PUBLIC void ObjectStore_reportUnallocated(ObjectStore * this);
PUBLIC void ObjectStore_report(ObjectStore * this);
#endif /* _OBJECTSTORE_H_ */
