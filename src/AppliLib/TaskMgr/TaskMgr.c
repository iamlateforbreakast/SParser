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

#define MAX_TASKS (10)
#define MAX_THREADS (2)

#define DEBUG (0)

PRIVATE TaskMgr* taskMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE void TaskMgr_waitForThread(TaskMgr* this);
PRIVATE int TaskMgr_findAvailableTask(TaskMgr* this);
PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this);
PRIVATE int TaskMgr_initLock(TaskMgr* this);
PRIVATE int TaskMgr_destroyLock(TaskMgr* this);
PRIVATE int TaskMgr_lock(TaskMgr* this);
PRIVATE int TaskMgr_unlock(TaskMgr* this);
PRIVATE int TaskMgr_initSemaphores(TaskMgr* this);
PRIVATE int TaskMgr_destroySemaphores(TaskMgr* this);
PRIVATE int TaskMgr_waitNotFull(TaskMgr* this);
PRIVATE int TaskMgr_waitNotEmpty(TaskMgr* this);
PRIVATE int TaskMgr_signalNotEmpty(TaskMgr* this);
PRIVATE int TaskMgr_signalNotFull(TaskMgr* this);
PRIVATE void* TaskMgr_threadBody(void* p);
#ifdef WIN32
PRIVATE VOID WINAPI TaskMgr_threadWinBody(LPVOID Parameter);
#endif
/**********************************************//**
  @class TaskMgr
**************************************************/
struct TaskMgr
{
#ifndef WIN32
  Object object;
  int nbMaxThreads;
  int nbActiveThreads;
  Task* taskId[MAX_TASKS];
  pthread_t threadHandle[MAX_THREADS];
  //pthread_cond_t isWork;
  sem_t semEmpty;
  sem_t semFull;
  pthread_mutex_t mutex;
  volatile int isStopping;
#else
  Object object;
  int nbMaxThreads;
  int nbActiveThreads;
  Task* taskId[MAX_TASKS];
  int isWorkAvailable;
  HANDLE threadHandle[MAX_THREADS];
  DWORD threadId[MAX_THREADS];
  HANDLE semEmpty;
  HANDLE semFull;
  HANDLE mutex;
  volatile int isStopping;
  /*
  CRITICAL_SECTION cond;
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
PRIVATE TaskMgr* TaskMgr_new(int nbMaxThreads)
{
  if ((nbMaxThreads > MAX_THREADS) || (nbMaxThreads < 1)) return 0;

  TaskMgr* this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

  if (OBJECT_IS_INVALID(this)) return 0;

  this->isStopping = 0;
  this->nbMaxThreads = nbMaxThreads;
  this->nbActiveThreads = 0;

  for (int i = 0; i < MAX_TASKS; ++i) this->taskId[i] = 0;

  // Create Empty and full semaphores
  TaskMgr_initSemaphores(this);
  // Create Mutex protecting work queue
  TaskMgr_initLock(this);

  // Create all threads
  TaskMgr_createWorkerThreads(this);

  return this;
}

/**********************************************//**
  @brief Get reference to singleton TaskMgr.
    NOT THREAD SAFE
  @memberof TaskMgr
  @param[in] TBD
  @return Reference to the TaskMgr.
**************************************************/
PUBLIC TaskMgr* TaskMgr_getRef()
{
  if (!taskMgr) taskMgr = TaskMgr_new(MAX_THREADS);

  return taskMgr;
}

/**********************************************//**
  @brief Delete the TaskMgr singleton
  @memberof TaskMgr
  @return none
**************************************************/
PUBLIC void TaskMgr_delete(TaskMgr* this)
{
  TaskMgr_destroySemaphores(this);
  TaskMgr_destroyLock(this);
  Object_deallocate(&this->object);
  taskMgr = 0;
}

/**********************************************//**
  @brief Queue a task for execution.
  @memberof TaskMgr
  @param[in] task
  @return 1 if successful.
**************************************************/
PUBLIC int TaskMgr_start(TaskMgr* this, Task* task)
{
  int isQueued = 0;

  if (TaskMgr_waitNotFull(this))
  {
    TaskMgr_lock(this);

    /* Place task in buffer */
    for (int i = 0; i < MAX_TASKS; i++)
    {
      if (this->taskId[i] == 0)
      {
        this->taskId[i] = task;
        isQueued = 1;
        TaskMgr_signalNotEmpty(this);
        break;
      }
    }

    TaskMgr_unlock(this);
  }

  return isQueued;
}

/**********************************************//**
  @brief Request all worker threads to stop.
  @memberof TaskMgr
  @return 1 if successful.
**************************************************/
PUBLIC void TaskMgr_stop(TaskMgr* this)
{
  this->isStopping = 1;
  for (int i = 0; i < this->nbMaxThreads; i++) {
    TaskMgr_signalNotEmpty(this);
  }
  TaskMgr_waitForThread(this);
}

/**********************************************//**
  @brief Print the content of the TaskMgr.
  @public
  @memberof TaskMgr
**************************************************/
PUBLIC void TaskMgr_print(TaskMgr* this)
{

}

/**********************************************//**
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PUBLIC unsigned int TaskMgr_getSize(TaskMgr* this)
{
  if (this == 0) return sizeof(TaskMgr);

  return sizeof(*this);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PRIVATE void* TaskMgr_threadBody(void* p)
{
  TaskMgr* this = (TaskMgr*)p;

  while (1)
  {
    if (TaskMgr_waitNotEmpty(this))
    {
      if (this->isStopping)
      {
        TaskMgr_lock(this);
        this->nbActiveThreads--;
        TaskMgr_unlock(this);

        return 0; // Terminate the worker thread
      }
      Task* taskToExecute = 0;

      TaskMgr_lock(this);

      int nextTask = TaskMgr_findAvailableTask(this);

      if (nextTask >=0)
      { 
        taskToExecute = this->taskId[nextTask];
        this->taskId[nextTask] = 0;
      }

      TaskMgr_unlock(this);

      if (taskToExecute != 0)
      {
        Task_executeBody(taskToExecute);
      }
      TaskMgr_signalNotFull(this);
    }
  }
}

#ifdef WIN32
PRIVATE VOID WINAPI TaskMgr_threadWinBody(LPVOID Parameter)
{
  TaskMgr_threadBody((void*)Parameter);
}
#endif

/**********************************************//**
  @brief TBD
  @memberof TaskMgr
  @param[in] TBD
  @return TBD
**************************************************/
PRIVATE void TaskMgr_waitForThread(TaskMgr* this)
{
#ifndef WIN32
  for (int i = 0; i < this->nbMaxThreads; ++i)
  {
    pthread_join(this->threadHandle[i], NULL);
  }
#else
  WaitForMultipleObjects(this->nbMaxThreads, this->threadHandle, TRUE, INFINITE);
#endif
  
}

/**********************************************//**
  @brief Create all worker threads.
  @memberof TaskMgr
  @return TBD
**************************************************/
PRIVATE int TaskMgr_createWorkerThreads(TaskMgr* this)
{
  for (int i = 0; i < this->nbMaxThreads; ++i)
  {
#ifndef WIN32
    int err = pthread_create(&(this->threadHandle[i]), NULL, &TaskMgr_threadBody, this);
    //pthread_detach(this->threadHandle[i]);
    if (err == 0)
    {
      this->nbActiveThreads++;
    }
    else
    {
      PRINT(("CreateThread error.\n"));
      return 0;
    }
#else
    this->threadHandle[i] = CreateThread(
      NULL,                // default security attributes
      0,                   // default stack size
      (LPTHREAD_START_ROUTINE)TaskMgr_threadWinBody,
      (LPVOID)this,        // taskMgr is argument
      0,                   // default creation flags
      &this->threadId[i]); // receive thread identifier

    if (this->threadHandle[i] != 0)
    {
      this->nbActiveThreads++;
    }
    else
    {
      PRINT(("CreateThread error: %d\n", GetLastError()));
      return 0;
    }
#endif
  }
  return (this->nbActiveThreads == this->nbMaxThreads);
}

/**********************************************//**
  @brief TBD
  @memberof TaskMgr
  @return TBD
**************************************************/
PRIVATE int TaskMgr_findAvailableTask(TaskMgr* this)
{
  int nextTask = -1;
  for (int i = 0; i < MAX_TASKS; i++)
  {
    if ((this->taskId[i]!=0) && (Task_isReady(this->taskId[i])))
    {
      nextTask = i;
      break;
    }
  }

  return nextTask;
}

/**********************************************//**
  @brief Initialise empty and full semaphores.
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_initSemaphores(TaskMgr* this)
{
  int isSuccessful = 1;

#ifndef WIN32
  // Create nb task empty semaphore
  if (sem_init(&this->semEmpty, 0, MAX_TASKS)!=0)
  {
    PRINT(("CreateSemaphore Empty error.\n"));
    isSuccessful = 0;
  }
  // Create nb task full semaphore
  if (sem_init(&this->semFull, 0, 0)!=0)
  {
    PRINT(("CreateSemaphore Full error.\n"));
    isSuccessful = 0;
  }
#else
  // Create nb task empty semaphore
  this->semEmpty = CreateSemaphore(NULL, MAX_TASKS, MAX_TASKS, NULL);

  if (this->semEmpty == NULL)
  {
    PRINT(("CreateSemaphore Empty error: %d\n", GetLastError()));
    isSuccessful = 0;
  }
  // Create nb task full semaphore
  this->semFull = CreateSemaphore(NULL, 0, MAX_TASKS, NULL);

  if (this->semFull == NULL)
  {
    PRINT(("CreateSemaphore Full error: %d\n", GetLastError()));
    isSuccessful = 0;
  }
#endif

  return isSuccessful;
}

/**********************************************//**
  @brief Destroy empty and full semaphores.
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_destroySemaphores(TaskMgr* this)
{
  int isSuccessful = 1;

#ifndef WIN32
  if (sem_destroy(&this->semEmpty)!= 0) isSuccessful = 0;
  if (sem_destroy(&this->semFull)!= 0) isSuccessful = 0;;
#else
  if (this->semEmpty != NULL)
  {
    if (!CloseHandle(this->semEmpty)) isSuccessful = 0;
    this->semEmpty = NULL;
  }
  if (this->semFull != NULL)
  {
    if (!CloseHandle(this->semFull)) isSuccessful = 0;
    this->semFull = NULL;
  }
#endif

  return isSuccessful;
}

/**********************************************//**
  @brief Wait until there is a space to add a task.
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_waitNotFull(TaskMgr* this)
{
  int isSuccessful = 0;

#ifndef WIN32
  isSuccessful = !sem_wait(&this->semEmpty);
#else
  DWORD dwWaitResult = WaitForSingleObject(this->semEmpty, INFINITE);
  isSuccessful = (dwWaitResult == WAIT_OBJECT_0);
#endif

  return isSuccessful;
}

/**********************************************//**
  @brief Wait until there is a task in the queue.
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_waitNotEmpty(TaskMgr* this)
{
  int isSuccessful = 0;

#ifndef WIN32
  isSuccessful = !sem_wait(&this->semFull);
#else
  DWORD dwWaitResult = WaitForSingleObject(this->semFull, INFINITE);
  isSuccessful = (dwWaitResult == WAIT_OBJECT_0);
#endif

  return isSuccessful;
}

/**********************************************//**
  @brief Signal that task can be added to the queue.
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_signalNotFull(TaskMgr* this)
{
#ifndef WIN32
  if (!sem_post(&this->semEmpty)) return 1; else return 0;
#else
  if (!ReleaseSemaphore(this->semEmpty, 1, NULL)) return 0; else return 1;
#endif
}

/**********************************************//**
  @brief Signal that there are task to process.
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_signalNotEmpty(TaskMgr* this)
{
#ifndef WIN32
  if (sem_post(&this->semFull)) return 0;
#else
  if (!ReleaseSemaphore(this->semFull, 1, NULL)) return 0;
#endif
  return 1;
}

/**********************************************//**
  @brief Create taskMgr lock.
  @private
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_initLock(TaskMgr* this)
{
#ifndef WIN32
  if (pthread_mutex_init(&this->mutex, NULL) != 0)
  {
    PRINT(("TaskMgr createMutex error.\n"));
    return 0; // Unsuccessful
  }
  return 1;
#else
  this->mutex = CreateMutex(
    NULL,              // default security attributes
    FALSE,             // initially not owned
    NULL);             // unnamed mutex

  if (this->mutex == NULL)
  {
    PRINT(("TaskMgr createMutex error: %d\n", GetLastError()));
    return 0; // Unsuccessful
  }
  return 1;
#endif
}

/**********************************************//**
  @brief Destroy tasMgr lock.
  @private
  @memberof TaskMgr
  @return 1 indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_destroyLock(TaskMgr* this)
{
#ifndef WIN32
  if (pthread_mutex_destroy(&this->mutex) == 0) return 1; else return 0;
#else
  if (CloseHandle(this->mutex) == 0) return 0; else return 1;
#endif
}

/**********************************************//**
  @brief Lock exclusive use of TaskMgr data.
  @private
  @memberof TaskMgr
  @return Indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_lock(TaskMgr* this)
{
  int isSuccessful = 0;

#ifndef WIN32
  pthread_mutex_lock(&this->mutex);
  isSuccessful = 1;
#else
  DWORD dwWaitResult = WaitForSingleObject(this->mutex, INFINITE);  // no time-out interval
  isSuccessful = (dwWaitResult == WAIT_OBJECT_0);
#endif

  return isSuccessful;
}

/**********************************************//**
  @brief Unlock exclusive use of TaskMgr data.
  @private
  @memberof TaskMgr
  @return Indicates if operation was successful.
**************************************************/
PRIVATE int TaskMgr_unlock(TaskMgr* this)
{
  int isSuccessful = 0;

#ifndef WIN32
  if (!pthread_mutex_unlock(&this->mutex)) isSuccessful = 1;
#else
  isSuccessful = ReleaseMutex(this->mutex);
#endif

  return isSuccessful;
}
