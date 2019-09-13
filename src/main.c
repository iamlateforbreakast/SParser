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
#include "TimeMgr.h"
#include "Error.h"
#include "SParse.h"
#include "Memory.h"
#include "ObjectMgr.h"

#include <signal.h>

/**********************************************//** 
  @private
**************************************************/
struct sigaction action;

PRIVATE void sighandler(int signum, siginfo_t *info, void *ptr) ;

/**********************************************//** 
  @brief Inital entry point for SParse. This function creates
  @public
**************************************************/
PUBLIC int main(const int argc, const char** argv)
{
  SParse *sparse = 0;
  String * inputDir = 0;
  String *totalExecutionTime = 0;
  OptionMgr *optionMgr = OptionMgr_getRef();
  FileMgr *fileMgr = FileMgr_getRef();
  TimeMgr *timeMgr = TimeMgr_getRef();
  ObjectMgr * objMgr = ObjectMgr_getRef();
  
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
  OptionMgr_readFromFile(optionMgr);
  
  totalExecutionTime = String_new("Total Execuion Time");
  
  /* Add Directory to FileMgr */
  inputDir = OptionMgr_getOption(optionMgr, "Input Directory");

  FileMgr_setRootLocation(fileMgr, String_getBuffer(inputDir));
  FileMgr_addDirectory(fileMgr, ".");
  
  TimeMgr_latchTime(timeMgr, totalExecutionTime);
  
  sparse = SParse_new(OptionMgr_getOption(optionMgr, "DB Name"));
  
  SParse_parse(sparse, "*.c");
  
  TimeMgr_latchTime(timeMgr, totalExecutionTime);
  
  SParse_delete(sparse);
  
  /* Cleanup */
  //String_delete(inDir);
  String_delete(totalExecutionTime);
  OptionMgr_delete(optionMgr);
  FileMgr_delete(fileMgr);
  TimeMgr_delete(timeMgr);
  
  /* Generate Memory report */
  ObjectMgr_report(objMgr);
  
  ObjectMgr_delete(objMgr);
  
  Memory_report();
  
  return 0;
}

/**********************************************//** 
  @brief TBD
  @public
**************************************************/
PRIVATE void sighandler(int signum, siginfo_t *info, void *ptr) 
{ 
  Error_new(ERROR_NORMAL, "Received signal %d\n", signum); 
  Error_new(ERROR_FATAL, 
                  "Signal originates from process %lu\n",
                  (unsigned long)info->si_pid);
}