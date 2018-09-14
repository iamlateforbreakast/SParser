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
#include "Error.h"
#include "SParse.h"
#include <signal.h>

struct sigaction action;


void sighandler(int signum, siginfo_t *info, void *ptr) 
{ 
  Error_new(ERROR_NORMAL, "Received signal %d\n", signum); 
  Error_new(ERROR_NORMAL, "Signal originates from process %lu\n", (unsigned long)info->si_pid); 
} 

int main(int argc, char** argv)
{
  SParse *sparse = 0;
  String * inDir = 0;
  OptionMgr *optionMgr = OptionMgr_getRef();
  FileMgr *fileMgr = FileMgr_getRef();
  
  action.sa_sigaction = sighandler;
  action.sa_flags = SA_SIGINFO;
 
  sigaction(SIGTERM, &action, 0);
  sigaction(SIGSEGV, &action, 0);
  sigaction(SIGSEGV, &action, 0);
  sigaction(SIGINT, &action, 0);
  sigaction(SIGTERM, &action, 0);
  sigaction(SIGHUP, &action, 0);


  
  /* Initialise OptionMgr from command line */
  OptionMgr_readFromCmdLine(optionMgr, argc, argv);
  
  /* Initialise OptionMgr from file */
  /* OptionMgr_readFromFile(optionMgr, "options.txt"); */
   
  /* Add Directory to FileMgr */
  inDir = OptionMgr_getOption(optionMgr, "Input Directory");
  FileMgr_addDirectory(fileMgr, String_getBuffer(inDir));
  
  sparse = SParse_new(OptionMgr_getOption(optionMgr, "DB Name"));
  
  SParse_parse(sparse, "*.c");
  
  SParse_delete(sparse);
  
  /* Cleanup */
  //String_delete(inDir);
  OptionMgr_delete(optionMgr);
  FileMgr_delete(fileMgr);

  /* Generate Memory report */
  Memory_report();
  
  return 0;
}
