/* TaskMgr.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Mutex.h"

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

typedef struct Job{
  fct;
  **arg
} Job;

struct TaskMgr
{
  Object object;
  int maxTask;
  int taskId[2];
  int nbJobs;
  Mutex runMutex;
  Mutex clockMutex;
};

Class taskMgrClass = {
  .f_new = 0,
  .f_delete = (Destructor)&TaskMgr_delete
};

PUBLIC TaskMgr * TaskMgr_new(int maxTask)
{
  // Mutex_create(this->runMutex, TRUE);
  // Mutex_create(this->clockMutex, FALSE);
  // 
}

// start
// stop

PUBLIC void TaskMgr_delete(TaskMgr * this)
{
  // Mutex_release(this->runMutex);
  // 
  //     // Tell all threads to die
  ReleaseMutex(hRunMutex);

  while (ThreadNr > 0)
  {
    // Wait for each thread to complete
    WaitForSingleObject(hThreads[--ThreadNr], INFINITE);
  }

  // Clean up display when done
  WaitForSingleObject(hScreenMutex, INFINITE);
  ClearScreen();

  // All threads done. Clean up handles.
  //if (hScreenMutex) CloseHandle(hScreenMutex);
  //if (hRunMutex) CloseHandle(hRunMutex);
}

PUBLIC int TaskMgr_createTask(TaskMgr* this, funct, ** args)
{
#ifndef WIN32
err = pthread_create(&(tid[i]), NULL, &TaskMgr_executeTaskBody, NULL);
#else
hThreads[ThreadNr] = 
                (HANDLE)_beginthread(TaskMgr_executeTaskBody, 0, (void*)(uintptr_t)ThreadNr);
#endif
}

PRIVATE void TaskMgr_executeTaskBody()
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


