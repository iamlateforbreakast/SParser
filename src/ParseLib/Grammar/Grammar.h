/* Grammar.h */
#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include "Types.h"
#include "Object.h"

typedef struct Grammar Grammar;

struct Grammar
{
  Object object;
  Grammar * (*new)(void);
  void (*delete)(Grammar * this);
  Grammar * (*copy)(Grammar * this);
  void (*print)(Grammar * this);
};

PUBLIC Grammar * Grammar_new();
PUBLIC void Grammar_delete(Grammar * this);
PUBLIC void Grammar_process(Grammar * this);
PUBLIC void Grammar_print(Grammar * this);

#endif /* _GRAMMAR_H_ */
