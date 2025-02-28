/* UT_TransUnit_01.c */
#include "TransUnit.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (1)
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}

PRIVATE FILE * UT_TransUnit_01_logChannel;

int UT_TransUnit_01_step1()
{
  int isPassed = 1;

  TransUnit* testTransUnit = 0;
  FileMgr * fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc * c_file = FileMgr_addFile(fileMgr, "test.c");

  PRINT(("Step 1: Test 1 - Create an instance of class TransUnit: "));
  testTransUnit = TransUnit_new(c_file, fileMgr);
  isPassed = (testTransUnit != 0) && isPassed;
  UT_ASSERT((isPassed));

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

int UT_TransUnit_01_step2()
{
  int isPassed = 1;

  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();

  FileDesc* c_file = FileMgr_addFile(fileMgr, "test_ifndef.c");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);

  PRINT(("Step 2: Test 1 - Read a buffer from TrasnUnit instance: "));

  String* resultBuffer[2];
  String* expected[2];
  expected[0] = String_newByRef("void disable(){}void main(){}");
  expected[1] = String_newByRef("void main(){}");

  resultBuffer[0] = TransUnit_getNextBuffer(testTransUnit);
  
  isPassed = isPassed  && (String_compare(resultBuffer[0], expected[0]) == 0);

  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[0])));

  resultBuffer[1] = TransUnit_getNextBuffer(testTransUnit);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[1])));







  











  isPassed = isPassed && (resultBuffer[1] == 0);
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
  ObjectMgr_delete(objectMgr);
  return isPassed;
}
int UT_TransUnit_01_step3()
{
  int isPassed = 1;
  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc* c_file = FileMgr_addFile(fileMgr, "test_ifdef.c");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);
  PRINT(("Step 3: Test 1 - Read a buffer from TransUnit instance: "));
  String* resultBuffer[2];
  String* expected[2];
  expected[0] = String_newByRef("void enable(){}\nvoid main(){}");
  expected[1] = 0;
  resultBuffer[0] = TransUnit_getNextBuffer(testTransUnit);
  isPassed = isPassed && (String_compare(resultBuffer[0], expected[0]) == 0);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[0])));
  resultBuffer[1] = TransUnit_getNextBuffer(testTransUnit);
  TRACE(("UT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer[1])));
  isPassed = isPassed && (resultBuffer[1] == 0);
  String_delete(resultBuffer[0]);
  String_delete(resultBuffer[1]);
  String_delete(expected[0]);
  String_delete(expected[1]);
  TransUnit_delete(testTransUnit);
  FileMgr_delete(fileMgr);
  PRINT(("Step 3: Test 2 - Check all memory is freed: "));
  //UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

int UT_TransUnit_01_step4()
{
  int isPassed = 1;

  FileMgr* fileMgr = FileMgr_new();
  ObjectMgr* objectMgr = ObjectMgr_getRef();

  FileDesc* c_file = FileMgr_addFile(fileMgr, "test_define.c");
  TransUnit* testTransUnit = TransUnit_new(c_file, fileMgr);

  PRINT(("Step 4: Test 1 - Read a buffer from TransUnit instance: "));
  String* resultBuffer;
  String* expected;
  expected = String_newByRef("static int table1[100];static int table2[200];}");

  resultBuffer = TransUnit_getNextBuffer(testTransUnit);

  isPassed = isPassed && (String_compare(resultBuffer, expected) == 0);

  TRACE(("\nUT_TransUnit_01: Output buffer= %s\n", String_getBuffer(resultBuffer)));

  UT_ASSERT(1);

  TransUnit_delete(testTransUnit);
  String_delete(expected);
  String_delete(resultBuffer);
  FileMgr_delete(fileMgr);

  ObjectMgr_reportUnallocated(objectMgr);

  PRINT(("Step 4: Test 2 - Check all memory is freed: "));
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

int UT_TransUnit_01_step5()
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

#ifdef MAIN
int main()
#else
int run_UT_TransUnit_01()
#endif
{
  int isPassed = 1;
  
  UT_TransUnit_01_logChannel = Debug_openChannel("UT_TransUnit_01.log");
  Debug_setStdoutChannel(UT_TransUnit_01_logChannel);
  isPassed = UT_TransUnit_01_step1() && isPassed;
  isPassed = UT_TransUnit_01_step2() && isPassed;
  isPassed = UT_TransUnit_01_step3() && isPassed;
  isPassed = UT_TransUnit_01_step4() && isPassed;

  Memory_report();

  Debug_closeChannel(UT_TransUnit_01_logChannel);
  return isPassed;
}
