/* UT_TaskMgr_01.c */
#include "Task.h"
#include "TaskMgr.h"
#include "Debug.h"
#include <pthread.h>

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

pthread_t thrId;
pthread_cond_t cond;
pthread_mutex_t mutex;
int somethingToBeDone = 0;

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
	{10, testTask1}, {20, testTask2}, {30, testTask3}
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

void * thread(void*)
{
  PRINT(("Thread created\n"));
  pthread_mutex_lock(&mutex);
  PRINT(("Thread takes mutex\n"));
  while (!somethingToBeDone)
    pthread_cond_wait(&cond, &mutex);
  PRINT(("Doing stuff\n"));
  somethingToBeDone = 0;
  pthread_mutex_unlock(&mutex);
}

int step2()
{
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&cond,0);

  pthread_mutex_lock(&mutex);
  PRINT(("Main thread takes mutex\n"));
  int err = pthread_create(&thrId, NULL, &thread, 0);
  pthread_detach(thrId);

  somethingToBeDone = 1;
  pthread_cond_broadcast(&cond);
  PRINT(("Main thread broaadcast\n"));
  pthread_mutex_unlock(&mutex);
  PRINT(("Main thread release mutex\n"));
  msleep(1000);

  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
}

int main()
{
  int isPassed = 1;

  step1();

  return isPassed;
}
