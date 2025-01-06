#include "Debug.h"
#include <stdio.h>

extern int run_UT_BTree_01();
extern int run_UT_Debug_01();
extern int run_UT_Error_01();
extern int run_UT_FileIo_01();
extern int run_UT_List_01();
extern int run_UT_List_02();
extern int run_UT_Map_01();
extern int run_UT_SkipList_01();
extern int run_UT_SkipList_02();
extern int run_UT_String_01();
extern int run_UT_TimeMgr_01();

enum TestRunStatus
{
  DISABLED = 0,
  ENABLED = 1
};

typedef struct TestDefinition TestDefinition;

struct TestDefinition
{
  char *  name;
  int (*run)();
  enum TestRunStatus isEnabled;
};

TestDefinition parseTests[] =
{
  { "UT_BTree_01",    &run_UT_BTree_01,    DISABLED }
};

TestDefinition appliTests[] = 
{
  { "UT_TimeMgr_01",    &run_UT_TimeMgr_01,    DISABLED }
};

TestDefinition commonTests[] =
{
  { "UT_BTree_01",    &run_UT_BTree_01,    DISABLED },
  { "UT_Debug_01",    &run_UT_Debug_01,    DISABLED },
  { "UT_Error_01",    &run_UT_Error_01,    DISABLED },
  { "UT_FileIo_01",   &run_UT_FileIo_01,   DISABLED },
  { "UT_List_01",     &run_UT_List_01,     DISABLED },
  { "UT_List_02",     &run_UT_List_02,     DISABLED },
  { "UT_Map_01",      &run_UT_Map_01,      DISABLED },
  { "UT_SkipList_01", &run_UT_SkipList_01, DISABLED },
  { "UT_SkipList_02", &run_UT_SkipList_02, DISABLED },
  { "UT_String_01",   &run_UT_String_01,   ENABLED }
};

int runTests(TestDefinition* tests, int nbTests)
{
  int isPassed = 1;

  PRINT2((stdout, "Nb tests %d \n", nbTests));

  for (int i = 0; i < nbTests; i++)
  { 
    if (tests[i].isEnabled == ENABLED)
    {
      int result = tests[i].run();
      isPassed = isPassed && result;

      if (result)
        PRINT2((stdout, "%-30s:  PASS\n", tests[i].name));
      else
        PRINT2((stdout, "%-30s:  FAILED\n", tests[i].name));
    }
    else
    {
      PRINT2((stdout, "%-30s:  DISABLED\n", tests[i].name));
    }
  }

  return isPassed;
}

int main()
{
  int isPassed = 1;

  Debug_setStdoutChannel(stdout);
  
  PRINT2((stdout, "--- CommonLib tests ---\n"));
  isPassed = runTests(commonTests, sizeof(commonTests) / sizeof(TestDefinition)) && isPassed;

  PRINT2((stdout, "\n--- AppliLib tests ---\n"));
  isPassed = runTests(appliTests, sizeof(appliTests) / sizeof(TestDefinition)) && isPassed;

  PRINT2((stdout, "\n--- ParseLib tests ---\n"));
  isPassed = runTests(parseTests, sizeof(parseTests) / sizeof(TestDefinition)) && isPassed;

  return isPassed;
}
