/**********************************************//** 
  @file GRammar2.c
  
  @brief A test grammar class.
  The Grammar2 class implements a test grammar
  parsing comments code blocks and allowing includes files.
**************************************************/
#include "Grammar2.h"
#include "Class.h"
#include "Object.h"
#include "FileReader.h"
#include "SdbRequest.h"
#include "Error.h"
#include "List.h"

#include "Grammar2.parse.h"

#define MAX_BUFFER_SIZE  (16384)

extern int Grammar2_parse (void * scanner, Grammar2 * this);
extern void * Grammar2_scan_string (const char * yystr , void * yyscanner);
extern int Grammar2lex_init (void * scanner);
extern int Grammar2lex_destroy  (void * yyscanner);

PRIVATE void Grammar2_initSdbTables(Grammar2 * this);
PRIVATE unsigned int nodeId = 0;
PRIVATE unsigned int codeNodeId = 0;
PRIVATE unsigned int commentNodeId = 0;
PRIVATE unsigned int includeNodeId = 0;
PRIVATE unsigned int unitId = 0;
PRIVATE unsigned int isInitialised = 0;

PRIVATE unsigned int Grammar2_isFileToBeIgnored(Grammar2 * this, String * fileName);
PRIVATE unsigned int Grammar2_isIncludeNodeProcessed(Grammar2 * this, String * name);

struct GrammarContext
{
  Object object;
  unsigned int lastNode;
};

typedef struct GrammarContext GrammarContext;

/**********************************************//**
  @class Grammar2
**************************************************/
struct Grammar2
{
  Object object;
  void * scanner;
  SdbMgr * sdbMgr;
  FileReader * reader;
  char buffer[MAX_BUFFER_SIZE];
  //char * node_text;
  int node_text_position;
  GrammarContext * current;
  List * contexts;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class grammar2Class =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Grammar2_delete,
  .f_copy = (Copy_Operator)&Grammar2_copy,
  .f_equal = (Equal_Operator)0,
  .f_print = (Printer)0
};

/**********************************************//** 
  @brief Create an instance of the class Grammar2.
  @public
  @memberof Grammar2
  @return New instance.
**************************************************/
PUBLIC Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr)
{
  Grammar2 * this = 0;
  
  this = (Grammar2*)Object_new(sizeof(Grammar2), &grammar2Class);
  this->reader = fr;
  this->sdbMgr = sdbMgr;
  Grammar2lex_init(&this->scanner);
  
  this->contexts = List_new(this->contexts);
  this->current = (GrammarContext*)Object_new(sizeof(GrammarContext),0);
  this->current->lastNode = 0;
  List_insertHead(this->contexts, this->current);
  
  Memory_set(this->buffer, 0, MAX_BUFFER_SIZE);
  this->node_text_position = 0;
  
  if (!isInitialised) Grammar2_initSdbTables(this);

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Grammar2.
  @public
  @memberof Grammar2
**************************************************/
PUBLIC void Grammar2_delete(Grammar2 * this)
{
  GrammarContext * o = 0;
  
  if (this!=0)
  {
     if (this->object.refCount==1)
     {
       Grammar2lex_destroy(this->scanner);
       o = (GrammarContext*)List_removeHead(this->contexts);
       Object_delete(o);
       List_delete(this->contexts);
       Object_delete(&this->object);
     }
     else if (this->object.refCount>1)
     {
       this->object.refCount--;
     }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class Grammar2.
  @public
  @memberof Grammar2
  @return Copied instance.
**************************************************/
PUBLIC Grammar2 * Grammar2_copy(Grammar2 * this)
{
  Grammar2 * copy = 0;

  return copy;
}

PUBLIC void Grammar2_process(Grammar2 * this)
{
  SdbRequest * insertTransUnit = 0;
  
  unitId++;
  
  insertTransUnit = SdbRequest_new(
    "INSERT INTO Translation_Units(Unit_Id, File_Name) "
    "VALUES (%d, '%s');"
    );
    
  SdbRequest_execute(insertTransUnit, unitId, String_getBuffer(FileReader_getName(this->reader)));
  
  Grammar2_scan_string(FileReader_getBuffer(this->reader), this->scanner);
  //if (Memory_ncmp(String_getBuffer(FileReader_getName(this->reader)),"Timer.c",7))
  //{
  //         printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  //}
  Grammar2_parse(this->scanner, this);
  
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
  SdbRequest * dropIncludeNodeTable = 0;
  SdbRequest * createIncludeNodeTable = 0;
  
  dropTransUnitTable = SdbRequest_new(
	  "DROP TABLE "
	  "IF EXIST Translation_Units;"
	  );
  createTransUnitTable = SdbRequest_new(
	 "CREATE TABLE Translation_Units ("
   "Unit_Id integer PRIMARY_KEY,"
	 "File_Name text NOT NULL "
	 ");");
   
  dropCommentNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Comment_Nodes;"
   );
  createCommentNodeTable = SdbRequest_new(
   "CREATE TABLE Comment_Nodes ("
   "NodeId integer PRIMARY_KEY,"
   "Comment text NOT NULL "
   ");");
   
  dropCodeNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Code_Nodes;"
   );
  createCodeNodeTable = SdbRequest_new(
   "CREATE TABLE Code_Nodes ("
   "NodeId integer PRIMARY_KEY,"
   "Code text NOT NULL "
   ");");
   
   dropIncludeNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Include_Nodes;"
   );
   
  createIncludeNodeTable = SdbRequest_new(
   "CREATE TABLE Include_Nodes ("
   "NodeId integer PRIMARY_KEY,"
   "Name text NOT NULL,"
   "EntryNode integer NOT NULL"
   ");");
  
  dropNodeTable = SdbRequest_new(
   "DROP TABLE "
   "IF EXIST Nodes;"
   );
   
  createNodeTable = SdbRequest_new(
   "CREATE TABLE Nodes ("
   "NodeId integer PRIMARY_KEY,"
   "NodeType integer NOT NULL,"
   "NodePtr integer NOT NULL,"
   "NodeNext integer,"
   "NodePrev integer"
   ");");
   
  SdbRequest_execute(dropNodeTable);
  SdbRequest_execute(createNodeTable);
  SdbRequest_execute(dropTransUnitTable);
  SdbRequest_execute(createTransUnitTable);
  SdbRequest_execute(dropCommentNodeTable);
  SdbRequest_execute(createCommentNodeTable);
  SdbRequest_execute(dropCodeNodeTable);
  SdbRequest_execute(createCodeNodeTable);
  SdbRequest_execute(dropIncludeNodeTable);
  SdbRequest_execute(createIncludeNodeTable);
  SdbRequest_delete(dropCodeNodeTable);
  SdbRequest_delete(createCodeNodeTable);
  SdbRequest_delete(dropNodeTable);
  SdbRequest_delete(createNodeTable);
  SdbRequest_delete(dropCommentNodeTable);
  SdbRequest_delete(createCommentNodeTable);
  SdbRequest_delete(dropTransUnitTable);
  SdbRequest_delete(createTransUnitTable);
  SdbRequest_delete(dropIncludeNodeTable);
  SdbRequest_delete(createIncludeNodeTable);
  
  isInitialised = 1;
}

PUBLIC FileReader * Grammar2_getFileReader(Grammar2 * this)
{
  return this->reader;
}

PUBLIC SdbMgr * Grammar2_getSdbMgr(Grammar2 * this)
{
  return this->sdbMgr;
}

PUBLIC void Grammar2_addToBuffer(Grammar2 * this, char * text)
{
  this->buffer[this->node_text_position] = text[0];
  this->node_text_position++;
  if (this->node_text_position>(MAX_BUFFER_SIZE-1)) 
  {
    /* Error case: Cannot obtain the FileMgr root location. */
    Error_new(ERROR_FATAL, "Grammar internal buffer too small");
  }
}

PUBLIC void Grammar2_addNode(Grammar2 * this, unsigned int type, int nodePtr)
{
  SdbRequest * insertNode = 0;
  
  insertNode = SdbRequest_new(
  "INSERT INTO Nodes (NodeId, NodeType, NodePtr, NodeNext, NodePrev) "
  "VALUES (%d,%d,%d,%d,%d);"
  );
  nodeId++;
     
  SdbRequest_execute(insertNode, nodeId, type, nodePtr, 0, this->current->lastNode);
  
  SdbRequest_delete(insertNode);
}

PUBLIC void Grammar2_addComment(Grammar2 * this)
{
  SdbRequest * insertCommentNode = 0;
  
  printf("Grammar2_addComment: 1\n");
  insertCommentNode = SdbRequest_new(
  "INSERT INTO Comment_Nodes (NodeId, Comment) "
  "VALUES (%d,'%s');"
  );
  
  this->buffer[this->node_text_position] = 0;
  //printf("\nComment found: %s\n", this->buffer);
  this->node_text_position = 0;
  commentNodeId++;
  
  Grammar2_addNode(this, 1, commentNodeId);
  
  this->current->lastNode = nodeId;
  
  SdbRequest_execute(insertCommentNode, commentNodeId, this->buffer);
  SdbRequest_delete(insertCommentNode);
  
  printf("Grammar2_addComment: 2\n");
}

PUBLIC void Grammar2_addCodeNode(Grammar2 * this)
{
  SdbRequest * insertCodeNode = 0;
  
  printf("Grammar2_addCodeNode: 1\n");
  if (this->node_text_position!=0)
  {
    insertCodeNode = SdbRequest_new(
      "INSERT INTO Code_Nodes (NodeId, Code) "
      "VALUES (%d, '%s');"
    );
  
    this->buffer[this->node_text_position] = 0;
    this->node_text_position = 0;
    codeNodeId++;

    
    Grammar2_addNode(this, 2, codeNodeId);
    this->current->lastNode = nodeId;
    
    SdbRequest_execute(insertCodeNode, codeNodeId, this->buffer);
    SdbRequest_delete(insertCodeNode);
  }
  printf("Grammar2_addCodeNode: 2\n");
}

PUBLIC void Grammar2_addIncludeNode(Grammar2 * this, char * name)
{
  SdbRequest * insertIncludeNode = 0;
  
  insertIncludeNode = SdbRequest_new(
      "INSERT INTO Include_Nodes (NodeId, Name, EntryNode) "
      "VALUES (%d, '%s', '%s');"
    );
  
  includeNodeId++;
   

  Grammar2_addNode(this, 3, includeNodeId);
  this->current->lastNode = nodeId;
  
  SdbRequest_execute(insertIncludeNode, includeNodeId, name, 0);
  SdbRequest_delete(insertIncludeNode);
}

PUBLIC char * Grammar2_processNewFile(Grammar2 * this, String * fileName)
{
   char * result = 0;
   GrammarContext * o = 0;

   if (Grammar2_isFileToBeIgnored(this, fileName))
   {
     return 0;
   }

   if (Grammar2_isIncludeNodeProcessed(this, fileName))
   {
     return 0;
   }
 
   result = FileReader_addFile(this->reader, fileName);
   
   if (result==0)
   {
     //Grammar2_returnToFile(this);
     Error_new(ERROR_FATAL,"Grammar2_processNewFile: Cannot find %s\n", String_getBuffer(fileName));
   }
   else
   {
    Grammar2_addIncludeNode(this, String_getBuffer(fileName));
   
    o = (GrammarContext*)Object_new(sizeof(GrammarContext),0);
    o->lastNode = this->current->lastNode;
    this->current = o;
   
   List_insertHead(this->contexts, o);
   }
   
   return result;
}

PUBLIC void Grammar2_returnToFile(Grammar2 * this)
{
  GrammarContext * o = 0;
  
  o = (GrammarContext*)List_removeHead(this->contexts);
  Object_delete(&(o->object));
  this->current = (GrammarContext*)List_getHead(this->contexts);
   Error_new(ERROR_DBG,"Grammar2_returnToFile: %d\n", this->current->lastNode);
}

PRIVATE unsigned int Grammar2_isFileToBeIgnored(Grammar2 * this, String * fileName)
{
    unsigned int result = 0;

    char * buffer = String_getBuffer(fileName);
    if (Memory_ncmp(buffer, "signal.h", 8)) return 1;
    if (Memory_ncmp(buffer, "unistd.h", 8)) return 1;
    if (Memory_ncmp(buffer, "dirent.h", 8)) return 1;
    if (Memory_ncmp(buffer, "stdio.h", 7)) return 1;
    if (Memory_ncmp(buffer, "sqlite3.h", 9)) return 1;
    if (Memory_ncmp(buffer, "stdarg.h", 8)) return 1;
    if (Memory_ncmp(buffer, "stdlib.h", 8)) return 1;
    if (Memory_ncmp(buffer, "string.h", 8)) return 1;
    if (Memory_ncmp(buffer, "time.h", 6)) return 1;
    if (Memory_ncmp(buffer, "Grammar2.parse.h", 16)) return 1;
    if (Memory_ncmp(buffer, "sys/time.h", 10)) return 1;
    
    return result;
}

PRIVATE unsigned int Grammar2_isIncludeNodeProcessed(Grammar2 * this, String * name)
{
  unsigned int result = 0;
  SdbRequest * checkIncludeNode = 0;
  
  checkIncludeNode = SdbRequest_new(
    "SELECT * FROM Include_Nodes WHERE Name='%s';");
  SdbRequest_execute(checkIncludeNode, String_getBuffer(name));
  SdbRequest_delete(checkIncludeNode);
  
  return SdbRequest_getNbResult(checkIncludeNode);
}