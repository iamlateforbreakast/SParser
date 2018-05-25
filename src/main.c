/**********************************************//** 
  @file main.c
     
  @brief This file initialises:
  1) The OptionMgr object
  2) The FileMgr object
        
  This file contains only one function main() 
  which initialises the OptionMgr and FileMgr objects.
  The function also processes each source file in turn.
**************************************************/

#include "OptionMgr.h"
#include "FileMgr.h"
//#include "ErrorMgr.h"
#include "SParse.h"
#include <signal.h>

struct sigaction action;


void sighandler(int signum, siginfo_t *info, void *ptr) 
{ 
  printf("Received signal %d\n", signum); 
  printf("Signal originates from process %lu\n", 
  (unsigned long)info->si_pid); 
} 

int main(int argc, char** argv)
{
  SParse *sparse = NULL;
  
  /* ErrorMgr *errMgr = ErrorMgr_getErrorMgr(); */
  /* OptionMgr *optionMgr = OptionMgr_getOptionMgr(); */
  /* FileMgr *fileMgr = FileMgr_getFileMgr(); */
  
  /* Initialise OptionMgr from command line */
  /* OptionMgr_readFromCmdLine(optionMgr, argc, argv); */
  
  /* Initialise OptionMgr from file */
  /* OptionMgr_readFromFile(optionMgr, "options.txt"); */
   
  /* Add Directory to FileMgr */
  /* FileMgr_addDirectory(OptionMgr_get(optionMgr, "Input Directory")); */

  /* SParse_new(OptionMgr_get("SDB name")); */
  
  /* SParse_parse("*.c"); */
  
  /* SParse_delete(sparse); */
  
  /* Generate Memory report */

  /* Dump error log */
  /* ErrorMgr_dump() */
  
  /* Cleanup */
  /* OptionMgr_delete(optionMgr); */
  /* FileMgr_delete(fileMgr); */
  /* ErrorMgr_delete(errorMgr); */
  action.sa_sigaction = sighandler;
  action.sa_flags = SA_SIGINFO;
 
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGSEGV, &action, NULL);
  sigaction(SIGSEGV, &action, NULL);
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGHUP, &action, NULL);

  int *test = 0;

  *test = 1;;
  sleep(100);
  return 0;
}
