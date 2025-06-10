/**********************************************//** 
  @file Object.c
  
  @brief This file contains the implementation for the class Object
  
  The class Object is TBD
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
  
  if (Object_objMgrPtr==0)
  {
    // TODO: Not re-entrant
    Object_objMgrPtr = ObjectMgr_getRef();
  }
  this = ObjectMgr_allocate(Object_objMgrPtr, size);
  this->class = class;
  this->marker = OBJECT_MARKER;
  if (this->class!=0)
  {
    this->delete = class->f_delete;
    this->copy = class->f_copy;
    this->size = class->f_size(0);
  }
  else
  {
    this->delete = (Destructor)0;
    this->copy = (Copy_Operator)0;
    this->size = 0;
  }
  this->refCount = 1;
  this->allocator = 0;
  
  return this;
}

/**********************************************//**
  @brief TBD
  @public
  @param[in] Class to instanciate
  @memberof Object
**************************************************/
PUBLIC Object* Object_newFromAllocator(Class* class, Allocator * allocator)
{
  Object* this = 0;

  if (Object_objectStore==0)
  {
    Object_objectStore = ObjectStore_getRef();
  }
  this = ObjectStore_createObject(Object_objectStore, class, allocator);
  if (this != 0)
  {
    this->marker = OBJECT_MARKER;
    this->class = class;
    if (this->class != 0)
    {
      this->delete = class->f_delete;
      this->copy = class->f_copy;
      this->size = class->f_size(0);
    }
    this->refCount = 1;
    this->allocator = allocator;
  }

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Object.
  @public
  @memberof Object
**************************************************/
PUBLIC void Object_delete(Object * this)
{
  if ((this->class != 0) && (this->class->f_delete != 0))
  {
    this->class->f_delete(this);
  }
}

/**********************************************//**
  @brief De-allocate an instance of the class Object.
  @public
  @memberof Object
**************************************************/
PUBLIC void Object_deallocate(Object* this)
{
  if (this->marker != OBJECT_MARKER)
  {
    Error_new(ERROR_DBG, "Object_de-allocate uses a invalid object\n");
    return;
  }
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

  if ((this!=0) && (this->copy!=0))
  {
    copy = this->copy((Object*)this);
  }
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
  if ((this->refCount)>1)
    this->refCount--;
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
  if (this == 0) return 0;

  return ((MEMORY_ISVALID(this) && (this->marker == OBJECT_MARKER)));
}
