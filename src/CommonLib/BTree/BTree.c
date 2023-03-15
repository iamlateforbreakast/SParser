/*********************************************************************************
* BTree.c
*
*********************************************************************************/
#include "BTree.h"
#include "Node.h"
#include "Pool.h"

#define MAX_NODES (10000)

/*********************************************************************************
* BTree_new
* input: number of total beamweight ranges to store in tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
BTree * BTree_new(unsigned int order)
{
	BTree* tree;

	tree = (BTree*)malloc(sizeof(BTree));
	tree->order = order;
	tree->nodeSize = Node_getSize(tree->order);
	//tree->pool = Pool_newFromFile("test.pool", MAX_NODES, tree->nodeSize);
	tree->pool = Pool_new(MAX_NODES, tree->nodeSize);
	tree->root = 0;
	tree->nbNodes = Pool_reportNbNodes(tree->pool);
	tree->depth = 0;
	tree->nbObjects = Pool_reportNbNodes(tree->pool);


	//printf("Node size in bytes: %d\n", sizeof(Node));
	return tree;
}

/*********************************************************************************
* BTree_add
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
void BTree_add(BTree* tree, unsigned int key, void * object)
{
	if (tree->nbObjects == MAX_NODES)
	{
		 printf("Tree is full\n");
		 return;
	}

 	if (tree->nbObjects == 0)
	{
		 unsigned int newLeafIdx;
		 void* ptrBuffer = Pool_alloc(tree->pool, &newLeafIdx);
		 Node node = Node_read(newLeafIdx, tree->order, ptrBuffer);

		 *node.nbKeyUsed = 1;
		 *node.isLeaf = 1;
		 node.keys[0] = (unsigned int)key;
		 //Pool_addToChunkCache(tree->pool, &key, sizeof(key)); //unsigned int keys[ORDER * 2 - 1];
		 for (unsigned int i = 1; i < tree->order * 2 - 1; i++)
		 {
			 node.keys[i] = 0;
			 //Pool_addToChunkCache(tree->pool, &null, sizeof(null));
		 }
		 node.leaves[0] = object;
		 //Pool_addToChunkCache(tree->pool, (void*)object, sizeof(object)); //Object leaves[ORDER * 2];
		 for (unsigned int i = 1; i < tree->order * 2; i++)
		 {
			 node.leaves[i] = 0;
			 //Pool_addToChunkCache(tree->pool, &null, sizeof(null));
		 }
		 for (unsigned int i = 0; i < tree->order * 2; i++)
		 {
			 node.children[i] = 0;
			 //Pool_addToChunkCache(tree->pool, &null, sizeof(null)); //unsigned int children[ORDER * 2];
		 }
		 Pool_write(tree->pool, newLeafIdx, ptrBuffer);
		 tree->depth = 1;
		 tree->root = newLeafIdx;
		 tree->nbObjects = 1;
		 return;
	}
	else
	{
		Node node = Node_read(tree->root, tree->order, Pool_read(tree->pool, tree->root));

		if (*node.nbKeyUsed == tree->order * 2 - 1)
		{
			printf("Splitting root\n");
			unsigned int newRootIdx;
			void * ptrBuffer = Pool_alloc(tree->pool, &newRootIdx);  //Node newLeaf;
			Node newRoot = Node_read(newRootIdx, tree->order, ptrBuffer);
			*newRoot.nbKeyUsed = 0;
			*newRoot.isLeaf = 0;
			newRoot.children[0] = tree->root;
			Pool_write(tree->pool, newRootIdx, ptrBuffer);
			
			unsigned int childForInsertionIdx = Node_splitNode(newRootIdx, tree->root, key, tree->order, tree->pool);
			Node testNode = Node_read(newRootIdx, tree->order, Pool_read(tree->pool, newRootIdx));
			printf("Top node\n");
			printf("nbUsedKeys=%d\n",*testNode.nbKeyUsed);
			printf("children 0=%d\n", testNode.children[0]);
			printf("children 1=%d\n", testNode.children[1]);
			Node_insert(childForInsertionIdx, key, object, tree->order, tree->pool);
			tree->root = newRootIdx;
			tree->depth++;
			tree->nbObjects++;
			return;
		}
		Node_insert(tree->root, key, object, tree->order, tree->pool);
		Pool_discardCache(tree->pool, tree->root);
		tree->nbObjects++;
	}
}

/*********************************************************************************
* BTree_get
* input: key
* output: A reference to a beamWeightRange
*********************************************************************************/
PUBLIC void BTree_get(BTree* tree, Key key, void ** object)
{
	Node_search(tree->root, tree->order, key, object, 0, tree->pool);

	return;
}

/*********************************************************************************
* BTree_remove
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
void * BTree_remove(BTree* tree, unsigned int key)
{
	void * object = 0;
    void * ptrContent = Pool_read(tree->pool, tree->root);
    Node root = Node_read(tree->root, tree->order, ptrContent);

  	if (*root.nbKeyUsed == 0) return 0;

	if (*root.isLeaf)
	{
		for (int i = 0; i < *root.nbKeyUsed; i++)
		{
			if (key == root.keys[i])
			{
				object = root.leaves[i];
				for (int j = i; j < *root.nbKeyUsed; j++)
				{
					root.keys[j] = root.keys[j + 1];
				}
				for (int j = 0; j <= *root.nbKeyUsed; j++)
				{
					root.children[j] = root.children[j + 1];
					root.leaves[j] = root.leaves[j + 1];
				}
				(*root.nbKeyUsed)--;
				tree->nbObjects--; // Pool_write();
				Pool_write(tree->pool, tree->root, ptrContent);
				return object;
			}
		}
		Pool_discardCache(tree->pool, tree->root);
		return 0;
	}
	else
	{
		object = Node_remove(tree->root, tree->order, key, 0, tree->pool);
		// Check the resulting tree so that there is at least 1 Key used at root level
		if (*root.nbKeyUsed < 1)
		{
			printf("Tree should collapse\n");
			Node_free(tree->root, tree->order, tree->pool);
			tree->root = root.children[0];
			tree->depth--;
			Node_free(tree->root, tree->order, tree->pool);
		}
		// Check something was actually removed
		if (object != 0)
		{
			tree->nbObjects--;
		}
		else
		{
			printf("Key %d removed returned NULL\n", key);
		}
		return object;
	}
	return object;
}

/*********************************************************************************
* BTree_print
* input: None
* output: None
*********************************************************************************/
void BTree_print(BTree* tree)
{
	if (tree == 0) return;
	printf("Tree:\n");
	printf(" Nb items: %d\n", tree->nbObjects);
	printf(" Nb Nodes: %d\n", tree->nbNodes);
	printf(" Depth: %d\n", tree->depth);

	if (tree->depth > 0)
	{
		printf("Root 0-0:\n");
		Node_print(tree->root, tree->order, tree->depth - 1, tree->pool);
	}
	    
}

/*********************************************************************************
* BTree_newFromFile
* input: None
* output: None
*********************************************************************************/
PUBLIC BTree * BTree_newFromFile(char * fileName)
{
	BTree* tree = 0;

	return tree;
}

/*********************************************************************************
* BTree_free
* input: None
* output: None
*********************************************************************************/
PUBLIC void BTree_free(BTree* tree)
{
	Node_free(tree->root, tree->order, tree->pool);
	Pool_free(tree->pool);

	free(tree);
}

/*********************************************************************************
* BTree_reportSize
* input: None
* output: None
*********************************************************************************/
PUBLIC unsigned int BTree_reportSizeInBytes(BTree * tree)
{
	return Pool_reportSizeInBytes(tree->pool);
}
