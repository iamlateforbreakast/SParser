#include "XmlReader.h"
#include "String2.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Debug.h"
#include "Memory.h"

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

PRIVATE FILE * logChannel;
PRIVATE char * testXmlBuffer = "<?xml version='1.0'?>"
                               "<!-- This file represents a test -->"
                               "<root><person></person></root>";

PRIVATE String* testXmlString;

PRIVATE int UT_XmlReader_01_step1()
{
  int isPassed = 1;

  PRINT2((logChannel, "Step 1: Test 1 - Create an XmlReader object: "));

  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  isPassed = isPassed && (OBJECT_IS_VALID(testXmlReader));

  UT_ASSERT(isPassed);

  PRINT2((logChannel, "Step 1: Test 2 - Delete an XmlReader object: "));

  XmlReader_delete(testXmlReader);

  isPassed = isPassed && (OBJECT_IS_INVALID(testXmlReader));
  UT_ASSERT(isPassed);

  return isPassed;
}

PRIVATE int UT_XmlReader_01_step2()
{
  int isPassed = 1;

  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  PRINT2((logChannel, "Step 2: Test 1 - Read XML version: "));

  XmlNode node = XmlReader_read(testXmlReader);

  PRINT2((logChannel, "Step 2: Test 2 - Read XML comment: "));

  node = XmlReader_read(testXmlReader);

  XmlReader_delete(testXmlReader);

  return isPassed;
}

int main()
{
  int isPassed = 1;

  logChannel = Debug_openChannel("UT_XmlReader_01.log");
  Debug_setStdoutChannel(logChannel);

  testXmlString = String_newByRef(testXmlBuffer);

  isPassed = isPassed && UT_XmlReader_01_step1();
  isPassed = isPassed && UT_XmlReader_01_step2();

  String_delete(testXmlString);

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(logChannel);

  return isPassed;
}
