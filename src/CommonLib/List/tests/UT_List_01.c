#include "List.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "TestObject.h"
#include "Debug.h"
#include "Memory.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_OBJECT_NB (10)

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

PRIVATE TestObject* items[MAX_OBJECT_NB * 2];

PRIVATE FILE * logChannel;

PRIVATE int UT_List_01_init_testobjects()
{
  for (int i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    items[i] = TestObject_new();
  }

  return 1;
}

PRIVATE int UT_List_01_delete_testobjects()
{
  for (int i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    TestObject_delete(items[i]);
  }

  return 1;
}

PRIVATE int UT_List_01_step1()
{
  List* testList = 0;

  TestObject* removed[MAX_OBJECT_NB * 2];

  int i = 0;

  PRINT2((logChannel, "Step 1: Test 1 - Create a list: "));
  testList = List_new();

  UT_ASSERT((testList!=0));

  PRINT2((logChannel, "Step 1: Test 2 - Insert %d objects in list: ", MAX_OBJECT_NB * 2));
  for (i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    List_insertHead(testList, items[i], 1);
    TRACE2((logChannel, "Nb items %d\n", List_getNbNodes(testList)));
    TRACE2((logChannel, "  Allocated %d bytes at %x\n", ((Object*)items[i])->class->f_size(0), items[i]));
  }
  UT_ASSERT((List_getNbNodes(testList) == MAX_OBJECT_NB * 2));

  PRINT2((logChannel, "Step 1: Test 3 - Remove %d objects from list: ", MAX_OBJECT_NB * 2));
  for (i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    removed[i] = List_removeHead(testList);
    TRACE2((logChannel, "  Removed %d bytes at %x\n", ((Object*)removed[i])->class->f_size(0), removed[i]));
    TestObject_delete(removed[i]);
  }
  UT_ASSERT((1));

  PRINT2((logChannel, "Step 1: test 4 - Delete List: "));
  List_delete(testList);
  UT_ASSERT((1));

  PRINT2((logChannel, "Step 1: Test 5 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE2((logChannel, "Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_delete(objectMgr);

  return 0;
} 

PRIVATE int UT_List_01_step2()
{
  List * testList1 = 0;
  List * testList2 = 0;
  List * testList3 = 0;

  int i = 0;

  PRINT2((logChannel, "Step 2: Test 1 - Build 2 Lists: "));
  testList1 = List_new();
  testList2 = List_new();

  UT_ASSERT((1));

  PRINT2((logChannel, "Step 2: test 2 - Insert head and insert tail: \n"));

  for (i=0; i< MAX_OBJECT_NB; i++)
  {
    List_insertHead(testList1, items[i], 1);
    TRACE2((logChannel, " Nb items %d\n", List_getNbNodes(testList1)));
  }
  for (i= MAX_OBJECT_NB; i < MAX_OBJECT_NB * 2; i++)
  {
    List_insertTail(testList2, items[i], 1);
    TRACE2((logChannel, " Nb items %d\n", List_getNbNodes(testList2)));
  }

  PRINT2((logChannel, "Step 2: test 3 - Print both lists: \n"));
  PRINT2((logChannel, " List1 ------------->\n"));
  List_forEach(testList1, (void (*)(void* o))&TestObject_print);
  PRINT2((logChannel, " List2 ------------->\n"));
  List_forEach(testList2, (void (*)(void* o))&TestObject_print);

  PRINT2((logChannel, "Step 2: test 4 - Merge both lists: \n"));
  List_merge(testList1, testList2);
  PRINT2((logChannel, " List1 + List2 ----->\n"));
  List_forEach(testList1, (void (*)(void* o))&TestObject_print);

  testList3 = List_copy(testList1);
  PRINT2((logChannel, " Copy of List1 ------>\n"));
  List_forEach(testList3, (void (*)(void* o))&TestObject_print);

  List_delete(testList1);
  List_delete(testList3);

  //Memory_report();

  return 0;
}

/* int step2()
{
  List * testList1 = 0;
  List * testList2 = 0;

  TestObject * output1 = 0;
  TestObject * output2 = 0;

  int i;
  
  testList1 = List_new();
  testList2 = List_new();

  for (i=0; i< MAX_OBJECT_NB/2;i++)
  {
    items[i] = TestObject_new();

    List_insertHead(testList1, items[i], 1);
    List_insertHead(testList2, items[i], 1);
    printf("Test List1: Nb items %d\n", List_getNbNodes(testList1));
    printf("Test List2: Nb items %d\n", List_getNbNodes(testList2));
  }
  
  output1 = List_getHead(testList1);
  output2 = List_getHead(testList2);

  for (i=0; i< MAX_OBJECT_NB/2; i++)
  {
    output1 = List_removeHead(testList1);
    output2 = List_removeTail(testList2);
    //TestItem_delete(output1);
    printf("Test List 1: Nb items %d\n", List_getNbNodes(testList1));
    printf("Test List 2: Nb items %d\n", List_getNbNodes(testList2));
  }

  output1 = List_removeHead(testList1);
  output2 = List_removeTail(testList2);

  List_delete(testList1);
  List_delete(testList2);

  return 0;
}*/

int run_UT_List_01()
{
  int isPassed = 1;
  
  logChannel = Debug_openChannel("UT_List_01.log");

  UT_List_01_init_testobjects();

  isPassed = isPassed && UT_List_01_step1();

  UT_List_01_init_testobjects();

  isPassed = isPassed && UT_List_01_step2();

  UT_List_01_delete_testobjects();

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(logChannel);

  return isPassed;
}
