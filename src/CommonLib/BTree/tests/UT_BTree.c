/*********************************************************************************
* Main.c
* 
*********************************************************************************/
#include <stdlib.h>
#include "CommonTypes.h"
#include "BTree.h"
#include "BeamWeightRange.h"

#define NB_WEIGHTRANGE (200)

/*********************************************************************************
* main
* input: None
* output: None
*********************************************************************************/
unsigned int main(void)
{
	unsigned int isFailed = FALSE;

	BTree * beamWeights;
	BeamWeightRange * beamWeightRange[NB_WEIGHTRANGE];
	BeamWeightRange * resultBeamWeightRange = NULL;
	unsigned int keys[NB_WEIGHTRANGE];
	/* unsigned int keys[] = {   500,  1000,    25,  1001,   499,  3000,     1,     2,  2000, 65000,
		                       10,   200,    90, 65001,   300,   400,   600,   800,   900,  1100,
	                            5,  7000,  2001, 50000,  8500,   999,    19,    37,  1789,  2950,
	                          501,  1002,    26 , 1003,   498,  3001,     3,     4,  2002, 20000,
	                          502,  1004,    27,  1004,  8501,   998,   601,   801,   901,  1101,
	                            6,  7001,  2003, 50001,  8502,   997,     7,     8,  1812, 2900};*/

	for (int i = 0; i < NB_WEIGHTRANGE; i++) keys[i] = i + 1;
	int prev = 0;
	for (int i = 0; i < NB_WEIGHTRANGE; i++)
	{
		unsigned int next = (NB_WEIGHTRANGE * rand()/RAND_MAX);
		unsigned int swap = keys[next];
		keys[next] = keys[prev];
		keys[prev] = swap;
		prev = next;
	}
	//for (int i = 0; i < NB_WEIGHTRANGE; i++) printf("Key[%d]=%d\n", i, keys[i]);
	/* Initialise beamWeightRange to some default values*/
	for (int i = 0; i < NB_WEIGHTRANGE; i++)
	{
		beamWeightRange[i] = (BeamWeightRange*)malloc(sizeof(BeamWeightRange));
		beamWeightRange[i]->range = 1;
		beamWeightRange[i]->frequency = 100; /* 30200MHz */ 
		for (int j = 0; j < 160; j++)
		{
			beamWeightRange[i]->weights[j] = j + i;
		}
	}

	printf("Step 1 - Create a storage tree.\n");

	beamWeights = BTree_new();

	printf("Step 2 - Add %d beamWeightRange\n", NB_WEIGHTRANGE);

	for (int i = 0; i < NB_WEIGHTRANGE; i++)
	{
		BTree_add(beamWeights, keys[i], beamWeightRange[i]);
	}

	printf("Step 3 - Obtain a given beamWeightRange\n");

	unsigned int nbNotFound = 0;
	for (int i = 1; i <= NB_WEIGHTRANGE; i++)
	{
		resultBeamWeightRange = (BeamWeightRange*)BTree_get(beamWeights, keys[i-1]);
		if (resultBeamWeightRange == NULL)
		{
			printf(" BTree_get %d:%d result is not found\n", i, keys[i - 1]);
			nbNotFound++;
		}
	}
	resultBeamWeightRange = (BeamWeightRange*)BTree_get(beamWeights, 501);
	if (resultBeamWeightRange == NULL)
	{
		printf(" BTree_get %d:%d result is not found\n", NB_WEIGHTRANGE+1, 501);
		nbNotFound++;
	}
	printf("Items not found: %d\n", nbNotFound);
	
 	printf("Step 4 - Print the content of the tree\n");

	BTree_print(beamWeights);

	printf("Step 5 - Remove beamWeightRange\n");
	for (int i = 1; i <= NB_WEIGHTRANGE; i++)
	{
		printf("Removing %d \n", keys[i - 1]);
		if (keys[i - 1] == 177)
		{
			BTree_print(beamWeights);
			resultBeamWeightRange = (BeamWeightRange*)BTree_remove(beamWeights, keys[i - 1]);
		}
		else
		{
			resultBeamWeightRange = (BeamWeightRange*)BTree_remove(beamWeights, keys[i - 1]);
		}
	}

	BTree_print(beamWeights);

	printf("Step 6 - De-allocate the beamweight tree\n");

	BTree_free(beamWeights);

	return isFailed;
}
