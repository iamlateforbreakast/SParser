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
DWORD WINAPI TaskMgr_threadBody(LPVOID lpParam);
#endif
PRIVATE void TaskMgr_waitForThread(TaskMgr* this);
PRIVATE int TaskMgr_isWorkAvailable(TaskMgr * this);
PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this);

/**********************************************//**
  @class TaskMgr
**************************************************/
struct TaskMgr
{
  Object object;
  int nbThreads;
  Task * taskId[MAX_TASKS];
  int isWorkAvailable;
#ifndef WIN32
  pthread_t threadHandle[MAX_THREADS];
  pthread_mutex_t mutex;
  pthread_cond_t isWork;
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
PRIVATE TaskMgr* TaskMgr_new()
{
  TaskMgr * this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

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

  this->nbThreads = MAX_THREADS;
  this->isWorkAvailable = 0;

  for (int i = 0; i < MAX_TASKS; ++i) this->taskId[i] = 0;

  TaskMgr_createWorkerThreads(this);

#ifndef WIN32
  pthread_mutex_unlock(&this->mutex);
#endif


  return this;
}

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
    this->isWorkAvailable = 1;
    pthread_cond_broadcast(&this->isWork);
    TRACE(("Broadcast\n"));
    pthread_mutex_unlock(&this->mutex);
    TRACE(("Main thread releases mutex\n"));
  }
#else
  if (isQueued) LeaveCriticalSection(&this->cond);
  for (int i = 0; i < this->nbThreads; ++i)
  {
    ResumeThread(this->threadHandle[i]);
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
  TRACE(("Starting thread\n"));
  while (1)
  {
    #ifndef WIN32
    pthread_mutex_lock(&this->mutex);
    TRACE(("Worker took mutex\n"));
    while (!this->isWorkAvailable)
      pthread_cond_wait(&(this->isWork), &(this->mutex));
    #else
    EnterCriticalSection(&this->cond);
    #endif
  
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
#ifndef WIN32
    pthread_mutex_unlock(&this->mutex);  
#else
    LeaveCriticalSection(&this->cond);
#endif
    TRACE(("Next task %d\n", nextTask));
    if (nextTask == -1)
    {
      // No work to do
#ifndef WIN32
#else
      SuspendThread(GetCurrentThread());
#endif
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

PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this)
{
  for (int i = 0; i < this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
    pthread_detach(this->threadHandle[i]);
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
  return 0;
}
PRIVATE int TaskMgr_isWorkAvailable(TaskMgr * this)
{
  int nextTask = -1;

  // check for work
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
