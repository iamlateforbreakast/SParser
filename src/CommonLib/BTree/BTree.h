#ifndef _BTREE_
#define _BTREE_
/*********************************************************************************
* BTree.h
*
*********************************************************************************/
#include "CommonTypes.h"
#include "Node.h"
#include "Pool.h"

typedef struct BTree
{
	Pool * pool;
        unsigned int root;
	unsigned int depth;
	unsigned short int nbObjects;
	unsigned short int nbNodes;
	unsigned int order;
	unsigned int nodeSize;
} BTree;

PUBLIC BTree * BTree_new(unsigned int order);
PUBLIC BTree * BTree_newFromFile(char* fileName);
PUBLIC void BTree_free(BTree * tree);
PUBLIC void BTree_add(BTree * tree, Key key, Object object);
PUBLIC void BTree_get(BTree * tree, Key key, Object * object);
PUBLIC Object BTree_remove(BTree * tree, Key key);
PUBLIC void BTree_print(BTree * tree);
PUBLIC unsigned int BTree_sizeof(BTree* tree);
PUBLIC unsigned int BTree_reportSizeInBytes(BTree * tree);

#endif /* _BTREE_ */
