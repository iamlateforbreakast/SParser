/* TaskMgr.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Mutex.h"
#include "Task.h"
#include "Memory.h"
#include "Debug.h"

#include <stdlib.h>
#include <stdio.h>

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#include <conio.h>
#include <process.h>
#endif

#define MAX_TASKS (5)
#define MAX_THREADS (2)

#define DEBUG (0)

PRIVATE TaskMgr * taskMgr = 0;

/**********************************************//**
  @private
**************************************************/
#ifndef WIN32
PRIVATE void* TaskMgr_threadBody(void* this);
#else
PRIVATE VOID CALLBACK TaskMgr_threadWinBody(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work);
#endif
PRIVATE void TaskMgr_waitForThread(TaskMgr* this);
PRIVATE int TaskMgr_isWorkAvailable(TaskMgr * this);
PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this);

/**********************************************//**
  @class TaskMgr
**************************************************/
struct TaskMgr
{
#ifndef WIN32
  Object object;
  int nbThreads;
  Task * taskId[MAX_TASKS];
  int isWorkAvailable;
  pthread_t threadHandle[MAX_THREADS];
  pthread_mutex_t mutex;
  pthread_cond_t isWork;
#else
  Object object;
  int nbThreads;
  Task* taskId[MAX_TASKS];
  int isWorkAvailable;
  //HANDLE threadHandle[MAX_THREADS];
  //DWORD   dwThreadIdArray[MAX_THREADS];
  CRITICAL_SECTION cond;
  TP_CALLBACK_ENVIRON CallBackEnviron;
  PTP_POOL pool;
  PTP_CLEANUP_GROUP cleanupgroup;
  PTP_WORK_CALLBACK workcallback;
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
PRIVATE TaskMgr* TaskMgr_new()
#ifdef WIN32
{
  TaskMgr * this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);
  // Check this != 0
  InitializeThreadpoolEnvironment(&this->CallBackEnviron);
  this->pool = CreateThreadpool(NULL);
  // Need to check pool != NULL
  SetThreadpoolThreadMaximum(this->pool, 1);
  SetThreadpoolThreadMinimum(this->pool, MAX_THREADS);
  this->cleanupgroup = CreateThreadpoolCleanupGroup();
  SetThreadpoolCallbackPool(&this->CallBackEnviron, this->pool);
  SetThreadpoolCallbackCleanupGroup(&this->CallBackEnviron, this->cleanupgroup, NULL);
  this->workcallback = TaskMgr_threadWinBody;

  return this;
}
#else
{
  TaskMgr* this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

  if (pthread_mutex_init(&this->mutex, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 0; 
    }
  pthread_cond_init(&this->isWork, 0);
  pthread_mutex_lock(&this->mutex);
  //InitializeCriticalSection(&this->cond);
  //EnterCriticalSection(&this->cond);


  this->nbThreads = MAX_THREADS;
  this->isWorkAvailable = 0;

  for (int i = 0; i < MAX_TASKS; ++i) this->taskId[i] = 0;

  TaskMgr_createWorkerThreads(this);

  pthread_mutex_unlock(&this->mutex);


  return this;
}
#endif

PUBLIC TaskMgr * TaskMgr_getRef()
{
  if (!taskMgr) taskMgr = TaskMgr_new();
  return taskMgr;
}

PUBLIC void TaskMgr_delete(TaskMgr* this)
{
#ifndef WIN32
  pthread_mutex_destroy(&this->mutex);
  pthread_cond_destroy(&this->isWork);
#else
#endif
  Object_deallocate(&this->object);
}


PUBLIC int TaskMgr_start(TaskMgr * this, Task * task)
{
  int isQueued = 0;
#ifdef WIN32
  PTP_WORK work = NULL;
  work = CreateThreadpoolWork(this->workcallback, (PVOID)task, &this->CallBackEnviron);
  SubmitThreadpoolWork(work);
#else
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
  if (isQueued) 
  {
    this->isWorkAvailable = 1;
    pthread_cond_broadcast(&this->isWork);
    TRACE(("Broadcast\n"));
    pthread_mutex_unlock(&this->mutex);
    TRACE(("Main thread releases mutex\n"));
  }
#endif

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
{
  TaskMgr * this = (TaskMgr*)p;
  int nextTask = 0;

  TRACE(("Starting thread\n"));
  while (1)
  {
  //wait run mutex
  //pthread_mutex_lock(&lock);

  // wait clock mutex
    pthread_mutex_lock(&this->mutex);
    TRACE(("Worker took mutex\n"));
    while (!this->isWorkAvailable)
      pthread_cond_wait(&(this->isWork), &(this->mutex));

  
    TRACE(("Waking up\n"));
    // check for work
    for (int i = 0; i < MAX_TASKS; i++)
    {
      if (Task_isReady(this->taskId[i]))
      {
        nextTask = i;
        break;
      }
    }
    pthread_mutex_unlock(&this->mutex);  

    TRACE(("Next task %d\n", nextTask));
    if (nextTask == -1)
    {
      // No work to do

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
//return 0;
/*do
  {
      // Wait for display to be available, then lock it.
      WaitForSingleObject(hScreenMutex, INFINITE);

      ReleaseMutex(hScreenMutex);
  }
  // Repeat while RunMutex is still taken.
  while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);*/
}
#else
PRIVATE VOID CALLBACK TaskMgr_threadWinBody(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work)
{
  Task* this = (Task*)Parameter;
  Task_executeBody(this);
}
#endif

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

PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this)
{
  for (int i = 0; i < this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
    pthread_detach(this->threadHandle[i]);
#else
    //this->threadHandle[i] = CreateThread(
    //  NULL,                         // default security attributes
    //  0,                            // use default stack size  
    //  TaskMgr_threadBody,           // thread function name
    //  this,                         // argument to thread function 
    //  CREATE_SUSPENDED,             // use default creation flags 
    //  &this->dwThreadIdArray[i]);   // returns the thread identifier 
#endif
  }
  return 0;
}
PRIVATE int TaskMgr_isWorkAvailable(TaskMgr * this)
{
  int nextTask = -1;

  for (int i = 0; i < MAX_TASKS; i++)
  {
    if (Task_isReady(this->taskId[i]))
    {
      nextTask = i;
      this->isWorkAvailable = 1;
      break;
    }
  }
  if (nextTask == -1) this->isWorkAvailable = 0;

  return nextTask;
}
