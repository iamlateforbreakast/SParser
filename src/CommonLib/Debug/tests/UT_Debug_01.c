/* UT_Debug_01.c */

#include "Types.h"
#include "Debug.h"

PRIVATE unsigned int UT_Debug_01_step1()
{
   #define DEBUG (1)
   TRACE(("DEBUG Flag was set to 1\n"));
   #undef DEBUG
   #define DEBUG (0)
   TRACE(("DEBUG Flag was undefined\n"));
   
   return 1;
}

PRIVATE unsigned int UT_Debug_01_step2()
{
   PRINT(("Print Statement\n"));

   return 1;
}

PUBLIC int run_UT_Debug_01()
{
   int isPassed = 1;

   isPassed = isPassed && UT_Debug_01_step1();
   isPassed = isPassed && UT_Debug_01_step2();

   return isPassed;
}
