/* UT_Ast_01.c */
#include "Configuration.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"
#include "Ast.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}

FILE * UT_Ast_01_logChannel;

int UT_Ast_01_step1()
{
  int isPassed = 1;

  PRINT(("Step 1: Test 1 - Create an Ast object: "));

  UT_ASSERT((0));

  return isPassed;
}

#ifdef MAIN
int main()
#else
int run_UT_Ast_01()
#endif
{
  int isPassed = 1;
  
  UT_Ast_01_logChannel = Debug_openChannel("UT_Ast_01.log");
  Debug_setStdoutChannel(UT_Ast_01_logChannel);

  isPassed = UT_Ast_01_step1() && isPassed;

  Memory_report();

  Debug_closeChannel(UT_Ast_01_logChannel);

  return isPassed;
}
