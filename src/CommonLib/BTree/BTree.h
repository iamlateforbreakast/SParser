#ifndef _BTREE_
#define _BTREE_
/*********************************************************************************
* BTree.h
*
*********************************************************************************/
#include "Types.h"
#include "Node.h"
#include "Object.h"
#include "Allocator.h"

typedef struct BTree
{
	Node* root;
	unsigned int order;
	unsigned int depth;
	unsigned short int nbObjects;
	unsigned short int nbNodes;
	unsigned int nodeSize;
} BTree;

PUBLIC BTree * BTree_new(unsigned int order);
PUBLIC BTree * BTree_newFromAllocator(Allocator * allocator);
PUBLIC BTree * BTree_newFromFile(char* fileName);
PUBLIC void BTree_delete(BTree * tree);
PUBLIC void BTree_add(BTree * tree, Object * key, Object * object, int isOwner);
PUBLIC Object * BTree_get(BTree * tree, Object * key);
PUBLIC Object * BTree_remove(BTree * tree, Object * key);
PUBLIC void BTree_print(BTree * tree);
PUBLIC unsigned int BTree_sizeof(BTree* tree);
PUBLIC unsigned int BTree_reportSizeInBytes(BTree * tree);


#endif /* _BTREE_ */

