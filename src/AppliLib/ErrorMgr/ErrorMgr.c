/**********************************************//** 
  @file ErrorMgr.c
  
  The class ErrorMgr is TBD
**************************************************/

#include "ErrorMgr.h"
#include "Object.h"

struct ErrorMgr{
  Object object;
};

PRIVATE ErrorMgr * ErrorMgr_new()
{
  ErrorMgr * this = 0;
  
  return this;
}

PUBLIC void ErrorMgr_delete(ErrorMgr * this)
{
}

PUBLIC ErrorMgr * ErrorMgr_copy(ErrorMgr * this)
{
  ErrorMgr * copy;
  
  return copy;
}