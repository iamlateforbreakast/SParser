/* Ast.c */

#include "Ast.h"
#include "Object.h"

/**********************************************//**
  @class Ast
**************************************************/
struct Ast
{
  Object object;
  int a;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class astClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Ast_delete,
  .f_copy = (Copy_Operator)&Ast_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&Ast_print,
  .f_size = (Sizer)&Ast_getSize
};

Ast* Ast_new()
{
  return 0;
}

void Ast_delete(Ast* this)
{
}

Ast* Ast_copy(Ast* this)
{
  return 0;
}

void Ast_print(Ast* this)
{
}

int Ast_getSize(Ast* this)
{
  return sizeof(Ast);
}
