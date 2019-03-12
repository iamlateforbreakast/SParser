/* Timer.c*/

#include "TimerMgr.h"

struct Timer
{
};

Timer * Timer_new()
{
  Timer * this = 0;
  
  this = (SdbRequest*)Object_new(sizeof(SdbRequest),(Destructor)&SdbRequest_delete, (Copy_operator)&SdbRequest_copy);
  this->object.size = sizeof(SdbRequest);
  
  return this;
}

void Timer_delete(Timer * this)
{
}
