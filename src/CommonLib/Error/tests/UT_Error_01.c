/* UT_Error_01.c */

#include "Error.h"

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(()"Failed\n")); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n"));} \
                          else { PRINT(()"\x1b[31mFailed\x1b[0m\n")); return 0;}
#endif
                          
unsigned int nbMessages = 1;

PRIVATE int UT_Error_01_step1()
{
   Error_new(ERROR_DBG, "Message %d: This is a Debug message\n", nbMessages);

   nbMessages++;   
   Error_new(ERROR_INFO, "Messages %d: This is a Info message\n", nbMessages);

   nbMessages++;   
   Error_new(ERROR_NORMAL, "Messages %d: This is a Normal message\n", nbMessages);
   return 0;
}

PRIVATE int UT_Error_01_step2()
{
   nbMessages++;   
   Error_new(ERROR_FATAL, "Messages %d: This is a Fatal message\n", nbMessages);
   return 0;
}

PUBLIC int run_UT_Error_01()
{
   unsigned int isPassed = 1;

   isPassed = isPassed && UT_Error_01_step1();
   isPassed = isPassed && UT_Error_01_step2();

   return isPassed;
}
