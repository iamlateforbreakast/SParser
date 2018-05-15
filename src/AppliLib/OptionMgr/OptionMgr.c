/**********************************************//** 
  @file OptionMgr.c
  
  @brief The OptionMgr class manages the application configuration.
  
  The class OptionMgr is TBD
**************************************************/

#include "OptionMgr.h"
#include "Object.h"
#include "String2.h"
#include "Map.h"

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

PRIVATE OptionMgr * optionMgr = 0;

PRIVATE struct OptionDefault optionDefault[] = {
{"DB Name","-o","test.db"},
{"Input directory","-d","."}
                                                                     };

/**********************************************//** 
  @brief TBD
  @private
  @memberof OptionMgr
**************************************************/
PRIVATE OptionMgr * OptionMgr_new()
{
  OptionMgr * this = 0;
  
  this = (OptionMgr*)Object_new(sizeof(OptionMgr),(Destructor)&OptionMgr_delete, (Copy_operator)&OptionMgr_copy);
  this->options = Map_new();
  
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
  @details TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC OptionMgr* OptionMgr_getOption(OptionMgr * this, const char * optionName)
{
  OptionMgr * result = 0;
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC void OptionMgr_setOption(OptionMgr * this, const char * optionName, String * value)
{
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC unsigned int OptionMgr_readFromFile(OptionMgr * this, const char * fileName)
{
  unsigned int result = 0;
  /* FileMgr * fileMgr = FileMgr_getRef() */
  /* Add Filename to FileMgr */
  /* FileMgr_addFile(fileMgr, fileName); */
  /* FIleMgr_load */
  /* OptionMgr parse */
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof OptionMgr
**************************************************/
PUBLIC unsigned int OptionMgr_readFromCmdLine(OptionMgr * this, const int argc, const char ** argv)
{
  unsigned int result = 0;
  
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
  
  return result;
}
