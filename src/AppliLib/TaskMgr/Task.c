/* Task.c */
#include "TaskMgr.h"
#include "Object.h"
#include "Task.h"
#include "Memory.h"
#include "Debug.h"

/* Max number of params for body of callback */
#define MAX_PARAMS (5)

PRIVATE TaskMgr* Task_taskMgr = 0;

/**********************************************//**
  @class TaskMgr
**************************************************/
struct Task
{
  void* (*body)(void* p);
  int nbParams;
  void * params[5];
  int isReady;
  int isRunning;
  int isCompleted;
  int execTime;
};

/********************************************************//**
  @brief Create a task object.
  @public
  @memberof Task
  @return The new instance of a task.
************************************************************/
PUBLIC Task* Task_create(void * (*body)(void* p), int nbParams, void ** params)
{
  Task* this = 0;

  if (Task_taskMgr == 0) Task_taskMgr = TaskMgr_getRef();

  this = (Task*)Memory_alloc(sizeof(Task));
  this->body = body;
  this->nbParams = nbParams;
  for (int i = 0; i < this->nbParams; ++i) this->params[i] = params[i];
  this->isReady = 0;
  this->isRunning = 0;
  this->isCompleted = 0;
  this->execTime = 0;
  
  return this;
}

/********************************************************//**
  @brief Destroy a task object.
  @public
  @memberof Task
************************************************************/
PUBLIC void Task_destroy(Task* this)
{
  Memory_free(this, sizeof(Task));
}

/********************************************************//**
  @brief Obtain the status of the ready flag.
  @public
  @memberof Task
  @return Ready flag.
************************************************************/
PUBLIC int Task_isReady(Task* this)
{
  if (this == 0) return 0;

  return this->isReady;
}

/********************************************************//**
  @brief Mark a task as ready.
  @public
  @memberof Task
************************************************************/
PUBLIC void Task_setReady(Task* this)
{
  this->isReady = 1;
  this->isRunning = 0;
  this->isCompleted = 0;
}

/********************************************************//**
  @brief Obtain the status of the running flag.
  @public
  @memberof Task
  @return Running flag.
************************************************************/
PUBLIC int Task_isRunning(Task* this)
{
  return this->isRunning;
}

/********************************************************//**
  @brief Mark a task as running.
  @public
  @memberof Task
************************************************************/
PUBLIC void Task_setRunning(Task* this)
{
  this->isReady = 0;
  this->isRunning = 1;
  this->isCompleted = 0;
}

/********************************************************//**
  @brief Obtain the status of the completion flag.
  @public
  @memberof Task
  @return Completion flag.
************************************************************/
PUBLIC int Task_isCompleted(Task* this)
{
  return this->isCompleted;
}

/********************************************************//**
  @brief Mark a task as completed
  @public
  @memberof Task
************************************************************/
PUBLIC void Task_setCompleted(Task* this)
{
  this->isReady = 0;
  this->isRunning = 0;
  this->isCompleted = 1;
}

/********************************************************//**
  @brief Start a Task.
  @public
  @memberof Task
************************************************************/
PUBLIC void Task_start(Task* this)
{
  this->isReady = 1;
  this->isRunning = 0;
  this->isCompleted = 0;
  TaskMgr_start(Task_taskMgr, this);
}

/********************************************************//**
  @brief Execute the body of the task.
  @public
  @memberof Task
  @return TBD
************************************************************/
PUBLIC void Task_executeBody(Task* this)
{

  //PRINT(("Arg: %d\n", (int)this->params[0]));
  this->body(&this->params[0]);

  Task_setCompleted(this);
}
