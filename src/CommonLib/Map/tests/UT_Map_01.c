#include "Map.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

#define NB_KEYS (250)

const char* text = "For a long time I used to go to bed early. Sometimes, when I had put out my candle,"
"my eyes would close so quickly that I had not even time to say I'm going to sleep. And half an hour"
"later the thought that it was time to go to sleep would awaken me; I would try to put away the book"
"which, I imagined, was still in my hands, and to blow out the light; I had been thinking all the time,"
"while I was asleep, of what I had just been reading, but my thoughts had run into a channel of their"
"own, until I myself seemed actually to have become the subject of my book : a church, a quartet, the"
"rivalry between François Iand Charles V. This impression would persist for some moments after I was"
"awake; it did not disturb my mind, but it lay like scales upon my eyesand prevented them from"
"registering the fact that the candle was no longer burning. Then it would begin to seem unintelligible,"
"as the thoughts of a former existence must be to a reincarnate spirit; the subject of my book would"
"separate itself from me, leaving me free to choose whether I would form part of it or no;and at the"
"same time my sight would return and I would be astonished to find myself in a state of darkness,"
"pleasantand restful enough for the eyes, and even more, perhaps, for my mind, to which it appeared"
"incomprehensible, without a cause, a matter dark indeed.";

String * keys[NB_KEYS];
TestObject * testObjects[NB_KEYS];
int nbTokens = 0;

int init_keys()
{
  String * fullText = String_new(text);
  List * tokens = 0;

  tokens = String_splitToken(fullText, " ");

  for (int i = 0; ((i < List_getNbNodes(tokens)) && (i< NB_KEYS)); i++)
  {
    keys[i] = (String*)List_getNext(tokens);
    testObjects[i] = TestObject_new();
    nbTokens++;
  }

  List_delete(tokens);
  String_delete(fullText);
}

int delete_keys()
{
  for (int i = 0; i < nbTokens; i++)
  {
    TestObject_delete(testObjects[i]);
    String_delete(keys[i]);
  }
}

int step1()
{
  Map* testMap = 0;

  Memory_report();

  PRINT(("Step 1: Test 1 - Build a Map: "));

  testMap = Map_new();

  UT_ASSERT((1));

  PRINT(("Step 1: Test 2 - Insert an object: "));

  Map_insert(testMap, keys[0], testObjects[0]);

  UT_ASSERT((1));

  PRINT(("Step 1: Test 3 - Delete the Map: "));
  Map_delete(testMap);

  UT_ASSERT((1));

  Memory_report();

  return 0;
}

int step2()
{
  Map * testMap = Map_new();
  
  for (int i = 0; i < 10; i++)
  {
    printf("-->%s\n", String_getBuffer(keys[i]));
    Map_insert(testMap, keys[i], testObjects[i]);
  }
  
  Map_delete(testMap);

  Memory_report();

  return 0;
}

int step3()
{
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = 0;
  
  Map_find(testMap, s, (void**)&item);
 
  printf("Value : %s\n", String_getBuffer(item));

  String_delete(s);
  
  Map_delete(testMap);

  return 0;
}

int step4()
{
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = String_new("The new value");
  String * newItem = 0;
  
  Map_insert(testMap, s, item);
  Map_find(testMap, s, (void**)&newItem);
  
  printf("New value : %s\n",
          String_getBuffer(newItem));
  
  Map_delete(testMap);

  return 0;
}

int step5()
{
  int i = 0;
  String * s = 0;
  String * c = 0;
  List * l = 0;
  Map* testMap = Map_new();

  const char * testNames[] =
  {
    "banana", "strawberry", "cherry", "apple", "orange", "pear", "blackberry"
  };

  const char * testColor[] =
  {
    "yellow", "red", "red", "yellow", "orange", "green",    "black"
  };

  for (i=0; i<sizeof(testNames)/sizeof(const char *); i++)
  {
    s = String_new(testNames[i]);
    c = String_new(testColor[i]);
    Map_insert(testMap, s, c);
  }

  l = Map_getAll(testMap);

  List_delete(l);

  Map_delete(testMap);

  return 0;
}

int step6()
{
  
  return 0;
}

int main()
{
  init_keys();

  step1();
  step2();
  step3();
  step4();
  step5();
  step6();

  delete_keys();

  Memory_report();

  return 0;
}
