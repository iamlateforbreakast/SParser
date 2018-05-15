/**********************************************//** 
  @file SdbMgr.c
  
  @brief TBD
  @details TBD
**************************************************/

#include "SdbMgr.h"
#include "Object.h"
#include <sqlite3.h>

/**********************************************//** 
  @class SdbMgr
**************************************************/
struct SdbMgr
{
  Object object;
  //String* name;
  sqlite3* db;
  //unsigned int isQueryReady;
  //unsigned int queryCount;
  //char queryResult[512];
  unsigned int refCount;
};


/**********************************************//** 
  @private
**************************************************/
PRIVATE SdbMgr* sdbMgr = 0;

PRIVATE SdbMgr* SdbMgr_new()
{
    SdbMgr* this = 0;

    /* this = (SdbMgr*)Memory_alloc(sizeof(SdbMgr)); */
    
    return this;
}

PUBLIC void SdbMgr_delete(SdbMgr* this)
{
    /*this->refCount = this->refCount - 1;
    
    if (this->refCount==0)
    {
      SdbMgr_close(this);
      this->name = NULL;
      this->db = NULL;
      this->isQueryReady = 0;
      this->queryCount = 0;
      memset(this->queryResult, 0, sizeof(this->queryResult));
      Memory_free(this, sizeof(SdbMgr));
    } */
}

PUBLIC SdbMgr* SdbMgr_getSdbMgr()
{ 
  /*if (sdbMgr==NULL)
  {
    sdbMgr = SdbMgr_new();
  }
  sdbMgr->refCount++;
  
  return sdbMgr;*/
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
