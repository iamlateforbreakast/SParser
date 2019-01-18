/**********************************************//** 
  @file GRammar2.c
  
  @brief A test grammar class.
  The Grammar2 class implements a test grammar
  parsing comments code blocks and allowing includes files.
**************************************************/
#include "Grammar2.h"
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
PRIVATE unsigned int isInitialised = 0;

struct GrammarContext
{
  Object object;
  unsigned int lastNode;
};

typedef struct GrammarContext GrammarContext;

struct Grammar2
{
  Object object;
  void * scanner;
  SdbMgr * sdbMgr;
  FileReader * reader;
  char buffer[MAX_BUFFER_SIZE];
  char * node_text;
  int node_text_position;
  GrammarContext * current;
  List * contexts;
};

PUBLIC Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr)
{
  Grammar2 * this = 0;
  
  this = (Grammar2*)Object_new(sizeof(Grammar2),(Destructor)&Grammar2_delete, (Copy_operator)&Grammar2_copy);
  this->reader = fr;
  this->sdbMgr = sdbMgr;
  Grammar2lex_init(&this->scanner);
  
  this->contexts = List_new(this->contexts);
  this->current = (GrammarContext*)Object_new(sizeof(GrammarContext),0,0);
  this->current->lastNode = 0;
  List_insertHead(this->contexts, this->current);
  
  if (!isInitialised) Grammar2_initSdbTables(this);

  return this;
}

PUBLIC void Grammar2_delete(Grammar2 * this)
{
  GrammarContext * o = 0;
  
  printf("Grammar delete start\n");
  if (this!=0)
  {
     if (this->object.refCount==1)
     {
       Grammar2lex_destroy(this->scanner);
       o = (GrammarContext*)List_removeHead(this->contexts);
       List_delete(this->contexts);
       Object_delete(&this->object);
     }
     else if (this->object.refCount>1)
     {
       this->object.refCount--;
     }
  }
  printf("Grammar delet end\n");
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
  
  this->current->lastNode = nodeId;
    
  SdbRequest_delete(insertNode);
}

PUBLIC void Grammar2_addComment(Grammar2 * this)
{
  SdbRequest * insertCommentNode = 0;
  
  insertCommentNode = SdbRequest_new(
  "INSERT INTO Comment_Nodes (NodeId, Comment) "
  "VALUES (%d,'%s');"
  );
  
  this->buffer[this->node_text_position] = 0;
  //printf("\nComment found: %s\n", this->buffer);
  this->node_text_position = 0;
  commentNodeId++;
  
  Grammar2_addNode(this, 1, commentNodeId);
  SdbRequest_execute(insertCommentNode, commentNodeId, this->buffer);
  SdbRequest_delete(insertCommentNode);
}

PUBLIC void Grammar2_addCodeNode(Grammar2 * this)
{
  SdbRequest * insertCodeNode = 0;
  
  if (this->node_text_position!=0)
  {
    insertCodeNode = SdbRequest_new(
      "INSERT INTO Code_Nodes (NodeId, Code) "
      "VALUES (%d, '%s');"
    );
  
    this->buffer[this->node_text_position] = 0;

    //this->node_text_position++;
    //printf("\nCode found: %s\n", this->buffer);
      this->node_text_position = 0;
      codeNodeId++;
    
      Grammar2_addNode(this, 2, codeNodeId);
    
      SdbRequest_execute(insertCodeNode, codeNodeId, this->buffer);
    SdbRequest_delete(insertCodeNode);
  }
}

PUBLIC void Grammar2_addIncludeNode(Grammar2 * this, char * name)
{
  SdbRequest * insertIncludeNode = 0;
  
  insertIncludeNode = SdbRequest_new(
      "INSERT INTO Include_Nodes (NodeId, EntryNode) "
      "VALUES (%d, '%s');"
    );
  
  includeNodeId++;
    
  Grammar2_addNode(this, 3, includeNodeId);
    
  SdbRequest_execute(insertIncludeNode, includeNodeId, name);
  SdbRequest_delete(insertIncludeNode);
}

PUBLIC char * Grammar2_processNewFile(Grammar2 * this, String * fileName)
{
   char * result = 0;
   GrammarContext * o = 0;

   Grammar2_addIncludeNode(this, String_getBuffer(fileName));
   o = (GrammarContext*)Object_new(sizeof(GrammarContext),0,0);
   this->current = o;
   this->current->lastNode = 0;
   
   List_insertHead(this->contexts, o);
   
   result = FileReader_addFile(this->reader, fileName);
   
   return result;
}

PUBLIC void Grammar2_returnToFile(Grammar2 * this)
{
  GrammarContext * o = 0;
  
  o = (GrammarContext*)List_removeHead(this->contexts);
  printf("o= %x\n", o);
  printf("o->object= %x\n", &o->object);
  
  Object_delete(&(o->object));
  
  printf("o good\n");
  this->current = (GrammarContext*)List_getHead(this);
  
}