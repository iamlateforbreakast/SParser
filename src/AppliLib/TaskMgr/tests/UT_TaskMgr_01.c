/* UT_TaskMgr_01.c */

#include "TaskMgr.h"

int step1()
{
  int isPassed = 1;
  
  TaskMgr * testTaskMgr = 0;

  testTaskMgr = TaskMgr_new();

  TaskMgr_start(testTaskMgr, 0);

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
