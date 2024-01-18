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

/**********************************************//**
  @class BTree
**************************************************/
struct BTree
{
	Object object;
	Node* root;
	unsigned int order;
	unsigned int depth;
	unsigned short int nbObjects;
	unsigned int nodeSize;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class btreeClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&BTree_delete,
  .f_copy = (Copy_Operator)&BTree_copy,
  .f_comp = (Comp_Operator)&BTree_comp,
  .f_print = (Printer)&BTree_print,
  .f_size = (Sizer)&BTree_getSize
};

/*********************************************************************************
* @brief BTree_new
* input: number of total beamweight ranges to store in tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
PUBLIC BTree * BTree_new(unsigned int order)
{
  BTree* tree;

  //tree = (BTree*)Memory_alloc(sizeof(BTree));
  tree = (BTree*)Object_new(sizeof(BTree),&btreeClass);
    
  if (tree == 0) return 0;

  tree->root = 0;
  tree->order = order;
  tree->depth = 0;
  tree->nbObjects = 0;

  return tree;
}

/*********************************************************************************
* @brief BTree_delete
* input: None
* output: None
*********************************************************************************/
PUBLIC void BTree_delete(BTree* this)
{
  if (this!=0)
  {
    Node_free(this->root, this->order);
	
    //Memory_free(this, sizeof(BTree));
	/* De-allocate the base object */
    Object_deallocate(&this->object);
  }
}

/*********************************************************************************
* @brief BTree_copy
* input: None
* output: Copy of the instance
*********************************************************************************/
PUBLIC BTree * BTree_copy(BTree * this)
{
  return 0;
}

/*********************************************************************************
* @brief BTree_comp
* input: None
* output: 0 if equal
*********************************************************************************/
PUBLIC int BTree_comp(BTree * this, BTree * compared)
{
	return 0;
}

/*********************************************************************************
* @brief BTree_add
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
PUBLIC void BTree_add(BTree* tree, Object * key, Object * object, int isOwner)
{
	if (tree->root == 0)
	{
		Node* newLeaf = Node_new(1, tree->order);
		newLeaf->nbKeyUsed = 1;
		newLeaf->leaves[0] = object;
		newLeaf->keys[0] = key;
		tree->depth = 1;
		tree->root = newLeaf;
		tree->nbObjects=1;
		return;
	}
	if (tree->root->nbKeyUsed == tree->order * 2 - 1)
	{ 
		PRINT(("Splitting root\n"));
		Node* newRoot = Node_new(0, tree->order);
		newRoot->children[0] = tree->root;
		Node* childForInsertion = Node_splitNode(newRoot, tree->order, tree->root, key);
		Node_insert(childForInsertion, tree->order, key, object, isOwner);
		tree->root = newRoot;
		tree->depth++;
		tree->nbObjects++;
		return;
	}
	/* Root is not empty and not full */
  	Node_insert(tree->root, tree->order, key, object, isOwner);
	tree->nbObjects++;
}

/*********************************************************************************
* @brief BTree_get
* input: key
* output: A reference to a beamWeightRange
*********************************************************************************/
PUBLIC Object * BTree_get(BTree* tree, Object * key)
{
	Object * object = 0;

	object = Node_search(tree->root, tree->order, key, 0);

	return object;
}

/*********************************************************************************
* @brief BTree_remove
* input: key
* input: a beamweight range to store in the tree
* output: A fully allocated beamweihgt tree
*********************************************************************************/
PUBLIC Object * BTree_remove(BTree* tree, Object * key)
{
	Object * object = 0;
	Node* root = tree->root;

  	if (root->nbKeyUsed == 0) return 0;

	if (root->isLeaf)
	{
		for (unsigned int i = 0; i < root->nbKeyUsed; i++)
		{
			if (Object_comp(key, root->keys[i])==0)
			{
				object = root->leaves[i];
				for (unsigned int j = i; j < root->nbKeyUsed; j++)
				{
					root->keys[j] = root->keys[j + 1];
				}
				for (unsigned int j = 0; j <= root->nbKeyUsed; j++)
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
		object = Node_remove(root, tree->order, key, 0);
		// Check the resulting tree so that there is at least 1 Key used at root level
		if (tree->root->nbKeyUsed < 1)
		{
			PRINT(("Tree should collapse\n"));
			Node * newRoot = root->children[0];
			tree->root->isLeaf = 1; /* Hack to avoid recursion when freeing */
			Node_free(tree->root, tree->order);
			tree->root = newRoot;
			tree->depth--;
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
* @brief BTree_print
* input: None
* output: None
*********************************************************************************/
PUBLIC void BTree_print(BTree* tree)
{
  if (tree == 0) return;
  PRINT(("Tree:\n"));
  PRINT((" Nb items: %d\n", tree->nbObjects));
  PRINT((" Depth: %d\n", tree->depth));

  if (tree->depth > 0)
  {
    PRINT(("Root 0-0:\n"));
    Node_print(tree->root, tree->order, tree->depth - 1);
  }
}

/*********************************************************************************
* @brief BTree_newFromFile
* @public
* @memberof BTree
* @return Instance of BTree
*********************************************************************************/
PUBLIC BTree * BTree_newFromFile(char * fileName)
{
  BTree* tree = 0;

  return tree;
}

/*********************************************************************************
* @brief BTree_getSize
* @public
* @memberof BTree
* @return Size of instance
*********************************************************************************/
PUBLIC unsigned int BTree_getSize(BTree * this)
{
  if (this==0) return sizeof(BTree);

  return sizeof(BTree); 
}

/*********************************************************************************
* @brief BTree_getNbNodes
* @public
* @memberof BTree
* @return Size of instance
*********************************************************************************/
PUBLIC unsigned int BTree_getNbNodes(BTree* this)
{
  return Node_getNbNodes(this->root);
}
