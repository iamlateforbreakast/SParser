#include "TimeMgr.h"
#include "Object.h"
#include "Map.h"
#include "Memory.h"

#include <unistd.h>
#include <stdio.h>

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
                          
typedef struct TestTimeMgr
{
  Object object;
  Map * timers;
} TestTimeMgr;

int step1()
{
  TimeMgr * testTimeMgr1 = 0;
  TimeMgr * testTimeMgr2 = 0;

  testTimeMgr1 = TimeMgr_getRef();
  testTimeMgr2 = TimeMgr_getRef();

  /* Test 1 */
  printf("Step 1: Test 1 - Check there is only one TimeMgr: ");

  UT_ASSERT((testTimeMgr1==testTimeMgr2))

  TimeMgr_delete(testTimeMgr1);
  TimeMgr_delete(testTimeMgr2);

  Memory_report();

  return 0;
}

int step2()
{
  TimeMgr * testTimeMgr = 0;
  String * testTimerName = 0;

  testTimerName = String_new("timer1");

  testTimeMgr = TimeMgr_getRef();

  for (int i=0; i<10; i++)
  {
    TimeMgr_latchTime(testTimeMgr, testTimerName);

    sleep(1);
  }
  TimeMgr_latchTime(testTimeMgr, testTimerName);  

  TimeMgr_report(testTimeMgr);

  TimeMgr_delete(testTimeMgr);

  String_delete(testTimerName);

  Memory_report();

  return 0;
}

int main()
{
  step1();
  step2();

  return 0;
}
