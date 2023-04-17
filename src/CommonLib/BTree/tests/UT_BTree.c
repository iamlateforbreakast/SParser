/*********************************************************************************
* Main.c
* 
*********************************************************************************/
#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "BTree.h"

#define NB_ITEMS (6)

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
	BTree* testTree;
	unsigned int keys[NB_ITEMS];
	unsigned int items[NB_ITEMS];

	unsigned int * pTestItem = 0;

	for (int i = 1; i <= NB_ITEMS; i++)
	{
		keys[i-1] = i * 5;
		items[i-1] = i * 10;
	}

	testTree = BTree_new(3);
	printf("Btree size of pool in   bytes: %d\n", BTree_reportSizeInBytes(testTree));
	printf("Btree size of pool in Kibytes: %d\n", BTree_reportSizeInBytes(testTree) / 1024);
	printf("Btree size of pool in Mibytes: %d\n", BTree_reportSizeInBytes(testTree) / (1024 * 1024));
	//BTree_print(testTree);

	double cpu_time0 = get_cpu_time();
	double wall_time0 = get_wall_time();
	
	for (int i = 0; i < NB_ITEMS; i++)
	{
		BTree_add(testTree, keys[i], &items[i]);
		BTree_print(testTree);
	}
	
	double cpu_time1 = get_cpu_time();
	double wall_time1 = get_wall_time();
	
	printf("Insert CPU time %f\n", cpu_time1 - cpu_time0);
	printf("Insert Wall time %f\n", wall_time1 - wall_time0);

    cpu_time0 = get_cpu_time();
	wall_time0 = get_wall_time();

	for (int i = 0; i< NB_ITEMS; i++)
	{
		//BTree_get(testTree, keys[i], &pTestItem);
		//printf("test item = %x %x\n", pTestItem, &items[i]);
	}

	cpu_time1 = get_cpu_time();
	wall_time1 = get_wall_time();

	printf("Search CPU time %f\n", cpu_time1 - cpu_time0);
	printf("Search Wall time %f\n", wall_time1 - wall_time0);

    for (int i = 0; i< NB_ITEMS; i++)
	{
		void * ptrRemoved = BTree_remove(testTree, keys[i]);
		printf("test item = %i %x %x\n", i, ptrRemoved, &items[i]);
		BTree_print(testTree);
	}

	BTree_free(testTree);

	return 0;
}
