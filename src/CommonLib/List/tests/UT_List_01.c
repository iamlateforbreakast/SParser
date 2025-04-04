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
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          {  PRINT(("\x1b[32mPassed\x1b[0m\n"));} \
                          else {  PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0;}
#endif

PRIVATE TestObject* UT_List_01_items[MAX_OBJECT_NB * 2];

PRIVATE FILE * UT_List_01_logChannel;

PRIVATE int UT_List_01_init_testobjects()
{
  for (int i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    UT_List_01_items[i] = TestObject_new();
  }

  return 1;
}

PRIVATE int UT_List_01_delete_testobjects()
{
  for (int i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    TestObject_delete(UT_List_01_items[i]);
  }

  return 1;
}

PRIVATE int UT_List_01_step1()
{
  int isPassed = 1;
  List* testList = 0;

  TestObject* removed[MAX_OBJECT_NB * 2];

  int i = 0;

  PRINT(("Step 1: Test 1 - Create a list: "));
  testList = List_new();
  isPassed = isPassed && OBJECT_IS_VALID(testList);

  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 2 - Insert %d objects in list: ", MAX_OBJECT_NB * 2));
  for (i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    List_insertHead(testList, UT_List_01_items[i], 1);
    TRACE2((UT_List_01_logChannel, "Nb items %d\n", List_getNbNodes(testList)));
    TRACE2((UT_List_01_logChannel, "  Allocated %d bytes at %x\n", ((Object*)UT_List_01_items[i])->class->f_size(0), UT_List_01_items[i]));
  }
  isPassed = isPassed && (List_getNbNodes(testList) == MAX_OBJECT_NB * 2);

  UT_ASSERT(isPassed);
  PRINT(("Step 1: Test 3 - Remove %d objects from list: ", MAX_OBJECT_NB * 2));
  for (i = 0; i < MAX_OBJECT_NB * 2; i++)
  {
    removed[i] = List_removeHead(testList);
    TRACE2((UT_List_01_logChannel, "  Removed %d bytes at %x\n", ((Object*)removed[i])->class->f_size(0), removed[i]));
    TestObject_delete(removed[i]);
  }
  isPassed = isPassed && (List_getNbNodes(testList) == 0);
  UT_ASSERT((isPassed));

  PRINT(("Step 1: test 4 - Delete List: "));
  List_delete(testList);
  isPassed = isPassed && (OBJECT_IS_INVALID(testList));
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 5 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE2((UT_List_01_logChannel, "Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_delete(objectMgr);

  return isPassed;
} 

PRIVATE int UT_List_01_step2()
{
  int isPassed = 1;
  List * testList1 = 0;
  List * testList2 = 0;
  List * testList3 = 0;

  int i = 0;

  PRINT(("Step 2: Test 1 - Build 2 Lists: "));
  testList1 = List_new();
  testList2 = List_new();

  isPassed = isPassed && (OBJECT_IS_VALID(testList1) && (OBJECT_IS_VALID(testList2)));
  UT_ASSERT((isPassed));

  PRINT(("Step 2: test 2 - Insert head and insert tail: \n"));

  for (i=0; i< MAX_OBJECT_NB; i++)
  {
    List_insertHead(testList1, UT_List_01_items[i], 1);
    TRACE2((UT_List_01_logChannel, " Nb items %d\n", List_getNbNodes(testList1)));
  }
  for (i= MAX_OBJECT_NB; i < MAX_OBJECT_NB * 2; i++)
  {
    List_insertTail(testList2, UT_List_01_items[i], 1);
    TRACE2((UT_List_01_logChannel, " Nb items %d\n", List_getNbNodes(testList2)));
  }

  isPassed = isPassed && (List_getNbNodes(testList1) == MAX_OBJECT_NB) && (List_getNbNodes(testList2) == MAX_OBJECT_NB);
  UT_ASSERT((isPassed));
  PRINT(("Step 2: test 3 - Print both lists: \n"));
  PRINT((" List1 ------------->\n"));
  List_forEach(testList1, (void (*)(void* o))&TestObject_print);
  PRINT((" List2 ------------->\n"));
  List_forEach(testList2, (void (*)(void* o))&TestObject_print);

  PRINT(("Step 2: test 4 - Merge both lists: \n"));
  List_merge(testList1, testList2);
  PRINT((" List1 + List2 ----->\n"));
  List_forEach(testList1, (void (*)(void* o))&TestObject_print);

  testList3 = List_copy(testList1);
  PRINT((" Copy of List1 ------>\n"));
  List_forEach(testList3, (void (*)(void* o))&TestObject_print);

  List_delete(testList1);
  List_delete(testList3);

  //Memory_report();

  return isPassed;
}

#ifdef MAIN
int main()
#else
int run_UT_List_01()
#endif
{
  int isPassed = 1;
  
  UT_List_01_logChannel = Debug_openChannel("UT_List_01.log");
  Debug_setStdoutChannel(UT_List_01_logChannel);

  UT_List_01_init_testobjects();

  isPassed = UT_List_01_step1() && isPassed;

  UT_List_01_init_testobjects();

  isPassed = isPassed && UT_List_01_step2() && isPassed;

  UT_List_01_delete_testobjects();

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(UT_List_01_logChannel);

  return isPassed;
}
