/*
* Node.h
*/
#ifndef _NODE_
#define _NODE_

#include "Types.h"
#include "Pool.h"

typedef unsigned int Key;

typedef struct Node Node;

typedef struct Node
{
	unsigned int idx;
	unsigned short int nbKeyUsed;
	unsigned short int isLeaf;
	unsigned int * keys;
	void ** leaves;
	unsigned int * children;	
} Node;

PUBLIC unsigned int Node_new(Pool* pool);
PUBLIC unsigned int Node_splitNode(unsigned int nodeIdx, unsigned int nodeToSplitIdx, Key key, Pool* pool);
PUBLIC void Node_insert(unsigned int nodeIdx, Key key, void * object, unsigned int order, Pool* pool);
PUBLIC void ** Node_remove(unsigned int nodeIdx, Key key, unsigned int* keyToUpdate, Pool* pool);
PUBLIC void Node_search(unsigned int nodeIdx, Key key, void ** object, unsigned int isFoundAlready, Pool* pool);
PUBLIC void Node_free(unsigned int nodeIdx, Pool* pool);
PUBLIC void Node_print(unsigned int nodeIdx, unsigned int order, unsigned int depth, Pool* pool);
PUBLIC unsigned int Node_getSize(unsigned int order);
PUBLIC Node Node_read(unsigned int nodeIdx, Pool* pool, void * ptrContent);

#endif /* _NODE_ */
