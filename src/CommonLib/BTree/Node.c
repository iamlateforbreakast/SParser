#include "Node.h"

/*********************************************************************************
*
* Private Functions Declarations
*
*********************************************************************************/
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int idx1, unsigned int idx2, Pool * pool);
PRIVATE void Node_shiftRight(Node* node, unsigned int idxKey, Pool * pool);
PRIVATE void Node_shiftLeft(Node* node, unsigned int idxKey, Pool * pool);
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo, Pool * pool);
PRIVATE void Node_stealRightKey(Node* node, unsigned int idxKeyStealFrom, unsigned int idxKeyGiveTo, Pool * pool);

/*********************************************************************************
* Node_new
* input: isLeaf TRUE if creating a terminal node
* output: the newly created beam weight node
*********************************************************************************/
PUBLIC unsigned int Node_new(unsigned short int isLeaf, Pool * pool)
{
	// SP6: Node* node = NULL;

	Node node;
	unsigned int nodeIdx;

	if (Pool_alloc(pool, &nodeIdx) == ALLOC_OK)
	{
		node.isLeaf = isLeaf;
		node.nbKeyUsed = 0;
		// SP6: node = (Node*)malloc(sizeof(Node));
		// SP6: node->isLeaf = isLeaf;
		// SP6: node->nbKeyUsed = 0;

		for (int i = 0; i < ORDER * 2; i++)
		{
			node.children[i] = NULL;
			node.leaves[i] = 0;
		}
		for (int i = 0; i < ORDER * 2 - 1; i++)
		{
			node.keys[i] = 0;
		}
		Pool_write(pool, nodeIdx, &node);
	}
    return nodeIdx;
}

/*********************************************************************************
* Node_search
* input: the key to look for
* output: the beam weight range if found otherwise NULL 
*********************************************************************************/
PUBLIC Object Node_search(Node* node, Key key, unsigned int isFoundAlready, Pool * pool)
{
	if (node == NULL)
	{
		printf("ERROR: Node_search node== NULL\n");
		exit(0);
	}
	if (node->isLeaf == TRUE)
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
			return NULL;
	}
	else
	{
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (key < node->keys[i])
			{
				if (node->children[i] == NULL)
				{
					printf("ERROR: Node_search node== NULL\n");
					exit(0);
				}
				return Node_search(node->children[i], key, isFoundAlready, pool);
			}
			if (key == node->keys[i])
				return Node_search(node->children[i], key, TRUE, pool);
		}
		return Node_search(node->children[node->nbKeyUsed], key, isFoundAlready, pool);
	}

	return NULL;
 }

/*********************************************************************************
* Node_free
* input: none
* output: none
*********************************************************************************/
PUBLIC void Node_free(Node* node, Pool * pool)
{
	if (node->isLeaf == TRUE)
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
PUBLIC void Node_insert(Node* node, Key key, Object object, Pool * pool)
{
	if (node->isLeaf == TRUE) 
	{
		for (int i = 0; i < node->nbKeyUsed; i++)
		{
			if (key == node->keys[i])
			{
				printf("Error: Duplicate Key\n");
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
		if (node->children[i] == NULL)
		{
			printf("ERROR: insert key: %d\n", key);
			Node_print(node, 3, pool);
			exit(0);
		}
		if (node->children[i]->nbKeyUsed < ORDER * 2 - 1) //&& (node->nbKeyUsed < ORDER * 2 - 1))
		{
			Node_insert(node->children[i], key, object, pool);
			return;
		}
		else
		{
			printf("Splitting node.\n");
			Node* newChild = Node_splitNode(node, node->children[i], key, pool);
			Node_insert(newChild, key, object, pool);
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
PUBLIC Object Node_remove(Node* node, Key key, unsigned int * keyToUpdate, Pool * pool)
{ 
	Object object = NULL;

	if (node->isLeaf == TRUE)
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
		if (keyToUpdate != NULL)
		{
			*keyToUpdate = node->keys[node->nbKeyUsed - 1];
			node->nbKeyUsed--;
			object = node->leaves[node->nbKeyUsed - 1];
			return object;
		}
		else 
			return NULL;

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
				object = Node_remove(node->children[i], key, keyToUpdate, pool);
				// Check if the number of children is at least ORDER
				if (node->children[i]->nbKeyUsed < ORDER - 1)
				{
					if (node->children[i + 1]->nbKeyUsed <= ORDER - 1)
					{
						// Merge node left and right
						Node_mergeNodes(node, i, i + 1, pool);
					}
					else
					{
						// Steal key right
						Node_stealRightKey(node, i + 1, i, pool);
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
		object = Node_remove(node->children[node->nbKeyUsed], key, keyToUpdate, pool);
		// Now assess if the node needs to be re-balanced
		if (node->children[node->nbKeyUsed]->nbKeyUsed < ORDER - 1)
		{
			if (node->children[node->nbKeyUsed - 1]->nbKeyUsed <= ORDER - 1)
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
		
    }

	return NULL;
}

/*********************************************************************************
* Node_print
* input: None
* output: None
*********************************************************************************/
PUBLIC void Node_print(unsigned int nodeIdx, unsigned int depth, Pool * pool)
{
	Node node;

	Pool_read(pool, nodeIdx, &node);

	//if (node == NULL) return;
	printf(" Node NbUsed: %d\n", node.nbKeyUsed);
	printf(" Keys: ");
	for (int i = 0; i < ORDER * 2 - 1; i++)
	{
		if (i < node.nbKeyUsed)
			printf("%d ", node.keys[i]);
		else
			printf(".. ");
	}
	printf("\n");
	if ((node.isLeaf==FALSE) && (depth>0))
	{
		for (int i = 0; i <= node.nbKeyUsed; i++)
		{
			printf("Child %d-%d:\n", depth, i);
		    Node_print(node.children[i], depth - 1, pool);
		}
	}
}

/*********************************************************************************
* Node_splitNode
* input: A node to split
* input: the key that caused the split
* output: The sub tree where the key should be inserted
*********************************************************************************/
PUBLIC Node* Node_splitNode(Node* node, Node* nodeToSplit, Key key, Pool * pool)
{
	Node* newChild;

	newChild = Node_new(nodeToSplit->isLeaf, pool);

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
		nodeToSplit->leaves[ORDER + i] = NULL;
		nodeToSplit->children[ORDER + i] = NULL;
	}
	newChild->nbKeyUsed = ORDER - 1;
	nodeToSplit->nbKeyUsed = ORDER - 1;


	if (key < node->keys[k])
		return node->children[k];
	else
		return node->children[k + 1];
}

PRIVATE void Node_shiftRight(Node* node, unsigned int idxKey, Pool* pool)
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

PRIVATE void Node_shiftLeft(Node* node, unsigned int idxKey, Pool* pool)
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
PRIVATE Node * Node_mergeNodes(Node* node, unsigned int idxLeft, unsigned idxRight, Pool * pool)
{
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
}

/*********************************************************************************
* Node_stealLeftKey
* input: index of the child (left or right) to steal from
* input: index of the child (left or right) to give to
* output: none
*********************************************************************************/
PRIVATE void Node_stealLeftKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo, Pool * pool)
{
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
}

/*********************************************************************************
* Node_stealRightKey
* input: index of the child (left or right) to steal from
* input: index of the child (left or right) to give to
* output: none
*********************************************************************************/
PRIVATE void Node_stealRightKey(Node* node, unsigned int idxChildStealFrom, unsigned int idxChildGiveTo, Pool * pool)
{
	// Shift
	Node* stealFromChild = node->children[idxChildStealFrom];
	Node* giveToChild = node->children[idxChildGiveTo];

	//printf("Stealing Right Key from % to % d\n", idxChildStealFrom, idxChildGiveTo);
	giveToChild->children[giveToChild->nbKeyUsed + 1] = stealFromChild->children[0];
	giveToChild->leaves[giveToChild->nbKeyUsed + 1] = stealFromChild->leaves[0];
	giveToChild->keys[giveToChild->nbKeyUsed] = node->keys[idxChildGiveTo];
	node->keys[idxChildGiveTo] = stealFromChild->keys[0];
	giveToChild->nbKeyUsed++;
	Node_shiftLeft(stealFromChild, 0, pool);
	
}
