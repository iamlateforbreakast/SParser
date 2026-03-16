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
PRIVATE char * testXmlBuffer3 = "<!-- This file represents a test -->\n"
                                "<directory><person country=\"USA\">John Smith\n"
                                "</person><person country \"UK\">Jane Doe</person> </directory>";

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
  String* textContent = XmlReader_getContent(testXmlReader);
  String* checkTextContent = String_newByRef("John Smith");
  node2 = XmlReader_read(testXmlReader);
  isPassed = isPassed && (node==XMLTEXT);
  isPassed = isPassed && (node2==XMLENDELEMENT);
  isPassed = isPassed && (String_compare(textContent, checkTextContent)==0);
  String_delete(textContent);
  String_delete(checkTextContent);

  UT_ASSERT((isPassed));

  PRINT2((logChannel, "Step 2: Test 5 - Read XML text and end element: "));

  node = XmlReader_read(testXmlReader);
  node2 = XmlReader_read(testXmlReader);
  textContent = XmlReader_getContent(testXmlReader);
  checkTextContent = String_newByRef("Jane Doe");
  isPassed = isPassed && (node==XMLELEMENT);
  isPassed = isPassed && (node2==XMLTEXT);
  isPassed = isPassed && (String_compare(textContent, checkTextContent)==0);
  String_delete(textContent);
  String_delete(checkTextContent);
  
  UT_ASSERT((isPassed));

  PRINT2((logChannel, "Step 2: Test 6 - Read XML end element: "));

  node = XmlReader_read(testXmlReader);
  isPassed = isPassed && (node==XMLENDELEMENT);
  
  UT_ASSERT((isPassed));

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

  String * attributeValue = XmlReader_getAttributeValue(testXmlReader);
  String * checkAttributeValue = String_newByRef("123");
  isPassed = isPassed && (String_compare(attributeValue, checkAttributeValue)==0);
  String_delete(attributeValue);
  String_delete(checkAttributeValue);

  UT_ASSERT((isPassed));

  String_delete(attributeName);
  String_delete(check3Name);

  XmlReader_delete(testXmlReader);
  String_delete(testXmlString);

  return isPassed;
}

int UT_XmlReader_01_step4()
{
  int isPassed = 1;

  String * testXmlString = String_newByRef(testXmlBuffer3);
  XmlReader * testXmlReader = XmlReader_new(testXmlString);

  PRINT2((logChannel, "Step 4: Test 1 - Read XML attribute with double quotes: "));

  XmlNode node = XMLNONE;
  while (node != XMLATTRIBUTE)
  {
    node = XmlReader_read(testXmlReader);
  }
  String* attributeName = XmlReader_getContent(testXmlReader);
  String* checkName = String_newByRef("country");
  
  isPassed = isPassed && (String_compare(attributeName, checkName)==0);

  String_delete(attributeName);
  String_delete(checkName);

  UT_ASSERT((isPassed));

  PRINT2((logChannel, "Step 4: Test 2 - Read multiple attributes on one element: "));
  XmlNode node2 = XmlReader_read(testXmlReader);
  while (((node2 != XMLATTRIBUTE) && (node2 != XMLNONE)))
  {
    node2 = XmlReader_read(testXmlReader);
  }
  String* attribute2Name = XmlReader_getContent(testXmlReader);
  String* check2Name = String_newByRef("country");
  isPassed = isPassed && (node2==XMLATTRIBUTE);
  isPassed = isPassed && (String_compare(attribute2Name, check2Name)==0);

  String_delete(attribute2Name);
  String_delete(check2Name);

  UT_ASSERT((isPassed));
  
  PRINT2((logChannel, "Step 4: Test 3 - Read XML text content: "));
  node = XmlReader_read(testXmlReader);;
  while ((node != XMLTEXT) && (node!=XMLNONE))
  { 
    node = XmlReader_read(testXmlReader);
  } 
  String* textContent = XmlReader_getContent(testXmlReader);
  String* checkTextContent = String_newByRef("Alan Ackerman");
  isPassed = isPassed && (node==XMLTEXT);
  isPassed = isPassed && (String_compare(textContent, checkTextContent)==0);

  XmlReader_delete(testXmlReader);
  String_delete(testXmlString);

  UT_ASSERT((isPassed));

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
  isPassed = isPassed && UT_XmlReader_01_step4();

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(logChannel);

  return isPassed;
}

/* TODO:
High value — gaps in already-wired functionality:
-------------------------------------------------

[-] consumeAttribute with double quotes (id="123") — only single-quote path is covered

[-] Multiple attributes on one element (site='uk' id='124' in testXmlBuffer2 is right there but not walked through)

[X] getContent after XMLTEXT — the text content reading path is completely unverified

Medium value — error/edge paths:
--------------------------------

[-] Unterminated comment and unterminated multiline — these return different values (0 vs 1) but neither is tested

[-] Missing = in attribute and unquoted attribute value — both Error_new paths in consumeAttribute

[-] A test string with newlines to exercise the line/col tracking in consumeOneChar

Low value for now — stubs:
--------------------------

[-] copy, compare, print, getSize are all stubs so testing them adds little until they're implemented

*/
