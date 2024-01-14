/**********************************************//**
  @file BTree.c
     
  @brief This file contains the implementation of the class BTree.
				        
  The class BTree implements the BTree operations:
  - init
  - add
  - remove
****************************************************/
#include "BTree.h"
#include "Node.h"
#include "Memory.h"
#include "Debug.h"

/*********************************************************************************
* BTree_new
* input: number of total beamweight ranges to store in tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
BTree * BTree_new(unsigned int order)
{
	BTree* tree;

	tree = (BTree*)Memory_alloc(sizeof(BTree));
	//tree->pool = 0; //Pool_new(nodePool, nbObjects);
	tree->root = 0;
	tree->nbNodes = 0;
	tree->order = order;
	tree->depth = 0;
	tree->nbObjects = 0;

	return tree;
}

/*********************************************************************************
* BTree_add
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
void BTree_add(BTree* tree, Object * key, Object * object, int isOwner)
{
	if (tree->root == 0)
	{
		Node* newLeaf = Node_new(1);
		newLeaf->nbKeyUsed = 1;
		newLeaf->leaves[0] = object;
		newLeaf->keys[0] = key;
		tree->depth = 1;
		tree->root = newLeaf;
		tree->nbObjects=1;
		return;
	}
	if (tree->root->nbKeyUsed == ORDER * 2 - 1)
	{ 
		printf("Splitting root\n");
		Node* newRoot = Node_new(0);
		newRoot->children[0] = tree->root;
		Node* childForInsertion = Node_splitNode(newRoot, tree->root, key);
		Node_insert(childForInsertion, key, object, isOwner);
		tree->root = newRoot;
		tree->depth++;
		tree->nbObjects++;
		return;
	}
  	Node_insert(tree->root, key, object, isOwner);
	tree->nbObjects++;
}

/*********************************************************************************
* BTree_get
* input: key
* output: A reference to a beamWeightRange
*********************************************************************************/
Object * BTree_get(BTree* tree, Object * key)
{
	Object * object = 0;

	object = Node_search(tree->root,key, 0);

	return object;
}

/*********************************************************************************
* BTree_remove
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
Object * BTree_remove(BTree* tree, Object * key)
{
	Object * object = 0;
	Node* root = tree->root;

  	if (root->nbKeyUsed == 0) return 0;

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
		object = Node_remove(root, key, 0);
		// Check the resulting tree so that there is at least 1 Key used at root level
		if (tree->root->nbKeyUsed < 1)
		{
			printf("Tree should collapse\n");
			tree->root = root->children[0];
			tree->depth--;
			Node_free(root);
		}
		// Check something was actually removed
		if (object != 0)
		{
			tree->nbObjects--;
		}
		else
		{
			PRINT(("Key %d removed returned 0\n", key));
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
  PRINT(("Tree:\n"));
  PRINT((" Nb items: %d\n", tree->nbObjects));
  PRINT((" Nb Nodes: %d\n", tree->nbNodes));
  PRINT((" Depth: %d\n", tree->depth));

  if (tree->depth > 0)
  {
    PRINT(("Root 0-0:\n"));
    Node_print(tree->root, tree->depth - 1);
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
* BTree_delete
* input: None
* output: None
*********************************************************************************/
PUBLIC void BTree_delete(BTree* tree)
{
  Node_free(tree->root);
	
  Memory_free(tree, sizeof(BTree));
}
	
