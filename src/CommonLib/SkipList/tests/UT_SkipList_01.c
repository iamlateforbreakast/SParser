// UT_SkipList_01.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SkipList.h"
#include "Object.h"
#include "Debug.h"

#define DEBUG (1)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

#define NB_OBJECTS (10000)

SkipList* testList;

unsigned int keys[] = { 55, 30, 80, 10, 40, 120, 5, 20, 500 };
unsigned int value[] = { 1972, 2005, 1945, 1875, 2001, 1515, 1066, 0, 1789 };

unsigned int randomKeys[NB_OBJECTS];
unsigned int randomValues[NB_OBJECTS];

int nbItems = sizeof(keys) / sizeof(unsigned int);

int step1()
{
  int isPassed = 1;

  PRINT(("Step 1: Test 1 - Check SkipList struct is aligned: "));

  isPassed = isPassed && ((SkipList_getSize(0) % MEM_ALIGN) == 0);

  PRINT((" Size: %d\n", SkipList_getSize(0)));
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 2 - Build a SkipList: "));

  testList = SkipList_new(NB_OBJECTS + 1);

  char * checkObjectPtr = (char*)testList;
  char * checkSkipListPtr = (char*)testList + (sizeof(Object)/MEM_ALIGN)*MEM_ALIGN;

  TRACE(("\n  Object id: %d\n",*(unsigned int*)checkObjectPtr));
  isPassed = isPassed && ((*(unsigned int*)checkObjectPtr) == 1);
  checkObjectPtr +=sizeof(unsigned int) + 3 * sizeof(void*);
  TRACE(("  Ref count: %d\n",*(unsigned int*)checkObjectPtr));
  isPassed = isPassed && ((*(unsigned int*)checkObjectPtr) == 1);
  TRACE(("  Level: %d\n", *(unsigned int*)checkSkipListPtr));
  isPassed = isPassed && ((*(unsigned int*)checkSkipListPtr) == 0);

  UT_ASSERT((isPassed));

  return isPassed;
}

int step2()
{
  int isPassed = 1;

  PRINT(("Step 2: Test 1 - Add %d items: ", nbItems));

  for (int i = 0; i < sizeof(keys)/sizeof(unsigned int); i++)
  {
    SkipList_add(testList, keys[i], &value[i]);
    //SkipList_print(testList);
  }
        
  UT_ASSERT((isPassed));

  return isPassed;
}

int step3()
{
  int isPassed = 1;

  PRINT(("Step 3: Test 1 - Print %d items: ", nbItems));
  SkipList_print(testList);

  return isPassed;
}

int step4()
{
  void* itemPtr = 0;
  int isPassed = 1;

  PRINT(("Step 4: Test 1 - Retrieve %d items: ", nbItems));
  TRACE(("\n"));
  for (int i = 0; i < sizeof(keys) / sizeof(unsigned int); i++)
  {
    itemPtr = SkipList_get(testList, keys[i]);
    isPassed = isPassed && (*(unsigned int*)itemPtr == value[i]);
    
    if (isPassed) TRACE(("OK\n")); else TRACE(("NOK\n"));
  }

  UT_ASSERT((isPassed));

  return isPassed;
}

int step5()
{
  int isPassed = 1;

  PRINT(("Step 5: Test 1 - Delete the SkipList instance: "));
  SkipList_delete(testList);

  UT_ASSERT((isPassed));

  return isPassed;
}

int step6()
{
  testList = SkipList_new(NB_OBJECTS + 1);

  for (int i=0; i<NB_OBJECTS; i++)
  {
    randomKeys[i] = (i + 1) * 5;
    randomValues[i] = (i + 1) * 15;
  }

  for (int i=0; i<NB_OBJECTS; i++)
  {
    int j = (rand() % NB_OBJECTS);
    unsigned int swap = randomKeys[i];
    randomKeys[i] = randomKeys[j];
    randomKeys[j] = swap;
    //printf("Swapping %d and %d\n",i,j);
  }

  for (int i=0; i<NB_OBJECTS; i++)
  {
    SkipList_add(testList, randomKeys[i], &randomValues[i]);
    //printf("Adding %d\n", randomKeys[i]);
  }
  SkipList_print(testList);
  SkipList_delete(testList);
  return 1;
}

int main()
{
  step1();
  step2();
  step3();
  step4();
  step5();
}