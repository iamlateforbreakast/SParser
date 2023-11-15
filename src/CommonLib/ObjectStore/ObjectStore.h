/* ObjectStore.h */

#ifndef _OBJECTSTORE_H_
#define _OBJECTSTORE_H_

#include "Types.h"
#include "Object.h"
#include "Allocator.h"

typedef struct AllocInfo AllocInfo;
typedef struct ObjectStore ObjectStore;

PUBLIC void ObjectStore_delete(ObjectStore * this);
PUBLIC ObjectStore * ObjectStore_copy(ObjectStore* this);
PUBLIC ObjectStore * ObjectStore_getRef();
PUBLIC AllocInfo * ObjectStore_createAllocator(ObjectStore * this, Allocator * allocator);
PUBLIC void ObjectStore_deleteAllocator(ObjectStore * this, AllocInfo * allocInfo);
PUBLIC Object * ObjectStore_createObject(ObjectStore * this, Class * class, Allocator * allocator);
PUBLIC void ObjectStore_deleteObject(ObjectStore * this, Object * object);
PUBLIC void ObjectStore_reportUnallocated(ObjectStore * this);
PUBLIC void ObjectStore_report(ObjectStore * this);
PUBLIC unsigned int ObjectStore_getNbAllocatedObjects(ObjectStore * this);
PUBLIC int ObjectStore_compare(ObjectStore * this, ObjectStore * compared);
PUBLIC void ObjectStore_print(ObjectStore * this);

#endif /* _OBJECTSTORE_H_ */
