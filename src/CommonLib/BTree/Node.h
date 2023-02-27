/*
* Node.h
*/
#ifndef _NODE_
#define _NODE_

#include "Types.h"
#include "Pool.h"

#define ORDER (3) // ORDER must be 2 or more

typedef struct Node Node;

typedef struct Node
{
	unsigned short int nbKeyUsed;
	unsigned short int isLeaf;
	unsigned int keys[ORDER * 2 - 1];
	Object leaves[ORDER * 2];
	unsigned int children[ORDER * 2];	
} Node;

PUBLIC unsigned int Node_new(Pool* pool);
PUBLIC unsigned int Node_splitNode(unsigned int nodeIdx, unsigned int nodeToSplitIdx, Key key, Pool* pool);
PUBLIC void Node_insert(unsigned int nodeIdx, Key key, Object object, unsigned int order, Pool* pool);
PUBLIC Object Node_remove(unsigned int nodeIdx, Key key, unsigned int* keyToUpdate, Pool* pool);
PUBLIC void Node_search(unsigned int nodeIdx, Key key, Object object, unsigned int isFoundAlready, Pool* pool);
PUBLIC void Node_free(unsigned int nodeIdx, Pool* pool);
PUBLIC void Node_print(unsigned int nodeIdx, unsigned int order, unsigned int depth, Pool* pool);

#endif /* _NODE_ */
