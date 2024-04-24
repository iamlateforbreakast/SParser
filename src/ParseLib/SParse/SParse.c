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
#include "Configuration.h"
#include "Product.h"
#include "SdbMgr.h"
#include "Error.h"
#include "Grammar2.h"
#include "FileMgr.h"
#include "FileDesc.h"
#include "List.h"
#include "OptionMgr.h"
#include "Debug.h"

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
  Configuration * configuration;
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
  .f_print = (Printer)&SParse_print,
  .f_size = (Sizer)&SParse_getSize
};

static const SParseDefault SParse_default[] = 
{{"*.c",(Constructor)&Grammar2_new, (unsigned int (*)(void* g))&Grammar2_process,(Destructor)&Grammar2_delete},
 {"*.d",0,0,0/*&Grammar2_process*/}};

 #if 0
static const char * SParse_ignoreFiles[] = 
{"test1.c"};
#endif

PRIVATE unsigned int SParse_parseFile(SParse * this, FileDesc * fileDesc);

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
    Configuration_delete(this->configuration);
    SdbMgr_delete(this->sdbMgr);
    Object_deallocate(&this->object);
  }
}

/**********************************************//** 
  @brief Copy a SParse object instance.
  @public
  @memberof SParse
  @return Copy of instance.
**************************************************/
PUBLIC SParse * SParse_copy(SParse * this)
{
  SParse * copy = 0;
  
  return copy;
}

/**********************************************//** 
  @brief Print a SParse object.
  @public
  @memberof SParse
**************************************************/
PUBLIC void SParse_print(SParse * this)
{

}

PUBLIC unsigned int SParse_getSize(SParse * this)
{
  if (this == 0) return sizeof(SParse);

  return sizeof(SParse);
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
  
  OptionMgr* optionMgr = OptionMgr_getRef();
  FileDesc * fd = 0;
  
  String * productList = OptionMgr_getOption(optionMgr, "Product list");
  PRINT(("Product List = %s\n", String_getBuffer(productList)));
  if (productList)
  {
    this->configuration = Configuration_new(productList);

    if (this->configuration == 0) Error_new(ERROR_FATAL, "Cannot read configuration file.\n");

  // {
    // What does it mean?
  }
  List* products = Configuration_getProducts(this->configuration);
  
  Product* p;
  while ((p = List_getNext(products)) != 0)
  {
    FileMgr * fileMgr = Product_getSourceFiles(p);
  /* List all files with extension in all the input directories */
    List * fileList = FileMgr_filterFiles(fileMgr, extension);

  //List_forEach(fileList, (void (*)(void* , void *))&SParse_parseFile, (void*)this);
  while ((fd = List_getNext(fileList))!=0)
    SParse_parseFile(this, fd);
  FileMgr_delete(fileMgr);
    List_delete(fileList);
  }
  OptionMgr_delete(optionMgr);
  
  return result;
}

/**********************************************//** 
  @brief Parse a file.
  @memberof SParse
  @private
  @param Filename.
  @return Status of the operation.
**************************************************/
PRIVATE unsigned int SParse_parseFile(SParse * this, FileDesc * fileDesc)
{
  unsigned int error = 0;
  unsigned int i = 0;
  unsigned int nbRows = sizeof(SParse_default)/sizeof(SParseDefault);
  void * g = 0;
  /* 1) If fileName is to be ignored */
  PRINT(("SParse: Parsing %s\n", String_getBuffer(FileDesc_getName(fileDesc))));
  for (i=0; i<nbRows; i++)
  {
    if (String_matchWildcard(FileDesc_getName(fileDesc), SParse_default[i].extension))
    {
      FileReader * fileReader = FileReader_new(fileDesc);
      g = SParse_default[i].function_new(fileReader, this->sdbMgr);
      SParse_default[i].function_process(g);
      SParse_default[i].function_delete(g);
      FileReader_delete(fileReader);
    }
  }

  return error;
}
