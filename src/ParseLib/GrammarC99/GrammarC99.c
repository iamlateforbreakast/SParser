/* GrammarC99 */

#include "GrammarC99.h"
#include "Object.h"

/**********************************************//**
  @class GrammarC99
**************************************************/
struct GrammarC99
{
  Object object;
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
  return 0;
}

PUBLIC void GrammarC99_delete(Grammar * this)
{
}

PUBLIC void GrammarC99_print(Grammar * this)
{

}

PUBLIC unsigned int GrammarC99_getSize(GrammarC99 * this)
{
  return sizeof(GrammarC99);
}