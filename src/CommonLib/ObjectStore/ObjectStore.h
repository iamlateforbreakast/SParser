/* ObjectStore.h */

#ifndef _OBJECTSTORE_H_
#define _OBJECTSTORE_H_

#include "Types.h"
#include "Pool.h"

typedef struct ObjectStore ObjectStore;

PUBLIC void ObjectStore_delete(ObjectStore * this);
PUBLIC ObjectStore * ObjectStore_getRef();
PUBLIC Pool * ObjectStore_createPool(ObjectStore * this);
PUBLIC void ObjectStore_deletePool(ObjectStore * this, Pool * pool);
PUBLIC void ObjectStore_reportUnallocated(ObjectStore * this);
PUBLIC void ObjectStore_report(ObjectStore * this);
#endif /* _OBJECTSTORE_H_ */
