/**********************************************//** 
   @file Grammar1.c
			      
   @brief TBD
   @details TBD
**************************************************/
#include "Grammar1.h"
#include "Object.h"
#include "SdbMgr.h"
#include "FileReader.h"

struct Grammar1
{
  Object object;
};

PUBLIC Grammar1 * Grammar1_new(FileReader * f, SdbMgr * sdbMgr)
{
  Grammar1 * this = 0;
  
  return this;
}

PUBLIC void Grammar1_delete(Grammar1 * this)
{
}

PUBLIC Grammar1 * Grammar_copy(Grammar1 * this)
{
  Grammar1 * result = 0;
  
  return result;
}

void Grammar1_process(Grammar1 * this)
{
  /* Grammar1_scan_string(s->buffer); */
  /* Grammar1_parse(this); */
}

PUBLIC void Grammar1_changeBuffer(Grammar1 * this)
{
}
