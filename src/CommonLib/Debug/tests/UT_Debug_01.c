/* UT_Debug_01.c */

#include "Debug.h"

unsigned int step1()
{
   #define DEBUG (1)
   TRACE(("DEBUG Flag was set to 1\n"));
   #undef DEBUG
   #define DEBUG (0)
   TRACE(("DEBUG Flag was undefined\n"));
   
   return 0;
}

unsigned int step2()
{
   PRINT(("Print Statement\n"));

   return 0;
}

unsigned int main()
{
   unsigned int result = 0;

   result |= step1();
   result |= step2();

   return result;
}

