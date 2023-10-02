#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SkipList.h"

#define NB_OBJECTS (5000)

SkipList* testList;

unsigned int randomKeys[NB_OBJECTS];
unsigned int randomValues[NB_OBJECTS];

int step1()
{
    testList = SkipList_new(NB_OBJECTS + 1);

    for (int i=0; i<NB_OBJECTS; i++)
    {
        randomKeys[i] = (i + 1) * 5;
        randomValues[i] = (i + 1) * 15;
    }

    for (int i=0; i<NB_OBJECTS; i++)
    {
        int j = (rand() % NB_OBJECTS);
        unsigned int swap = randomKeys[i];
        randomKeys[i] = randomKeys[j];
        randomKeys[j] = swap;
        //printf("Swapping %d and %d\n",i,j);
    }

    long double cpu_time0 = get_cpu_time();
    long double wall_time0 = get_wall_time();

    for (int i=0; i<NB_OBJECTS; i++)
    {
        SkipList_add(testList, randomKeys[i], &randomValues[i]);
        //printf("Adding %d\n", randomKeys[i]);
    }

    long double cpu_time1 = get_cpu_time();
    long double wall_time1 = get_wall_time();

   
    SkipList_print(testList);
    SkipList_delete(testList);

    printf("Insert CPU time %Lf\n", (cpu_time1- cpu_time0)/NB_OBJECTS);
    printf("Insert Wall Time %Lf\n", (wall_time1 - wall_time0)/NB_OBJECTS);

    return 1;
}

int main()
{
    step1();
}
