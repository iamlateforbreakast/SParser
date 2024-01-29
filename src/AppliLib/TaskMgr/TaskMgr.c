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
  pthread_mutex_t mutex;
  pthread_cond_t isWork;
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

#ifndef WIN32
  if (pthread_mutex_init(&this->mutex, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 0; 
    }
  pthread_cond_init(&this->isWork, 0);
  pthread_mutex_lock(&this->mutex);
#else
  InitializeCriticalSection(&this->cond);
  EnterCriticalSection(&this->cond);
#endif
  for (int i = 0; i < this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
    pthread_detach(&(this->threadHandle[i]));
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
#ifndef WIN32
  pthread_mutex_destroy(&this->mutex);
  pthread_cond_destroy(&this->isWork);
#else
#endif
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
#ifndef WIN32
  if (isQueued) 
  {
    pthread_cond_broadcast(&this->isWork);
    pthread_mutex_unlock(&this->mutex);
  }
#else
  if (isQueued) LeaveCriticalSection(&this->cond);
#endif
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

#ifndef WIN32
PRIVATE void* TaskMgr_threadBody(void* p)
#else
DWORD WINAPI TaskMgr_threadBody(LPVOID lpParam)
#endif
{
  int nextTask = -1;
  Task* task = 0;
  #ifndef WIN32
  TaskMgr * this = (TaskMgr*)p;
  #else
  TaskMgr * this = (TaskMgr*)lpParam;
  #endif
  while (1)
  {
    #ifndef WIN32
    pthread_mutex_lock(&this->mutex);
    while (nextTask == -1 /*&& !tm->stop*/)
            pthread_cond_wait(&(this->isWork), &(this->mutex));
    #else
    EnterCriticalSection(&this->cond);
    #endif

    // check for work
    for (int i = 0; i < MAX_TASKS; i++)
    {
      if (Task_isReady(this->taskId[i]))
      {
        nextTask = i;
        break;
      }
    }
#ifndef WIN32
    pthread_mutex_unlock(&this->mutex);  
#else
    LeaveCriticalSection(&this->cond);
#endif
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
