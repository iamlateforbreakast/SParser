/**********************************************//**
  @file ObjectStore.c

  @brief Registry and broker for all runtime object allocators.

  @details ObjectStore is a singleton that maintains a registry of Allocator
  instances and acts as the dispatch layer between the object lifecycle API
  and the underlying memory backends. Rather than managing memory directly,
  it delegates all allocation and deallocation to whichever Allocator was
  registered at object creation time, allowing different objects to be backed
  by different memory strategies (heap, pool, arena, etc.) transparently.

  Responsibilities:
  - Registering and unregistering Allocator instances at runtime
  - Allocating raw object memory through the appropriate Allocator
  - Deallocating objects by routing back to their originating Allocator
  - Tracking the total number of objects created across all allocators
  - Reporting per-allocator usage statistics

  Only one instance of this class may exist at any time. Callers obtain a
  reference via ObjectStore_getRef() and release it via ObjectStore_delete(),
  which uses reference counting to defer destruction until all references
  are dropped.
**************************************************/
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
  .f_print = (Printer)&ObjectStore_print,
  .f_size = 0
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
  if (OBJECT_IS_INVALID(this)) return;

  this->object.refCount = this->object.refCount - 1;
  
  if (this->object.refCount == 0)
  {
    AllocInfo * iterator = this->allocList;
    AllocInfo * current = this->allocList;
    
    iterator=iterator->next;
    current->ptr->delete(current->ptr);
    Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)current);
    current = iterator;

    while (iterator!=0)
    {
      iterator=iterator->next;
      current->ptr->delete(current->ptr);
      Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)current);
      current = iterator;
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
  @details Return reference to the singleton ObjectStore
  instance, creating it if it does not already exist. 
  Increments the reference count on the ObjectStore
   instance before returning.
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
  @details Register an Allocator with the objectStore by creating an AllocInfo
  instance for it and adding it to the linked list of allocators. 
  The ObjectStore maintains a linked list of AllocInfo structures, 
  each of which contains a pointer to an Allocator and
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC AllocInfo * ObjectStore_createAllocator(ObjectStore * this, Allocator * allocator)
{
  AllocInfo * allocInfo;

  allocInfo = (AllocInfo*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(AllocInfo));
  
  allocInfo->ptr = allocator;
  
  if (this->allocList != 0)
  {
    allocInfo->prev = 0;
    allocInfo->next = this->allocList;
    this->allocList->prev = allocInfo;
    this->allocList = allocInfo;
  }
  else
  {
    allocInfo->prev = 0;
    allocInfo->next = 0;
    this->allocList = allocInfo;
  }
  return allocInfo;
}

/**********************************************//** 
  @brief Unregister an Allocator from the objectStore
  @details Unregister an Allocator from the objectStore by removing its AllocInfo
  instance from the linked list of allocators and deleting it.
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_deleteAllocator(ObjectStore * this, AllocInfo * allocInfo)
{
  if (allocInfo->prev!=0)
    allocInfo->prev->next = allocInfo->next;
  if (allocInfo->next)
    allocInfo->next->prev = allocInfo->prev;
  if (this->allocList == allocInfo)
    this->allocList = allocInfo->next;

  allocInfo->ptr->delete(allocInfo->ptr);
  Malloc_deallocate((Allocator*)Malloc_getRef(), (void*)allocInfo);
}

/**********************************************//** 
  @brief Create an object in the object store.
  @details Create an object in the object store by 
  allocating memory for it through the appropriate Allocator
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC Object * ObjectStore_createObject(ObjectStore * this, Class * class, Allocator * allocator)
{
  Object * object = 0;
  
  object = (Object *)allocator->allocate(allocator, class->f_size(0));//0B5EC7
  if (object!=0) //return object;
  {
    object->id = this->nbAllocatedObjects;
    object->class = class;
    object->size = class->f_size(0);
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
  @details Delete an object from the object store by routing the deallocation
  request back to the Allocator that was used to create the object.
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
  @details TBD
   Iterates through the list of registered Allocators and reports 
   the number of allocated objects for each one, as well as the 
   total number of allocated objects across all allocators.
   This function can be used to monitor memory usage and identify 
   potential leaks or inefficiencies in the allocation strategy.
   The report includes:
   - Total number of allocated objects across all allocators
   - Number of allocated objects for each individual allocator
   - Any additional statistics provided by the Allocator's report 
     function (e.g. peak usage, fragmentation, etc.)
   The output can be formatted as needed, such as in a tabular format
    or with additional context for each allocator.
  
  @public
  @memberof ObjectStore
**************************************************/
PUBLIC void ObjectStore_report(ObjectStore * this)
{
  AllocInfo * iterator = this->allocList;
  
  PRINT(("Object Store Usage report:\n"));
  PRINT((" Nb allocated objects: %d\n", iterator->ptr->report(iterator->ptr)));
  iterator = iterator->next;
  while (iterator!=0)
  {
    PRINT((" Nb allocated objects: %d\n", iterator->ptr->report(iterator->ptr)));
    iterator = iterator->next;
  }
  

}

/**********************************************//** 
  @brief Reports the number of allocated objects in the ObjectStore.
  @details TBD
   Returns the total number of allocated objects across all allocators in the ObjectStore.
   This function can be used to quickly check the overall memory usage of the application
   without needing to iterate through each individual allocator. It provides a high-level
   overview of how many objects are currently allocated in the system, which can be useful
   for monitoring and debugging purposes.
  
   The count is maintained by the ObjectStore and should be updated whenever an object is created or deleted through the ObjectStore's API.
   This function simply returns the current value of that count, which represents the total number of allocated
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
  @details Initializes the singleton ObjectStore 
  instance by allocating memory for it and setting up 
  its initial state, including the linked list of AllocInfo s
  tructures for registered allocators. This function is called 
  internally by ObjectStore_getRef() when the first reference 
  to the ObjectStore is requested. It should not be called directly by external code.
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
