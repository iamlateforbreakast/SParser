#include <time.h>
#include <stdio.h>
#include <errno.h>
#include "Times.h"

#define MAX_LOOP (10)

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

int step1()
{
  for (int i=0; i<MAX_LOOP; i++)
  {
    printf("Get Wall Time = %Lf\n", get_wall_time());
    msleep(500);
  }
}

int step2()
{
  for (int i=0; i<MAX_LOOP; i++)
  {
    printf("Get CPU Time = %Lf\n", get_cpu_time());
    msleep(500);
  }

}

void main()
{
  step1();
  step2();
}
