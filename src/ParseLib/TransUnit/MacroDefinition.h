/* MacroDefinition.h */
#ifndef _MACRODEFINITION_H_
#define _MACRODEFINITION_H_

#include "Types.h"
#include "Class.h"
#include "Object.h"

typedef struct MacroDefinition MacroDefinition;

PRIVATE MacroDefinition * MacroDefinition_new();
PRIVATE void MacroDefinition_delete(MacroDefinition * this);
PRIVATE void MacroDefinition_print(MacroDefinition * this);
PRIVATE unsigned int MacroDefinition_getSize(MacroDefinition * this);

struct MacroDefinition
{
  Object object;
  String* body;
  List* parameters;
};

PRIVATE Class macroDefinitionClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&MacroDefinition_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&MacroDefinition_print,
  .f_size = (Sizer)&MacroDefinition_getSize
};

PRIVATE MacroDefinition* MacroDefinition_new()
{

}

PRIVATE void MacroDefinition_delete(MacroDefinition* this)
{

}

PRIVATE void MacroDefinition_print(MacroDefinition* this)
{

}

PRIVATE unsigned int MacroDefinition_getSize(MacroDefinition* this)
{
  return sizeof(MacroDefinition);
}
#endif /* _MACRODEFINITION_H_ */
