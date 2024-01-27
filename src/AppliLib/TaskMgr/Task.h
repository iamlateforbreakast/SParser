/* Task.h */
#ifndef _TASK_H_
#define _TASK_H_

#include "Types.h"
#include "Memory.h"
#include "TaskMgr.h"

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#include <conio.h>
#include <process.h>
#endif

TaskMgr * taskMgr = 0;

typedef struct Task
{
  void (*body)(Object * this, int nbParams, void **params);
  int nbParams;
  void **params;
  int isFinished;
  int execTime;
} Task;

PUBLIC Task * Task_create(void (*body)(Object * this, int nbParams, void **params))
{
  Task * this = 0;
  
  this = (Task*)Memory_alloc(sizeof(Task));

  //TaskMgr_startTask(this);

  return 0;
}

    // Wait for each thread to complete
    //WaitForSingleObject(hThreads[--ThreadNr], INFINITE);

PRIVATE void Task_executeBody()
{
  //wait run mutex

  // wait clock mutex

  //wait run mutex with timeout

  //terminate

  //this->body(this->object, this->nbParams, this->params);

  //Task_delete();
}

#endif /* _TASK_H_ */