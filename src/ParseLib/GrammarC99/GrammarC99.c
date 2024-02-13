/* GrammarC99 */

#include "GrammarC99.h"
#include "Object.h"

/**********************************************//**
  @class GrammarC99
**************************************************/
struct GrammarC99
{
  Grammar grammar;
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

PUBLIC Grammar * GrammarC99_new()
{
  Grammar * this = 0;
  
  this = (Grammar*)Object_new(sizeof(GrammarC99), &grammarC99Class);
  //this->reader = fr;
  //this->sdbMgr = sdbMgr;
  //Grammar2lex_init(&this->scanner);
  
  //this->contexts = List_new(this->contexts);
  //this->current = (GrammarContext*)Object_new(sizeof(GrammarContext),&grammarContextClass);
  //this->current->lastNode = 0;
  //this->current->includeNodeBranch = 0;
  //List_insertHead(this->contexts, this->current, 1);
  
  //this->buffer = &internalBuffer[0];
  //Memory_set(this->buffer, 0, MAX_BUFFER_SIZE);
  
  //this->node_text_position = 0;
  
  //if (!isInitialised) Grammar2_initSdbTables(this);

  return this;
}

PUBLIC void GrammarC99_delete(Grammar * this)
{
  if (this!=0)
  {
     if (this->object.refCount==1)
     {
       //Grammar2lex_destroy(this->scanner);
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
