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
  if (this->class!=0)
  {
    this->delete = class->f_delete;
    this->copy = class->f_copy;
  }
  else
  {
    this->delete = (Destructor)0;
    this->copy = (Copy_Operator)0;
  }
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
  @return 1 if equal, 0 else.
**************************************************/
PUBLIC unsigned int Object_isEqual(Object * this, Object * compared)
{
  unsigned int result = 0;
  
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
