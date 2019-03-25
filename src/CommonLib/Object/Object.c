/**********************************************//** 
  @file Object.c
  
  @brief This file contains the implementation for the class Object
  
  The class Object is TBD
**************************************************/

#include "Class.h"
#include "Object.h"
#include "ObjectMgr.h"

/**********************************************//**
  @private
**************************************************/
PRIVATE ObjectMgr * Object_objMgrPtr = 0;

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
  this->delete = class->f_delete;
  this->copy = class->f_copy;
  this->refCount = 1;
  this->size = size;
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Object.
  @public
  @memberof Object
**************************************************/
PUBLIC void Object_delete(Object * this)
{
  ObjectMgr_deallocate(Object_objMgrPtr, this);
}

/**********************************************//** 
  @brief Copy an instance of the class Object.
  @public
  @memberof Object
  @return New instance
**************************************************/
PUBLIC Object * Object_copy(Object * this)
{
  Object * copy;
  
  copy = ObjectMgr_allocate(Object_objMgrPtr, this->size);
  copy->delete = this->delete;
  copy->copy = this->copy;
  copy->size = this->size;
  copy->refCount = 1;
  
  return copy;
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
