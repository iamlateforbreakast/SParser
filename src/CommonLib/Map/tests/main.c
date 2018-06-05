#include "Map.h"
#include "String.h"
#include "ObjectMgr.h"

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
  
  Map_insert(testMap, s, item);
  
  return 0;
}

int step3()
{
  String * s = String_new("Hello world");
  String * item = 0;
  
  Map_find(testMap, s, &item);
  
  return 0;
}

int step4()
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
  
  return 0;
}