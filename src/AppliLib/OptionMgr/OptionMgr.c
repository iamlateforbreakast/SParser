/**********************************************//** 
  @file OptionMgr.c
  
  @brief The OptionMgr class manages the application configuration.
  
  The class OptionMgr is TBD
**************************************************/

#include "OptionMgr.h"
#include "Class.h"
#include "Object.h"
#include "String2.h"
#include "Map.h"
#include "FileMgr.h"
#include "Memory.h"
#include "Error.h"

#include "Debug.h"
/**********************************************//**
  @class OptionMgr
**************************************************/
struct OptionMgr
{
  Object object;
  Map * options;
};

struct OptionDefault{
  char * name;
  char * flag;
  char * value;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class optionMgrClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&OptionMgr_delete,
  .f_copy = (Copy_Operator)&OptionMgr_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0,
  .f_size = (Sizer)&OptionMgr_getSize
};

/**********************************************//**
  @private
**************************************************/
PRIVATE OptionMgr * optionMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE const struct OptionDefault optionDefault[] = 
{
  {"DB Name","-o","test.db"},
  {"Input Directory","-d","."},
  {"Config file name","-c","sparse.txt"},
  {"Print help", "-help", 0}
};

PRIVATE unsigned int OptionMgr_parseFile(OptionMgr * this, String * fileContent);

/**********************************************//** 
  @brief TBD
  @private
  @memberof OptionMgr
**************************************************/
PRIVATE OptionMgr * OptionMgr_new()
{
  OptionMgr * this = 0;
  int j = 0;
  String * optionName = 0;
  String * optionValue = 0;
  const int nbOptions = sizeof(optionDefault)/sizeof(struct OptionDefault);

  this = (OptionMgr*)Object_new(sizeof(OptionMgr), &optionMgrClass);

  if (OBJECT_IS_INVALID(this)) return 0;
  
  this->options = Map_new();
  
  for (j=0; j<nbOptions; j++)
  {
    optionName = String_new(optionDefault[j].name);
    optionValue = String_new(optionDefault[j].value);
    Map_insert(this->options, optionName, optionValue, 1);
  }

  return this;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC void OptionMgr_delete(OptionMgr * this)
{
  if (OBJECT_IS_INVALID(this)) return;
  
  if (this->object.refCount == 1)
  {
    Map_delete(this->options);
    Object_deallocate(&this->object);
    optionMgr = 0;
  }
  else if (this->object.refCount > 1)
  {
    this->object.refCount = this->object.refCount - 1;
  } 
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC OptionMgr * OptionMgr_copy(OptionMgr * this)
{
  /* TODO: refcount++ */
  return this;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC OptionMgr* OptionMgr_getRef()
{ 
  if (optionMgr==0)
  {
    optionMgr = OptionMgr_new();
  }
  else
  {
    optionMgr->object.refCount++;
  }
  
  return optionMgr;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC unsigned int OptionMgr_getSize(OptionMgr * this)
{
  if (this==0) return sizeof(OptionMgr);

  return sizeof(OptionMgr);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC String * OptionMgr_getOption(OptionMgr * this, const char * name)
{
  String * result = 0;
  String * optionName = String_newByRef(name);

  Map_find(this->options, optionName, (void**)&result);
 
  String_delete(optionName);

  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC void OptionMgr_setOption(OptionMgr * this, const char * optionName, String * value)
{
  String * option = String_new(optionName);
  /* find optionName in this->map */
  /* if not found insert value */
  Map_insert(this->options, option, value, 1);
  /* else modify value found in map */
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC unsigned int OptionMgr_readFromFile(OptionMgr * this)
{
  unsigned int result = 0;
  FileMgr * fileMgr = FileMgr_new();
  String * fileName = 0;
  String * fileContent = 0;
  String * path1 = 0;
  
  fileName = OptionMgr_getOption(this,"Config file name");
  path1 = String_copy(fileName);
#ifdef _WIN32
  String_prepend(path1,".\\");
#else
  String_prepend(path1,"./");
#endif
  if (FileMgr_addFile(fileMgr, String_getBuffer(path1)) != 0)
  {
    /* File exists and is managed */
    fileContent = FileMgr_load(fileMgr, String_getBuffer(fileName));
    OptionMgr_parseFile(this, fileContent);
  }
  /* TODO: Try home directory */
  
  String_delete(path1);
  String_delete(fileContent);
  FileMgr_delete(fileMgr);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof OptionMgr
  @param[in] argc Number of commandline arguments.
  @param[in] argv List os commandline arguments.
  @return Status of operation.
**************************************************/
PUBLIC unsigned int OptionMgr_readFromCmdLine(OptionMgr * this, const int argc, const char ** argv)
{
  unsigned int result = 0;
  int i = 0, j = 0;
  String * optionName = 0;
  String * optionValue = 0;
  unsigned int isFound = 0;
  
  const int nbOptions = sizeof(optionDefault)/sizeof(struct OptionDefault);

  for (i=1; i<argc; i++)
  {
    for (j=0; j<nbOptions; j++)
    {
      if (Memory_cmp(optionDefault[j].flag, (void*)argv[i])==0)
      {
        optionName = String_new(optionDefault[j].name);
        if (((i+1)<=argc))
        {
          if (optionDefault[j].value!=0)
          {
            optionValue = String_new(argv[i+1]);
          }
          else
          {
            optionValue = String_new("Yes");
          }
          i++;  
        }
        isFound = 1;
        break;
      }
    }
    if ((isFound) && (optionValue!=0))
    {
      Map_insert(this->options, optionName, optionValue, 1);
    }
    else
    {
      /* Error case: Illegal arg */
      Error_new(ERROR_FATAL, "Illegal argument\n");
    }
  }
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @private
  @memberof OptionMgr
**************************************************/
PUBLIC unsigned int OptionMgr_isOptionEnabled(OptionMgr* this, const char * optionName)
{
  unsigned int result = 0;
  String* enabled = 0;
  String* yes = String_new("Yes");
  
  enabled = OptionMgr_getOption(this, optionName);
  if (String_compare(enabled,yes)==0) result = 1;
  
  String_delete(yes);
  //String_delete(enabled);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @private
  @memberof OptionMgr
**************************************************/
PRIVATE unsigned int OptionMgr_parseFile(OptionMgr * this, String * fileContent)
{
  unsigned int result = 0;
  char * p = 0;
  unsigned int idx = 0;
  unsigned int length =0;
  String * optionName = 0;
  String * optionValue = 0;
  unsigned int state = 0;
  
  for (p = String_getBuffer(fileContent);
       p < String_getBuffer(fileContent)+String_getLength(fileContent);
       p++)
  {
    switch(state)
    {
      case 0:
        if (*p=='[') 
        {
          state = 1;
          idx = p - String_getBuffer(fileContent) + 1;
          length = 0;
        }
        break;
      case 1:
        if (*p==']') 
        {
          state = 2;
          optionName = String_subString(fileContent, idx, length);
          PRINT(("Using Option %s = ", String_getBuffer(optionName)));
        }
        else length++;
        break;
      case 2:
        if (*p == '{')
          {
            // Single line
          state = 3;
          idx = p - String_getBuffer(fileContent) + 1;
          length = 0;
        }
        break;
      case 3:
        if (*p == '}')
        {
          state = 0;
          /* TODO: Case of windows file */
          optionValue = String_subString(fileContent, idx, length);
          PRINT(("%s\n", String_getBuffer(optionValue)));
          OptionMgr_setOption(optionMgr, String_getBuffer(optionName), optionValue);
          String_delete(optionName);
        }
        else
        {
          length++;
        }
        break;
      default:
        /* Error case : */
        break;
      }
    }
    if ((state == 1) || (state == 2))
    {
      /* Error case: Syntax error */
    }
    else if (state == 3)
    {
      optionValue = String_subString(fileContent, idx, length);
      OptionMgr_setOption(optionMgr, String_getBuffer(optionName), optionValue);
      String_delete(optionName);
    }
  
  
  return result;
}
