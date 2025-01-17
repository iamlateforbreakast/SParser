#include "TimeMgr.h"
#include "Object.h"
#include "Map.h"
#include "Memory.h"
#include "Debug.h"
#include <time.h>
#include <stdio.h>
#include <errno.h>

#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}
                          
#ifdef _WIN32
#include <windows.h>	/* WinAPI */
static int msleep(long msec) {
  HANDLE timer;	/* Timer handle */
  LARGE_INTEGER li;	/* Time definition */
  if (!(timer = CreateWaitableTimer(NULL, 1, NULL)))
	return 0;
  li.QuadPart = -10000LL * msec;
  if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, 0)) {
	CloseHandle(timer);
	return 0;
  }
  WaitForSingleObject(timer, 0xFFFFFFFF);
  CloseHandle(timer);
  return 1;
}
#else
static int msleep(long msec)
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
typedef struct TestTimeMgr
{
  Object object;
  Map * timers;
} TestTimeMgr;

PRIVATE FILE * UT_TimeMgr_01_logChannel;

int UT_TimeMgr_01_step1()
{
  int isPassed = 1;

  TimeMgr * testTimeMgr1 = 0;
  TimeMgr * testTimeMgr2 = 0;

  testTimeMgr1 = TimeMgr_getRef();
  testTimeMgr2 = TimeMgr_getRef();

  /* Test 1 */
  PRINT(("Step 1: Test 1 - Check there is only one TimeMgr: "));

  isPassed = (testTimeMgr1==testTimeMgr2) && isPassed;

  UT_ASSERT((isPassed))

  TimeMgr_delete(testTimeMgr1);
  TimeMgr_delete(testTimeMgr2);

  Memory_report();

  return isPassed;
}

int UT_TimeMgr_01_step2()
{
  TimeMgr * testTimeMgr = 0;
  String * testTimerName = 0;

  /* Test 1 */
  PRINT(("Step 2: Test 1 - Create a timer: "));

  testTimerName = String_new("timer1");

  PRINT(("Step 2: test 2 - Latch timer 10 times: "));

  testTimeMgr = TimeMgr_getRef();

  for (int i=0; i<10; i++)
  {
    TimeMgr_latchTime(testTimeMgr, testTimerName);

    msleep(1000);
  }
  TimeMgr_latchTime(testTimeMgr, testTimerName);  

  PRINT(("Step 2: Test 3 - Report times: "));

  TimeMgr_report(testTimeMgr);

  TimeMgr_delete(testTimeMgr);

  String_delete(testTimerName);

  Memory_report();

  return 0;
}

#ifdef MAIN
int main()
#else
int run_UT_TimeMgr_01()
#endif
{
  int isPassed = 1;

  UT_TimeMgr_01_logChannel = Debug_openChannel("UT_TimeMgr_01.log");
  Debug_setStdoutChannel(UT_TimeMgr_01_logChannel);

  isPassed = UT_TimeMgr_01_step1() && isPassed;
  isPassed = UT_TimeMgr_01_step2() && isPassed;

  Debug_closeChannel(UT_TimeMgr_01_logChannel);

  return isPassed;
}
