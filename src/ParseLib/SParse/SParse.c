/**********************************************//** 
  @file SParse.c
  
  @brief This file contains the implementation for the class SParse
  
  The class SParse parses all files with extension .X
  and stores the result of the parsing in the SQLite DB name.
**************************************************/
#include "SParse.h"
#include "Class.h"
#include "Object.h"
#include "FileReader.h"
#include "SdbMgr.h"
#include "Error.h"
#include "Grammar2.h"
#include "FileMgr.h"
#include "List.h"

/**********************************************//** 
  @private
**************************************************/
typedef struct SParseDefault
{
  char* extension;
  //void * (*function_new)(FileReader * fr, SdbMgr * sdbMgr);
  Constructor function_new;
  unsigned int (*function_process)(void * g);
  //void (*function_delete)(void * g);
  Destructor function_delete;
} SParseDefault;

/**********************************************//** 
  @class SParse
**************************************************/
struct SParse
{
  Object object;
  char * extension;
  SdbMgr * sdbMgr;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class sparseClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&SParse_delete,
  .f_copy = (Copy_Operator)&SParse_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0
};

static const SParseDefault SParse_default[] = 
{{"*.c",(Constructor)&Grammar2_new, &Grammar2_process,(Destructor)&Grammar2_delete},
 {"*.d",0,0,0/*&Grammar2_process*/}};

 #if 0
static const char * SParse_ignoreFiles[] = 
{"test1.c"};
#endif

PRIVATE unsigned int SParse_parseFile(SParse * this, String * file);
PRIVATE void SParse_buildPreferredDirList(SParse * this, const char * extension);

/**********************************************//** 
  @brief Create a new SParse object.
  @public
  @memberof SParse
  @return New SParse object.
**************************************************/
PUBLIC SParse *SParse_new(String * sdbName)
{
  SParse * this = 0;

  this = (SParse*)Object_new(sizeof(SParse), &sparseClass);
  this->object.size = sizeof(SParse);
  
  /* Initialise SdbMgr */
  this->sdbMgr = SdbMgr_new(sdbName);
  
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
  if (this!=0)
  {
    SdbMgr_delete(this->sdbMgr);
    Object_delete(&this->object);
  }
}

PUBLIC SParse * SParse_copy(SParse * this)
{
  SParse * copy = 0;
  
  return copy;
}

/**********************************************//** 
  @brief Parse all files with a given extension.
  @memberof SParse
  @param[in] extension Extension of the files to parse.
  @return Status of the operation.
**************************************************/
PUBLIC unsigned int SParse_parse(SParse * this, const char * extension)
{
  unsigned int result = 0;
  
  FileMgr* fileMgr  = FileMgr_getRef();
  String * fileName = 0;
  List * fileList = 0;
  
  /* List all files with extension in all the input directories */
  fileList = FileMgr_filterFiles(fileMgr, extension);

  //List_forEach(fileList, (void (*)(void* , void *))&SParse_parseFile, (void*)this);
  while ((fileName = list_getNext(fileList))!=0)
    SParse_parseFile(this, fileName);
  FileMgr_delete(fileMgr);
  List_delete(fileList);
  
  return result;
}

/**********************************************//** 
  @brief Parse a file.
  @memberof SParse
  @private
  @param Filename.
  @return Status of the operation.
**************************************************/
PRIVATE unsigned int SParse_parseFile(SParse * this, String * file)
{
  unsigned int error = 0;
  unsigned int i = 0;
  unsigned int nbRows = sizeof(SParse_default)/sizeof(SParseDefault);
  void * g = 0;
  /* 1) If fileName is to be ignored */
  for (i=0; i<nbRows; i++)
  {
    if (String_matchWildcard(file, SParse_default[i].extension))
    {
      FileReader * fileReader = FileReader_new(file);
      g = SParse_default[i].function_new(fileReader, this->sdbMgr);
      SParse_default[i].function_process(g);
      SParse_default[i].function_delete(g);
      FileReader_delete(fileReader);
    }
  }

  return error;
}

PRIVATE void SParse_buildPreferredDirList(SParse * this, const char * extension)
{
}

#if 0
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
#endif

