/* UT_TaskMgr_01.c */
#include "Task.h"
#include "TaskMgr.h"
#include "Debug.h"

#ifdef _WIN32
#include <windows.h>	/* WinAPI */

/* Windows sleep in 100ns units */
int msleep(long msec) {
  /* Declarations */
  HANDLE timer;	/* Timer handle */
  LARGE_INTEGER li;	/* Time definition */
  /* Create timer */
  if (!(timer = CreateWaitableTimer(NULL, 1, NULL)))
	return 0;
  /* Set timer properties */
  li.QuadPart = -10000LL * msec;
  if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, 0)) {
	CloseHandle(timer);
	return 0;
  }
  /* Start & wait for timer */
  WaitForSingleObject(timer, 0xFFFFFFFF);
  /* Clean resources */
  CloseHandle(timer);
  /* Slept without problems */
  return 1;
}
#else
#include <time.h>
#include <errno.h>
int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0)
  {
	errno = EINVAL;
	return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
	res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}
#endif

void* taskBody1(void* this)
{
  for (int i = 0; i < 20; ++i)
  {
    PRINT(("Here 1-%d\n",i));
    msleep(100);
  }

  return 0;
}

void* taskBody2(void* this)
{
  for (int i = 0; i < 5; ++i)
  {
	PRINT(("Here 2-%d\n", i));
	msleep(500);
  }

  return 0;
}

void* taskBody3(void* this)
{
  for (int i = 0; i < 5; ++i)
  {
	PRINT(("Here 3-%d\n", i));
	msleep(500);
  }

  return 0;
}

int step1()
{
  struct event
  {
	int t;
	Task * f;
  };

  Task* testTask1 = Task_create(&taskBody1);
  Task* testTask2 = Task_create(&taskBody2);
  Task* testTask3 = Task_create(&taskBody3);

  struct event events[] = {
	{50, testTask1}, {20, testTask2}, {10, testTask3}
  };

  int evtIdx = 0;
  int isPassed = 1;
  int clock = 0;



  //Task_start(testTask1);

  while (clock<100)
  {
	PRINT(("Main thread\n"));
	msleep(100);
	clock = clock + 1;
	while ((evtIdx<(sizeof(events)/sizeof(struct event))) && (clock<events[evtIdx].t))
	{
	  Task_start(events[evtIdx].f);
	  evtIdx++;
	}
	clock = clock + 1;
  }

  return isPassed;
}

int main()
{
  int isPassed = 1;

  step1();

  return isPassed;
}
