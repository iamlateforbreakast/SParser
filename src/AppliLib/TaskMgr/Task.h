/* Task.h */
#ifndef _TASK_H_
#define _TASK_H_

#include "Types.h"

typedef struct Task Task;

PUBLIC Task* Task_create(void * (*body)(void* p), int nbParams, void * params);
PUBLIC void Task_start(Task* this);
PUBLIC int Task_isReady(Task* this);
PUBLIC int Task_isRunning(Task* this);
PUBLIC void Task_destroy(Task* this);
PUBLIC void Task_executeBody(Task* this);

#endif /* _TASK_H_ */
