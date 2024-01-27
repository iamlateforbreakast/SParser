/* UT_TaskMgr_01.c */
#include "Task.h"
#include "TaskMgr.h"
#include "Debug.h"

void * taskBody(void * this)
{
  PRINT(("Here\n"));
  
  return 0;
}

int step1()
{
  int isPassed = 1;
  Task * testTask = Task_create(&taskBody);
  
  TaskMgr * testTaskMgr = 0;

  testTaskMgr = TaskMgr_new();

  TaskMgr_start(testTaskMgr, testTask);

  //TaskMgr_stop(testTaskMgr);

  TaskMgr_delete(testTaskMgr);

  return isPassed;
}

int main()
{
  int isPassed = 1;

  step1();

  return isPassed;
}
