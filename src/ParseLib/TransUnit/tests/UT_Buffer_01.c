/* UT_Buffer_01.c */
#include "Buffer.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (1)
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}

PRIVATE FILE * UT_Buffer_01_logChannel;

struct InspectBuffer
{
  Object object;
  char* content;
  char* readPtr;
  int nbCharRead;
  char* writePtr;
  int nbCharWritten;
  int size;
};

int UT_Buffer_01_step1()
{
  int isPassed = 1;

  Buffer *testBuffer;

  PRINT(("Step 1: Test 1 - Create an instance of class Buffer: "));
  testBuffer = Buffer_new();
  struct InspectBuffer* inspector = (struct InspectBuffer*)testBuffer;

  isPassed = (OBJECT_IS_VALID(testBuffer)) && isPassed;
  isPassed = (sizeof(inspector) == sizeof(testBuffer)) && isPassed;
  isPassed = (inspector->readPtr == inspector->content) && isPassed;
  isPassed = (inspector->writePtr == inspector->content) && isPassed;
  isPassed = (inspector->nbCharWritten == 0) && isPassed;
  isPassed = (inspector->nbCharRead == 0) && isPassed;
  isPassed = (inspector->size == 1024) && isPassed;
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 2 - Write to instance of class Buffer: "));
  Buffer_writeNChar(testBuffer, "ABCD", 4);
  isPassed = (inspector->nbCharRead == 0) && isPassed;
  isPassed = (inspector->nbCharWritten == 4) && isPassed;
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 3 - Read ABCD from an instance of class Buffer: "));
  Buffer_accept(testBuffer, "ABCD");
  isPassed = (inspector->nbCharRead == 4) && isPassed;
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 4 - Delete an instance of class Buffer: "));
  Buffer_delete(testBuffer);
  isPassed = (OBJECT_IS_INVALID(testBuffer)) && isPassed;
  UT_ASSERT((isPassed));

  return isPassed;
}

#ifdef MAIN
int main()
#else
int run_UT_Buffer_01()
#endif
{
  int isPassed = 1;
  
  UT_Buffer_01_logChannel = Debug_openChannel("UT_Buffer_01.log");
  Debug_setStdoutChannel(UT_Buffer_01_logChannel);
  isPassed = UT_Buffer_01_step1() && isPassed;

  Memory_report();

  Debug_closeChannel(UT_Buffer_01_logChannel);
  return isPassed;
}
