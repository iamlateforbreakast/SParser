/**********************************************//** 
  @file FileMgr.c
  
  @class FileMgr
  
  @brief This file contains the prototye for the class FileMgr
  
  The class FileMgr is TBD
**************************************************/

#include "FileMgr.h"
#include "Object.h"

struct FileMgr
{
  Object object;
};

PRIVATE FileMgr * FileMgr_new()
{
  FileMgr * this;
  
  return this;
}

PUBLIC void FileMgr_delete(FileMgr * this)
{
}

PUBLIC FileMgr * FileMgr_copy(FileMgr * this)
{
  FileMgr * copy;
  
  return copy;
}