
/* UT_TaskMgr_01.c */
#include "Task.h"
#include "TaskMgr.h"
#include "Memory.h"
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
#include <pthread.h>
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

//pthread_t thrId;
//pthread_cond_t cond;
//pthread_mutex_t mutex;

void* taskBody(void* params)
{
  int id = *(int*)params;
  int duration = *((int*)params+1);

  for (int i = 0; i < duration; ++i)
  {
    PRINT(("Here %d-%d\n",id,i));
    msleep(100);




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

  Task * testTasks[5];
  int params[][2] = { { 1, 50 }, { 2, 20 }, { 3, 40 }, {4, 20}, {5, 50} };

  for (int i=0;i<5;++i) testTasks[i] = Task_create(&taskBody, 2, (void **)&params[i]);

  struct event events[] = {
	{10, testTasks[0]}, {50, testTasks[1]}, {60, testTasks[2]}, {60, testTasks[3]}, {80, testTasks[4]}
  };

  int evtIdx = 0;
  int isPassed = 1;
  int clock = 0; // In 100 ms slices

  TaskMgr* taskMgr = TaskMgr_getRef();

  //Task_start(testTask1);

  while (clock<100)
  {
	PRINT(("Main thread clock: %d ms\n", clock * 100));
	msleep(100);
	while ((evtIdx<(sizeof(events)/sizeof(struct event))) && (clock==events[evtIdx].t))
	{
	  Task_start(events[evtIdx].f);
	  evtIdx++;
	}
	clock = clock + 1;
  }

  for (int i=0;i<5;++i) Task_destroy(testTasks[i]);


  TaskMgr_delete(taskMgr);

  return isPassed;
}


int main()
{
  int isPassed = 1;

  step1();

  Memory_report();

  return isPassed;
}
