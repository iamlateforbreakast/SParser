/**********************************************//**
  @file TimeMgr.c 

  @brief This file contains the implementation for the class TimeMgr

  The class TimeMgr provides an interface to the creation of timers.
**************************************************/

#include "TimeMgr.h"
#include "Timer.h"
#include "Class.h"
#include "Object.h"
#include "Map.h"

/**********************************************//**
  @class TimeMgr
**************************************************/
struct TimeMgr
{
  Object object;
  Map * timers;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class timeMgrClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&TimeMgr_delete,
  .f_copy = (Copy_Operator)&TimeMgr_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0,
  .f_size = (Sizer)&TimeMgr_getSize
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
 
  this->timers = Map_new();

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
      Map_delete(this->timers);
      Object_deallocate(&this->object);
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
  @brief Provide the size of the class or an instance
  @public
  @memberof TimeMgr
  @return Size in byte
**************************************************/
PUBLIC unsigned int TimeMgr_getSize(TimeMgr * this)
{
  if (this==0) return sizeof(TimeMgr);

  return sizeof(TimeMgr);
}

/**********************************************//** 
  @brief Latch the current time under the specified name
  @public
  @memberof TimeMgr
  @param name of the timer to create
**************************************************/
PUBLIC void TimeMgr_latchTime(TimeMgr * this, String * s)
{
  Timer * timer = 0;
  
  Map_find(this->timers, s, (void**)&timer);
  if (timer)
  {
    Timer_latchTime(timer);
  }
  else
  {
    timer = Timer_new();
    Map_insert(this->timers, s, (void**)timer);
  }
}

PUBLIC void TimeMgr_report(TimeMgr * this)
{
  List * timers = Map_getAll(this->timers);

  List_forEach(timers, &Timer_print);
}
