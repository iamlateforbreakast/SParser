/*
* Node.h
*/
#ifndef _NODE_
#define _NODE_

#include "Types.h"
#include "CommonTypes.h"
#include "Pool.h"

#define ORDER (3)

typedef struct Node Node;

typedef struct Node
{
	unsigned short int nbKeyUsed;
	unsigned short int isLeaf;
	unsigned int keys[ORDER * 2 - 1];
	Object leaves[ORDER * 2];
	Node* children[ORDER * 2];	
} Node;

PUBLIC Node * Node_new(unsigned short int isLeaf, Pool * pool);
PUBLIC Node* Node_splitNode(Node* node, Node* nodeToSplit, Key key, Pool * pool);
PUBLIC void Node_insert(Node* node, Key key, Object object, Pool * pool);
PUBLIC Object Node_remove(Node* node, Key key, unsigned int* keyToUpdate, Pool * pool);
PUBLIC Object Node_search(Node* node, Key key, unsigned int isFoundAlready, Pool * pool);
PUBLIC void Node_free(Node* node, Pool * pool);
PUBLIC void Node_print(Node* node, unsigned int depth, Pool * pool);

#endif /* _NODE_ */
