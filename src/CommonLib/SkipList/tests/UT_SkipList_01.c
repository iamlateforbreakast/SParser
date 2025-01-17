// UT_SkipList_01.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SkipList.h"
#include "Object.h"
#include "Debug.h"
#include "TestObject.h"
#include "Memory.h"
#include "Times.h"
#include "List.h"
#include "String2.h"

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n"));} \
                          else { PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0;}
#endif

#define NB_OBJECTS (10000)

SkipList* UT_SkipList_01_testList;
extern char words1000[];

PRIVATE int nbWords;
PRIVATE String** UT_SkipList_01_wordKeys;
PRIVATE TestObject** UT_SkipList_01_testObjects;
PRIVATE FILE* UT_SkipList_01_channelLog = 0;

int UT_SkipList_01_init_keys()
{
  String* allWords = String_new(words1000);
  List* tokens = 0;

  tokens = String_splitToken(allWords, " ");
  nbWords = List_getNbNodes(tokens);

  UT_SkipList_01_wordKeys = (String**)Memory_alloc(nbWords * sizeof(String*));
  UT_SkipList_01_testObjects = (TestObject**)Memory_alloc(nbWords * sizeof(TestObject*));

  for (int i = 0; i < nbWords; i++)
  {
    UT_SkipList_01_wordKeys[i] = (String*)List_getNext(tokens);
    UT_SkipList_01_testObjects[i] = TestObject_new();
  }

  List_delete(tokens);
  String_delete(allWords);

  return 1;
}

int UT_SkipList_01_delete_keys()
{
  for (int i = 0; i < nbWords; i++)
  {
    TestObject_delete(UT_SkipList_01_testObjects[i]);
    String_delete(UT_SkipList_01_wordKeys[i]);
  }
  Memory_free(UT_SkipList_01_wordKeys, sizeof(UT_SkipList_01_wordKeys));
  Memory_free(UT_SkipList_01_testObjects, sizeof(UT_SkipList_01_testObjects));

  return 1;
}

int UT_SkipList_01_step1()
{
  int isPassed = 1;

  PRINT(("Step 1: Test 1 - Check SkipList struct is aligned: "));

  isPassed = isPassed && ((SkipList_getSize(0) % MEM_ALIGN) == 0);

  TRACE(("\n  Size: %d\n", SkipList_getSize(0)));
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 2 - Build a SkipList: "));

  UT_SkipList_01_testList = SkipList_new(NB_OBJECTS + 1);

  char* checkObjectPtr = (char*)UT_SkipList_01_testList;
  char* checkSkipListPtr = (char*)UT_SkipList_01_testList + (sizeof(Object) / MEM_ALIGN) * MEM_ALIGN;

  TRACE(("\n  Object id: %d\n", *(unsigned int*)checkObjectPtr));
  isPassed = isPassed && ((*(unsigned int*)checkObjectPtr) == 1);
  checkObjectPtr += sizeof(unsigned int) + 3 * sizeof(void*);
  TRACE(("  Ref count: %d\n", *(unsigned int*)checkObjectPtr));
  isPassed = isPassed && ((*(unsigned int*)checkObjectPtr) == 1);
  TRACE(("  Level: %d\n", *(unsigned int*)checkSkipListPtr));
  isPassed = isPassed && ((*(unsigned int*)checkSkipListPtr) == 0);

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_01_step2()
{
  int isPassed = 1;
  int aFewItems = 10;

  PRINT(("Step 2: Test 1 - Add %d items: ", aFewItems));

  for (int i = 0; i < aFewItems; i++)
  {
    //TRACE(("  Inserting: ")); String_print(UT_SkipList_01_wordKeys[i]); PRINT(("\n"));
    SkipList_add(UT_SkipList_01_testList, (Object*)UT_SkipList_01_wordKeys[i], (Object*)UT_SkipList_01_testObjects[i]);
    SkipList_print(UT_SkipList_01_testList);
  }

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_01_step3()
{
  int isPassed = 1;

  PRINT(("Step 3: Test 1 - Print %d items: ", 10));
  SkipList_print(UT_SkipList_01_testList);

  return isPassed;
}

int UT_SkipList_01_step4()
{
  void* itemPtr = 0;
  int isPassed = 1;

  PRINT(("Step 4: Test 1 - Retrieve %d items: ", 10));
  TRACE(("\n"));
  for (int i = 0; i < 10; i++)
  {
    itemPtr = SkipList_get(UT_SkipList_01_testList, (Object*)UT_SkipList_01_wordKeys[i]);
    isPassed = isPassed && (itemPtr == UT_SkipList_01_testObjects[i]);

    if (isPassed) TRACE(("Item %d retrieved.\n", i)); else TRACE(("Item %d failed to retrieved.\n", i));
  }

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_01_step5()
{
  int isPassed = 1;
  void * itemPtr = 0;

  PRINT(("Step 5: Test 1 - Remove some items: "));
  itemPtr = SkipList_remove(UT_SkipList_01_testList, (Object*)UT_SkipList_01_wordKeys[0]);
  SkipList_print(UT_SkipList_01_testList);

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_01_step6()
{
  int isPassed = 1;

  PRINT(("Step 5: Test 1 - Delete the SkipList instance: "));
  SkipList_delete(UT_SkipList_01_testList);

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_01_step7()
{
  SkipList * UT_SkipList_01_testList = SkipList_new();

  long double cpu_time0 = get_cpu_time();
  long double wall_time0 = get_wall_time();

  for (int i = 0; i < nbWords; i++)
  {
    SkipList_add(UT_SkipList_01_testList, (Object*)UT_SkipList_01_wordKeys[i], (Object*)UT_SkipList_01_testObjects[i]);
    //PRINT(("Adding %d\n", randomKeys[i]));
  }

  long double cpu_time1 = get_cpu_time();
  long double wall_time1 = get_wall_time();

  SkipList_delete(UT_SkipList_01_testList);
  
  PRINT(("Insert CPU time %Lf\n", (cpu_time1- cpu_time0)/nbWords));
  PRINT(("Insert Wall Time %Lf\n", (wall_time1 - wall_time0)/nbWords));
  
  return 1;
}

int run_UT_SkipList_01()
{
  int isPassed = 1;

  UT_SkipList_01_channelLog = Debug_openChannel("UT_SkipList_01.log");
  Debug_setStdoutChannel(UT_SkipList_01_channelLog);

  UT_SkipList_01_init_keys();

  isPassed = isPassed && UT_SkipList_01_step1();
  isPassed = isPassed && UT_SkipList_01_step2();
  isPassed = isPassed && UT_SkipList_01_step3();
  isPassed = isPassed && UT_SkipList_01_step4();
  isPassed = isPassed && UT_SkipList_01_step5();
  isPassed = isPassed && UT_SkipList_01_step6();
  isPassed = isPassed && UT_SkipList_01_step7();

  UT_SkipList_01_delete_keys();

  Memory_report();
  Debug_closeChannel(UT_SkipList_01_channelLog);
  return isPassed;
}
