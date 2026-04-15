/**********************************************//**
  @file Object.c

  @brief Base object lifecycle implementation for the class framework.

  @details Object is the root base class of the class framework. It provides
  the core services that every managed object in the system depends on:
  construction, destruction, copying, comparison, printing, and reference
  counting. All higher-level classes are built on top of this foundation.

  Two construction paths are supported:
  - Object_new(): allocates through the ObjectMgr pool, suitable for
    general-purpose objects with no specific memory placement requirement.
  - Object_newFromAllocator(): allocates through a caller-specified Allocator
    registered with the ObjectStore, suitable for objects that must reside in
    a particular memory region (e.g. a pool, arena, or DMA buffer).

  In both cases the constructed object is wired to its class descriptor,
  which provides the virtual function table (delete, copy, compare, print)
  and size. The object carries a validity marker (OBJECT_MARKER) that is
  checked at the entry point of every mutating operation and cleared on
  deallocation, providing a basic defence against use-after-free.

  Reference counting is supported via Object_getRef() and Object_deRef().
  The reference count expresses how many owners hold a reference to the
  object. Deallocation is a two-step operation: Object_delete() invokes the
  class destructor to release owned resources, and Object_deallocate()
  returns the memory to whichever backend (ObjectMgr or ObjectStore) created
  it.

  @note Object_new() and Object_newFromAllocator() initialise their
  respective singleton backends (ObjectMgr, ObjectStore) on first use.
  These initialisations are not re-entrant and must not be called
  concurrently without external synchronisation.
**************************************************/

#include "Class.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "ObjectStore.h"
#include "Allocator.h"
#include "Memory.h"
#include "Error.h"

/**********************************************//**
  @private
**************************************************/
PRIVATE ObjectMgr * Object_objMgrPtr = 0;
PRIVATE ObjectStore * Object_objectStore = 0;

#define OBJECT_MARKER (0x0B5EC7)

/**********************************************//** 
  @brief Create an instance of the class Object.
  @public
  @param[in] Class to instanciate
  @memberof Object
**************************************************/
PUBLIC Object * Object_new(unsigned int size, Class * class)
{
  Object * this = 0;
  
  if (class == 0) return 0;

  if (Object_objMgrPtr==0)
  {
    // TODO: Not re-entrant
    Object_objMgrPtr = ObjectMgr_getRef();
  }
  this = ObjectMgr_allocate(Object_objMgrPtr, size);
  
  if (MEMORY_IS_INVALID(this)) return 0;

  this->class = class;
  this->marker = OBJECT_MARKER;
  
  this->delete = class->f_delete;
  this->copy = class->f_copy;
  this->size = class->f_size(0);
  
  this->refCount = 1;
  this->allocator = 0;
  
  return this;
}

/**********************************************//**
  @brief Create an instance of the specified class using 
  specified allocator.
  @public
  @param[in] Class to instanciate
  @param[in] Allocator to use
  @memberof Object
**************************************************/
PUBLIC Object* Object_newFromAllocator(Class* class, Allocator * allocator)
{
  Object* this = 0;

  if (class == 0) return 0;

  if (Object_objectStore==0)
  {
    Object_objectStore = ObjectStore_getRef();
  }

  this = ObjectStore_createObject(Object_objectStore, class, allocator);
  
  if (MEMORY_IS_INVALID(this)) return 0;

  this->marker = OBJECT_MARKER;
  this->class = class;
  
  this->delete = class->f_delete;
  this->copy = class->f_copy;
  this->size = class->f_size(0);
  
  this->refCount = 1;
  this->allocator = allocator;
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Object.
  @public
  @memberof Object
**************************************************/
PUBLIC void Object_delete(Object * this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->class->f_delete(this);
}

/**********************************************//**
  @brief De-allocate an instance of the class Object.
  @public
  @memberof Object
**************************************************/
PUBLIC void Object_deallocate(Object* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->marker = 0;
  if (this->allocator == 0)
    ObjectMgr_deallocate(Object_objMgrPtr, this);
  else
    ObjectStore_deleteObject(Object_objectStore, this);
}

/**********************************************//** 
  @brief Copy an instance of the class Object.
  @public
  @memberof Object
  @return New instance
**************************************************/
PUBLIC Object * Object_copy(Object * this)
{
  Object * copy = 0;

  if (MEMORY_IS_INVALID(this)) return 0;

  copy = this->copy((Object*)this);
  
  return copy;
}

/**********************************************//** 
  @brief Compare 2 instances of the class Object.
  @public
  @memberof Object
  @return 0 if O1=O2, negative if O1<O2, positive if O1>O2
**************************************************/
PUBLIC int Object_comp(Object * this, Object * compared)
{
  int result = 0;
  
  if ((this!=0) && (this->class->f_comp!=0))
  {
    return this->class->f_comp(this, compared);
  }
  // TBC: Need an error
  return result;
}

/**********************************************//** 
  @brief Print an instance of the class Object into
  a buffer of characters.
  @public
  @memberof Object
  @return Buffer of characters
**************************************************/
PUBLIC char * Object_print(Object * this)
{
  char * result = 0;
  
  if ((this != 0) && (this->class->f_print != 0))
  {
    return this->class->f_print(this);
  }
  return result;
}

/**********************************************//** 
  @brief Get a reference to an instance of the class Object.
  @public
  @memberof Object
  @return Reference to instance
**************************************************/
PUBLIC Object* Object_getRef(Object* this)
{ 
  if (OBJECT_IS_INVALID(this)) return 0;

  this->refCount++;
  
  return this;
}

/**********************************************//** 
  @brief De-reference to an instance of the class Object.
  @public
  @memberof Object
**************************************************/
PUBLIC void Object_deRef(Object * this)
{
  if (OBJECT_IS_INVALID(this)) return;

  if ((this->refCount)>1)
    this->refCount--;
  else if (this->refCount == 1)
  {
    // The last owner is done with the object.
    // 1. Run the class-specific destructor (clean up internal resources)
    Object_delete(this);
    
    // 2. Free the memory back to the manager/allocator
    Object_deallocate(this);
  }
  else
    Error_new(ERROR_NORMAL, "Attempt to de-reference an object not referenced.\n");
}

/**********************************************//**
  @brief Check the pointed object is allocated.
  @public
  @memberof Object
  @return 1 if valid, 0 otherwise
**************************************************/
PUBLIC int Object_isValid(Object* this)
{
  return ((MEMORY_IS_VALID(this) && (this->marker == OBJECT_MARKER)));
}
