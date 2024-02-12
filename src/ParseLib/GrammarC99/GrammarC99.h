/* GrammarC99.h */

#include "Types.h"
#include "Grammar.h"

typedef struct GrammarC99 GrammarC99;

PUBLIC Grammar * GrammarC99_new();
PUBLIC void GrammarC99_delete(Grammar * this);
PUBLIC void GrammarC99_print(GrammarC99 * this);
PUBLIC unsigned int GrammarC99_getSize(GrammarC99 * this);
