#include "Debug.h"
#include <stdio.h>

extern int run_UT_List_01();

typedef struct TestDefinition TestDefinition;

struct TestDefinition
{
  char *  name;
  int (*run)();
};

TestDefinition tests[] =
{
  { "UT_List_01", &run_UT_List_01 }
};

int main()
{
  int nbTests = sizeof(tests)/sizeof(TestDefinition);
  
  for (int i = 0; i<nbTests; i++)
  {
    int result = tests[i].run();
    if (result) PRINT2((stdout, "%s: OK\n", tests[i].name));
    else
      PRINT2((stdout, "%s: FAILED\n", tests[i].name));
  }
}
