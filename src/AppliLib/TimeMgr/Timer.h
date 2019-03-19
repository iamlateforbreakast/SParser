/* Timer.h */

#include "Types.h"

typedef struct Timer Timer;

PUBLIC Timer * Timer_new();
PUBLIC void Timer_delete(Timer * this);
PUBLIC Timer * Timer_copy(Timer * this);
