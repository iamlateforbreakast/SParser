/* List.h */

#ifndef _LIST_H_
#define _LIST_H_

#include "Types.h"

typedef struct List List;

PUBLIC List* List_new();
PUBLIC void List_delete(List* this);
PUBLIC List* List_copy(List* this);
PUBLIC void List_insert(List* this, void* item);
PUBLIC void List_merge(List* this, List* l1);
PUBLIC void List_iterator(List* this, void (*f)(void* t));
PUBLIC void* List_getNext(List* this);
PUBLIC void* List_getHead(List* this);

#endif /* _LIST_H_ */