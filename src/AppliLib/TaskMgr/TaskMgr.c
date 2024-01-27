/* TaskMgr.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Mutex.h"
#include "Task.h"
#include "Memory.h"

#include <stdlib.h>
#include <stdio.h>

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#include <conio.h>
#include <process.h>
#endif

//HANDLE  hRunMutex;                   // "Keep Running" mutex
//HANDLE  hScreenMutex;                // "Screen update" mutex
//HANDLE  hThreads[MAX_THREADS] = { NULL }; // Handles for created threads

//hScreenMutex = CreateMutexW(NULL, FALSE, NULL);  // Cleared
//hRunMutex = CreateMutexW(NULL, TRUE, NULL);      // Set

//WaitForSingleObject(hScreenMutex, INFINITE);

//WaitForSingleObject(hThreads[--ThreadNr], INFINITE);

//hThreads[ThreadNr] = (HANDLE)_beginthread(BounceProc, 0, (void*)(uintptr_t)ThreadNr);

//while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);

#define MAX_TASKS (10)
#define MAX_THREADS (1)

struct TaskMgr
{
  Object object;
  int maxTask;
  int nbThreads;
  int isStarted;
  Task * taskId[MAX_TASKS];
  Mutex runMutex;
#ifndef WIN32
  pthread_t threadHandle[MAX_THREADS];
#else
  HANDLE threadHandle[MAX_THREADS];
#endif
};

Class taskMgrClass = {
  .f_new = 0,
  .f_delete = (Destructor)&TaskMgr_delete,
  .f_comp = 0,
  .f_copy = 0,
  .f_print = (Printer)&TaskMgr_print,
  .f_size = (Sizer)&TaskMgr_getSize
};

PRIVATE void * TaskMgr_threadBody(void * this);
PRIVATE void TaskMgr_waitForThread();

PUBLIC TaskMgr * TaskMgr_new(int maxTask)
{
  TaskMgr * this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

  //Mutex_new(&this->runMutex, 0);

  this->nbThreads = MAX_THREADS;

  for (int i; i<this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
#else
    hThreads[ThreadNr] = 
                (HANDLE)_beginthread(TaskMgr_threadBody, 0, (void*)this);
#endif
  }

  return this;
}

PUBLIC int TaskMgr_start(TaskMgr * this, Task * task)
{
  int isStarted = 0;
  // Release mutex
  for (int i=0; i<this->maxTask; i++)
  {
    if (this->taskId[i] == 0) 
    {
      this->taskId[i] = task;
      isStarted = 1;
      break;
    }
  }

  TaskMgr_waitForThread(this);

  return isStarted;
}

PUBLIC void TaskMgr_stop(TaskMgr * this)
{
  Mutex_release(&this->runMutex);
  while (this->nbThreads > 0)
  {
    TaskMgr_waitForThread(this);
    this->nbThreads--;
  }
}

PUBLIC void TaskMgr_delete(TaskMgr * this)
{
  // Mutex_release(this->runMutex);

  Mutex_delete(&this->runMutex);
}

PUBLIC void TaskMgr_print(TaskMgr * this)
{

}

PUBLIC unsigned int TaskMgr_getSize(TaskMgr * this)
{
  if (this == 0) return sizeof(TaskMgr);

  return sizeof(this);
}

PRIVATE void * TaskMgr_threadBody(void * this)
{
  //wait run mutex

  // wait clock mutex

  //wait run mutex with timeout

  //terminate
  for (int i=0; i<20; ++i)
  {
    printf("Here\n");
  }
  /*do
    {
        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        ReleaseMutex(hScreenMutex);
    }
    // Repeat while RunMutex is still taken.
    while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);*/
}

PRIVATE void TaskMgr_waitForThread(TaskMgr * this)
{
  for (int i=0; i<this->nbThreads; ++i)
  {
    pthread_join(this->threadHandle[i], NULL);
  } 
}
