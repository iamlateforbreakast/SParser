/*********************************************************************************
* Main.c
* 
*********************************************************************************/
#include <stdlib.h>
#include "Types.h"
#include "BTree.h"

#define NB_ITEMS (10)

/*********************************************************************************
* main
* input: None
* output: None
*********************************************************************************/
unsigned int main(void)
{
	unsigned int isFailed = FALSE;

	BTree * testTree;
	unsigned int * items[NB_ITEMS];
	unsigned int * resultItem = NULL;
	unsigned int keys[NB_ITEMS];

	for (int i = 0; i < NB_ITEMS; i++) keys[i] = i + 1;
	int prev = 0;
	for (int i = 0; i < NB_ITEMS; i++)
	{
		unsigned int next = (NB_ITEMS * rand()/RAND_MAX);
		unsigned int swap = keys[next];
		keys[next] = keys[prev];
		keys[prev] = swap;
		prev = next;
	}
	//for (int i = 0; i < NB_ITEMS; i++) printf("Key[%d]=%d\n", i, keys[i]);
	/* Initialise beamWeightRange to some default values*/
	for (int i = 0; i < NB_ITEMS; i++)
	{
		items[i] = (unsigned int *)malloc(sizeof(unsigned int));
		items[i] = i;
	}

	printf("Step 1 - Create a storage tree.\n");

	testTree = BTree_new(3);

	printf("Step 2 - Add %d beamWeightRange\n", NB_ITEMS);

	for (int i = 0; i < NB_ITEMS; i++)
	{
		BTree_add(testTree, keys[i], items[i]);
	}

	printf("Step 3 - Obtain a given item\n");

	unsigned int nbNotFound = 0;
	for (int i = 1; i <= NB_ITEMS; i++)
	{
		BTree_get(testTree, keys[i-1], &resultItem);
		if (resultItem == NULL)
		{
			printf(" BTree_get %d:%d result is not found\n", i, keys[i - 1]);
			nbNotFound++;
		}
	}
	BTree_get(testTree, 501, &resultItem);
	if (resultItem == NULL)
	{
		printf(" BTree_get %d:%d result is not found\n", NB_ITEMS+1, 501);
		nbNotFound++;
	}
	printf("Items not found: %d\n", nbNotFound);
	
 	printf("Step 4 - Print the content of the tree\n");

	BTree_print(testTree);

	printf("Step 5 - Remove beamWeightRange\n");
	for (int i = 1; i <= NB_ITEMS; i++)
	{
		printf("Removing %d \n", keys[i - 1]);
		if (keys[i - 1] == 177)
		{
			BTree_print(testTree);
			resultItem = (unsigned int *)BTree_remove(testTree, keys[i - 1]);
		}
		else
		{
			resultItem = (unsigned int *)BTree_remove(testTree, keys[i - 1]);
		}
	}

	BTree_print(testTree);

	printf("Step 6 - De-allocate the beamweight tree\n");

	BTree_free(testTree);

	return isFailed;
}
