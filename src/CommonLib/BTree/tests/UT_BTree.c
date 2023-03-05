/*********************************************************************************
* Main.c
* 
*********************************************************************************/
#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "BTree.h"

#define NB_ITEMS (10)

//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time() {
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) {
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)) {
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |
				((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else {
		//  Handle error
		return 0;
	}
}
//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time() {
	struct timeval time;
	if (gettimeofday(&time, NULL)) {
		//  Handle error
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time() {
	return (double)clock() / CLOCKS_PER_SEC;
}
#endif

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
	/* unsigned int keys[] = {   500,  1000,    25,  1001,   499,  3000,     1,     2,  2000, 65000,
		                       10,   200,    90, 65001,   300,   400,   600,   800,   900,  1100,
	                            5,  7000,  2001, 50000,  8500,   999,    19,    37,  1789,  2950,
	                          501,  1002,    26 , 1003,   498,  3001,     3,     4,  2002, 20000,
	                          502,  1004,    27,  1004,  8501,   998,   601,   801,   901,  1101,
	                            6,  7001,  2003, 50001,  8502,   997,     7,     8,  1812, 2900};*/

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

	testTree = BTree_new();

	printf("Step 2 - Add %d beamWeightRange\n", NB_ITEMS);
	double cpu_time0 = get_cpu_time();
	double wall_time0 = get_wall_time();
	
	for (int i = 0; i < NB_ITEMS; i++)
	{
		BTree_add(testTree, keys[i], items[i]);
	}
	
	double cpu_time1 = get_cpu_time();
	double wall_time1 = get_wall_time();
	
	printf("Insert CPU time %f\n", cpu_time1 - cpu_time0);
	printf("Insert Wall time %f\n", wall_time1 - wall_time0);
	
	printf("Step 3 - Obtain a given item\n");

	unsigned int nbNotFound = 0;
	for (int i = 1; i <= NB_ITEMS; i++)
	{
		resultItem = (unsigned int *)BTree_get(testTree, keys[i-1]);
		if (resultItem == NULL)
		{
			printf(" BTree_get %d:%d result is not found\n", i, keys[i - 1]);
			nbNotFound++;
		}
	}
	resultItem = (unsigned int *)BTree_get(testTree, 501);
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
