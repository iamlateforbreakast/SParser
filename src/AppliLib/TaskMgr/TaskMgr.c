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

//while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);

#define MAX_TASKS (5)
#define MAX_THREADS (1)

/**********************************************//**
  @private
**************************************************/
#ifndef WIN32
PRIVATE void* TaskMgr_threadBody(void* this);
#else
DWORD WINAPI TaskMgr_threadBody(LPVOID lpParam);
#endif
PRIVATE void TaskMgr_waitForThread(TaskMgr* this);
/**********************************************//**
  @class TaskMgr
**************************************************/
struct TaskMgr
{
  Object object;
  int nbThreads;
  int isStarted;
  Task * taskId[MAX_TASKS];
#ifndef WIN32
  pthread_t threadHandle[MAX_THREADS];
  Mutex runMutex;
#else
  HANDLE threadHandle[MAX_THREADS];
  DWORD   dwThreadIdArray[MAX_THREADS];
  CRITICAL_SECTION cond;
#endif
};

/**********************************************//**
  @private Class Description
**************************************************/
Class taskMgrClass = {
  .f_new = 0,
  .f_delete = (Destructor)&TaskMgr_delete,
  .f_comp = 0,
  .f_copy = 0,
  .f_print = (Printer)&TaskMgr_print,
  .f_size = (Sizer)&TaskMgr_getSize
};

/********************************************************//**
  @brief Create a new instance of the class TaskMgr.
  @public
  @memberof Map
  @return New taskMgr instance or NULL if failed to allocate.
************************************************************/
PUBLIC TaskMgr* TaskMgr_new()
{
  TaskMgr * this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

  //Mutex_new(&this->runMutex, 0);

  this->nbThreads = MAX_THREADS;

  for (int i = 0; i < MAX_TASKS; ++i) this->taskId[i] = 0;
  InitializeCriticalSection(&this->cond);
  EnterCriticalSection(&this->cond);
  for (int i = 0; i < this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
#else
    this->threadHandle[i] = CreateThread(
      NULL,                         // default security attributes
      0,                            // use default stack size  
      TaskMgr_threadBody,           // thread function name
      this,                         // argument to thread function 
      CREATE_SUSPENDED,             // use default creation flags 
      &this->dwThreadIdArray[i]);   // returns the thread identifier 
#endif
  }

  return this;
}

PUBLIC void TaskMgr_delete(TaskMgr* this)
{
  //Mutex_release(this->runMutex);
  //Mutex_delete(&this->runMutex);
}
PUBLIC TaskMgr* TaskMgr_getRef()
{
  return 0;
}
PUBLIC int TaskMgr_start(TaskMgr * this, Task * task)
{
  int isQueued = 0;
  // Release mutex
  for (int i = 0; i < MAX_TASKS; i++)
  {
    if (this->taskId[i] == 0) 
    {
      this->taskId[i] = task;
      isQueued = 1;
      break;
    }
  }
  if (isQueued) LeaveCriticalSection(&this->cond);
  
  //pthread_cond_signal(&cond);
  //TaskMgr_waitForThread(this);
  for (int i = 0; i < this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
#else
    ResumeThread(this->threadHandle[i]);
#endif
  }

  return isQueued;
}

PUBLIC void TaskMgr_stop(TaskMgr * this)
{
  //Mutex_release(&this->runMutex);
  while (this->nbThreads > 0)
  {
    TaskMgr_waitForThread(this);
    this->nbThreads--;
  }
}



PUBLIC void TaskMgr_print(TaskMgr * this)
{

}

PUBLIC unsigned int TaskMgr_getSize(TaskMgr * this)
{
  if (this == 0) return sizeof(TaskMgr);

  return sizeof(this);
}

DWORD WINAPI TaskMgr_threadBody(LPVOID lpParam)
//PRIVATE void* TaskMgr_threadBody(void* this)
{
  int nextTask = -1;
  Task* task = 0;
  TaskMgr * this = (TaskMgr*)lpParam;
  while (1)
  {
    EnterCriticalSection(&this->cond);
  //wait run mutex
  //pthread_mutex_lock(&lock);
  
  // wait clock mutex
#ifndef WIN32
  pthread_cond_wait(&cond, &lock); 
#else
    // check for work
    for (int i = 0; i < MAX_TASKS; i++)
    {
      if (Task_isReady(this->taskId[i]))
      {
        nextTask = i;
        break;
      }
    }
    LeaveCriticalSection(&this->cond);
    if (nextTask == -1)
    {
      // No work to do
      SuspendThread(GetCurrentThread());
    }
    else
    {
      // Something to do
      Task_executeBody(this->taskId[nextTask]);
    }
#endif
    nextTask = -1;
  //wait run mutex with timeout

  //terminate
    
  }



  return 0;
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
#ifndef WIN32
    pthread_join(this->threadHandle[i], NULL);
#else
    //WaitForMultipleObjects();
#endif
  } 
}
