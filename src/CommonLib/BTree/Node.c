#include "Node.h"
#include "Pool.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#define DEBUG (0)
/*********************************************************************************
*
* Private Functions Declarations
*
*********************************************************************************/
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int idx1, unsigned int idx2);
PRIVATE void Node_shiftRight(Node* node, unsigned int idxKey);
PRIVATE void Node_shiftLeft(Node* node, unsigned int idxKey);
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo);
PRIVATE void Node_stealRightKey(Node* node, unsigned int idxKeyStealFrom, unsigned int idxKeyGiveTo);

/*********************************************************************************
* Node_new
* input: isLeaf TRUE if creating a terminal node
* output: the newly created beam weight node
*********************************************************************************/
PUBLIC Node* Node_new(unsigned short int isLeaf)
{
	Node* node = 0;
    const unsigned int size = sizeof(node->nbKeyUsed) + sizeof(node->isLeaf)
	                        + (ORDER * 2 - 1) * sizeof(Object*)
							+ (ORDER * 2) * sizeof(Object*)
							+ (ORDER * 2) * sizeof(Node*);
	node = (Node*)Memory_alloc(size);
	node->isLeaf = isLeaf;
	node->nbKeyUsed = 0;
    node->keys = node + sizeof(node->nbKeyUsed) + sizeof(node->isLeaf);
	node->leaves = node->keys + (ORDER * 2 - 1) * sizeof(Object*);
    node->children = node->leaves + (ORDER * 2) * sizeof(Object*);

	for (int i = 0; i < ORDER * 2; i++)
	{
		node->children[i] = 0;
		node->leaves[i] = 0;
	}
	for (int i = 0; i < ORDER * 2 - 1; i++)
	{
		node->keys[i] = 0;
	}

	return node;
}

/*********************************************************************************
* Node_search
* input: the key to look for
* output: the beam weight range if found otherwise 0
*********************************************************************************/
PUBLIC Object * Node_search(Node* node, Object * key, unsigned int isFoundAlready)
{
	if (node == 0)
	{
		Error_new(ERROR_FATAL,"Node_search node== 0\n");
	}
	if (node->isLeaf == 1)
	{
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (node->keys[i] == key)
			{
				return node->leaves[i];
			}
		}
		if (isFoundAlready)
			return node->leaves[node->nbKeyUsed - 1];
		else
			return 0;
	}
	else
	{
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (key < node->keys[i])
			{
				if (node->children[i] == 0)
				{
					Error_new(ERROR_FATAL, "Node_search node== 0\n");
				}
				return Node_search(node->children[i], key, isFoundAlready);
			}
			if (key == node->keys[i])
				return Node_search(node->children[i], key, 1);
		}
		return Node_search(node->children[node->nbKeyUsed], key, isFoundAlready);
	}

	return 0;
 }

/*********************************************************************************
* Node_free
* input: none
* output: none
*********************************************************************************/
PUBLIC void Node_free(Node* node)
{
  if (node == 0) return;
  if (node->isLeaf == 1)
  {
    for (int i = 0; i < node->nbKeyUsed + 1; i++)
	{
		//free(node->leaves[i]);
	}
	return;
  }
}

/*********************************************************************************
* Node_insert
* input: key
* input: beamWeightRange
* output: none
*********************************************************************************/
PUBLIC void Node_insert(Node* node, Object * key, Object * object, int isOwner)
{
	if (node->isLeaf == 1) 
	{
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (key == node->keys[i])
			{
				Error_new(ERROR_NORMAL, "Duplicate Key\n");
				return;
			}
			if (key < node->keys[i])
			{
				for (int j = node->nbKeyUsed-1; j >= i; j--)
					node->keys[j + 1] = node->keys[j];
				for (int j = node->nbKeyUsed; j >= i; j--) 
					node->leaves[j + 1] = node->leaves[j];
				node->keys[i] = key;
				node->leaves[i] = object;
				node->nbKeyUsed++;
				return;
			}
		}
		/* POsition to insrt is the last */
		node->keys[node->nbKeyUsed] = key;
		node->leaves[node->nbKeyUsed] = object;
		node->nbKeyUsed++;
		return;
	}
	else
	{
		/* TBC: Check if node is full */
		int i = 0;
		for (i = 0; i < node->nbKeyUsed; i++)
		{
			if (key < node->keys[i])
				break;
		}
		if (node->children[i] == 0)
		{
			Error_new(ERROR_NORMAL, "insert key: %d\n", key);
			Node_print(node, 3);
			Error_new(ERROR_FATAL,"Exiting\n");
		}
		if (node->children[i]->nbKeyUsed < ORDER * 2 - 1) //&& (node->nbKeyUsed < ORDER * 2 - 1))
		{
			Node_insert(node->children[i], key, object, isOwner);
			return;
		}
		else
		{
			TRACE(("Splitting node.\n"));
			Node* newChild = Node_splitNode(node, node->children[i], key);
			Node_insert(newChild, key, object, isOwner);
			return;
		}
	    //Node* childNode = Node_split(node, key);
	    //Node_insert(childNode, key, beamWeightRange);
	    return;
	}
}

/*********************************************************************************
* Node_remove
* input: the key to remove
* output: none
*********************************************************************************/
PUBLIC Object * Node_remove(Node* node, Object * key, unsigned int * keyToUpdate)
{ 
	Object * object = 0;

	if (node->isLeaf == 1)
	{
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (node->keys[i] == key)
			{
				//Can we remove if node->nbKeyUsed => ORDER then can remove
				object = node->leaves[i];
				// Shift all children and leaves left
				for (int j = i; j < node->nbKeyUsed; j++)
				{
					node->keys[j] = node->keys[j + 1];
				}
				for (int j = i; j <= node->nbKeyUsed; j++)
				{
					node->children[j] = node->children[j + 1];
					node->leaves[j] = node->leaves[j + 1];
				}
				node->nbKeyUsed--;
				return object;
			}
		}
		// Check the key was found will descending
		if (keyToUpdate != 0)
		{
			*keyToUpdate = node->keys[node->nbKeyUsed - 1];
			node->nbKeyUsed--;
			object = node->leaves[node->nbKeyUsed - 1];
			return object;
		}
		else 
			return 0;

	}
	else
	{
		// Search which child contains the key
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (key <= node->keys[i])
			{
				// The key is found already while descending the tree, remember it
				if (key <= node->keys[i]) keyToUpdate = &node->keys[i]; /* BUG: <= should be == */
				object = Node_remove(node->children[i], key, keyToUpdate);
				// Check if the number of children is at least ORDER
				if (node->children[i]->nbKeyUsed < ORDER - 1)
				{
					if (node->children[i + 1]->nbKeyUsed <= ORDER - 1)
					{
						// Merge node left and right
						Node_mergeNodes(node, i, i + 1);
					}
					else
					{
						// Steal key right
						Node_stealRightKey(node, i + 1, i);
					}
					return object;
				}
				else
				{
					return object;
				}
			}
			/* SHould there be more checks here */
		}
		object = Node_remove(node->children[node->nbKeyUsed], key, keyToUpdate);
		// Now assess if the node needs to be re-balanced
		if (node->children[node->nbKeyUsed]->nbKeyUsed < ORDER - 1)
		{
			if (node->children[node->nbKeyUsed - 1]->nbKeyUsed <= ORDER - 1)
			{
				// Merge node left and right
				TRACE(("ERROR: Need to merge\n"));
				Node_mergeNodes(node, node->nbKeyUsed - 1, node->nbKeyUsed);
			}
			else
			{
				// Steal key left
				Node_stealLeftKey(node, node->nbKeyUsed - 1, node->nbKeyUsed);
			}
			return object;
		}
		else
		{
			return object;
		}
		
    }

	return 0;
}

/*********************************************************************************
* Node_print
* input: None
* output: None
*********************************************************************************/
PUBLIC void Node_print(Node* node, unsigned int depth)
{
	if (node == 0) return;
	TRACE((" Node NbUsed: %d\n", node->nbKeyUsed));
	TRACE((" Keys: "));
	for (int i = 0; i < ORDER * 2 - 1; i++)
	{
		if (i < node->nbKeyUsed)
			PRINT(("%d ", node->keys[i]));
		else
			PRINT((".. "));
	}
	TRACE(("\n"));
	if ((node->isLeaf==0) && (depth>0))
	{
		for (int i = 0; i <= node->nbKeyUsed; i++)
		{
			TRACE(("Child %d-%d:\n", depth, i));
		    Node_print(node->children[i], depth - 1);
		}
	}
}

/*********************************************************************************
* Node_splitNode
* input: A node to split
* input: the key that caused the split
* output: The sub tree where the key should be inserted
*********************************************************************************/
PUBLIC Node* Node_splitNode(Node* node, Node* nodeToSplit, Object * key)
{
	Node* newChild;

	newChild = Node_new(nodeToSplit->isLeaf);

	unsigned int k;

	for (k = 0; k < node->nbKeyUsed; k++)
	{
		/* TBC: node->nbKeyUsed is assumed less than ORDER*2-1 */
		if (nodeToSplit->keys[ORDER - 1] < node->keys[k]) break;
	}
	for (unsigned int j = node->nbKeyUsed; j > k; j--)
		node->keys[j] = node->keys[j - 1];
	for (unsigned int j = node->nbKeyUsed + 1; j > k; j--)
	{
		node->leaves[j] = node->leaves[j - 1];
		node->children[j] = node->children[j - 1];
	}
	node->keys[k] = nodeToSplit->keys[ORDER - 1];
	node->children[k] = nodeToSplit;
	node->children[k + 1] = newChild;
	node->nbKeyUsed++;
	for (int i = 0; i < ORDER - 1; i++)
		newChild->keys[i] = nodeToSplit->keys[ORDER + i];
	for (int i = 0; i < ORDER; i++)
	{
		newChild->leaves[i] = nodeToSplit->leaves[ORDER + i];
		newChild->children[i] = nodeToSplit->children[ORDER + i];
		nodeToSplit->leaves[ORDER + i] = 0;
		nodeToSplit->children[ORDER + i] = 0;
	}
	newChild->nbKeyUsed = ORDER - 1;
	nodeToSplit->nbKeyUsed = ORDER - 1;


	if (key < node->keys[k])
		return node->children[k];
	else
		return node->children[k + 1];
}

PRIVATE void Node_shiftRight(Node* node, unsigned int idxKey)
{
	for (unsigned int j = node->nbKeyUsed; j > idxKey; j--)
	{
		node->keys[j] = node->keys[j - 1];
	}
	for (unsigned int j = node->nbKeyUsed + 1; j > idxKey; j--)
	{
		node->children[j] = node->children[j - 1];
		node->leaves[j] = node->leaves[j - 1];
	}
	node->nbKeyUsed++; /* BUG: if idxKey == node->nbKeyUsed this incremented incorrectly */
}

PRIVATE void Node_shiftLeft(Node* node, unsigned int idxKey)
{
	for (int j = idxKey; j < node->nbKeyUsed; j++)
	{
		node->keys[j] = node->keys[j + 1];
	}
	for (int j = idxKey; j <= node->nbKeyUsed; j++)
	{
		node->children[j] = node->children[j + 1];
		node->leaves[j] = node->leaves[j + 1];
	}
	node->nbKeyUsed--; /* BUG: if idxKey == node->nbKeyUsed this decremented incorrectly */
}

/*********************************************************************************
* Node_merge
* input: index of the left node to merge
* input: index of the right node to merge
* output: The merged node
*********************************************************************************/
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int idxLeft, unsigned idxRight)
{
	Node* mergedNode = 0;

	Node * leftChild = node->children[idxLeft];
	Node * rightChild = node->children[idxRight];

	// Demote the key from the parent node into the left child
	leftChild->keys[leftChild->nbKeyUsed] = node->keys[idxLeft];
	// Promote the last key of the right child to the parent node
	node->keys[idxLeft] = node->keys[idxRight]; /* BUG: Statement not useful */
	leftChild->nbKeyUsed++;

	// Move all key, children, leaves from the right child node to the left
	for (int i = 0; i < rightChild->nbKeyUsed; i++)
	{
		leftChild->keys[leftChild->nbKeyUsed + i] = rightChild->keys[i];
	}
	for (int i = 0; i <= rightChild->nbKeyUsed; i++)
	{
		leftChild->children[leftChild->nbKeyUsed + i] = rightChild->children[i];
		leftChild->leaves[leftChild->nbKeyUsed + i] = rightChild->leaves[i];
	}
	leftChild->nbKeyUsed = leftChild->nbKeyUsed + rightChild->nbKeyUsed;

	// Update parent node and discard right child
	Node_shiftLeft(node, idxRight);
	Node_free(rightChild);

	return mergedNode;
}

/*********************************************************************************
* Node_stealLeftKey
* input: index of the child (left or right) to steal from
* input: index of the child (left or right) to give to
* output: none
*********************************************************************************/
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo)
{
	// Shift
	Node * stealFromChild = node->children[idxChildStealFrom];
	Node * giveToChild = node->children[idxChildGiveTo];

	//TRACE(("Stealing Left Key from % to % d\n", idxChildStealFrom, idxChildGiveTo));
	Node_shiftRight(giveToChild, 0);
	giveToChild->children[0] = stealFromChild->children[stealFromChild->nbKeyUsed];
	giveToChild->leaves[0] = stealFromChild->leaves[stealFromChild->nbKeyUsed];
	giveToChild->keys[0] = node->keys[idxChildStealFrom];
	node->keys[idxChildStealFrom] = stealFromChild->keys[stealFromChild->nbKeyUsed - 1];
	//giveToChild->nbKeyUsed++;
	stealFromChild->nbKeyUsed--;
}

/*********************************************************************************
* Node_stealRightKey
* input: index of the child (left or right) to steal from
* input: index of the child (left or right) to give to
* output: none
*********************************************************************************/
PRIVATE void Node_stealRightKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo)
{
	// Shift
	Node* stealFromChild = node->children[idxChildStealFrom];
	Node* giveToChild = node->children[idxChildGiveTo];

	//TRACE(("Stealing Right Key from % to % d\n", idxChildStealFrom, idxChildGiveTo));
	giveToChild->children[giveToChild->nbKeyUsed + 1] = stealFromChild->children[0];
	giveToChild->leaves[giveToChild->nbKeyUsed + 1] = stealFromChild->leaves[0];
	giveToChild->keys[giveToChild->nbKeyUsed] = node->keys[idxChildGiveTo];
	node->keys[idxChildGiveTo] = stealFromChild->keys[0];
	giveToChild->nbKeyUsed++;
	Node_shiftLeft(stealFromChild, 0);
	
}
