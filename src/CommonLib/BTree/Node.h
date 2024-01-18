/*
* Node.h
*/
#ifndef _NODE_
#define _NODE_

#include "Types.h"
#include "Object.h"

typedef struct Node Node;

typedef struct Node
{
	unsigned int nbKeyUsed;
	unsigned int isLeaf;
	Object ** keys;
	Object ** leaves;
	Node ** children;
	Object* buffer[18];	
} Node;

PUBLIC Node * Node_new(unsigned short int isLeaf, unsigned int order);
PUBLIC Node* Node_splitNode(Node * node, unsigned int order, Node* nodeToSplit, Object * key);
PUBLIC void Node_insert(Node * node, unsigned int order, Object * key, Object * object, int isOwner);
PUBLIC Object * Node_remove(Node * node, unsigned int order, Object * key, Object ** keyToUpdate);
PUBLIC Object * Node_search(Node * node, unsigned int order, Object * key, unsigned int isFoundAlready);
PUBLIC void Node_free(Node * node, unsigned int order);
PUBLIC void Node_print(Node * node, unsigned int order, unsigned int depth);
PUBLIC unsigned int Node_getNbNodes(Node * node);
#endif /* _NODE_ */

