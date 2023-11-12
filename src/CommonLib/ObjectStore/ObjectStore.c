/* ObjectStore.c */

#include "ObjectStore.h"
#include "Memory.h"

typedef struct PoolInfo
{
  Pool * ptr;
  Pool * prevPool;
  Pool * nextPool; 
} PoolInfo;

struct ObjectStore
{
  PoolInfo * poolList;
};

PRIVATE ObjectStore * objectStore = 0;

PUBLIC Pool * mallocPool = 0;

PUBLIC void ObjectStore_delete(ObjectStore * this)
{
}

PUBLIC ObjectStore * ObjectStore_getRef()
{
    if (objectStore==0)
  {
    objectStore = ObjectStore_new();
  }
  else
  {
    objectStore->object.refCount++;
  }
  
  return objectMgr;
}

PUBLIC Pool * ObjectStore_createPool(ObjectStore * this)
{
}

PUBLIC void ObjectStore_deletePool(ObjectStore * this, Pool * pool)
{
}

PUBLIC Object ObjectStore_createObject(ObjectStore * this, Class * class)
{
  Object object;
  
  // object = Allocator_allocate(malloc, Class_getSize(class));

  return object;
}

PUBLIC void ObjectStore_deleteObject(ObjectStore * this, Object object)
{
  // Allocator_deallocate(object->allocator, object);
}

PRIVATE ObjectStore * ObjectStore_new()
{
  ObjectStore * objectStore;
  // Create Malloc Pool
  //mallocPool = Pool_new(MALLOC_TYPE);
  //objecStore = Pool_allocate(mallocPool,sizeof(ObjectStore));
  //objectStore->poolList = Pool_allocate(mallocPool,sizeof(PoolInfo));
  //objectStore->poolList->ptr = mallocPool;
  //objectStore->poolList->nextPool = 0;
  //objectStore->poolList->prevPoool = 0; 
  return objectStore;
}
#endif /* _OBJECTSTORE_H_ */
