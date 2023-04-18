#include <time.h>
#include <errno.h>
#include "Times.h"

#define MAX_LOOP (10)

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
