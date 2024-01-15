/* UT_Error_01.c */

#include "Error.h"

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("\x1b[32mPassed\x1b[0m\n");} \
                          else { printf("\x1b[31mFailed\x1b[0m\n"); return 0;}
#endif
                          
unsigned int nbMessages = 1;

unsigned int step1()
{
   Error_new(ERROR_DBG, "Message %d: This is a Debug message\n", nbMessages);

   nbMessages++;   
   Error_new(ERROR_INFO, "Messages %d: This is a Info message\n", nbMessages);

   nbMessages++;   
   Error_new(ERROR_NORMAL, "Messages %d: This is a Normal message\n", nbMessages);
   return 0;
}

unsigned int step2()
{
   nbMessages++;   
   Error_new(ERROR_FATAL, "Messages %d: This is a Fatal message\n", nbMessages);
   return 0;
}

unsigned int main()
{
   unsigned int result = 0;

   result |= step1();
   result |= step2();

   return result;
}
