#include "Map.h"
#include "String2.h"
#include "ObjectMgr.h"
#include "Memory.h"

Map * testMap = 0;

int step1()
{
  testMap = Map_new();
  
  return 0;
}

int step2()
{
  String * s = String_new("Hello world");
  String * item = String_new("The value");
  
  Map_insert(testMap, s, (void**)item);
  
  return 0;
}

int step3()
{
  String * s = String_new("Hello world");
  String * item = 0;
  
  Map_find(testMap, s, (void**)&item);
 
  printf("Value : %s\n", String_getBuffer(item));

  String_delete(s);
  
  return 0;
}

int step4()
{
  String * s = String_new("Hello world");
  String * item = String_new("The new value");
  String * newItem = 0;
  
  Map_insert(testMap, s, item);
  Map_find(testMap, s, (void**)&newItem);
  
  printf("New value : %s\n",
          String_getBuffer(newItem));
  
  return 0;
}

int step5()
{
  Map_delete(testMap);
  
  return 0;
}

int main()
{
  step1();
  step2();
  step3();
  step4();
  step5();

  Memory_report();

  return 0;
}
