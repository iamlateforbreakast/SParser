/* Timer.c*/

#include "TimeMgr.h"
#include "Timer.h"
#include "Class.h"
#include "Object.h"

struct Timer
{
  Object object;
};

PRIVATE Class timerClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Timer_delete,
  .f_copy = (Copy_Operator)&Timer_copy,
  .f_equal = (Equal_Operator)0,
  .f_print = (Printer)0
};

Timer * Timer_new()
{
  Timer * this = 0;
  
  this = (Timer*)Object_new(sizeof(Timer), &timerClass);
  this->object.size = sizeof(Timer);
  
  return this;
}

void Timer_delete(Timer * this)
{
}
