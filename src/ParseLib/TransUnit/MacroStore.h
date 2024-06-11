/* MacroStore.h */
#ifndef _MACROSTORE_H_
#define _MACROSTORE_H_

#include "Types.h"
#include "Class.h"
#include "Object.h"
#include "String2.h"
#include "Memory.h"
#include "MacroDefinition.h"
#include "Debug.h"

#define MAX_CHILDREN (28)

typedef struct MacroStore MacroStore;


PRIVATE char convert[256];
PRIVATE char convertBack[MAX_CHILDREN];

enum MacroEvalName
{
  E_NOT_MACRO,
  E_POSSIBLE_MACRO,
  E_DEFINED_MACRO
};

struct MacroStoreNode
{
  int isLeaf;
  MacroDefinition * def;
  void * children[MAX_CHILDREN];
};

struct MacroStore
{
  Object object;
  struct MacroStoreNode * root;
};

PRIVATE MacroStore * MacroStore_new();
PRIVATE void MacroStore_delete(MacroStore * this);
PRIVATE void MacroStore_print(MacroStore * this);
PRIVATE unsigned int MacroStore_getSize(MacroStore * this);
PRIVATE int MacroStore_insertName(MacroStore* this, String* name, MacroDefinition* body);
PRIVATE int MacroStore_removeName(MacroStore* this, String* name);
PRIVATE enum MacroEvalName MacroStore_evalName(MacroStore* this, char* ptr, int length);
PRIVATE void MacroStore_printChildrenNode(MacroStore* this, struct MacroStoreNode* node, char * name);

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

  this->root = (struct MacroStoreNode * )Memory_alloc(sizeof(struct MacroStoreNode));

  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    this->root->children[i] = 0;
    this->root->isLeaf = 1;
    this->root->def = 0;
  }

  for (int c = 0; c < 255; c++)
  {
    if ((c >= 'A') && (c <= 'Z')) 
    {
      convert[c] = c - 'A' + 2;
      convertBack[c - 'A' + 2] = c;
    }
    else if (c == '_') 
    {
      convert[c] = 1;
      convertBack[1] = c;
    }
    else
    {
      convert[c] = 0;
      convertBack[0] = 0;
    }
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
  char * name = Memory_alloc(256); // MAX Macro name length
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    if (currentNode->children[i])
    {
      *name = convertBack[i];
      if (!currentNode->isLeaf) 
        MacroStore_printChildrenNode(this, currentNode->children[i], name);
      else
      {
        *(name+1) = 0;
        PRINT(("%s\n", name));
      }
    }
  }
  Memory_free(name, 256);
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
    if (currentNode->isLeaf)
    {
      currentNode->isLeaf = 0;
      currentNode->children[c] = Memory_alloc(sizeof(struct MacroStoreNode));
      currentNode = currentNode->children[c];
    }
    else if (currentNode->children[c])
      currentNode = currentNode->children[c];
    else
    {
      currentNode->children[c] = Memory_alloc(sizeof(struct MacroStoreNode));
      currentNode = currentNode->children[c];
    }
  }
  currentNode->isLeaf = 1;
  currentNode->def = body;

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

  }

  return E_NOT_MACRO;
}

PRIVATE void MacroStore_printChildrenNode(MacroStore* this, struct MacroStoreNode* node, char * name)
{
  if (node == 0) return;

  for (int i = 0; i < MAX_CHILDREN; i++)
  {

  }
}
#endif /* _MACROSTORE_H_ */
