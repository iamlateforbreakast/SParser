#include "OptionMgr.h"
#include "String2.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include <stdio.h>

OptionMgr * testOptionMgr = 0;

int step1()
{
  testOptionMgr = OptionMgr_getRef();

  return 0;
}

int step2()
{
  String * option = 0;

  option = OptionMgr_getOption(testOptionMgr,"DB Name");
  printf("DB Name option = %s\n", String_getBuffer(option));

  option = OptionMgr_getOption(testOptionMgr, "Input directory");
  printf("Input option = %s\n", String_getBuffer(option));

  return 0;
}

int step3()
{
  String * option = String_new("Yes");

  OptionMgr_setOption(testOptionMgr, "Reset after initialisation", option);

  option = OptionMgr_getOption(testOptionMgr, "Reset after initialisation");
  printf("Reset after init. option = %s\n", String_getBuffer(option));

  return 0;
}

int step4()
{
  const char *argv[] = { "-o", "test2.db" };
  int argc = 2;
  String * option = 0;

  OptionMgr_readFromCmdLine(testOptionMgr, argc, argv);

  option = OptionMgr_getOption(testOptionMgr,"DB Name");
  printf("DB Name option = %s\n", String_getBuffer(option));

  return 0;
}

int step5()
{
  String * option = 0;

  OptionMgr_readFromFile(testOptionMgr);
  option = OptionMgr_getOption(testOptionMgr,"DB Name");
  printf("DB Name option = %s\n", String_getBuffer(option));

  return 0;
}

int step6()
{
  OptionMgr_delete(testOptionMgr);

  Memory_report();

  return 0;
}

int main()
{
  step1();
  step2();
  step3();
  step4();
  step5();
  step6();

  return 0;
}
