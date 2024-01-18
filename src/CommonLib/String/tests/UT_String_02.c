/* UT_String_02.c */
#include "String2.h"
#include "MyAllocator.h"
#include "ObjectStore.h"
#include "Debug.h"

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("\x1b[32mPassed\x1b[0m\n");} \
                          else { printf("\x1b[31mFailed\x1b[0m\n"); return 0;}
#endif

int step1()
{
  int isPassed = 1;
  PRINT(("Step 1: Test 1 - "));
  
  return isPassed;
}

int main()
{
  int isPassed = 1;

  isPassed = isPassed && step1();

  return isPassed;
}