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
  
  return this;
}

PUBLIC void SdbMgr_delete(SdbMgr* this)
{
  SdbMgr_close(this);
  this->db = 0;
  Object_delete(&this->object);
}

PUBLIC SdbMgr * SdbMgr_copy(SdbMgr* this)
{
  SdbMgr * copy = 0;

  return copy;
}

PUBLIC unsigned int SdbMgr_execute(SdbMgr* this, const char* statement)
{
  /*int rc = 0;
  sqlite3_stmt *res = NULL;
  const unsigned char *text = NULL;
  const unsigned char *text1 = NULL;
  int step = 0;
  
  printf("SdbMgr: %s\n", statement);
  rc = sqlite3_prepare_v2(this->db, statement, -1, &res, 0);
  
  step = sqlite3_step(res);
  
  if (step == SQLITE_ROW)
  {
    //printf("SdbMgr: Query performed\n");
    text = sqlite3_column_text(res, 0);
    text1 = sqlite3_column_text(res, 1);
    if (text)
    {
      memcpy(this->queryResult, text, strlen(text));
    }
    else
    {
      memcpy(this->queryResult, text1, strlen(text1));
    }
    printf("SdbMgr: %s\n", this->queryResult);
    sdbMgr->queryCount = 1;
  }
  sqlite3_finalize(res);*/
  
  return 0;
}

PRIVATE unsigned int SdbMgr_open(SdbMgr* this, String* sdbName)
{
  unsigned int result = 0;
  
  result = sqlite3_open(String_getBuffer(sdbName), &(this->db));
  (void)SdbMgr_execute(this, "PRAGMA synchronous=NORMAL;");
  
  return result;
}

PRIVATE void SdbMgr_close(SdbMgr* this)
{
  sqlite3_close(this->db);
}
