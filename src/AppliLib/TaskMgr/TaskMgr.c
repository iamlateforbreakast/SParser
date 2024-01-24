/* TaskMgr.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Mutex.h"

#ifdef WIN32
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
//#include <process.h>
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

struct TaskMgr
{
  Object object;
  int maxTask;
  Mutex runMutex;
  Mutex clockMutex;
};

Class taskMgrClass = {
  .f_new = 0
};

PUBLIC TaskMgr * TaskMgr_new(int maxTask)
{
  // Mutex_create(this->runMutex, TRUE);
  // Mutex_create(clockMutex, FALSE);
  // Create N task
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

PUBLIC void TaskMgr_createTask(TaskMgr* this)
{

}

PRIVATE void Mutex_createMutex()
{
}

PRIVATE
/*

void KbdFunc(void) // Dispatch and count threads.
{
    int         KeyInfo;

        {
            ++ThreadNr;
            hThreads[ThreadNr] = 
                (HANDLE)_beginthread(BounceProc, 0, (void*)(uintptr_t)ThreadNr);
 

    ShutDown();
}

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


