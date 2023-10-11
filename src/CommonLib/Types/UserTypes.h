/* User Types */
#ifndef _USERTYPES_
#define _USERTYPES_
#include "Class.h"

enum {
  List,
  BTree,
  Nb_types
};

Class userTypes[Nb_types-1];

Class userTypes[List] = {
  .f_new = 0,
  .f_delete = (Destructor)&List_delete,
  .f_copy = (Copy_Operator)&List_copy,
  .f_comp = (Comp_Operator)&List_compare,
  .f_print = (Printer)&List_print
};

Class usertypes[BTree] = {
  .f_new = 0,
  .f_delete = (Destructor)&BTree_delete,
  .f_copy = (Copy_Operator)&BTree_copy,
  .f_comp = (Comp_Operator)&BTree_compare,
  .f_print = (Printer)&BTree_print
};

