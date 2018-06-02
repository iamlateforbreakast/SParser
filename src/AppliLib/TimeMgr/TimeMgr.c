/**********************************************//** 
  @file TimeMgr.c
  
  @brief This file implements the class TimeMgr
  
  The class TimeMgr is TBD
**************************************************/
#include "TimeMgr.h"
#include "Timer.h"
#include "String2.h"
#include "List.h"

/**********************************************//**
  @class TimeMgr
**************************************************/
struct TimeMgr
{
  List * timers;
}

/**********************************************//**
  @private
**************************************************/
PRIVATE TimeMgr * TimeMgr_new()
{
  TimeMgr * this = 0;
  
  return this;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_delete(TimeMgr * this)
{
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
  /* Find timer s */
  
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof TimeMgr
**************************************************/
PUBLIC void TimeMgr_report(TimeMgr * this)
{
}