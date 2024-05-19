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
#include <semaphore.h>
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
PRIVATE VOID WINAPI TaskMgr_threadWinBody(LPVOID Parameter);
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
  sem_t semEmpty;
  sem_t semFull;
#else
  Object object;
  int nbThreads;
  Task* taskId[MAX_TASKS];
  int isWorkAvailable;
  HANDLE threadHandle[MAX_THREADS];
  DWORD threadId[MAX_THREADS];
  HANDLE semEmpty;
  HANDLE semFull;
  HANDLE mutex;
  int isStopping;
  //DWORD   dwThreadIdArray[MAX_THREADS];
  /*
  CRITICAL_SECTION cond;
  TP_CALLBACK_ENVIRON CallBackEnviron;
  PTP_POOL pool;
  PTP_CLEANUP_GROUP cleanupgroup;
  PTP_WORK_CALLBACK workcallback;
  */
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
  if (this == 0) return 0;

  this->isStopping = 0;
  for (int i = 0; i < MAX_TASKS; ++i) this->taskId[i] = 0;

  // Create nb task empty semaphore
  this->semEmpty = CreateSemaphore(
    NULL,           // default security attributes
    MAX_TASKS,      // all slots are empty
    MAX_TASKS,      // maximum count
    NULL);          // unnamed semaphore
  if (this->semEmpty == NULL)
  {
    printf("CreateSemaphore Empty error: %d\n", GetLastError());
    return 0;
  }

  // Create nb task full semaphore
  this->semFull = CreateSemaphore(
    NULL,           // default security attributes
    0,              // no slot is full
    MAX_TASKS,      // maximum count
    NULL);          // unnamed semaphore
  if (this->semFull == NULL)
  {
    printf("CreateSemaphore Full error: %d\n", GetLastError());
    CloseHandle(this->semEmpty);
    return 0;
  }

  // Create Mutex protecting work queue
  this->mutex = CreateMutex(
    NULL,              // default security attributes
    FALSE,             // initially not owned
    NULL);             // unnamed mutex
  if (this->mutex == NULL)
  {
    printf("CreateMutex error: %d\n", GetLastError());
    return 0;
  }

  // Create all threads
  for (int i = 0; i < MAX_THREADS; i++)
  {
    this->threadHandle[i] = CreateThread(
      NULL,                // default security attributes
      0,                   // default stack size
      (LPTHREAD_START_ROUTINE)TaskMgr_threadWinBody,
      (LPVOID)this,                // no thread function arguments
      0,                   // default creation flags
      &this->threadId[i]); // receive thread identifier
    if (this->threadHandle[i] == NULL)
    {
      printf("CreateThread error: %d\n", GetLastError());
      return 0;
    }
  }
  return this;
}
#else
{
  TaskMgr* this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

  // sem_init(&this->semFull, 0, MAX_TASKS);
  // sem_init(&this->semEmpty, MAX_TASKS, MAX_TASKS);
  if (pthread_mutex_init(&this->mutex, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 0; 
    }
  pthread_cond_init(&this->isWork, 0);
  pthread_mutex_lock(&this->mutex);

  this->nbThreads = MAX_THREADS;
  this->isWorkAvailable = 0;

  for (int i = 0; i < MAX_TASKS; ++i) this->taskId[i] = 0;

  TaskMgr_createWorkerThreads(this);

  pthread_mutex_unlock(&this->mutex);


  return this;
}
#endif

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PUBLIC TaskMgr * TaskMgr_getRef()
{
  if (!taskMgr) taskMgr = TaskMgr_new();
  return taskMgr;
}

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PUBLIC void TaskMgr_delete(TaskMgr* this)
{
#ifndef WIN32
  sem_destroy(&this->semEmpty);
  sem_destroy(&this->semFull);
  pthread_mutex_destroy(&this->mutex);
  pthread_cond_destroy(&this->isWork);
#endif
  Object_deallocate(&this->object);
}

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PUBLIC int TaskMgr_start(TaskMgr * this, Task * task)
{
  int isQueued = 0;
#ifdef WIN32
  /* PTP_WORK work = NULL;
  work = CreateThreadpoolWork(this->workcallback, (PVOID)task, &this->CallBackEnviron);
  SubmitThreadpoolWork(work);*/
  DWORD dwWaitResult;
  // wait(this->semEmpty);
  dwWaitResult = WaitForSingleObject(
    this->semEmpty,   // handle to semaphore
    0L);
  if (dwWaitResult == WAIT_OBJECT_0)
  {
    //wait(this->mutex);
    dwWaitResult = WaitForSingleObject(
      this->mutex,    // handle to mutex
      INFINITE);  // no time-out interval
    if (dwWaitResult == WAIT_OBJECT_0)
    {
      /* Place task in buffer */
      for (int i = 0; i < MAX_TASKS; i++)
      {
        if (this->taskId[i] == 0)
        {
          this->taskId[i] = task;
          isQueued = 1;
          break;
        }
      }
      // signal(this->mutex);
      ReleaseMutex(this->mutex);
      //signal(this->semFull);
    }
    if (!ReleaseSemaphore(this->semFull, 1, NULL))
    {
      printf("ReleaseSemaphore error: %d\n", GetLastError());
    }
  }
  return isQueued;
#else
  // sem_wait();
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
  //sem_post
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


/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PUBLIC void TaskMgr_print(TaskMgr * this)
{

}

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PUBLIC unsigned int TaskMgr_getSize(TaskMgr * this)
{
  if (this == 0) return sizeof(TaskMgr);

  return sizeof(this);
}

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
#ifndef WIN32
PRIVATE void* TaskMgr_threadBody(void* p)
{
  TaskMgr * this = (TaskMgr*)p;
  int nextTask;
  //TaskMgr* this = (TaskMgr*)Parameter;

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
PRIVATE VOID WINAPI TaskMgr_threadWinBody(LPVOID Parameter)
{
  TaskMgr* this = (TaskMgr*)Parameter;
  DWORD dwWaitResult;
  while (1)
  {
    dwWaitResult = WaitForSingleObject(
        this->semFull,   // handle to semaphore
        INFINITE);       // zero-second time-out interval
    if (dwWaitResult == WAIT_OBJECT_0)
    {
      dwWaitResult = WaitForSingleObject(this->mutex, 0L); // no time-out interval
      if (dwWaitResult == WAIT_OBJECT_0)
      {   
        for (int i = 0; i < MAX_TASKS; i++)
        {
          if (Task_isReady(this->taskId[i]))
          {
            Task_setRunning(this->taskId[i]);
            ReleaseMutex(this->mutex);
            Task_executeBody(this->taskId[i]);
            Task_setCompleted(this->taskId[i]);
            break;
          }
        }
      }
      ReleaseSemaphore(
        this->semEmpty,  // handle to semaphore
        1,               // increase count by one
        NULL);
    }
    else if (dwWaitResult == WAIT_FAILED)
    {
      //printf("Wait failed %d\n", GetLastError());
    }
  }
}
#endif

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PRIVATE void TaskMgr_waitForThread(TaskMgr * this)
{
  for (int i=0; i<this->nbThreads; ++i)
  {
#ifndef WIN32
    pthread_join(this->threadHandle[i], NULL);
#endif
  } 
}

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this)
{
  for (int i = 0; i < this->nbThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
    pthread_detach(this->threadHandle[i]);
#endif
  }
  return 0;
}

/**********************************************//** 
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
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
