/**********************************************//** 
  @file OptionMgr.c
  
  @class OptionMgr
  
  @brief The OptionMgr class manages the application configuration.
  
  The class OptionMgr is TBD
**************************************************/

#include "OptionMgr.h"
#include "Object.h"

struct OptionMgr
{
  Object object;
};

PRIVATE OptionMgr * OptionMgr_new()
{
  OptionMgr * this = 0;
  
  return this;
}

PUBLIC void OptionMgr_delete(OptionMgr * this)
{
}

PUBLIC OptionMgr * OptionMgr_copy(OptionMgr * this)
{
  OptionMgr * copy = 0;
  
  return copy;
}

PUBLIC OptionMgr* OptionMgr_getOptionMgr()
{
  OptionMgr * result = 0;
  
  return result;
}