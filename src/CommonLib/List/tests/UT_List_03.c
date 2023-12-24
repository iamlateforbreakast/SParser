/* UT_List_03.c */
#include "List.h"
#include "String2.h"
#include "Memory.h"
#include <stdlib.h>

#define NB_KEYS (10)
#define KEY_LENGTH (50)

int step1()
{
  int isPassed = 0;
  List * testList = List_new();
  String * key = 0;
  char * buffer = Memory_alloc(KEY_LENGTH);;

  for (int k=0; k<NB_KEYS;k++)
  {  
    for (int i=0; i<KEY_LENGTH; i++)
    {
      buffer[i] = 'a' + (rand() % 26); 
    }
    buffer[KEY_LENGTH] = 0;
    /* String_new copies the buffer */
    key = String_new(buffer);

    List_insertHead(testList, key, 1);
  }

  Memory_free(buffer, KEY_LENGTH) ;
  List_delete(testList);

  return isPassed;
}

void main()
{
  step1();
  Memory_report();
}