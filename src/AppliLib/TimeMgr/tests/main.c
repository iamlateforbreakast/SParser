  Map * timers;
} TestTimeMgr;

int step1()
{
  TimeMgr * testTimeMgr = 0;

  testTimeMgr = TimeMgr_getRef();

  TimeMgr_delete(testTimeMgr);

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

  //String_delete(testTimerName);

  Memory_report();

  return 0;
}

int main()
{
  step1();
  step2();

  return 0;
}
