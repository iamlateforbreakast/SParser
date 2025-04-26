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

int UT_TransUnit_01_step1()
{
  int isPassed = 1;

  Buffer *testBuffer;

  PRINT(("Step 1: Test 1 - Create an instance of class Buffer: "));
  testBuffer = Buffer_new();
  isPassed = (testBuffer != 0) && isPassed;
  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 2 - Delete an instance of class Buffer: "));
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
  //isPassed = UT_TransUnit_01_step2() && isPassed;
  //isPassed = UT_TransUnit_01_step3() && isPassed;
  //isPassed = UT_TransUnit_01_step4() && isPassed;
  //isPassed = UT_TransUnit_01_step5() && isPassed;

  Memory_report();

  Debug_closeChannel(UT_Buffer_01_logChannel);
  return isPassed;
}