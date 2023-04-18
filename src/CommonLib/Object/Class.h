/* Class. h */

#ifndef _CLASS_H_
#define _CLASS_H_

#include "Object.h"

struct Class
{
  Constructor f_new;
  Destructor f_delete;
  Copy_Operator f_copy;
  Comp_Operator f_comp;
  Printer f_print;
};

#endif /* _CLASS_H_ */