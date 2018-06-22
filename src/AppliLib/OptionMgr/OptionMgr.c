/**********************************************//** 
  @file OptionMgr.c
  
  @brief The OptionMgr class manages the application configuration.
  
  The class OptionMgr is TBD
**************************************************/

#include "OptionMgr.h"
#include "Object.h"
#include "String2.h"
#include "Map.h"
#include "FileMgr.h"

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
  @private
**************************************************/
PRIVATE OptionMgr * optionMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE struct OptionDefault optionDefault[] = 
{
  {"DB Name","-o","test.db"},
  {"Input directory","-d","."},
  {"Config file name","-c","sparse.txt"}
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

  this = (OptionMgr*)Object_new(sizeof(OptionMgr),(Destructor)&OptionMgr_delete, (Copy_operator)&OptionMgr_copy);
  this->options = Map_new();
  
  for (j=0; j<nbOptions; j++)
  {
    optionName = String_new(optionDefault[j].name);
    optionValue = String_new(optionDefault[j].value);
    Map_insert(this->options, optionName, optionValue);
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
  Map_delete(this->options);
  Object_delete(&this->object);
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
PUBLIC String * OptionMgr_getOption(OptionMgr * this, const char * name)
{
  String * result = 0;
  String * optionName = String_new(name);

  Map_find(this->options, optionName, &result);
 
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
  Map_insert(this->options, option, value);
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
  FileMgr * fileMgr = FileMgr_getRef();
  String * fileName = 0;
  String * fileContent = 0;
  
  fileName = OptionMgr_getOption(this,"Config file name");
  String_prepend(fileName,"./");
  if (FileMgr_addFile(fileMgr, String_getBuffer(fileName)))
  {
    /* File exists and is managed */
    fileContent = FileMgr_load(fileMgr, fileName);
    OptionMgr_parseFile(this, fileContent);
  }
  /* TODO: Try home director */
  
  //String_delete(fileName);
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

  for (i=0; i<argc; i++)
  {
    for (j=0; j<nbOptions; j++)
    {
      if (Memory_cmp(optionDefault[j].flag, argv[i]))
      {
        optionName = String_new(optionDefault[j].name);
        if ((optionDefault[j].value!=0)&&((i+1)<=argc))
        {
          optionValue = String_new(argv[i+1]);
          i++;
        }
        isFound = 1;
        break;
      }
    }
    if (isFound)
    {
      Map_insert(this->options, optionName, optionValue);
    }
    else
    {
      /* Error case: Illegal arg */
      exit(2);
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
PRIVATE unsigned int OptionMgr_parseFile(OptionMgr * this, String * fileContent)
{
  unsigned int result = 0;
  char * p = 0;
  
  #if 0
  for (p = String_getBuffer(fileContent);
       p < String_getBuffer(fileContent)+String_getLength(fileContent));
       p++)
  {
    switch(state)
    {
      case 0:
        if (p=='[') state = 1;
        break;
      case 1:
        if (p==']') state = 2;
        optioName = ;
        break;
      case 2:
        if (p!=' ') state = 3;
        break;
      case 3:
        if (p=='\n') state = 0;
        /* TODO: Case of windows file */
        optionValue = ;
        OptionMgr_setOption(optionMgr, optionName, optionValue);
        break;
      default:
        /* Error case : */
        break;
      if ((state == 1) || (state == 2))
      {
        /* Error case: Syntax error */
      }
    }
  }
  #endif
  
  return result;
}
