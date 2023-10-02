#include "Array.h"
#include "String2.h"
#include "Memory.h"
#include <stdio.h>

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

int step1()
{
  Array * refArray = 0;
  Array * testArray = 0;
  ArrayParam testParam = { .defaultSize = 10, .storageMode = 0, .autoresize = 0};

  /* Test 1 */
  printf("Step 1: Test 1 - Check the Array object is well constructed: ");
  testArray = Array_new(&testParam);
  UT_ASSERT((Memory_ncmp(testArray, refArray, sizeof(testArray))==1));


  /* Test 2 */
  printf("Step 1: Test 2 - Check the Array object is destructed: ");
  Array_delete(testArray);

  /* Test 3 */
  printf("Step 1: Test 3 - Check - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb()==(Memory_getFreeRequestNb()+1)))
  return 0;
}

int step2()
{
  return 0;
}
unsigned int main(void)
{
  step1();
  step2();
}
