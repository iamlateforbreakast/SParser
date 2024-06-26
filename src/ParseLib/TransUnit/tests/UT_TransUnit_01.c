/* UT_TransUnit_01.c */
#include "TransUnit.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

int step1()
{
  int isPassed = 1;
  TransUnit* testTransUnit = 0;
  FileMgr * fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc * c_file = FileMgr_addFile(fileMgr, "test.c");

  PRINT(("Step 1: Test 1 - Create an instance of class TransUnit: "));
  testTransUnit = TransUnit_new(c_file, fileMgr);
  UT_ASSERT((testTransUnit != 0));

  PRINT(("Step 1: Test 2 - Delete an instance of class TransUnit: "));
  TransUnit_delete(testTransUnit);
  UT_ASSERT((1));

  FileMgr_delete(fileMgr);

  PRINT(("Step 1: Test 3 - Check all memory is freed: "));
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);

  return isPassed; 
}

int step2()
{
  int isPassed = 1;

  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();

  FileDesc* c_file = FileMgr_addFile(fileMgr, "test_ifndef.c");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);

  PRINT(("Step 2: Test 1 - Read a buffer from TrasnUnit instance: "));

  String* resultBuffer[2];
  String* expected[2];
  expected[0] = String_newByRef("\nvoid disable()\n{\n}\n");
  expected[1] = String_newByRef("\nvoid main()\n{\n}\n");

  resultBuffer[0] = TransUnit_getNextBuffer(testTransUnit);
  
  isPassed = isPassed  && (String_compare(resultBuffer[0], expected[0]) == 0);

  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[0])));

  resultBuffer[1] = TransUnit_getNextBuffer(testTransUnit);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[1])));

  isPassed = isPassed && (String_compare(resultBuffer[1], expected[1]) == 0);
  UT_ASSERT(isPassed);

  String_delete(resultBuffer[0]);
  String_delete(resultBuffer[1]);
  String_delete(expected[0]);
  String_delete(expected[1]);

  TransUnit_delete(testTransUnit);

  FileMgr_delete(fileMgr);

  PRINT(("Step 2: Test 2 - Check all memory is freed: "));
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  //ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

int step3()
{
  int isPassed = 1;

  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc* c_file = FileMgr_addFile(fileMgr, "test_ifdef.c");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);
  PRINT(("Step 3: Test 1 - Read a buffer from TransUnit instance: "));
  String* resultBuffer[2];
  String* expected[2];
  expected[0] = String_newByRef("\nvoid enable()\n{\n}\n");
  expected[1] = String_newByRef("\nvoid main()\n{\n}\n");
  resultBuffer[0] = TransUnit_getNextBuffer(testTransUnit);
  isPassed = isPassed && (String_compare(resultBuffer[0], expected[0]) == 0);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[0])));
  resultBuffer[1] = TransUnit_getNextBuffer(testTransUnit);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[1])));
  isPassed = isPassed && (String_compare(resultBuffer[1], expected[1]) == 0);
  String_delete(resultBuffer[0]);
  String_delete(resultBuffer[1]);
  String_delete(expected[0]);
  String_delete(expected[1]);
  TransUnit_delete(testTransUnit);
  FileMgr_delete(fileMgr);
  PRINT(("Step 3: Test 2 - Check all memory is freed: "));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);
  return isPassed;
}
int step4()
{
  int isPassed = 1;
  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc* c_file = FileMgr_addFile(fileMgr, "test_define.c");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);
  PRINT(("Step 3: Test 1 - Read a buffer from TransUnit instance: "));
  String* resultBuffer[2];
  String* expected[2];
  expected[0] = String_newByRef("\nint table1[100];\nint table2[200];\n}\n");
  expected[1] = String_newByRef("\nint main()\n{\n  int x = 5, y;\n  y = getmax(x,2);\n\n  return 0;\n}\n");
  resultBuffer[0] = TransUnit_getNextBuffer(testTransUnit);
  isPassed = isPassed && (String_compare(resultBuffer[0], expected[0]) == 0);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[0])));
  resultBuffer[1] = TransUnit_getNextBuffer(testTransUnit);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[1])));
  isPassed = isPassed && (String_compare(resultBuffer[1], expected[1]) == 0);
  UT_ASSERT(isPassed);
  FileMgr_delete(fileMgr);
  PRINT(("Step 4: Test 2 - Check all memory is freed: "));
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);
  return isPassed;
}
int step5()
{
  int isPassed = 1;
  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  //FileDesc* c_file = FileMgr_addFile(fileMgr, "test.c");
  //FileDesc* h_file = FileMgr_addFile(fileMgr, "test.h");

  FileDesc* c_file = FileMgr_addFile(fileMgr, "BTree.c");
  FileDesc* h_file1 = FileMgr_addFile(fileMgr, "BTree.h");
  FileDesc* h_file2 = FileMgr_addFile(fileMgr, "Node.h");
  FileDesc* h_file3 = FileMgr_addFile(fileMgr, "Memory.h");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);

  PRINT(("Step 3: Test 1 - Read a buffer from TransUnit instance: "));

  for (int i = 0; i < 3; ++i)
  {
    String* buffer = TransUnit_getNextBuffer(testTransUnit);
    PRINT(("Output buffer: %s\n", String_getBuffer(buffer)));
    String_delete(buffer);
  }
  
  //PRINT(("Step 1: Test 2 - Delete an instance of class TransUnit: "));
  TransUnit_delete(testTransUnit);

  FileMgr_delete(fileMgr);

  PRINT(("Step 3: Test 2 - Check all memory is freed: "));
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

int main()
{
  int isPassed = 1;
  
  step1();
  step2();
  step3();
  //step4();

  Memory_report();

  return isPassed;
}
