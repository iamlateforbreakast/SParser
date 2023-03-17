// SkipList.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "SkipList.h"

#define NB_OBJECTS (6)

SkipList* testList;

int step1()
{
    testList = SkipList_new();

    return 1;
}

int step2()
{
    unsigned int keys[] = { 55, 30, 80, 10, 40, 120 };
    unsigned int value[] = { 1972, 2005, 1945, 1875, 2001, 1515 };

    for (int i = 0; i < NB_OBJECTS; i++)
    {
        SkipList_add(testList, keys[i], &value);
        SkipList_print(testList);
    }
        

    return 1;
}

int step3()
{
    
    SkipList_print(testList);

    return 1;
}

int step4()
{
    SkipList_free(testList);
    return 1;
}

int main()
{
    step1();
    step2();
    step3();
    step4();
}
