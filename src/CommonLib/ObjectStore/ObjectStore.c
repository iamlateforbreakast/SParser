/********************************************************//**
  @file ObjectStorage.c
 
  @brief An object storage class.
  @details This class provides an object allocation and
   de-allocation service. Only one instance of this class can 
   be created.
************************************************************/

#include "ObjectStore.h"
#include "Malloc.h"
#include "Debug.h"

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
  AllocInfo * allocList;
  unsigned int nbAllocatedObjects;
};

PRIVATE ObjectStore * objectStore = 0;

PRIVATE ObjectStore * ObjectStore_new();

PUBLIC Allocator * mem_alloc = 0;

/**********************************************//** 
  @brief Delete an instance of the class ObjectMgr.
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_delete(ObjectStore * this)
{
  this->object.refCount = this->object.refCount - 1;
  
  if (this->object.refCount == 0)
  {
    ObjectStore_report(this);
    /* TODO: memset(this, 0, sizeof(ObjectMgr)); */
    //Memory_free(this, sizeof(ObjectStore));
    this = 0;
  } 
}

/**********************************************//** 
  @brief Obtain the reference to the object store.
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
  
  object = (Object *)allocator->allocate(allocator, class->f_size());//0B5EC7

  //object->id = 0;
  //object->allocator = allocator;

  this->nbAllocatedObjects++;

  return object;
}

/**********************************************//** 
  @brief Delete an object from the object store.
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_deleteObject(ObjectStore * this, Object * object)
{
  if (object==0) return;
  
  //object->allocator->deallocate(object->allocator, object);
  this->nbAllocatedObjects--;

}

/**********************************************//** 
  @brief Reports the usage statistics for an instance of ObjectStore.
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_report(ObjectStore * this)
{
  PRINT(("Object Manager Usage report:\n"));
  PRINT(("Nb allocated objects: %d\n", this->nbAllocatedObjects));
  //printf("Max nb allocated objects: %d\n", this->maxNbObjectAllocated);
  //printf("Nb alloc request: %d\n", this->allocRequestId);
  //printf("Nb free requests: %d\n", this->freeRequestId);
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
  objectStore->allocList = (AllocInfo*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(AllocInfo));
  objectStore->allocList->ptr = (Allocator*)Malloc_getRef();
  objectStore->allocList->next = 0;
  objectStore->allocList->prev = 0;

  objectStore->nbAllocatedObjects = 0;

  //ObjectStore_addAllocator(); 
  return objectStore;
}
