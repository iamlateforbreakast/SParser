#include "SdbMgr.h"
#include "Object.h"
#include "Memory.h"

#include <stdio.h>

typedef struct TestSdbMgr
{
  Object object;
} TestSdbMgr;

int step1()
{
  SdbMgr * testSdbMgr = 0;
  String * sdbName = String_new("test.sql");

  testSdbMgr = SdbMgr_new(sdbName);

  SdbMgr_delete(testSdbMgr);

  Memory_report();

  return 0;
}

int step2()
{
  return 0;
}

int main()
{
  step1();
  step2();
  return 0;
}
