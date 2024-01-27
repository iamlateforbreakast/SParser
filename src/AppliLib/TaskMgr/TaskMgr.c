/* TaskMgr.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Mutex.h"
#include "Task.h"

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

struct TaskMgr
{
  Object object;
  int maxTask;
  int nbThreads;
  int isStarted;
  Task * taskId[MAX_TASKS];
  Mutex runMutex;
  //Mutex clockMutex;
#ifndef WIN32
  pthread_t id;
#else
  HANDLE id;
#endif
};

Class taskMgrClass = {
  .f_new = 0,
  .f_delete = (Destructor)&TaskMgr_delete,
  .f_comp = 0,
  .f_copy = 0,
  .f_print = (Printer),&TaskMgr_print,
  .f_size = (Sizer)&TaskMgr_getSize
};

PUBLIC TaskMgr * TaskMgr_new(int maxTask)
{
  TaskMgr * this = 0;
  this = (TaskMgr*)Object_new(sizeof(TaskMgr), &taskMgrClass);

  Mutex_create(this->runMutex, 1);

  // create thread();
  this->nbThreads = 1;
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
  return isStarted;
}

PUBLIC void TaskMgr_stop(TaskMgr * this)
{
  //ReleaseMutex(hRunMutex);
  Mutex_release();
  while (ThreadNr > 0)
  {
    // Wait for each thread to complete
    //WaitForSingleObject(hThreads[--ThreadNr], INFINITE);
    TaskMgr_waitForThread(this);
  }
}

PUBLIC void TaskMgr_delete(TaskMgr * this)
{
  // Mutex_release(this->runMutex);
  // 
  //     // Tell all threads to die
  

  // Clean up display when done
  //WaitForSingleObject(hScreenMutex, INFINITE);
  //ClearScreen();

  // All threads done. Clean up handles.
  //if (hScreenMutex) CloseHandle(hScreenMutex);
  //if (hRunMutex) CloseHandle(hRunMutex);
}

PUBLIC void TaskMgr_print(TaskMgr * this)
{

}

PUBLIC unsigned int TaskMgr_getSize(TaskMgr * this)
{
  if (this == 0) return sizeof(TaskMgr);

  return sizeof(this);
}

PRIVATE void TaskMgr_threadBody()
{
  //wait run mutex

  // wait clock mutex

  //wait run mutex with timeout

  //terminate
}

/*

void BounceProc(void* pMyID)
{
    do
    {
        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        ReleaseMutex(hScreenMutex);
    }
    // Repeat while RunMutex is still taken.
    while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);
}*/


