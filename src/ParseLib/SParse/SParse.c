/**********************************************//** 
  @file SParse.c
  
  @class SParse
  
  @brief This file contains the implementation for the class SParse
  
  The class SParse is TBD
**************************************************/

//#include "FileReader.h"
//#include "StreamParser.h"
//#include "SdbMgr.h"

typedef struct SParse
{
} SParse;

SParse * SParse_new(/* Sdb name */)
{
  SParse * result = 0;

  /* Initialise SdbMgr */
  
  return result;
}

unsigned int SParse_parse()
{
  unsigned int result = 0;
  
  /* FileMgr* fileMgr  = FileMgr_getFileMgr(); */
  /* List * fileList = new List(); */
  
  /* List all files with extension in all the input directories */
  /* FileMgr_listFiles(fileMgr); */

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
  /* FileReader * fileReader = FileReader_new(file); */
  
  /*   2) Create a StreamParser object */
  /*  StreamParser * streamParser = StreamParser_new(fileReader); */
  
  /*   3) If error in StreamParser_parse then exit */

  return result;
}
