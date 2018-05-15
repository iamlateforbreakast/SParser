/**********************************************//** 
  @file ErrorMgr.c
  
  The class ErrorMgr is TBD
**************************************************/

#include "ErrorMgr.h"
#include "Object.h"

/**********************************************//**
  @class ErrorMgr
**************************************************/
struct ErrorMgr{
  Object object;
};

PRIVATE ErrorMgr * errorMgr = 0;

PRIVATE ErrorMgr * ErrorMgr_new();

/**********************************************//** 
  @brief Create an instance of the class ErrorMgr.
  @private
  @memberof ErrorMgr
  @return Created instance.
**************************************************/
PRIVATE ErrorMgr * ErrorMgr_new()
{
  ErrorMgr * this = 0;
  
  this = (ErrorMgr*)Object_new(sizeof(ErrorMgr),(Destructor)&ErrorMgr_delete, (Copy_operator)&ErrorMgr_copy);
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class ErrorMgr.
  @public
  @memberof ErrorMgr
**************************************************/
PUBLIC void ErrorMgr_delete(ErrorMgr * this)
{
  Object_delete(&this->object);
}

/**********************************************//** 
  @brief Copy an instance of the class ErrorMgr.
  @public
  @memberof ErrorMgr
  @return Copy of the instance given as a parameter.
**************************************************/
PUBLIC ErrorMgr * ErrorMgr_copy(ErrorMgr * this)
{
  ErrorMgr * copy = 0;
  
  return copy;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof ErrorMgr
**************************************************/
PUBLIC ErrorMgr* ErrorMgr_getRef()
{ 
    if (errorMgr==0)
  {
    errorMgr = ErrorMgr_new();
  }
  else
  {
    errorMgr->object.refCount++;
  }
  
  return errorMgr;
}

/**********************************************//** 
  @brief Exit the processing of the main function and display an error message.
  @public
  @param[in] fmt Printf style display string.
  @memberof ErrorMgr
**************************************************/
PUBLIC void ErrorMgr_exit(ErrorMgr * this, const char *fmt, ...)
{
  /* va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
  exit(1); */
}