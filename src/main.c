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
//#include "SdbMgr.h"
#include "StreamParser.h"

int main(int argc, char** argv)
{
  /* OptionMgr* optionMgr = OptionMgr_getOptionMgr(); */
  /* FileMgr* fileMgr = FileMgr_getFileMgr(); */
  
  /* Initialise OptionMgr from file */
  /* OptionMgr_readFromFile(optionMgr, "options.txt"); */

  /* Initialise OptionMgr from command line */
  /* OptionMgr_readFromCmdLine(optionMgr, argc, argv);
   
  /* Add Directory to FileMgr */
  /* FileMgr_addDirectory(OptionMgr_get("Input Directory")); */
 
  /* Generate Memory report */

  /* Dump error log */
  /* ErrorMgr_dump() */
  
  /* Cleanup */
  /* OptionMgr_delete(optionMgr); */
  /* FileMgr_delete(fileMgr); */

  return 0;
}
