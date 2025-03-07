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

PRIVATE char * testXmlBuffer1 = "<?xml version='1.0'?>"
                               "<!-- This file represents a test -->"
                               "<directory><person>John Smith</person><person>Jane Doe</person></directory>";
PRIVATE char * testXmlBuffer2 = "<phone_book>"
                                "<employee id='123'>Maggie Mack</employee>"
                                "<employee site='uk' id='124'>Alan Ackerman</employee>"
                                "</phone_book>";

PRIVATE int UT_XmlReader_01_step1()
{
  int isPassed = 1;

  PRINT2((logChannel, "Step 1: Test 1 - Create an XmlReader object: "));

  String * testXmlString = String_newByRef(testXmlBuffer1);
  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  isPassed = isPassed && (OBJECT_IS_VALID(testXmlReader));

  UT_ASSERT(isPassed);

  PRINT2((logChannel, "Step 1: Test 2 - Delete an XmlReader object: "));

  XmlReader_delete(testXmlReader);
  String_delete(testXmlString);

  isPassed = isPassed && (OBJECT_IS_INVALID(testXmlReader));
  UT_ASSERT(isPassed);

  return isPassed;
}

PRIVATE int UT_XmlReader_01_step2()
{
  int isPassed = 1;

  String * testXmlString = String_newByRef(testXmlBuffer1);
  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  PRINT2((logChannel, "Step 2: Test 1 - Read XML version: "));

  XmlNode node = XmlReader_read(testXmlReader);

  UT_ASSERT((node==XMLVERSION));

  PRINT2((logChannel, "Step 2: Test 2 - Read XML comment: "));

  node = XmlReader_read(testXmlReader);

  UT_ASSERT((node==XMLCOMMENT));

  PRINT2((logChannel, "Step 2: Test 3 - Read XML element x 2: "));

  node = XmlReader_read(testXmlReader);
  XmlNode node2 = XmlReader_read(testXmlReader);

  UT_ASSERT(((node==XMLELEMENT)&&(node2==XMLELEMENT)));

  PRINT2((logChannel, "Step 2: Test 4 - Read XML text and end element: "));

  node = XmlReader_read(testXmlReader);
  node2 = XmlReader_read(testXmlReader);

  UT_ASSERT(((node==XMLTEXT)&&(node2==XMLENDELEMENT)));

  PRINT2((logChannel, "Step 2: Test 5 - Read XML text and end element: "));

  node = XmlReader_read(testXmlReader);
  node2 = XmlReader_read(testXmlReader);

  UT_ASSERT(((node==XMLENDELEMENT)&&(node2==XMLENDELEMENT)));

  XmlReader_delete(testXmlReader);
  String_delete(testXmlString);

  return isPassed;
}

int UT_XmlReader_01_step3()
{
  int isPassed = 1;
  XmlNode node = XMLNONE;

  String * testXmlString = String_newByRef(testXmlBuffer2);
  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  PRINT2((logChannel, "Step 3: Test 1 - Read XML first element: "));

  node = XmlReader_read(testXmlReader);
  String* element1Name = XmlReader_getContent(testXmlReader);
  String* check1Name = String_newByRef("phone_book");
  isPassed = isPassed && (node==XMLELEMENT);
  isPassed = isPassed && (String_compare(element1Name, check1Name)==0);

  UT_ASSERT((isPassed));

  String_delete(element1Name);
  String_delete(check1Name);

  PRINT2((logChannel, "Step 3: Test 2 - Read XML second element: "));

  node = XmlReader_read(testXmlReader);
  String* element2Name = XmlReader_getContent(testXmlReader);
  String* check2Name = String_newByRef("employee");
  isPassed = isPassed && (node==XMLELEMENT);
  isPassed = isPassed && (String_compare(element2Name, check2Name)==0);

  UT_ASSERT((isPassed));

  String_delete(element2Name);
  String_delete(check2Name);

  PRINT2((logChannel, "Step 3: Test 3 - Read XML attribute: "));

  node = XmlReader_read(testXmlReader);
  String* attributeName = XmlReader_getContent(testXmlReader);
  String* check3Name = String_newByRef("id");
  
  isPassed = isPassed && (node==XMLATTRIBUTE);

  UT_ASSERT((isPassed));

  String_delete(attributeName);
  String_delete(check3Name);

  XmlReader_delete(testXmlReader);
  String_delete(testXmlString);

  return isPassed;
}

#ifdef MAIN
int main()
#else
int run_UT_XmlReader_01()
#endif
{
  int isPassed = 1;

  logChannel = Debug_openChannel("UT_XmlReader_01.log");
  Debug_setStdoutChannel(logChannel);

  isPassed = isPassed && UT_XmlReader_01_step1();
  isPassed = isPassed && UT_XmlReader_01_step2();
  isPassed = isPassed && UT_XmlReader_01_step3();

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(logChannel);

  return isPassed;
}
