/* Timer.c*/

#include "TimeMgr.h"
#include "Timer.h"
#include "Memory.h"
#include "Class.h"
#include "Object.h"

#include <stdio.h>

/**********************************************//**
  @class Timer
**************************************************/
struct Timer
{
  Object object;
  String * name;
  unsigned int state;
  unsigned int nbCalls;
  double durationS;
  double latchedTime;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class timerClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Timer_delete,
  .f_copy = (Copy_Operator)&Timer_copy,
  .f_equal = (Equal_Operator)&Timer_isEqual,
  .f_print = (Printer)&Timer_print
};

/**********************************************//** 
  @brief Create an instance of the class Timer.
  @public
  @memberof Timer
  @return New instance.
**************************************************/
PUBLIC Timer * Timer_new(String * name)
{
  Timer * this = 0;
  
  this = (Timer*)Object_new(sizeof(Timer), &timerClass);
  this->object.size = sizeof(Timer);
  
  this->name = String_getRef(name);
  this->state = 0;
  this->nbCalls = 0;
  this->durationS = (float)0.0;
  this->latchedTime = (float)0.0;
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Timer.
  @public
  @memberof Timer
**************************************************/
PUBLIC void Timer_delete(Timer * this)
{
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      String_delete(this->name);
      Object_delete(&this->object);
    }
    else
    {
      this->object.refCount--;
    }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class Timer.
  @public
  @memberof Timer
  @return Copied instance.
**************************************************/
PUBLIC Timer * Timer_copy(Timer * this)
{
  Timer * result = 0;
  
  result = Timer_new(this->name);
  result->state = this->state;
  result->nbCalls = this->nbCalls;
  result->durationS = this->durationS;
  result->latchedTime = this->latchedTime;
  result->name = String_getRef(this->name);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Timer
**************************************************/
PUBLIC unsigned int Timer_isEqual(Timer * this, Timer * compared)
{
  unsigned int result = 0;

  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Timer
**************************************************/
PUBLIC char * Timer_print(Timer * this)
{
  char * result = 0;
  unsigned int size = 0;
  const char * format = "Timer %s : NbCalls %d Total %lfs Avg. %lfs";
  double average_duration = 0;
  
  if (this->nbCalls>0) average_duration = this->durationS/this->nbCalls;
  
  size = snprintf(0, 0, format, String_getBuffer(this->name), this->nbCalls, this->durationS, average_duration);
  result = Memory_alloc(size);
  snprintf(result, size, format, String_getBuffer(this->name), this->nbCalls, this->durationS, average_duration);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Timer
**************************************************/
PUBLIC void Timer_latchTime(Timer * this, double timeS)
{ 
  if (this->state == 0)
  {
    this->state = 1;
    this->latchedTime = timeS;
    //printf("Timer.c: %f\n", timeS);
  }
  else
  {
    this->state = 0;
    this->nbCalls++;
    //printf("Timer.c: %f\n", timeS);
    this->durationS = this->durationS + timeS - this->latchedTime;
    this->latchedTime = timeS;
  }
}
