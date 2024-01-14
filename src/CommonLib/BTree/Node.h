/*
* Node.h
*/
#ifndef _NODE_
#define _NODE_

#include "Types.h"
#include "CommonTypes.h"

#define ORDER (3)

typedef struct Node Node;

typedef struct Node
{
	unsigned short int nbKeyUsed;
	unsigned short int isLeaf;
	unsigned int * keys;
	Object * leaves;
	Node** children;	
} Node;

PUBLIC Node * Node_new(unsigned short int isLeaf);
PUBLIC Node* Node_splitNode(Node* node, Node* nodeToSplit, Key key);
PUBLIC void Node_insert(Node* node, Key key, Object object, int isOwner);
PUBLIC Object Node_remove(Node* node, Key key, unsigned int* keyToUpdate);
PUBLIC Object Node_search(Node* node, Key key, unsigned int isFoundAlready);
PUBLIC void Node_free(Node* node);
PUBLIC void Node_print(Node* node, unsigned int depth);

#endif /* _NODE_ */

