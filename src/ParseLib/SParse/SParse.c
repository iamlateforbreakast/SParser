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

/**********************************************//** 
  @private
**************************************************/
typedef struct SParseDefault
{
  char* extension;
  unsigned int (*function_parse)(/* FileReader, SdbMgr */);
} SParseDefault;

/**********************************************//** 
  @class SParse
**************************************************/
struct SParse
{
  Object object;
  char * extension;
  char * sdbName;
};

static const SParseDefault SParse_default[] = 
{{"*.c",0/*&Grammar1_process*/},
 {"*.c",0/*&Grammar2_process*/}};

static const char * SParse_ignoreFiles[] = 
{"test1.c"};

/**********************************************//** 
  @brief Create a new SParse object.
  @public
  @memberof SParse
  @return New SParse object.
**************************************************/
PUBLIC SParse *SParse_new(/* Sdb name */)
{
  SParse * this = 0;

  this = (SParse*)Object_new(sizeof(SParse),(Destructor)&SParse_delete, (Copy_operator)&SParse_copy);
  this->object.size = sizeof(SParse);
  
  /* Initialise SdbMgr */
  
  return this;
}

/**********************************************//** 
  @brief Delete a SParse object.
  @public
  @memberof SParse
  @param Object to delete.
**************************************************/
PUBLIC void SParse_delete(SParse * this)
{
    Object_delete(&this->object);
}

PUBLIC SParse * SParse_copy(SParse * this)
{
  SParse * copy = 0;
  
  copy = Object_copy(&this->object);
  
  return copy;
}

/**********************************************//** 
  @brief Parse all files with a given extension.
  @memberof SParse
  @param[in] extension Extension of the files to parse.
  @return Status of the operation.
**************************************************/
PUBLIC unsigned int SParse_parse(SParse * this /* extension */)
{
  unsigned int result = 0;
  
  /* FileMgr* fileMgr  = FileMgr_getFileMgr(); */
  /* List * fileList = new List(); */
  
  /* List all files with extension in all the input directories */
  /* FileMgr_listFiles(fileMgr); */

  /* List_forEach(fileList, &SParseFile, this); */
  
  return result;  
}

/**********************************************//** 
  @brief Parse a file.
  @memberof SParse
  @private
  @param Filename.
  @return Status of the operation.
**************************************************/
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

/**********************************************//** 
  @brief Check if a file should be ignored..
  @memberof SParse
  @private
  @param Filename.
  @return Status of the operation.
**************************************************/
PRIVATE unsigned int SParse_isFileIgnored(SParse * this /* FileName * file */)
{
  unsigned int result = 0;
  
  /* Check if a file needs to be ignored */
  
  return result;
}

