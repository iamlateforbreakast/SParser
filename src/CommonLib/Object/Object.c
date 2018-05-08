/**********************************************//** 
  @file Object.c
  
  @class Object
  
  @brief This file contains the implementation for the class Object
  
  The class Object is TBD
**************************************************/

#include "Object.h"
#include "ObjectMgr.h"

/* PRIVATE ObjectMgr * Object_objMgrPtr = 0; */

PUBLIC Object * Object_new(unsigned int size, void (*f_delete)(Object*), Object * (*f_copy)(Object*))
{
  Object * this = 0;
  
  /* if (object_objectPtr==NULL)
  {
    // TODO: Not re-entrant
    Object_objectPtr = ObjectMgr_getRef();
  } */
  /* this = ObjectMgr_allocate(objectMgr, size); */
  /* this->delete = f_delete; */
  /* this->copy = f_copy; */
  /* this->refCount = 0; */
  /* this->size = size; */
  
  return this;
}

PUBLIC void Object_delete(Object * this)
{
  /* ObjectMgr_deallocate(objectPtr, this); */
}

PUBLIC Object * Object_copy(Object * this)
{
  Object * copy;
  
  /* copy = this->f_copy(this); */
  
  return copy;
}

PUBLIC Object* Object_getRef(Object* this)
{ 
  /* this->refCount++; */
  return this;
}
