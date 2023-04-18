/* Timer.h */

#include "Types.h"

typedef struct Timer Timer;

PUBLIC Timer * Timer_new();
PUBLIC void Timer_delete(Timer * this);
PUBLIC Timer * Timer_copy(Timer * this);
PUBLIC unsigned int Timer_isEqual(Timer * this, Timer * compared);
PUBLIC void Timer_print(Timer * this);
PUBLIC void Timer_latchTime(Timer * this);