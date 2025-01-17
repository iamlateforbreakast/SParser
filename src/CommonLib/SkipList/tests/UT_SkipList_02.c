#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SkipList.h"
#include "Object.h"
#include "MyAllocator.h"
#include "ObjectStore.h"
#include "Debug.h"
#include "TestObject.h"
#include "Memory.h"
#include "Times.h"
#include "List.h"
#include "String2.h"
#include "Error.h"

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

PRIVATE MyAllocator* UT_SkipList_02_testAlloc = 0;
PRIVATE AllocInfo* allocInfo = 0;

PRIVATE SkipList* testList;

PRIVATE int UT_SkipLIst_01_nbWords;
PRIVATE String** wordKeys;
PRIVATE TestObject** UT_SkipLIst_01_testObjects;

extern char words1000[];

PRIVATE int UT_SkipList_02_init_keys()
{
  String* allWords = String_new(words1000);
  List* tokens = 0;

  tokens = String_splitToken(allWords, " ");
  UT_SkipLIst_01_nbWords = List_getNbNodes(tokens);

  wordKeys = (String**)Memory_alloc(UT_SkipLIst_01_nbWords * sizeof(String*));
  UT_SkipLIst_01_testObjects = (TestObject**)Memory_alloc(UT_SkipLIst_01_nbWords * sizeof(TestObject*));

  for (int i = 0; i < UT_SkipLIst_01_nbWords; i++)
  {
    wordKeys[i] = (String*)List_getNext(tokens);
    UT_SkipLIst_01_testObjects[i] = TestObject_new();
  }

  List_delete(tokens);
  String_delete(allWords);

  return 1;
}

PRIVATE int UT_SkipList_02_delete_keys()
{
  for (int i = 0; i < UT_SkipLIst_01_nbWords; i++)
  {
    TestObject_delete(UT_SkipLIst_01_testObjects[i]);
    String_delete(wordKeys[i]);
  }
  Memory_free(wordKeys, sizeof(wordKeys));
  Memory_free(UT_SkipLIst_01_testObjects, sizeof(UT_SkipLIst_01_testObjects));

  return 1;
}

PRIVATE int UT_SkipList_02_step1()
{
  int isPassed = 1;

  PRINT(("Step 1: Test 1 - Check SkipList struct is aligned: "));

  isPassed = isPassed && ((SkipList_getSize(0) % MEM_ALIGN) == 0);

  TRACE(("\n  Size: %d\n", SkipList_getSize(0)));
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 2 - Build a SkipList: "));

  testList = SkipList_newFromAllocator((Allocator*)UT_SkipList_02_testAlloc);

  if (testList == 0) Error_new(ERROR_FATAL, "Failed to create SkipList.\n");

  char* checkObjectPtr = (char*)testList;
  char* checkSkipListPtr = (char*)testList + (sizeof(Object) / MEM_ALIGN) * MEM_ALIGN;

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

PRIVATE int UT_SkipList_02_step2()
{
  int isPassed = 1;
  int aFewItems = 10;

  PRINT(("Step 2: Test 1 - Add %d items: ", aFewItems));

  for (int i = 0; i < aFewItems; i++)
  {
    //TRACE(("  Inserting: ")); String_print(wordKeys[i]); PRINT(("\n"));
    SkipList_add(testList, (Object*)wordKeys[i], (Object*)UT_SkipLIst_01_testObjects[i]);
    SkipList_print(testList);
  }

  UT_ASSERT((isPassed));

  return isPassed;
}

PRIVATE int UT_SkipList_02_step3()
{
  int isPassed = 1;

  PRINT(("Step 3: Test 1 - Print %d items: ", 10));
  SkipList_print(testList);

  return isPassed;
}

PRIVATE int UT_SkipList_02_step4()
{
  void* itemPtr = 0;
  int isPassed = 1;

  PRINT(("Step 4: Test 1 - Retrieve %d items: ", 10));
  TRACE(("\n"));
  for (int i = 0; i < 10; i++)
  {
    itemPtr = SkipList_get(testList, (Object*)wordKeys[i]);
    isPassed = isPassed && (itemPtr == UT_SkipLIst_01_testObjects[i]);

    if (isPassed) TRACE(("Item %d retrieved.\n", i)); else TRACE(("Item %d failed to retrieved.\n", i));
  }

  UT_ASSERT((isPassed));

  return isPassed;
}

PRIVATE int UT_SkipList_02_step5()
{
  int isPassed = 1;
  void * itemPtr = 0;

  PRINT(("Step 5: Test 1 - Remove some items: "));
  itemPtr = SkipList_remove(testList, (Object*)wordKeys[0]);
  SkipList_print(testList);

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_02_step6()
{
  int isPassed = 1;

  PRINT(("Step 2: Test 1 - Delete the SkipList instance: "));
  SkipList_delete(testList);

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SkipList_02_step7()
{
    /*testList = SkipList_newFromAllocator();

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
        //PRINT(("Swapping %d and %d\n",i,j));
    }

    long double cpu_time0 = get_cpu_time();
    long double wall_time0 = get_wall_time();

    for (int i=0; i<NB_OBJECTS; i++)
    {
        SkipList_add(testList, randomKeys[i], &randomValues[i]);
        //PRINT(("Adding %d\n", randomKeys[i]));
    }

    long double cpu_time1 = get_cpu_time();
    long double wall_time1 = get_wall_time();

   
    SkipList_print(testList);
    SkipList_delete(testList);

    PRINT(("Insert CPU time %Lf\n", (cpu_time1- cpu_time0)/NB_OBJECTS));
    PRINT(("Insert Wall Time %Lf\n", (wall_time1 - wall_time0)/NB_OBJECTS));*/

    return 1;
}

PUBLIC int run_UT_SkipList_02()
{
  ObjectStore * objectStore = 0;
  
  objectStore = ObjectStore_getRef();
  UT_SkipList_02_testAlloc = MyAllocator_new(5000);
  allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)UT_SkipList_02_testAlloc);

  UT_SkipList_02_init_keys();

  UT_SkipList_02_step1();
  UT_SkipList_02_step2();
  UT_SkipList_02_step3();
  UT_SkipList_02_step4();
  UT_SkipList_02_step5();
  UT_SkipList_02_step6();

  UT_SkipList_02_delete_keys();

  ObjectStore_deleteAllocator(objectStore, allocInfo);
  ObjectStore_delete(objectStore);
  return 0;
}
