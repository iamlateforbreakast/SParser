
#include "Grammar2.h"
#include "Object.h"
#include "FileReader.h"
#include "SdbRequest.h"

#include "Grammar2.parse.h"

extern int Grammar2_parse (void * scanner, FileReader * fr, SdbMgr * sdbMgr);
extern void * Grammar2_scan_string (const char * yystr , void * yyscanner);
extern int Grammar2lex_init (void * scanner);
extern int Grammar2lex_destroy  (void * yyscanner);

PRIVATE void Grammar2_initSdbTables(Grammar2 * this);

struct Grammar2
{
  Object object;
  void * scanner;
  SdbMgr * sdbMgr;
  FileReader * reader;
};

Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr)
{
  Grammar2 * this = 0;

  this = (Grammar2*)Object_new(sizeof(Grammar2),(Destructor)&Grammar2_delete, (Copy_operator)&Grammar2_copy);
  this->reader = fr;
  this->sdbMgr = sdbMgr;
  Grammar2lex_init(&this->scanner);

  Grammar2_initSdbTables(this);

  return this;
}

void Grammar2_delete(Grammar2 * this)
{
  if (this!=0)
  {
     if (this->object.refCount==1)
     {
       Grammar2lex_destroy(this->scanner);
       Object_delete(&this->object);
     }
     else if (this->object.refCount>1)
     {
       this->object.refCount--;
     }
  }
}

PUBLIC Grammar2 * Grammar2_copy(Grammar2 * this)
{
  Grammar2 * copy = 0;

  return copy;
}

PUBLIC void Grammar2_process(Grammar2 * this)
{
  SdbRequest * insertTransUnit = 0;
  
  insertTransUnit = SdbRequest_new(
    "INSERT INTO Translation_Units(File_Name) "
    "VALUES ('%s');"
    );
    
  SdbRequest_execute(insertTransUnit, String_getBuffer(FileReader_getName(this->reader)));
  
  Grammar2_scan_string(FileReader_getBuffer(this->reader), this->scanner);
  //Grammar2set_in(FileReader_getBuffer(this->reader), this->scanner);
  Grammar2_parse(this->scanner, this->reader, this->sdbMgr);
  
  SdbRequest_delete(insertTransUnit);
}

PRIVATE void Grammar2_initSdbTables(Grammar2 * this)
{
  SdbRequest * dropTransUnitTable = 0;
  SdbRequest * createTransUnitTable = 0;
  SdbRequest * dropCommentNodeTable = 0;
  SdbRequest * createCommentNodeTable = 0;
  SdbRequest * dropCodeNodeTable = 0;
  SdbRequest * createCodeNodeTable = 0;
  SdbRequest * dropNodeTable = 0;
  SdbRequest * createNodeTable = 0;

  dropTransUnitTable = SdbRequest_new(
	  "DROP TABLE "
	  "IF EXIST Translation_Units;"
	  );
  createTransUnitTable = SdbRequest_new(
	 "CREATE TABLE Translation_Units ("
	 "File_Name text NOT NULL "
	 ");");
   
  dropCommentNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Comment_Nodes;"
   );
  createCommentNodeTable = SdbRequest_new(
   "CREATE TABLE Comment_Nodes ("
   "Comment text NOT NULL "
   ");");
   
  dropCodeNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Code_Nodes;"
   );
  createCodeNodeTable = SdbRequest_new(
   "CREATE TABLE Code_Nodes ("
   "Code text NOT NULL "
   ");");
  
  dropNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Nodes;"
   );
  createNodeTable = SdbRequest_new(
   "CREATE TABLE Nodes ("
   "NodeId integre PRIMARY_KEY,"
   "NodeType integer NOT NULL,"
   "NodePtr integer NOT NULL "
   ");");
   
  SdbRequest_execute(dropNodeTable);
  SdbRequest_execute(createNodeTable);
  SdbRequest_execute(dropTransUnitTable);
  SdbRequest_execute(createTransUnitTable);
  SdbRequest_execute(dropCommentNodeTable);
  SdbRequest_execute(createCommentNodeTable);
  SdbRequest_execute(dropCodeNodeTable);
  SdbRequest_execute(createCodeNodeTable);
  SdbRequest_delete(dropCodeNodeTable);
  SdbRequest_delete(dropNodeTable);
  SdbRequest_delete(createNodeTable);
  SdbRequest_delete(dropCommentNodeTable);
  SdbRequest_delete(createCommentNodeTable);
  SdbRequest_delete(dropTransUnitTable);
  SdbRequest_delete(createTransUnitTable);
  SdbRequest_delete(createCodeNodeTable);
}
