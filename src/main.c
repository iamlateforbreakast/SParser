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
#include "TimeMgr.h"

#include <signal.h>

/**********************************************//** 
  @private
**************************************************/
struct sigaction action;

PRIVATE void sighandler(int signum, siginfo_t *info, void *ptr) ;
PRIVATE void start_application(String * inputDir, String * dbName);
PRIVATE void print_usage();

/**********************************************//** 
  @brief Inital entry point for SParse. This function creates
  @public
**************************************************/
PUBLIC int main(const int argc, const char** argv)
{
  OptionMgr *optionMgr = OptionMgr_getRef();  
  ObjectMgr * objMgr = ObjectMgr_getRef();
  String * inputDir = 0;
  String * dbName = 0;
  
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
  
  if (OptionMgr_isOptionEnabled(optionMgr, "Print help"))
  {
    print_usage();
    OptionMgr_delete(optionMgr);
    ObjectMgr_delete(objMgr);
  }
  else
  {
    /* Initialise OptionMgr from file */
    OptionMgr_readFromFile(optionMgr);
    
    /* Retrieve database name */
    dbName = OptionMgr_getOption(optionMgr, "DB Name");
    
    /* Add Directory to FileMgr */
    inputDir = OptionMgr_getOption(optionMgr, "Input Directory");
    
    start_application(inputDir, dbName);
    
    OptionMgr_delete(optionMgr);
  
    ObjectMgr_report(objMgr);
    ObjectMgr_delete(objMgr);
    
    /* Generate Memory report */
    Memory_report();    
  }
  
  
  return 0;
}

/**********************************************//** 
  @brief TBD
  @public
**************************************************/
PRIVATE void start_application(String * inputDir, String * dbName)
{
  SParse *sparse = 0;
  String *totalExecutionTime = 0;
  FileMgr *fileMgr = FileMgr_getRef();
  TimeMgr *timeMgr = TimeMgr_getRef();
  
  totalExecutionTime = String_new("Total Execution Time");
    
  FileMgr_setRootLocation(fileMgr, String_getBuffer(inputDir));
  FileMgr_addDirectory(fileMgr, ".");
  
  TimeMgr_latchTime(timeMgr, totalExecutionTime);
  
  sparse = SParse_new(dbName);
  
  SParse_parse(sparse, "*.c");
  
  TimeMgr_latchTime(timeMgr, totalExecutionTime);
  
  SParse_delete(sparse);
  
  TimeMgr_report(timeMgr);
    
  /* Cleanup */
  TimeMgr_delete(timeMgr);
  String_delete(totalExecutionTime);
  FileMgr_delete(fileMgr);
}

/**********************************************//** 
  @brief TBD
  @public
**************************************************/
PRIVATE void print_usage()
{
  printf("Usage: sparse [OPTION]\n");
  printf("   Run SParse in the current directory.\n");
  printf("\n");
  printf("-o\t\tSQLite database Name\n");
  printf("-d\t\tInput directory\n");
  printf("-c\t\tConfiguration file name\n");
  printf("-help\t\tDisplay this help and exit\n");
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
