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
  int i = 0;
  String * s = 0;
  String * c = 0;
  List * l = 0;
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

  return 0;
}

int step6()
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
  step6();

  Memory_report();

  return 0;
}
