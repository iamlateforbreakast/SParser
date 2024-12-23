#include "XmlReader.h"
#include "String2.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Debug.h"
#include "Memory.h"

PRIVATE FILE * logChannel;
PRIVATE char * testXmlBuffer = "<root><person></person></root>";
PRIVATE String* testXmlString;

PRIVATE int UT_XmlReader_01_step1()
{
  int isPassed = 1;

  PRINT2((logChannel, "Step 1: Test 1 - Create an XmlReader object: "));

  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  isPassed = isPassed && (OBJECT_IS_VALID(testXmlReader));

  PRINT2((logChannel, "Step 1: Test 2 - Delete an XmlReader object: "));

  XmlReader_delete(testXmlReader);

  isPassed = isPassed && (OBJECT_IS_INVALID(testXmlReader));

  return isPassed;
}

int main()
{
  int isPassed = 1;

  logChannel = Debug_openChannel("UT_List_01.log");
  
  testXmlString = String_newByRef(testXmlBuffer);

  isPassed = isPassed && UT_XmlReader_01_step1();

  String_delete(testXmlString);

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(logChannel);

  return isPassed;
}
