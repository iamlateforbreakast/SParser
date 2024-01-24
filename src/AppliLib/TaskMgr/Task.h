/* Task.h */
#ifndef _TASK_H_
#define _TASK_H_

#include "Types.h"

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#include <conio.h>
#include <process.h>
#endif

typedef struct Task
{
#ifndef WIN32
  pthread_t id;
#else
  HANDLE id;
#endif
  void (*body)(Object * this, int nbParams, void **params);
  int nbParams;
  void **params;
  int isFinished;
  int execTime;
} Task;

PUBLIC Task * Task_create(void (*body)(Object * this, int nbParams, void **params))
{
#ifndef WIN32
  err = pthread_create(&(tid[i]), NULL, &TaskMgr_executeTaskBody, NULL);
#else
  hThreads[ThreadNr] = 
                (HANDLE)_beginthread(TaskMgr_executeTaskBody, 0, (void*)(uintptr_t)ThreadNr);
#endif
  return 0;
}

PRIVATE void Task_executeBody(Task * this)
{
  
}

#endif /* _TASK_H_ */