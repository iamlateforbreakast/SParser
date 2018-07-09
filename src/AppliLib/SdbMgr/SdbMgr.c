/**********************************************//** 
  @file SdbMgr.c
  
  @brief TBD
  @details TBD
**************************************************/

#include "SdbMgr.h"
#include "Object.h"
#include "String2.h"
#include <sqlite3.h>

/**********************************************//** 
  @class SdbMgr
**************************************************/
struct SdbMgr
{
  Object object;
  sqlite3* db;
};

PRIVATE SdbMgr * sdbMgr = 0;

PRIVATE unsigned int SdbMgr_open(SdbMgr* this, String* sdbName);
PRIVATE void SdbMgr_close(SdbMgr* this);
/**********************************************//** 
  @private
**************************************************/
PUBLIC SdbMgr* SdbMgr_new(String * name)
{
  SdbMgr* this = 0;

  this = (SdbMgr*)Object_new(sizeof(SdbMgr),(Destructor)&SdbMgr_delete, (Copy_operator)&SdbMgr_copy);
  this->object.size = sizeof(SdbMgr);
  
  SdbMgr_open(this, name);
  sdbMgr = this;
  
  return this;
}

PUBLIC void SdbMgr_delete(SdbMgr* this)
{ 
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      SdbMgr_close(this);
      this->db = 0;
      Object_delete(&this->object);
      sdbMgr = 0;
    }
    else
    {
      this->object.refCount--;
    }
  }
}

PUBLIC SdbMgr * SdbMgr_copy(SdbMgr* this)
{
  SdbMgr * copy = 0;

  return copy;
}

PUBLIC SdbMgr * SdbMgr_getRef()
{  
    if (sdbMgr!=0)
  {
    sdbMgr->object.refCount++;
  }
  
  return sdbMgr;
}

PUBLIC unsigned int SdbMgr_execute(SdbMgr* this, const char* statement, void ** result)
{
  int rc = 0;
  sqlite3_stmt *res = 0;
  const unsigned char *text = 0;
  const unsigned char *text1 = 0;
  int step = 0;
  void **rows = 0;
  unsigned int count = 0;
  
  printf("SdbMgr: %s\n", statement);
  rc = sqlite3_prepare_v2(this->db, statement, -1, &res, 0);
  
  step = sqlite3_step(res);
  
  count = sqlite3_column_count(res);
  printf("Count = %d\n", count);
   if (count>0)
   {
    while (step != SQLITE_DONE)
    {
      //printf("SdbMgr: Query performed\n");
      text = sqlite3_column_text(res, 0);
      text1 = sqlite3_column_text(res, 1);

    step = sqlite3_step(res);
    }
  }
  sqlite3_finalize(res);
  
  return 0;
}

PRIVATE unsigned int SdbMgr_open(SdbMgr* this, String* sdbName)
{
  unsigned int result = 0;
  void ** requestResult = 0;
  
  result = sqlite3_open(String_getBuffer(sdbName), &(this->db));
  (void)SdbMgr_execute(this, "PRAGMA synchronous=NORMAL;", requestResult);
  
  return result;
}

PRIVATE void SdbMgr_close(SdbMgr* this)
{
  sqlite3_close(this->db);
}
