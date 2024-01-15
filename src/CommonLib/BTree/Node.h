/*
* Node.h
*/
#ifndef _NODE_
#define _NODE_

#include "Types.h"
#include "Object.h"

#define ORDER (3)

typedef struct Node Node;

typedef struct Node
{
	unsigned int nbKeyUsed;
	unsigned int isLeaf;
	Object ** keys;
	Object ** leaves;
	Node ** children;	
} Node;

PUBLIC Node * Node_new(unsigned short int isLeaf);
PUBLIC Node* Node_splitNode(Node* node, Node* nodeToSplit, Object * key);
PUBLIC void Node_insert(Node* node, Object * key, Object * object, int isOwner);
PUBLIC Object * Node_remove(Node* node, Object * key, Object ** keyToUpdate);
PUBLIC Object * Node_search(Node* node, Object * key, unsigned int isFoundAlready);
PUBLIC void Node_free(Node* node);
PUBLIC void Node_print(Node* node, unsigned int depth);

#endif /* _NODE_ */

