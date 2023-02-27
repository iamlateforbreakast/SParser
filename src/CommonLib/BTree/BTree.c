/*********************************************************************************
* BTree.c
*
*********************************************************************************/
#include "BTree.h"
#include "Node.h"
#include "Pool.h"

#define MAX_NODES (100)

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
	tree->nodeSize = sizeof(unsigned short int) * 2
		+ sizeof(unsigned int) * (tree->order * 2 - 1)
		+ sizeof(Object) * (tree->order * 2)
		+ sizeof(unsigned int) * (tree->order * 2);
	//tree->pool = Pool_newFromFile("test.pool", MAX_NODES, tree->nodeSize);
	tree->pool = Pool_new(MAX_NODES, tree->nodeSize);
	tree->root = 0;
	tree->nbNodes = Pool_reportNbNodes(tree->pool);
	tree->depth = 0;
	tree->nbObjects = Pool_reportNbNodes(tree->pool);


	printf("Node size in bytes: %d\n", sizeof(Node));
	return tree;
}

/*********************************************************************************
* BTree_add
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
void BTree_add(BTree* tree, Key key, Object object)
{
	if (tree->nbObjects == MAX_NODES)
	{
		 printf("Tree is full\n");
		 return;
	}

 	if (tree->nbObjects == 0)
	{
		 unsigned int newLeafIdx;
		 char* node = Pool_alloc(tree->pool, &newLeafIdx);
		 short unsigned int * nbKeyUsed = node;
		 short unsigned int * isLeaf = nbKeyUsed + sizeof(short unsigned int);
		 unsigned int * keys = isLeaf + sizeof(short unsigned int);
		 Object * leaves = keys + sizeof(unsigned int) * (2 * tree->order - 1);
		 unsigned int * children = leaves + sizeof(unsigned int) * (2 * tree->order);

		 *nbKeyUsed = 1;
		 *isLeaf = 1;
		 keys[0] = key;
		 //Pool_addToChunkCache(tree->pool, &key, sizeof(key)); //unsigned int keys[ORDER * 2 - 1];
		 for (int i = 1; i < tree->order * 2 - 1; i++)
		 {
			 keys[i] = 0;
			 //Pool_addToChunkCache(tree->pool, &null, sizeof(null));
		 }
		 leaves[0] = object;
		 //Pool_addToChunkCache(tree->pool, (Object)object, sizeof(object)); //Object leaves[ORDER * 2];
		 for (int i = 1; i < tree->order * 2; i++)
		 {
			 leaves[i] = 0;
			 //Pool_addToChunkCache(tree->pool, &null, sizeof(null));
		 }
		 for (int i = 0; i < tree->order * 2; i++)
		 {
			 children[i] = 0;
			 //Pool_addToChunkCache(tree->pool, &null, sizeof(null)); //unsigned int children[ORDER * 2];
		 }
		 Pool_writeCache(tree->pool, newLeafIdx);
		 tree->depth = 1;
		 tree->root = newLeafIdx;
		 tree->nbObjects = 1;
		 return;
	}
	else
	{
		Node root;
		Pool_read(tree->pool, tree->root, &root);
		if (root.nbKeyUsed == ORDER * 2 - 1)
		{
			//printf("Splitting root\n");
			Node newRoot;
			unsigned int newRootIdx = Node_new(tree->pool);  //Node newLeaf
			Pool_read(tree->pool, newRootIdx, &newRoot);
			newRoot.children[0] = tree->root;
			Pool_addToChunkCache(tree->pool, &newRoot.children[0], sizeof(newRoot.children[0]));
			Pool_writeCache(tree->pool, newRootIdx);
			unsigned int childForInsertionIdx = Node_splitNode(newRootIdx, tree->root, key, tree->pool);
			Node_insert(childForInsertionIdx, key, object, tree->order, tree->pool);
			tree->root = newRootIdx;
			tree->depth++;
			tree->nbObjects++;
			return;
		}
	}
  	Node_insert(tree->root, key, object, tree->order, tree->pool);
	tree->nbObjects++;
}

/*********************************************************************************
* BTree_get
* input: key
* output: A reference to a beamWeightRange
*********************************************************************************/
PUBLIC void BTree_get(BTree* tree, Key key, Object * object)
{
	Node_search(tree->root, key, object, FALSE, tree->pool);

	return;
}

/*********************************************************************************
* BTree_remove
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
Object BTree_remove(BTree* tree, Key key)
{
	Object object = NULL;
#if 0
	Node* root = tree->root;
	// Pool_read(root)
  	if (root->nbKeyUsed == 0) return NULL;

	if (root->isLeaf)
	{
		for (int i = 0; i < root->nbKeyUsed; i++)
		{
			if (key == root->keys[i])
			{
				object = root->leaves[i];
				for (int j = i; j < root->nbKeyUsed; j++)
				{
					root->keys[j] = root->keys[j + 1];
				}
				for (int j = 0; j <= root->nbKeyUsed; j++)
				{
					root->children[j] = root->children[j + 1];
					root->leaves[j] = root->leaves[j + 1];
				}
				root->nbKeyUsed--;
				tree->nbObjects--;
			}
		}
		return object;
	}
	else
	{
		object = Node_remove(root, key, NULL, tree->pool);
		// Check the resulting tree so that there is at least 1 Key used at root level
		if (tree->root->nbKeyUsed < 1)
		{
			printf("Tree should collapse\n");
			tree->root = root->children[0];
			tree->depth--;
			Node_free(root, tree->pool);
		}
		// Check something was actually removed
		if (object != NULL)
		{
			tree->nbObjects--;
		}
		else
		{
			printf("Key %d removed returned NULL\n", key);
		}
		return object;
	}
#endif
	return object;
}

/*********************************************************************************
* BTree_print
* input: None
* output: None
*********************************************************************************/
void BTree_print(BTree* tree)
{
	if (tree == NULL) return;
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
	BTree* tree = NULL;

	return tree;
}

/*********************************************************************************
* BTree_free
* input: None
* output: None
*********************************************************************************/
PUBLIC void BTree_free(BTree* tree)
{
	Node_free(tree->root, tree->pool);
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
