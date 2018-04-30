/**********************************************//** 
  @file SParse.c
  
  @class SParse
  
  @brief This file contains the implementation for the class SParse
  
  The class SParse parses all files with extension .X
  and stores the result of the parsing in the SQLite DB name.
**************************************************/

#include "Object.h"
#include "FileReader.h"
#include "SdbMgr.h"
//#include "ErrorMgr.h"
//#include "Grammar1.h"
#include "SParse.h"

typedef struct SParseDefault
{
  char* extension;
  unsigned int (*function_parse)(/* FileReader, SdbMgr */);
} SParseDefault;

struct SParse
{
  Object object;
  char * extension;
  char * sdbName;
};

static const SParseDefault SParse_default[] = {{"*.c",0/*&Grammar1_process*/},
                                               {"*.c",0/*&Grammar2_process*/}};

static const char * SParse_ignoreFiles[] = {"test.c"};

PUBLIC SParse *SParse_new(/* Sdb name */)
{
  SParse * result = 0;

  /* Initialise SdbMgr */
  
  return result;
}

PUBLIC void SParse_delete(SParse * this)
{
}

PUBLIC unsigned int SParse_parse(/* extension */)
{
  unsigned int result = 0;
  
  /* FileMgr* fileMgr  = FileMgr_getFileMgr(); */
  /* List * fileList = new List(); */
  
  /* List all files with extension in all the input directories */
  /* FileMgr_listFiles(fileMgr); */

  /* List_forEach(fileList, &SParseFile, this); */
  
  return result;  
}

PRIVATE unsigned int SParse_parseFile(SParse * this /* , Filename * file */ )
{
  unsigned int error = 0;
  /* 1) If fileName is to be ignored */
  
  /* 2) Create a FileReader object */
  /*   FileReader * fileReader = FileReader_new(fileName); */
  
  /* 3) Parse file with Grammar and stores in DB */
  /*   Grammar1_process(FileReader, sdbMgr); */
  
  /* 4) If error in StreamParser_parse then exit */

  return error;
}

PRIVATE unsigned int SParse_isFileIgnored(SParse * this /* FileName * file */)
{
  unsigned int result = 0;
  
  /* Check if a file needs to be ignored */
  
  return result;
}

