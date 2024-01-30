/* Task.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Task.h"
#include "Memory.h"

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#include <conio.h>
#include <process.h>
#endif

struct Task
{
  void* (*body)(void* p);
  int nbParams;
  void** params;
  int isReady;
  int isRunning;
  int isCompleted;
  int execTime;

};

PRIVATE TaskMgr* taskMgr = 0;

PUBLIC Task* Task_create(void * (*body)(void* p))
{
  Task* this = 0;

  if (taskMgr == 0) taskMgr = TaskMgr_new();

  this = (Task*)Memory_alloc(sizeof(Task));
  this->body = body;
  this->isReady = 0;
  this->isRunning = 0;
  this->isCompleted = 0;
  this->execTime = 0;

  return this;
}

PUBLIC void Task_destroy(Task* this)
{
  Memory_free(this, sizeof(Task));
}

PUBLIC int Task_isReady(Task* this)
{
  if (this == 0) return 0;

  return this->isReady;
}

PUBLIC int Task_isRunning(Task* this)
{
  return this->isRunning;
}

PUBLIC int Task_isCompleted(Task* this)
{
  return this->isCompleted;
}

PUBLIC void Task_start(Task* this)
{
  this->isReady = 1;
  this->isRunning = 0;
  this->isCompleted = 0;
  TaskMgr_start(taskMgr, this);
}

PUBLIC void Task_executeBody(Task* this)
{
  this->isRunning = 1;
  this->isCompleted = 0;
  this->isReady = 0;

  this->body(0);

  this->isCompleted = 1;
  this->isRunning = 0;
  this->isReady = 0;
}
