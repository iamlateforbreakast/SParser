/**********************************************//** 
  @file TimeMgr.c
  
  @brief This file implements the class TimeMgr
  
  The class TimeMgr is TBD
**************************************************/
#include "TimeMgr.h"
#include "Timer.h"
#include "String2.h"
#include "Map.h"
#include "Class.h"
#include "Object.h"
#include "Error.h"
#include "Memory.h"
#include <sys/time.h>

#define USEC_IN_SEC (1.0E6)

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
  .f_equal = (Equal_Operator)0,
  .f_print = (Printer)0
};

PRIVATE TimeMgr * timeMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE TimeMgr * TimeMgr_new()
{
  TimeMgr * this = 0;
  
  this = (TimeMgr*)Object_new(sizeof(TimeMgr), &timeMgrClass);
  this->object.size = sizeof(TimeMgr);
  
  this->timers = Map_new();
  
  return this;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_delete(TimeMgr * this)
{
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      Map_delete(this->timers);
      Object_delete(&this->object);
      timeMgr = 0;
    }
    else
    {
      this->object.refCount--;
    }
  }
}

/**********************************************//**
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC TimeMgr * TimeMgr_copy(TimeMgr * this)
{
  return this;
}

/**********************************************//**
  @brief TBD
  @public
  @memberof TimeMgr
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
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_latchTime(TimeMgr * this, String * s)
{
  struct timeval tv;
  Timer * timer = 0;
  double timeS = 0.0;
  
  if (timeMgr == 0) return;
  
  gettimeofday(&tv, 0);
  /* Find timer s */
  Map_find(this->timers, s, (void**)&timer);
  if (!timer)
  {
    timer = Timer_new(s);
    Map_insert(this->timers, s, timer);
  }
  timeS = (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;
  Timer_latchTime(timer, timeS);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_report(TimeMgr * this)
{
  List * timers = 0;
  Timer * timerInfo = 0;
  char * buf = 0;
  
  timers = Map_getAll(this->timers);
  
  Error_new(ERROR_INFO, "TimeMgr Report:\n");
  timerInfo = List_removeHead(timers);

  while (timerInfo!=0)
  {
    buf = Timer_print(timerInfo);
    Error_new(ERROR_INFO, "%s\n", buf);
    Memory_free(buf,Memory_len(buf));
    Timer_delete(timerInfo);
    timerInfo = List_removeHead(timers);
  }
  
  List_delete(timers);
  
}