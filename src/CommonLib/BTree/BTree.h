#ifndef _BTREE_
#define _BTREE_
/*********************************************************************************
* BTree.h
*
*********************************************************************************/
#include "Types.h"
#include "Object.h"
#include "Allocator.h"

typedef struct BTree BTree;

PUBLIC BTree * BTree_new(unsigned int order);
PUBLIC BTree * BTree_newFromAllocator(Allocator * allocator);
PUBLIC BTree * BTree_newFromFile(char* fileName);
PUBLIC void BTree_delete(BTree * tree);
PUBLIC BTree * BTree_copy(BTree * this);
PUBLIC int BTree_comp(BTree * this, BTree * compared);
PUBLIC void BTree_add(BTree * tree, Object * key, Object * object, int isOwner);
PUBLIC Object * BTree_get(BTree * tree, Object * key);
PUBLIC Object * BTree_remove(BTree * tree, Object * key);
PUBLIC void BTree_print(BTree * tree);
PUBLIC unsigned int BTree_sizeof(BTree* tree);
PUBLIC unsigned int BTree_reportSizeInBytes(BTree * tree);
PUBLIC unsigned int BTree_getSize(BTree * this);
PUBLIC unsigned int BTree_getNbNodes(BTree* this);

#endif /* _BTREE_ */

