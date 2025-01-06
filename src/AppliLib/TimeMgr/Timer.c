/* Timer.c*/

#include "TimeMgr.h"
#include "Timer.h"
#include "Times.h"
#include "Memory.h"
#include "Class.h"
#include "Object.h"
#include "Debug.h"

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
  long double cpuDurationS;
  long double wallDurationS;
  long double cpuLatchedTimeS;
  long double wallLatchedTimeS;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class timerClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Timer_delete,
  .f_copy = (Copy_Operator)&Timer_copy,
  .f_comp = (Comp_Operator)&Timer_isEqual,
  .f_print = (Printer)&Timer_print,
  .f_size = (Sizer)&Timer_getSize
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
  this->wallDurationS = (long double)0.0;
  this->wallLatchedTimeS = (long double)0.0;
  this->cpuDurationS = (long double)0.0;
  this->cpuLatchedTimeS = (long double)0.0;

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
      Object_deallocate(&this->object);
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
  result->wallDurationS = this->wallDurationS;
  result->wallLatchedTimeS = this->wallLatchedTimeS;

  result->cpuDurationS = this->cpuDurationS;
  result->cpuLatchedTimeS = this->cpuLatchedTimeS;

  result->name = String_getRef(this->name);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Timer
**************************************************/
PUBLIC unsigned int Timer_getSize(Timer * this)
{
  if (this==0) return sizeof(Timer);

  return sizeof(Timer);
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
PUBLIC void Timer_print(Timer * this)
{
  unsigned int size = 0;
  const char * format = "Timer %s : NbCalls %d\n"
                        "\tWall Total %Lfs Wall Avg. %Lfs\n"
                        "\tCPU Total %Lf CPU Avg. %Lfs\n";
  long double wallAverageDuration = 0.0;
  long double cpuAverageDuration = 0.0;

  if (this->nbCalls>0) 
  {
    wallAverageDuration = this->wallDurationS/this->nbCalls;
    cpuAverageDuration = this->cpuDurationS/this->nbCalls;
  }
  //size = snprintf(0, 0, format, String_getBuffer(this->name), this->nbCalls, this->durationS, average_duration);
  //result = Memory_alloc(size);
   PRINT((format, String_getBuffer(this->name), this->nbCalls, 
         this->wallDurationS, wallAverageDuration,
         this->cpuDurationS, cpuAverageDuration));
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Timer
**************************************************/
PUBLIC void Timer_latchTime(Timer * this)
{
   
  long double wallTimeS = get_wall_time();
  long double cpuTimeS = get_cpu_time();
  
  if (this->state == 0)
  {
    this->nbCalls++;
    this->state = 1;
    this->wallLatchedTimeS = wallTimeS;
    this->cpuLatchedTimeS = cpuTimeS;
    this->wallDurationS = 0;
    this->cpuDurationS = 0;
    PRINT(("Timer.c: %Lf\n", this->cpuLatchedTimeS));
    PRINT(("Timer.c: %Lf\n", get_wall_time()));
  }
  else
  { 
    this->nbCalls++;
    PRINT(("Timer.c: %Lf\n", this->cpuLatchedTimeS));
    PRINT(("Timer.c: %Lf\n", get_wall_time()));
    this->wallDurationS = this->wallDurationS + wallTimeS - this->wallLatchedTimeS;
    this->cpuDurationS = this->cpuDurationS + cpuTimeS - this->cpuLatchedTimeS;
    this->wallLatchedTimeS = wallTimeS;
    this->cpuDurationS = cpuTimeS;
  }
}
