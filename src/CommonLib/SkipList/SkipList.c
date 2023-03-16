/**********************************************//**
  @file SkipList.c
  @brief This file contains the implementation of the class SkipList.
  The class List implement the SkipList operations
  - Add
  - Remove
  - Get
**************************************************/

#include "SkipList.h"
#include "Pool.h"
#include <limits.h>
#include <stdlib.h>


#define MAX_OBJECTS (10)
#define SKIPLIST_MAX_LEVEL (6)

typedef struct SkipNode SkipNode;

typedef struct SkipNode
{
    unsigned int key;
    void* object;
    unsigned int level;
    unsigned int forward[SKIPLIST_MAX_LEVEL];
} SkipNode;

typedef struct SkipList
{
    unsigned int level;
    unsigned int nbObjects;
    Pool* pool;
    unsigned int headerIdx;
} SkipList;

PRIVATE unsigned int SkipList_randLevel(SkipList* this);

/**********************************************//**
  @brief SkipList_new
  @param[in] none
  @return New instance of class SkipList.
**************************************************/
PUBLIC SkipList* SkipList_new()
{
    SkipList* newSkipList = 0;

    newSkipList = (SkipList*)malloc(sizeof(SkipList));
    newSkipList->pool = Pool_new(MAX_OBJECTS, sizeof(SkipNode));
    newSkipList->level = 1;
    newSkipList->nbObjects = 0;
    SkipNode* skipNode = Pool_alloc(newSkipList->pool, &newSkipList->headerIdx);
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++)
    {
        skipNode->forward[i] = newSkipList->headerIdx;
    }
    skipNode->key = INT_MAX;
    skipNode->object = 0;
    skipNode->level = 1;
    Pool_write(newSkipList->pool, newSkipList->headerIdx, skipNode);

    return newSkipList;
}

/**********************************************//**
  @brief SkipList_new
  @param[in] Instance to destroy
  @return None
**************************************************/
PUBLIC void SkipList_free(SkipList* this)
{
    Pool_free(this->pool);
    free(this);
}

/**********************************************//**
  @brief SkipList_add
  @param[in] Key to index object
  @param[in] Object to add to SkipList object.
  @return None
**************************************************/
PUBLIC void SkipList_add(SkipList* this, unsigned int key, void* object)
 {
    unsigned int update[SKIPLIST_MAX_LEVEL + 1];
    unsigned int currentNodeIdx = this->headerIdx;
    unsigned int prevNodeIdx = this->headerIdx;
    SkipNode* prevNode = Pool_read(this->pool, this->headerIdx);
    SkipNode* currentNode = Pool_read(this->pool, this->headerIdx);

    for (int i = this->level; i == 1; i--)
    {
        while (currentNode->key < key)
        {
            prevNodeIdx = currentNodeIdx;
            prevNode = currentNode;
            currentNodeIdx = currentNode->forward[1];
            currentNode = Pool_read(this->pool, currentNodeIdx);   
        }
        update[i] = prevNode->forward[i];
    }
    //prevNodeIdx = prevNode->forward[1];
    //prevNode = Pool_read(this->pool, prevNodeIdx);
    
    if (key == currentNode->key)
    {
        currentNode->object = object;
        return 0;
    }
    else
    {
        unsigned int level;
        level = SkipList_randLevel(this);
        if (level > this->level)
        {
            for (int i = this->level + 1; i <= level; i++)
            {
                update[i] = this->headerIdx;
            }
            this->level = level;
        }
        unsigned int insertIdx;
        SkipNode* insert = Pool_alloc(this->pool, &insertIdx);
        insert->key = key;
        insert->object = object;
        insert->level = level;
        for (int i = 1; i <= level; i++)
        {
            insert->forward[i] = update[i];
            if (prevNode == currentNode) this->headerIdx = insertIdx;
            else prevNode->forward[i] = insertIdx;
        }
        Pool_write(this->pool, prevNodeIdx, prevNode);
        Pool_write(this->pool, insertIdx, insert);
        this->nbObjects++;

        return 0;
    }
}

//int skiplist_insert(skiplist* list, int key, int value) {
//    snode* update[SKIPLIST_MAX_LEVEL + 1];
//    snode* x = list->header;
//    int i, level;
//    for (i = list->level; i >= 1; i--) {
//        while (x->forward[i]->key < key)
//            x = x->forward[i];
//        update[i] = x;
//    }
//    x = x->forward[1];
//
//    if (key == x->key) {
//        x->value = value;
//        return 0;
//    }
//    else {
//        level = rand_level();
//        if (level > list->level) {
//            for (i = list->level + 1; i <= level; i++) {
//                update[i] = list->header;
//            }
//            list->level = level;
//        }
//
//        x = (snode*)malloc(sizeof(snode));
//        x->key = key;
//        x->value = value;
//        x->forward = (snode**)malloc(sizeof(snode*) * (level + 1));
//        for (i = 1; i <= level; i++) {
//            x->forward[i] = update[i]->forward[i];
//            update[i]->forward[i] = x;
//        }
//    }
//    return 0;
//}
/**********************************************//**
  @brief SkipList_delete
  @param[in] Key of object to remove
  @return Object removed from SkipList object.
**************************************************/
PUBLIC void* SkipList_delete(SkipList* this, unsigned int key)
{
    return 0;
}

//int skiplist_delete(skiplist* list, int key) {
//    int i;
//    snode* update[SKIPLIST_MAX_LEVEL + 1];
//    snode* x = list->header;
//    for (i = list->level; i >= 1; i--) {
//        while (x->forward[i]->key < key)
//            x = x->forward[i];
//        update[i] = x;
//    }
//
//    x = x->forward[1];
//    if (x->key == key) {
//        for (i = 1; i <= list->level; i++) {
//            if (update[i]->forward[i] != x)
//                break;
//            update[i]->forward[1] = x->forward[i];
//        }
//        skiplist_node_free(x);
//
//        while (list->level > 1 && list->header->forward[list->level]
//            == list->header)
//            list->level--;
//        return 0;
//    }
//    return 1;
//}

/**********************************************//**
  @brief SkipList_print
  @param[in] None
  @return None
**************************************************/
PUBLIC void SkipList_print(SkipList* this)
{
    printf("NbObject: %d\n", this->nbObjects);
    printf("Level: %d\n", this->level);

    SkipNode* skipNode = Pool_read(this->pool, this->headerIdx);
    for (int i = 0; i < this->nbObjects; i++)
    {
        printf("SkipNode: %d ", i);
        printf(" Level: %d ", skipNode->level);
        printf(" Key: %d ", skipNode->key);
        for (int j = 1; j < skipNode->level + 1; j++)
        {
            printf(" Forward[%d]: %d", j, skipNode->forward[i]);
        }
        printf("\n");
        skipNode = Pool_read(this->pool, skipNode->forward[1]);
        Pool_discardCache(this->pool, skipNode->forward[1]);
    }
}

PRIVATE unsigned int SkipList_randLevel(SkipList* this)
{
    int level = 1;
    while (rand() < RAND_MAX / 2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}
