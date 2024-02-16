/* GrammarC99 */

#include "GrammarC99.h"
#include "TransUnit.h"
#include "Object.h"
#include "Error.h"

extern int GrammarC99_parse(void* scanner, GrammarC99* this);
extern void* GrammarC99_scan_string(const char* yystr, void* yyscanner);
extern int GrammarC99lex_init(void* scanner);
extern int GrammarC99lex_destroy(void* yyscanner);
/**********************************************//**
  @class GrammarC99
**************************************************/
struct GrammarC99
{
  Grammar grammar;
  TransUnit* transUnit;
  void * scanner;
  //SdbMgr * sdbMgr;
  //FileReader * reader;
  //char * buffer;
  //char * node_text;
  //int node_text_position;
  //GrammarContext * current;
  //List * contexts;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class grammarC99Class =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&GrammarC99_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&GrammarC99_print,
  .f_size = (Sizer)&GrammarC99_getSize
};

/**********************************************//**
  @brief Create an instance of the class GrammarC99.
  @public
  @memberof GrammarC99
  @return New instance.
**************************************************/
PUBLIC Grammar* GrammarC99_new(FileDesc * fileDesc)
{
  GrammarC99 * this = 0;
  this = (GrammarC99*)Object_new(sizeof(GrammarC99), &grammarC99Class);
  
  if (this == 0) return 0;

  GrammarC99lex_init(&this->scanner);
  this->transUnit = TransUnit_new(fileDesc);
  //this->reader = fr;
  //this->sdbMgr = sdbMgr;
  
  //this->contexts = List_new(this->contexts);
  //this->current = (GrammarContext*)Object_new(sizeof(GrammarContext),&grammarContextClass);
  //this->current->lastNode = 0;
  //this->current->includeNodeBranch = 0;
  //List_insertHead(this->contexts, this->current, 1);
  
  //this->buffer = &internalBuffer[0];
  //Memory_set(this->buffer, 0, MAX_BUFFER_SIZE);
  
  //this->node_text_position = 0;
  
  //if (!isInitialised) Grammar2_initSdbTables(this);

  return (Grammar*)this;
}

PUBLIC void GrammarC99_delete(Grammar * this)
{
  GrammarC99* thisC99 = (GrammarC99*)this;
  if (this!=0)
  {
     if (this->object.refCount==1)
     {
      GrammarC99lex_destroy(thisC99->scanner);
       //o = (GrammarContext*)List_removeHead(this->contexts);
       //Object_delete((Object*)o);
       //List_delete(this->contexts);
       Object_deallocate(&this->object);
     }
     else if (this->object.refCount>1)
     {
       this->object.refCount--;
     }
  }
}

PUBLIC void GrammarC99_print(Grammar * this)
{

}

PUBLIC unsigned int GrammarC99_getSize(Grammar * this)
{
  return sizeof(GrammarC99);
}
PUBLIC void GrammarC99_process(GrammarC99* this)
{
  /* TODO: Store translation unit information */
  Error_new(ERROR_INFO, "GrammarC99: Processing file %s\n", TransUnit_getName(this->transUnit));
  String* buffer = TransUnit_getNextBuffer(this->transUnit);
  GrammarC99_scan_string(String_getBuffer(buffer), this->scanner);
  GrammarC99_parse(this->scanner, this);
}
