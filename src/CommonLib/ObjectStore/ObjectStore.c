/********************************************************//**
  @file ObjectStorage.c
 
  @brief An object storage class.
  @details This class purpose is to keep track of all objects
   created provides an object allocation and
   de-allocation service. This is performed by registering 
   every Allocator object with the ObjectStore.
   Only one instance of this class can be created.
************************************************************/

#include "ObjectStore.h"
#include "Class.h"
#include "Object.h"
#include "Malloc.h"
#include "Debug.h"
#include "Error.h"

struct AllocInfo
{
  Allocator * ptr;
  AllocInfo * prev;
  AllocInfo * next; 
};

/********************************************************//**
  @class ObjectStore
************************************************************/
struct ObjectStore
{
  Object object;
  unsigned int nbAllocatedObjects;
  AllocInfo * allocList;
};

/**********************************************//**
  @private Class Description
**************************************************/
Class objectStoreClass = 
{
  .f_new = 0,
  .f_delete = (Destructor)&ObjectStore_delete,
  .f_copy = (Copy_Operator)&ObjectStore_copy,
  .f_comp = (Comp_Operator)&ObjectStore_compare,
  .f_print = (Printer)&ObjectStore_print
};

PRIVATE ObjectStore * objectStore = 0;

PRIVATE ObjectStore * ObjectStore_new();

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
    AllocInfo * iterator = this->allocList;
    AllocInfo * current = this->allocList;
    
    iterator=iterator->prev;
    current->ptr->delete(current->ptr);
    Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)current);
    current = iterator;

    while (iterator!=0)
    {
       // TODO
    }

    /* TODO: memset(this, 0, sizeof(ObjectMgr)); */
    Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)this);
    objectStore = 0;
  } 
}

/**********************************************//** 
  @brief Copy an instance of the class ObjectStore.
  @public
  @memberof ObjectStore
  @return Copy of the given instance.
**************************************************/
PUBLIC ObjectStore * ObjectStore_copy(ObjectStore* this)
{
  return ObjectStore_getRef();
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
  @brief Register an Allocator with the objectStore
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC AllocInfo * ObjectStore_createAllocator(ObjectStore * this, Allocator * allocator)
{
  AllocInfo * allocInfo;

  allocInfo = (AllocInfo*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(AllocInfo));
  
  allocInfo->ptr = allocator;
  allocInfo->prev = this->allocList;
  allocInfo->next = 0;
  this->allocList->next = allocInfo;
  
  this->allocList = allocInfo;

  return allocInfo;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_deleteAllocator(ObjectStore * this, AllocInfo * allocInfo)
{
  allocInfo->prev->next = allocInfo->next;
  if (allocInfo->next)
    allocInfo->next->prev = allocInfo->prev;

  allocInfo->ptr->delete(allocInfo->ptr);
  Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)allocInfo);
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC Object * ObjectStore_createObject(ObjectStore * this, Class * class, Allocator * allocator)
{
  Object * object = 0;
  
  object = (Object *)allocator->allocate(allocator, class->f_size());//0B5EC7
  if (object!=0) //return object;
  {
    object->id = this->nbAllocatedObjects;
    object->class = class;
    object->size = class->f_size();
    object->allocator = allocator;

    this->nbAllocatedObjects++;
  }
  else
  {
    Error_new(ERROR_NORMAL,"Object allocation failed\n");
  }

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

  object->allocator->deallocate(object->allocator, (char*)object); 

}

/**********************************************//** 
  @brief Reports the usage statistics for an instance of ObjectStore.
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_report(ObjectStore * this)
{
  AllocInfo * iterator = this->allocList;
  
  PRINT(("Object Store Usage report:\n"));
  PRINT((" Nb allocated objects: %d\n", iterator->ptr->report(iterator->ptr)));
  iterator = iterator->prev;
  while (iterator!=0)
  {
    PRINT((" Nb allocated objects: %d\n", iterator->ptr->report(iterator->ptr)));
    iterator = iterator->next;
  }
  

}

/**********************************************//** 
  @brief Reports the number of allocated objects in the ObjectStore.
  @details TBD
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC unsigned int ObjectStore_getNbAllocatedObjects(ObjectStore * this)
{
  return this->nbAllocatedObjects;
}

/**********************************************//** 
  @brief Compare 2 instances of the class ObjjectStore.
  Since there is only one ObjectStore instance, always return 1.
  @public
  @memberof ObjectStore
  @return 0 if different, 1 if equal.
**************************************************/
PUBLIC int ObjectStore_compare(ObjectStore * this, ObjectStore * compared)
{
  int result = 1;

  return result;
}

/**********************************************//** 
  @brief Print an instance of the class ObjectStore.
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_print(ObjectStore * this)
{
}

/**********************************************//** 
  @brief Create the instance of the ObjectStore
  @details TBD
  @private
  @memberof ObjectStore
**************************************************/
PRIVATE ObjectStore * ObjectStore_new()
{
  ObjectStore * objectStore = (ObjectStore*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(ObjectStore));;
  objectStore->object.id = 0;
  objectStore->object.class = &objectStoreClass;
  objectStore->object.delete = 0;
  objectStore->object.copy = 0;
  objectStore->object.refCount = 1;
  objectStore->object.size = 0;
  objectStore->object.allocator = (Allocator*)Malloc_getRef();

  // Create Malloc Pool
  objectStore->allocList = (AllocInfo*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(AllocInfo));
  objectStore->allocList->ptr = (Allocator*)Malloc_getRef();
  objectStore->allocList->next = 0;
  objectStore->allocList->prev = 0;

  objectStore->nbAllocatedObjects =0;

  //ObjectStore_addAllocator(); 
  return objectStore;
}
