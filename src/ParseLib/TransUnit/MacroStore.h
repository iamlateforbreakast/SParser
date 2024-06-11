/* MacroStore.h */
#ifndef _MACROSTORE_H_
#define _MACROSTORE_H_

#include "Types.h"
#include "Class.h"
#include "Object.h"
#include "Memory.h"
#include "MacroDefinition.h"

#define MAX_CHILDREN (27)

typedef struct MacroStore MacroStore;

PRIVATE MacroStore * MacroStore_new();
PRIVATE void MacroStore_delete(MacroStore * this);
PRIVATE void MacroStore_print(MacroStore * this);
PRIVATE unsigned int MacroStore_getSize(MacroStore * this);
PRIVATE int MacroStore_insertName(MacroStore* this, String* name, MacroDefinition* body);
PRIVATE int MacroStore_removeName(MacroStore* this, String* name);
PRIVATE enum MacroEvalName MacroStore_evalName(MacroStore* this, char* ptr, int length);
PRIVATE char convert[255];

enum MacroEvalName
{
  E_NOT_MACRO,
  E_POSSIBLE_MACRO,
  E_DEFINED_MACRO
};

struct MacroStoreNode
{
  int isLeaf;
  void * children[MAX_CHILDREN];
};

struct MacroStore
{
  Object object;
  struct MacroStoreNode * root;
};

PRIVATE Class macroStoreClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&MacroStore_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&MacroStore_print,
  .f_size = (Sizer)&MacroStore_getSize
};

PRIVATE MacroStore* MacroStore_new()
{
  MacroStore * this = (MacroStore*)Object_new(sizeof(MacroStore), &macroStoreClass);

  this->root = (struct MacroStoreNode * )Memory_alloc(sizeof(struct MacroStore));

  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    this->root->children[i] = 0;
    this->root->isLeaf = 1;
  }

  for (char c = 0; c < 255; c++)
  {
    if ((c >= 'A') && (c <= 'Z')) convert[c] = c - 'A' + 2;
    else if (c == '_') convert[c] = 1;
    else
      convert[c] = 0;
  }
  return this;
}

PRIVATE void MacroStore_delete(MacroStore* this)
{
  if (this == 0) return;
  /* De-allocate the specific members */

  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

PRIVATE void MacroStore_print(MacroStore* this)
{
  struct MacroStoreNode* currentNode = this->root;

  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    if (!currentNode->isLeaf) MacroStoree_printChildrenNode(this, currentNode->children[i]);
  }
}

PRIVATE unsigned int MacroStore_getSize(MacroStore* this)
{
  return sizeof(MacroStore);
}

PRIVATE int MacroStore_insertName(MacroStore* this, String * name, MacroDefinition* body)
{
  char* buffer = String_getBuffer(name);
  int length = String_getLength(name);
  struct MacroStoreNode* currentNode = this->root;

  int c = 0;
  for (int i = 0; i < length; i++)
  {
    c = convert[buffer[i]];
    if (currentNode->children[c]) currentNode = currentNode->children[c];
    else
    {
      currentNode->children[c] = Memory_alloc(sizeof(struct MacroStoreNode*));
      currentNode = currentNode->children[c];
      currentNode->isLeaf = 0;
    }
  }
  currentNode->isLeaf = 1;
  currentNode->children[c] = body;

  return 0;
}

PRIVATE int MacroStore_removeName(MacroStore* this, String* name)
{
  char* buffer = String_getBuffer(name);
  int length = String_getLength(name);
  struct MacroStoreNode* currentNode = this->root;

  int c;
  for (int i = 0; i < length; i++)
  {
    c = convert[buffer[i]];
  }
  return 0;
}

PRIVATE enum MacroEvalName MacroStore_evalName(MacroStore* this, char* buffer, int length)
{
  struct MacroStoreNode* currentNode = this->root;

  int c = 0;
  for (int i = 0; i < length; i++)
  {
    c = convert[buffer[i]];
    if (currentNode->children[c]) currentNode = currentNode->children[c];
    else

  }

  return E_NOT_MACRO;
}

PRIVATE MacroStore_printChildrenNode(MacroStore* this, struct MacroStoreNode* node)
{
  if (node == 0) return;

  for (int i = 0; i < MAX_CHILDREN; i++)
  {

  }
}
#endif /* _MACROSTORE_H_ */
