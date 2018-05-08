/**********************************************//** 
  @file SParse.c
  
  @class SParse
  
  @brief This file contains the implementation for the class SParse
  
  The class SParse parses all files with extension
  and stores the result of the parsing in the SQLite DB name.
**************************************************/

#include "FileReader.h"
#include "NodeTree.h"
#include "Object.h"
#include "SParse.h"

struct SParse
{
  Object * object;
  char * extension;
  char * sdbName;
};

/*
{"*.c", Grammar1_parse}
*/

SParse *SParse_new(/* Sdb name */)
{
  SParse * result = 0;

  /* Initialise SdbMgr */
  
  return result;
}

unsigned int SParse_parse(/* extension */)
{
  unsigned int result = 0;
  
  /* FileMgr* fileMgr  = FileMgr_getFileMgr(); */
  /* List * fileList = new List(); */
  
  /* List all files with extension in all the input directories */
  /* FileMgr_listFiles(fileMgr, extension); */

  /* List_forEach(fileList, &SParseFile, this); */
  
  return result;  
}
 
void SParse_delete(SParse * this)
{
}

unsigned int SParse_parseFile(SParse * this /* , Filename * file */ )
{
  unsigned int result = 0;
  
  /*   1) Create a FileReader object */
  /*     FileReader * fileReader = FileReader_new(file); */
  
  /*   2) Create a StreamParser object */
  /*     NodeTree_new(fileReader, Grammar, SdBMgr); */
  /*     NodeTree_generate(); */
  /*     NodeTree_delete(); */
  
  /*   3) If error in StreamParser_parse then exit */

  return result;
}
