/* UT_Ast_01.c */
#include "Configuration.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}

int UT_Ast_01_step1()
{
  int isPassed = 1;

  return isPassed;
}

int run_UT_Ast_01()
{
  int isPassed = 1;
  
  isPassed = UT_Ast_01_step1() && isPassed;

  Memory_report();

  return isPassed;
}
