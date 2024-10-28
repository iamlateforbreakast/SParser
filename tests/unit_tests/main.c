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
extern int run_UT_String_01();

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

TestDefinition tests[] =
{
  { "UT_BTree_01",    &run_UT_BTree_01,    ENABLED },
  { "UT_Debug_01",    &run_UT_Debug_01,    ENABLED },
  { "UT_Error_01",    &run_UT_Error_01,    DISABLED },
  { "UT_FileIo_01",   &run_UT_FileIo_01,   ENABLED },
  { "UT_List_01",     &run_UT_List_01,     ENABLED },
  { "UT_List_02",     &run_UT_List_02,     DISABLED },
  { "UT_Map_01",      &run_UT_Map_01,      DISABLED },
  { "UT_SkipList_01", &run_UT_SkipList_01, DISABLED },
  { "UT_String_01",   &run_UT_String_01,   ENABLED }
};

int main()
{
  Debug_setStdoutChannel(stdout);

  int nbTests = sizeof(tests)/sizeof(TestDefinition);
  
  for (int i = 0; i < nbTests; i++)
  {
    if (tests[i].isEnabled == ENABLED)
    {
      int result = tests[i].run();
      if (result)
        PRINT2((stdout, "%-30s:  OK\n", tests[i].name));
      else
        PRINT2((stdout, "%-30s:  FAILED\n", tests[i].name));
    }
    else
    {
      PRINT2((stdout, "%-30s:  DISABLED\n", tests[i].name));
    }
  }
}

