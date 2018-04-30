/**********************************************//** 
  @file ObjectMgr.c
  
  @brief TBD
  @details TBD
**************************************************/

#include "ObjectMgr.h"
#include "Object.h"

struct ObjectMgr
{
  Object object;
};

PRIVATE ObjectMgr * ObjectMgr_new()
{
  ObjectMgr * this = 0;
  
  return this;
}

PUBLIC void ObjectMgr_delete(ObjectMgr * this)
{
}

PUBLIC ObjectMgr * ObjectMgr_copy(ObjectMgr * this)
{
  ObjectMgr * copy = 0;
  
  return copy;
}