/********************************************************//**
0123456789012345678901234567890123456789012345678901234567890
  @file ObjectStorage.c
 
  @brief An object storage class.
  @details This class provides an object allocation and
   de-allocation service. Only one instance of this class can 
   be created.
************************************************************/

#include "ObjectStore.h"
#include "Memory.h"

typedef struct AllocInfo
{
  Allocator * ptr;
  Allocator * prev;
  Allocator * next; 
} AllocInfo;

/********************************************************//**
  @class ObjectStore
************************************************************/
struct ObjectStore
{
  Object object;
  AllocInfo * AllocList;
};

PRIVATE ObjectStore * objectStore = 0;

PRIVATE ObjectStore * ObjectStore_new();

PUBLIC Allocator * mallocPool = 0;

/**********************************************//** 
  @brief Delete an instance of the class ObjectMgr.
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_delete(ObjectStore * this)
{
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
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
  
  return objectStore;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC Allocator * ObjectStore_createAllocator(ObjectStore * this)
{
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_deleteAllocator(ObjectStore * this, Allocator * allocator)
{
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC Object * ObjectStore_createObject(ObjectStore * this, Class * class, Allocator * allocator)
{
  Object * object;
  
  // object = Allocator_allocate(malloc, Class_getSize(class));

  return object;
}

/**********************************************//** 
  @brief Delete an object from the object store.
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_deleteObject(ObjectStore * this, Object object)
{
  // Allocator_deallocate(object->allocator, object);
}

/**********************************************//** 
  @brief Create the instance of the ObjectStore
  @details TBD
  @private
  @memberof ObjectStore
**************************************************/
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
