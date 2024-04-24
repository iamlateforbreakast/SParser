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
PRIVATE Node* Node_mergeNodes(Node* node, unsigned int order, unsigned int idxLeft, unsigned idxRight);
PRIVATE void Node_shiftRight(Node* node, unsigned int idxKey);
PRIVATE void Node_shiftLeft(Node* node, unsigned int idxKey);
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo);
PRIVATE void Node_stealRightKey(Node* node, unsigned int idxKeyStealFrom, unsigned int idxKeyGiveTo);

/*********************************************************************************
* Node_new
* input: isLeaf TRUE if creating a terminal node
* output: the newly created beam weight node
*********************************************************************************/
PUBLIC Node* Node_new(unsigned short int isLeaf, unsigned int order)
{
	Node* node = 0;
    const unsigned int size = sizeof(Node)
	                        + (order * 2 - 1) * sizeof(Object*)
							+ (order * 2) * sizeof(Object*)
							+ (order * 2) * sizeof(Node*);
	node = (Node*)Memory_alloc(size);
	Memory_set(node, 0, size);
	node->isLeaf = isLeaf;
	node->nbKeyUsed = 0;
    node->keys = &node->buffer[0];
	node->leaves = &node->buffer[order * 2 - 1];
    node->children = (Node**)&node->buffer[order * 2* 2 - 1];
    TRACE(("New node:\n"));
	TRACE(("Size - Total %d Header %d\n", size, sizeof(Node)));
	TRACE(("Node %x Keys %x Leaves %x Children %x\n", node, node->keys, node->leaves, node->children));
		//(*node->children)[i] = 0;
		//(*node->leaves)[i] = 0;
		//*node->keys[i] = 0;

	return node;
}

/*********************************************************************************
* Node_search
* input: the key to look for
* output: the beam weight range if found otherwise 0
*********************************************************************************/
PUBLIC Object* Node_search(Node* node, unsigned int order, Object* key, unsigned int isFoundAlready)
{
	if (node == 0)
	{
		Error_new(ERROR_FATAL,"Node_search node== 0\n");
	}
	else if (node->isLeaf == 1)
	{
		for (unsigned int i = 0; i < node->nbKeyUsed; i++)
		{
			if (Object_comp(node->keys[i], key)==0)
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
		for (unsigned int i = 0; i < node->nbKeyUsed; i++)
		{
			if (Object_comp(key,node->keys[i])<0)
			{
				if (node->children[i] == 0)
				{
					Error_new(ERROR_FATAL, "Node_search node== 0\n");
				}
				return Node_search(node->children[i], order, key, isFoundAlready);
			}
			if (Object_comp(key, node->keys[i])==0)
				return Node_search(node->children[i], order, key, 1);
		}
		return Node_search(node->children[node->nbKeyUsed], order, key, isFoundAlready);
	}

	return 0;
 }

/*********************************************************************************
* Node_free
* input: none
* output: none
*********************************************************************************/
PUBLIC void Node_free(Node* node, unsigned int order)
{
  if (node == 0) return;
  if (node->isLeaf == 1)
  {
    /* TBC */
  }
  Memory_free(node,sizeof(Node));
}

PUBLIC void Node_freeRecurse(Node * node, unsigned int order)
{
  if (node == 0) return;
  if (node->isLeaf == 0)
  {
    for (unsigned int i = 0; i < node->nbKeyUsed + 1; i++)
	{
      //Memory_free(node->leaves[i], sizeof(Node));
	}
  }
}

/*********************************************************************************
* Node_insert
* input: key
* input: beamWeightRange
* output: none
*********************************************************************************/
PUBLIC void Node_insert(Node* node, unsigned int order, Object * key, Object * object, int isOwner)
{
	if (node->isLeaf == 1) 
	{
		for (int i = 0; i < (int)node->nbKeyUsed; i++)
		{
			if (Object_comp(key, node->keys[i])==0)
			{
				Error_new(ERROR_NORMAL, "Duplicate Key\n");
				return;
			}
			if (Object_comp(key, node->keys[i])<0)
			{
				for (int j = node->nbKeyUsed-1; j >= i; j--)
			  {
				TRACE(("Node_insert j=%d i=%d\n", j, i));
					node->keys[j + 1] = node->keys[j];
			  }
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
		unsigned int i = 0;
		for (i = 0; i < node->nbKeyUsed; i++)
		{
			if (Object_comp(key, node->keys[i])<0)
				break;
		}
		if (node->children[i] == 0)
		{
			Error_new(ERROR_NORMAL, "insert key: %d\n", key);
			Node_print(node, order, 3);
			Error_new(ERROR_FATAL,"Exiting\n");
		} else
		if (node->children[i]->nbKeyUsed < order * 2 - 1) //&& (node->nbKeyUsed < ORDER * 2 - 1))
		{
			Node_insert(node->children[i], order, key, object, isOwner);
			return;
		}
		else
		{
			TRACE(("Splitting node.\n"));
			Node* newChild = Node_splitNode(node, order, node->children[i], key);
			Node_insert(newChild, order, key, object, isOwner);
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
PUBLIC Object * Node_remove(Node* node, unsigned int order, Object * key, Object ** keyToUpdate)
{ 
	Object * object = 0;

	if (node->isLeaf == 1)
	{
		for (unsigned int i = 0; i < node->nbKeyUsed; i++)
		{
			if (Object_comp(node->keys[i], key)==0)
			{
				//Can we remove if node->nbKeyUsed => ORDER then can remove
				object = node->leaves[i];
				// Shift all children and leaves left
				for (unsigned int j = i; j < node->nbKeyUsed; j++)
				{
					node->keys[j] = node->keys[j + 1];
				}
				for (unsigned int j = i; j <= node->nbKeyUsed; j++)
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
		for (unsigned int i = 0; i < node->nbKeyUsed; i++)
		{
			if (Object_comp(key, node->keys[i])<=0)
			{
				// The key is found already while descending the tree, remember it
				if (Object_comp(key, node->keys[i])==0) keyToUpdate = &node->keys[i]; /* BUG: <= should be == */
				object = Node_remove(node->children[i], order, key, keyToUpdate);
				// Check if the number of children is at least ORDER
				if (node->children[i]->nbKeyUsed < order - 1)
				{
					if (node->children[i + 1]->nbKeyUsed <= order - 1)
					{
						// Merge node left and right
						Node_mergeNodes(node, order, i, i + 1);
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
		object = Node_remove(node->children[node->nbKeyUsed], order, key, keyToUpdate);
		// Now assess if the node needs to be re-balanced
		if (node->children[node->nbKeyUsed]->nbKeyUsed < order - 1)
		{
			if (node->children[node->nbKeyUsed - 1]->nbKeyUsed <= order - 1)
			{
				// Merge node left and right
				TRACE(("ERROR: Need to merge\n"));
				Node_mergeNodes(node, order, node->nbKeyUsed - 1, node->nbKeyUsed);
				//node->nbKeyUsed--;
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
PUBLIC void Node_print(Node* node, unsigned int order, unsigned int depth)
{
	if (node == 0) return;
	TRACE((" Node NbUsed: %d\n", node->nbKeyUsed));
	TRACE((" Keys: "));
	for (unsigned int i = 0; i < order * 2 - 1; i++)
	{
		if (i < node->nbKeyUsed)
		PRINT(("%x ", node->keys[i]));
		//Object_print(node->keys[i]);
		else
			PRINT((".. "));
	}
	PRINT(("\n"));
	if ((node->isLeaf==0) && (depth>0))
	{
		for (unsigned int i = 0; i <= node->nbKeyUsed; i++)
		{
			PRINT(("Child %d-%d:\n",
			  depth, i));
		    Node_print(node->children[i], order, depth - 1);
		}
	}
}

/*********************************************************************************
* Node_splitNode
* input: A node to split
* input: the key that caused the split
* output: The sub tree where the key should be inserted
*********************************************************************************/
PUBLIC Node* Node_splitNode(Node* node, unsigned int order, Node* nodeToSplit, Object * key)
{
	Node* newChild;

	newChild = Node_new(nodeToSplit->isLeaf, order);

	unsigned int k;

	for (k = 0; k < node->nbKeyUsed; k++)
	{
		/* TBC: node->nbKeyUsed is assumed less than ORDER*2-1 */
		if (Object_comp(nodeToSplit->keys[order - 1], node->keys[k])<0) break;
	}
	for (unsigned int j = node->nbKeyUsed; j > k; j--)
		node->keys[j] = node->keys[j - 1];
	for (unsigned int j = node->nbKeyUsed + 1; j > k; j--)
	{
		node->leaves[j] = node->leaves[j - 1];
		node->children[j] = node->children[j - 1];
	}
	node->keys[k] = nodeToSplit->keys[order - 1];
	node->children[k] = nodeToSplit;
	node->children[k + 1] = newChild;
	node->nbKeyUsed++;
	for (unsigned int i = 0; i < order - 1; i++)
		newChild->keys[i] = nodeToSplit->keys[order + i];
	for (unsigned int i = 0; i < order; i++)
	{
		newChild->leaves[i] = nodeToSplit->leaves[order + i];
		newChild->children[i] = nodeToSplit->children[order + i];
		nodeToSplit->leaves[order + i] = 0;
		nodeToSplit->children[order + i] = 0;
	}
	newChild->nbKeyUsed = order - 1;
	nodeToSplit->nbKeyUsed = order - 1;


	if (Object_comp(key, node->keys[k])<0)
		return node->children[k];
	else
		return node->children[k + 1];
}

PUBLIC unsigned int Node_getSize(Node * node)
{
  return 0;
}
PUBLIC unsigned int Node_getNbNodes(Node* node)
{
  if (node->isLeaf) return 1;
  unsigned int result = node->nbKeyUsed;
  for (unsigned int i = 0; i <= node->nbKeyUsed; i++)
  {
	result = result + Node_getNbNodes(node->children[i]);
  }
  return result;
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
	for (unsigned int j = idxKey; j < node->nbKeyUsed; j++)
	{
		node->keys[j] = node->keys[j + 1];
	}
	for (unsigned int j = idxKey; j <= node->nbKeyUsed; j++)
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
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int order, unsigned int idxLeft, unsigned idxRight)
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
	for (unsigned int i = 0; i < rightChild->nbKeyUsed; i++)
	{
		leftChild->keys[leftChild->nbKeyUsed + i] = rightChild->keys[i];
	}
	for (unsigned int i = 0; i <= rightChild->nbKeyUsed; i++)
	{
		leftChild->children[leftChild->nbKeyUsed + i] = rightChild->children[i];
		leftChild->leaves[leftChild->nbKeyUsed + i] = rightChild->leaves[i];
	}
	leftChild->nbKeyUsed = leftChild->nbKeyUsed + rightChild->nbKeyUsed;

	// Update parent node and discard right child
	Node_shiftLeft(node, idxRight);
	Node_free(rightChild, order);

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
