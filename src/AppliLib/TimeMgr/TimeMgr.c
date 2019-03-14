/**********************************************//** 
  @file TimeMgr.c
  
  @brief This file implements the class TimeMgr
  
  The class TimeMgr is TBD
**************************************************/
#include "TimeMgr.h"
#include "Timer.h"
#include "String2.h"
#include "Map.h"
#include <time.h>

/**********************************************//**
  @class TimeMgr
**************************************************/
struct TimeMgr
{
  Map * timers;
}; 

PRIVATE TimeMgr * timeMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE TimeMgr * TimeMgr_new()
{
  TimeMgr * this = 0;
  
  this = (TimeMgr*)Object_new(sizeof(TimeMgr),(Destructor)&TimeMgr_delete, (Copy_operator)&TimeMgr_copy);
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
  TimeMgr * copy = 0;
  
  return copy;
}

/**********************************************//**
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC TimeMgr * TimeMgr_getRef()
{
  TimeMgr * result = 0;
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_createTimer(TimeMgr * this, String * s)
{
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_timeStamp(TimeMgr * this, String * s)
{
  struct time_spec * t;
  Timer * timer = 0;
  
  /* Find timer s */
  #if 0
  if (timeMgr == 0)
  {
  }
  if ((timer = Map_find(s))!=0)
  {
    clock_gettime(CLOCK_REALTIME, &t);
    timer = t;
    Timer_clock(timer);
  }
  else
  {
    timer = Timer_new();
    Map_insert();
    Timer_clock();
  }
  #endif
  
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_report(TimeMgr * this)
{
}