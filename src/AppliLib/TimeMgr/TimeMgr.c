/**********************************************//**
  @file TimeMgr.c 

  @brief This file contains the implementation for the class TimeMgr

  The class TimeMgr provides an interface to the creation of timers.
**************************************************/

#include "TimeMgr.h"
#include "Class.h"
#include "Object.h"

/**********************************************//**
  @class TimeMgr
**************************************************/
struct TimeMgr
{
  Object object;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class timeMgrClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&TimeMgr_delete,
  .f_copy = (Copy_Operator)&TimeMgr_copy,
  .f_equal = (Equal_Operator)0,
  .f_print = (Printer)0
};

PRIVATE TimeMgr * timeMgr = 0;

/**********************************************//** 
  @brief Create an instance of the class TimeMgr.
  @private
  @memberof TimeMgr
  @return New instance.
**************************************************/
PRIVATE TimeMgr * TimeMgr_new()
{
  TimeMgr * this = 0;

  this = (TimeMgr*)Object_new(sizeof(TimeMgr),&timeMgrClass);

  return this;
}

/**********************************************//** 
  @brief Delete a TimeMgr object.
  @public
  @memberof TimeMgr
  @param Object to delete.
**************************************************/
PUBLIC void TimeMgr_delete(TimeMgr * this)
{
  if (this != 0)
  {
    if (this->object.refCount==1)
    {
      Object_delete(&this->object);
      timeMgr = 0;
    }
    else if (this->object.refCount>1)
    {
      this->object.refCount--;
    }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class TimeMgr.
  @public
  @memberof TimeMgr
  @return New instance
**************************************************/
PUBLIC TimeMgr * TimeMgr_copy(TimeMgr * this)
{
  this->object.refCount++;

  return this;
}

/**********************************************//** 
  @brief Get a reference to the singleton instance of TimeMgr.
  @public
  @memberof TimeMgr
  @return Reference to the singleton.
**************************************************/
PUBLIC TimeMgr * TimeMgr_getRef()
{
  if (timeMgr==0)
  {
    timeMgr = TimeMgr_new();
  }
  else
  {
    timeMgr->object.refCount++;
  }

  return timeMgr;
}

/**********************************************//** 
  @brief Latch the current time under the specified name
  @public
  @memberof TimeMgr
  @param name of the timer to create
**************************************************/
PUBLIC void TimeMgr_latchTime(TimeMgr * this, String * s)
{
}

PUBLIC void TimeMgr_report(TimeMgr * this)
{
}
