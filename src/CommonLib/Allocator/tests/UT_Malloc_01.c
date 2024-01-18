/* UT_Malloc_01.c */
#include "Malloc.h"
#include "Debug.h"

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("\x1b[32mPassed\x1b[0m\n");} \
                          else { printf("\x1b[31mFailed\x1b[0m\n"); return 0;}

extern void set_data_malloc_result();

int step1()
{
  int isPassed = 1;
  Malloc * testMalloc = 0;

  PRINT(("Step 1: Test 1 - Create an instance of the Malloc class: "));
  testMalloc = Malloc_getRef();

  UT_ASSERT((isPassed));
  
  PRINT(("Step 1: Test 2 - Allocate a memory area using the Malloc instance: "));
  char * buff = 0;

  set_data_malloc_result(0xABCDEF);
  buff = Malloc_allocate((Allocator*)testMalloc, 500);

  isPassed = isPassed && (buff!=0);

  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 3 - Free the memory area using the Malloc instance: "));

  Malloc_deallocate((Allocator*)testMalloc, buff);

  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 4 - Delete the reference to the Malloc instance: "));

  Malloc_delete((Allocator*)testMalloc);

  UT_ASSERT((isPassed));

  return isPassed;
}

int main()
{
  int isPassed = 1;

  isPassed = isPassed && step1();

  return 1;
}