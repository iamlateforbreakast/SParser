/* Timer.c*/

#include "TimeMgr.h"
#include "Timer.h"
#include "Object.h"

struct Timer
{
  Object object;
};

Timer * Timer_new()
{
  Timer * this = 0;
  
  this = (Timer*)Object_new(sizeof(Timer),(Destructor)&Timer_delete, (Copy_operator)&Timer_copy);
  this->object.size = sizeof(Timer);
  
  return this;
}

void Timer_delete(Timer * this)
{
}
