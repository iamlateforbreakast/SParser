#include "Node.h"
#include "Pool.h"
#include "Error.h"
#include "Debug.h"

/*********************************************************************************
*
* Private Functions Declarations
*
*********************************************************************************/
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int idx1, unsigned int idx2, Pool * pool);
PRIVATE void Node_shiftRight(Node node, unsigned int idxKey, Pool * pool);
PRIVATE void Node_shiftLeft(Node node, unsigned int idxKey, Pool * pool);
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo, Pool * pool);
PRIVATE void Node_stealRightKey(Node node, unsigned int idxKeyStealFrom, unsigned int idxKeyGiveTo, unsigned int order, Pool * pool);

/*********************************************************************************
* Node_new
* input: isLeaf TRUE if creating a terminal node
* output: the newly created beam weight node
*********************************************************************************/
PUBLIC unsigned int Node_new(Pool * pool)
{
	// SP6: Node* node = NULL;

	Node node;
	unsigned int nodeIdx;

	if (Pool_alloc(pool, &nodeIdx) == ALLOC_OK) return nodeIdx;
	else
	{
		exit(2);
	}
}

/*********************************************************************************
* Node_search
* input: the key to look for
* output: the beam weight range if found otherwise NULL 
*********************************************************************************/
PUBLIC void Node_search(unsigned int nodeIdx, unsigned int order, Key key, void** object, unsigned int isFoundAlready, Pool * pool)
{
	void* ptrBuffer = Pool_read(pool, nodeIdx);
 	Node node = Node_read(nodeIdx, order, ptrBuffer);

	if (*node.isLeaf == 1)
	{
		for (int i = 0; i < *node.nbKeyUsed; i++)
		{
			if (node.keys[i] == (unsigned int)key)
			{
				*object = node.leaves[i];
				return;
			}
		}
		if (isFoundAlready)
		{
			*object = node.leaves[*node.nbKeyUsed - 1];
			return;
		}
		else
			return;
	}
	else
	{
		for (int i = 0; i < *node.nbKeyUsed; i++)
		{
			if ((unsigned int)key < node.keys[i])
			{
				Node_search(node.children[i], order, key, object, isFoundAlready, pool);
				return;
			}
			if (key == node.keys[i])
			{
				Node_search(node.children[i], order, key, object, 1, pool);
				return;
			}
		}
		Node_search(node.children[*node.nbKeyUsed], order, key, object, isFoundAlready, pool);
		return;
	}

	return 0;
 }

/*********************************************************************************
* Node_free
* input: none
* output: none
*********************************************************************************/
PUBLIC void Node_free(unsigned int nodeIdx, unsigned int order, Pool* pool)
{
        void* ptrBuffer = Pool_read(pool, nodeIdx);
 	Node node = Node_read(nodeIdx, order, ptrBuffer);

	if (*node.isLeaf == 1)
	{
		for (int i = 0; i < *node.nbKeyUsed + 1; i++)
		{
			//free(node->leaves[i]);
		}
                // Pool_dealloc(pool, nodeIdx);
		return;
	}
        else
        {
                for (int i = 0; i < *node.nbKeyUsed + 1; i++)
                {
                // TODO: go through all children
                }
        }
}

/*********************************************************************************
* Node_insert
* input: key
* input: beamWeightRange
* output: none
*********************************************************************************/
PUBLIC void Node_insert(unsigned int nodeIdx, Key key, void * object, unsigned int order, Pool* pool)
{
	void* ptrBuffer = Pool_read(pool, nodeIdx);
	Node node = Node_read(nodeIdx, order, ptrBuffer);
	
	if (*node.isLeaf == 1) 
	{	
		for (int i = 0; i < *node.nbKeyUsed; i++)
		{
			if (key == node.keys[i])
			{
				printf("Error: Duplicate Key\n");
				return;
			}
			if (key < node.keys[i])
			{
				for (int j = *node.nbKeyUsed-1; j >= i; j--)
					node.keys[j + 1] = node.keys[j];
				for (int j = *node.nbKeyUsed; j >= i; j--) 
					node.leaves[j + 1] = node.leaves[j];
				node.keys[i] = key;
				node.leaves[i] = object;
				*node.nbKeyUsed++;
				Pool_write(pool, nodeIdx, ptrBuffer);
				return;
			}
		}
		/* Position to insert is the last */
		node.keys[*node.nbKeyUsed] = key;
		node.leaves[*node.nbKeyUsed] = object;
		(*node.nbKeyUsed)++;
		Pool_write(pool, nodeIdx, ptrBuffer);
		return;
	}
	else
	{
		/* TBC: Check if node is full */
		int i = 0;
		for (i = 0; i < *node.nbKeyUsed; i++)
		{
			if (key < node.keys[i])
				break;
		}
		/*if (node.children[i] == NULL)
		{
			printf("ERROR: insert key: %d\n", key);
			Node_print(node, 3, pool);
			exit(0);
		}*/

		void * ptrBuffer2 = Pool_read(pool, node.children[i]);
		Node nodeForInsertion = Node_read(node.children[i], order, ptrBuffer2);
		if (*nodeForInsertion.nbKeyUsed < order * 2 - 1) //&& (node->nbKeyUsed < ORDER * 2 - 1))
		{
			Node_insert(node.children[i], key, object, order, pool);
			return;
		}
		else
		{
			//printf("Splitting node.\n");
			unsigned int newChildIdx = Node_splitNode(nodeIdx, node.children[i], key, order, pool);
			Node_insert(newChildIdx, key, object, order, pool);
			return;
		}

	    //Node* childNode = Node_split(node, key);
	    //Node_insert(childNode, key, beamWeightRange);
		Pool_discard(pool, nodeIdx);
	    return;
	}
}

/*********************************************************************************
* Node_remove
* input: the key to remove
* output: none
*********************************************************************************/
PUBLIC void * Node_remove(unsigned int nodeIdx, unsigned int order, Key key, unsigned int * keyToUpdate, Pool* pool)
{ 
	void * object = 0;
    void * ptrContent = Pool_read(pool, nodeIdx);
    Node node = Node_read(nodeIdx, order, ptrContent);

	if (*node.isLeaf == 1)
	{
		for (int i = 0; i < *node.nbKeyUsed; i++)
		{
			if (node.keys[i] == key)
			{
				//Can we remove if node->nbKeyUsed => ORDER then can remove
				object = node.leaves[i];
				// Shift all children and leaves left
				for (int j = i; j < *node.nbKeyUsed; j++)
				{
					node.keys[j] = node.keys[j + 1];
				}
				for (int j = i; j <= *node.nbKeyUsed; j++)
				{
					node.children[j] = node.children[j + 1];
					node.leaves[j] = node.leaves[j + 1];
				}
				(*node.nbKeyUsed)--;
				Pool_write(pool, nodeIdx, ptrContent);
				return object;
			}
		}
		// Check the key was found will descending
		if (keyToUpdate != 0)
		{
			*keyToUpdate = node.keys[*node.nbKeyUsed - 1];
			(*node.nbKeyUsed)--;
			object = node.leaves[*node.nbKeyUsed - 1];
			return object;
		}
		else 
			return 0;

	}
	else
	{
		// Search which child contains the key
		for (int i = 0; i < *node.nbKeyUsed; i++)
		{
			if (key <= node.keys[i])
			{
				// The key is found already while descending the tree, remember it
				if (key <= node.keys[i]) keyToUpdate = &node.keys[i]; /* BUG */
				object = Node_remove(node.children[i], order, key, keyToUpdate, pool);
				// Check if the number of children is at least ORDER
				void * ptrContent = Pool_read(pool, node.children[i]);
				Node child = Node_read(node.children[i], order, ptrContent);
				void * ptrContent2 = Pool_read(pool, node.children[i+1]);
				Node nextChild = Node_read(node.children[i+1], order, ptrContent2);
				if (*child.nbKeyUsed < order - 1)
				{
					if (*nextChild.nbKeyUsed <= order - 1)
					{
						// Merge node left and right
						Node_mergeNodes(nodeIdx, i, i + 1, pool);
					}
					else
					{
						// Steal key right
						Node_stealRightKey(node, i + 1, i, order, pool);
					}
					return object;
				}
				else
				{
					return object;
				}
			}
			/* Should there be more checks here */
		}
#if 0
		object = Node_remove(node.children[*node.nbKeyUsed], order, key, keyToUpdate, pool);
		// Now assess if the node needs to be re-balanced
		if (*node.children[*node.nbKeyUsed].nbKeyUsed < order - 1)
		{
			if (*node.children[*node.nbKeyUsed - 1].nbKeyUsed <= order - 1)
			{
				// Merge node left and right
				printf("ERROR: Need to merge\n");
				Node_mergeNodes(node, node->nbKeyUsed - 1, node->nbKeyUsed, pool);
			}
			else
			{
				// Steal key left
				Node_stealLeftKey(node, node->nbKeyUsed - 1, node->nbKeyUsed, pool);
			}
			return object;
		}
		else
		{
			return object;
		}
#endif
    }
    return 0;
}

/*********************************************************************************
* Node_print
* input: None
* output: None
*********************************************************************************/
PUBLIC void Node_print(unsigned int nodeIdx, unsigned int order, unsigned int depth, Pool * pool)
{
	void* ptrBuffer = Pool_read(pool, nodeIdx);
	Node node = Node_read(nodeIdx, order, ptrBuffer);

	//if (node == NULL) return;
	printf(" Node NbUsed: %d\n", *node.nbKeyUsed);
	printf(" Keys: ");
	for (unsigned int i = 0; i < order * 2 - 1; i++)
	{
		if (i < *node.nbKeyUsed)
			printf("%d ", node.keys[i]);
		else
 			printf(".. ");
	}
	printf("\n");
	if ((*node.isLeaf==0) && (depth>0))
	{
		for (int i = 0; i <= *node.nbKeyUsed; i++)
		{
			printf("Child %d-%d:\n", depth, i);
		    Node_print(node.children[i], order, depth - 1, pool);
		}
	}
	Pool_discardCache(pool, nodeIdx);
}

/*********************************************************************************
* Node_splitNode
* input: A node to split
* input: the key that caused the split
* output: The sub tree where the key should be inserted
*********************************************************************************/
PUBLIC unsigned int Node_splitNode(unsigned int nodeIdx, unsigned int nodeToSplitIdx, Key key, unsigned int order, Pool* pool)
{
	void * ptrBuffer1 = Pool_read(pool, nodeIdx);
	Node node = Node_read(nodeIdx, order, ptrBuffer1);

	void* ptrBuffer2 = Pool_read(pool, nodeToSplitIdx);
	Node nodeToSplit = Node_read(nodeToSplitIdx, order, ptrBuffer2);

	unsigned int newChildIdx;
	void* ptrBuffer3 = Pool_alloc(pool, &newChildIdx);
	Node newChild = Node_read(newChildIdx, order, ptrBuffer3);

	unsigned int k;

	if (*nodeToSplit.isLeaf) *newChild.isLeaf = 1;

	for (k = 0; k < *node.nbKeyUsed; k++)
	{
		/* TBC: node->nbKeyUsed is assumed less than ORDER*2-1 */
		if (nodeToSplit.keys[order - 1] < node.keys[k]) break;
	}

	for (unsigned int j = *node.nbKeyUsed; j > k; j--)
		node.keys[j] = node.keys[j - 1];
	for (unsigned int j = *node.nbKeyUsed + 1; j > k; j--)
	{
		node.leaves[j] = node.leaves[j - 1];
		node.children[j] = node.children[j - 1];
	}

	node.keys[k] = nodeToSplit.keys[order - 1];
	node.children[k] = nodeToSplitIdx;
	node.children[k + 1] = newChildIdx;
	(*node.nbKeyUsed)++;

	for (unsigned int i = 0; i < order - 1; i++)
		newChild.keys[i] = nodeToSplit.keys[order + i];
	for (unsigned int i = 0; i < order; i++)
	{
		newChild.leaves[i] = nodeToSplit.leaves[order + i];
		newChild.children[i] = nodeToSplit.children[order + i];
		nodeToSplit.leaves[order + i] = 0;
		nodeToSplit.children[order + i] = 0;
	}

	*newChild.nbKeyUsed = order - 1;
	*nodeToSplit.nbKeyUsed = order - 1;

	//Pool_addToChunkCache(pool, node, );
	//Pool_write(pool, nodeIdx, ptrBuffer1);
	//Pool_addToChunkCache(pool, nodeToSplit, );

	//Pool_read(pool, newChildIdx, &newChild);
	
	Pool_write(pool, nodeIdx, ptrBuffer1);
	Pool_write(pool, newChildIdx, ptrBuffer3);
	Pool_write(pool, nodeToSplitIdx, ptrBuffer2);

	if (key < node.keys[k])
		return node.children[k];
	else
		return node.children[k + 1];
}

/**********************************************//**
  @brief Node_getSize
  @param[in] none
  @return none
**************************************************/
PUBLIC unsigned int Node_getSize(unsigned int order)
{
	return sizeof(unsigned short int) * 2
		+ sizeof(unsigned int) * (order * 2 - 1)
		+ sizeof(void*) * (order * 2)
		+ sizeof(unsigned int) * (order * 2);
}

/**********************************************//**
  @brief Node_read
  @param[in] none
  @return none
**************************************************/
PUBLIC Node Node_read(unsigned int nodeIdx, unsigned int order, void * ptrContent)
{
	Node resultNode;
	resultNode.nbKeyUsed = ptrContent;
	resultNode.isLeaf = (unsigned int*)((char*)ptrContent + sizeof(unsigned int));
	resultNode.keys = (unsigned int*)((char*)ptrContent + sizeof(unsigned int) * 2);
	resultNode.children = (unsigned int*)((char*)ptrContent + sizeof(unsigned int) * 2 + sizeof(unsigned int) * (2 * order - 1));
	resultNode.leaves = (void**)((char*)ptrContent + sizeof(unsigned int) * 2 + sizeof(unsigned int) * (2 * order - 1) + sizeof(unsigned int) * (2 * order));
	return resultNode;
}
PRIVATE void Node_shiftRight(Node node, unsigned int idxKey, Pool* pool)
{
	for (unsigned int j = *node.nbKeyUsed; j > idxKey; j--)
	{
		node.keys[j] = node.keys[j - 1];
	}
	for (unsigned int j = *node.nbKeyUsed + 1; j > idxKey; j--)
	{
		node.children[j] = node.children[j - 1];
		node.leaves[j] = node.leaves[j - 1];
	}
	(*node.nbKeyUsed)++;
}

PRIVATE void Node_shiftLeft(Node node, unsigned int idxKey, Pool* pool)
{
	for (int j = idxKey; j < *node.nbKeyUsed; j++)
	{
		node.keys[j] = node.keys[j + 1];
	}
	for (int j = idxKey; j <= *node.nbKeyUsed; j++)
	{
		node.children[j] = node.children[j + 1];
		node.leaves[j] = node.leaves[j + 1];
	}
	(*node.nbKeyUsed)--; /* BUG: if idxKey == node->nbKeyUsed this decremented incorrectly */
}

/*********************************************************************************
* Node_merge
* input: index of the left node to merge
* input: index of the right node to merge
* output: The merged node
*********************************************************************************/
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int idxLeft, unsigned idxRight, Pool* pool)
{
#if 0
	Node* mergedNode = NULL;

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
	Node_shiftLeft(node, idxRight, pool);
	Node_free(rightChild, pool);

	return mergedNode;
#endif
}

/*********************************************************************************
* Node_stealLeftKey
* input: index of the child (left or right) to steal from
* input: index of the child (left or right) to give to
* output: none
*********************************************************************************/
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo, Pool* pool)
{
#if 0
	// Shift
	Node * stealFromChild = node->children[idxChildStealFrom];
	Node * giveToChild = node->children[idxChildGiveTo];

	//printf("Stealing Left Key from % to % d\n", idxChildStealFrom, idxChildGiveTo);
	Node_shiftRight(giveToChild, 0, pool);
	giveToChild->children[0] = stealFromChild->children[stealFromChild->nbKeyUsed]; /* BUG: should be nbUsedKey*/
	giveToChild->leaves[0] = stealFromChild->leaves[stealFromChild->nbKeyUsed];
	giveToChild->keys[0] = node->keys[idxChildStealFrom];
	node->keys[idxChildStealFrom] = stealFromChild->keys[stealFromChild->nbKeyUsed - 1];
	//giveToChild->nbKeyUsed++;
	stealFromChild->nbKeyUsed--;
#endif
}

/*********************************************************************************
* Node_stealRightKey
* input: index of the child (left or right) to steal from
* input: index of the child (left or right) to give to
* output: none
*********************************************************************************/
PRIVATE void Node_stealRightKey(Node node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo, unsigned int order, Pool* pool)
{
	// Shift
	void * ptrContent = Pool_read(pool, node.children[idxChildStealFrom]);
	Node stealFromChild = Node_read(node.children[idxChildStealFrom], order, ptrContent);
	void * ptrContent2 = Pool_read(pool, node.children[idxChildGiveTo]); 
	Node giveToChild = Node_read(node.children[idxChildGiveTo], order, ptrContent2);

	//printf("Stealing Right Key from % to % d\n", idxChildStealFrom, idxChildGiveTo);
	giveToChild.children[*giveToChild.nbKeyUsed + 1] = stealFromChild.children[0];
	giveToChild.leaves[*giveToChild.nbKeyUsed + 1] = stealFromChild.leaves[0];
	giveToChild.keys[*giveToChild.nbKeyUsed] = node.keys[idxChildGiveTo];
	node.keys[idxChildGiveTo] = stealFromChild.keys[0];
	(*giveToChild.nbKeyUsed)++;
	Node_shiftLeft(stealFromChild, 0, pool);
}
