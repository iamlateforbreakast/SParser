/**********************************************//** 
  @file ObjectMgr.c
 
  @brief An object management class.
  @details This class provides an object allocation and
   de-allocation service. Only one instance of this class can be created.
**************************************************/

#include "ObjectMgr.h"
#include "Object.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NB_OBJECTS (500)
#define END_OF_QUEUE (0xFFFFFFFF)

typedef struct ObjectInfo
{
  Object * ptr;
  unsigned int prevId;
  unsigned int nextId; 
} ObjectInfo;

/**********************************************//**
  @class ObjectMgr
**************************************************/
struct ObjectMgr
{
  Object object;
  unsigned int nbBytesAllocated; /**< This is member A */
  unsigned int maxNbBytesAllocated; /**< This is member B */
  unsigned int allocRequestId;
  unsigned int freeRequestId;
  unsigned int nbAllocatedObjects;
  ObjectInfo allocatedObjects[MAX_NB_OBJECTS];
  unsigned int freeSpace;
  unsigned int usedSpace;
};

PRIVATE ObjectMgr * objectMgr = 0;

/**********************************************//** 
  @brief TBD
  @details TBD
  @private
  @memberof ObjectMgr
**************************************************/
PRIVATE ObjectMgr * ObjectMgr_new()
{
  ObjectMgr * this = 0;
  int i = 0;
  
  this = (ObjectMgr*)malloc(sizeof(ObjectMgr));
    
  this->object.delete = (void(*)(Object*))&ObjectMgr_delete;
  this->object.copy = (Object*(*)(Object*))&ObjectMgr_copy;
  this->object.size = sizeof(ObjectMgr);
  this->object.refCount = 1;
  this->object.id =0;
  
  this->nbBytesAllocated = sizeof(ObjectMgr);
  this->maxNbBytesAllocated = sizeof(ObjectMgr);
  this->allocRequestId = 1;
  this->freeRequestId = 0;
  this->nbAllocatedObjects = 1;
  memset(this->allocatedObjects, 0, MAX_NB_OBJECTS * sizeof(Object*));
  
  for (i=2; i<MAX_NB_OBJECTS-1; i++)
  {
    this->allocatedObjects[i].prevId = i - 1;
    this->allocatedObjects[i].nextId = i + 1;
  }
  this->allocatedObjects[1].prevId = END_OF_QUEUE; 
  this->allocatedObjects[1].nextId = 2; 
  this->allocatedObjects[MAX_NB_OBJECTS-1].prevId = MAX_NB_OBJECTS-2;
  this->allocatedObjects[MAX_NB_OBJECTS-1].nextId = END_OF_QUEUE;
  this->freeSpace = 1;
  this->allocatedObjects[0].ptr = (Object*)&this->object;
  this->allocatedObjects[0].prevId = END_OF_QUEUE;
  this->allocatedObjects[0].nextId = END_OF_QUEUE;
  this->usedSpace = 0;
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class ObjectMgr.
  @public
  @memberof ObjectMgr
**************************************************/
PUBLIC void ObjectMgr_delete(ObjectMgr * this)
{
  this->object.refCount = this->object.refCount - 1;
  
  if (this->object.refCount == 0)
  {
    /* TODO: memset(this, 0, sizeof(ObjectMgr)); */
    free(this);
    this = 0;
  } 
}

/**********************************************//** 
  @brief Copy an instance of the class ObjectMgr.
  @public
  @memberof ObjectMgr
  @return New instance
**************************************************/
PUBLIC ObjectMgr * ObjectMgr_copy(ObjectMgr * this)
{
  return this;
}

/**********************************************//** 
  @brief Get a reference to the singleton instance of ObjectMgr.
  @public
  @memberof ObjectMgr
  @return Reference to the singleton.
**************************************************/
PUBLIC ObjectMgr * ObjectMgr_getRef()
{
  if (objectMgr==0)
  {
    objectMgr = ObjectMgr_new();
  }
  else
  {
    objectMgr->object.refCount++;
  }
  
  return objectMgr;
}

/**********************************************//** 
  @brief Reports the usage statistics for an instance of ObjectMgr.
  @public
  @memberof ObjectMgr
**************************************************/
PUBLIC void ObjectMgr_report(ObjectMgr * this)
{
	printf("Nb bytes not freed: %d\n", this->nbBytesAllocated);
  printf("Max nb bytes used: %d\n", this->maxNbBytesAllocated);
  printf("Nb alloc request %d\n", this->allocRequestId);
  printf("Nb free requests %d\n", this->freeRequestId);
}

/**********************************************//** 
  @brief Allocate a new object memory footprint of a given size.
  @public
  @memberof ObjectMgr
  @param[in] size size in bytes of the memory footprint.
  @return Reference to a instance of Object.
**************************************************/
PUBLIC Object * ObjectMgr_allocate(ObjectMgr * this, unsigned int size)
{
  Object * result = 0;
  
  /* Ensure size requested is >0 */
  if (size>0)
  {
    if (this->nbAllocatedObjects<MAX_NB_OBJECTS)
    {
      result = (Object*)malloc(size);
      
      if (result!=0)
      {
        this->allocatedObjects[this->usedSpace].nextId = this->freeSpace;
        this->allocatedObjects[this->freeSpace].prevId = this->usedSpace;
        this->usedSpace = this->freeSpace;
        this->freeSpace = this->allocatedObjects[this->freeSpace].nextId;
        this->allocatedObjects[this->freeSpace].prevId = END_OF_QUEUE;
        this->allocatedObjects[this->usedSpace].nextId = END_OF_QUEUE;
        this->nbAllocatedObjects++;
        this->allocatedObjects[this->usedSpace].ptr = result;
        result->id = this->usedSpace;
        this->nbBytesAllocated=this->nbBytesAllocated + size;
        if (this->nbBytesAllocated >this->maxNbBytesAllocated) this->maxNbBytesAllocated = this->nbBytesAllocated;
        this->allocRequestId++;
      }
      else
      {
        /* Error case: Failure to allocate object */
        exit(1);
      }
    }
    else
    {
      /* Error case: too many object already created */
      exit(1); 
    }
  }
  else
  {
    /* Error case: request to create a 0 bytes object */
    exit(1);
  }
  
  return result;
}

/**********************************************//** 
  @brief De Allocate a given object.
  @public
  @memberof ObjectMgr
  @param[in] object Reference to instance of Object.
**************************************************/
PUBLIC void ObjectMgr_deallocate(ObjectMgr * this, Object * object)
{
  unsigned int idx =  object->id;
  
  if (object!=0)
  {
    if (this->nbAllocatedObjects>1)
    {
      free(this->allocatedObjects[idx].ptr);
      
      if (this->allocatedObjects[idx].nextId != END_OF_QUEUE)
      {
        this->allocatedObjects[this->allocatedObjects[idx].nextId].prevId = this->allocatedObjects[idx].prevId;  
      }
      else
      {
        this->usedSpace = this->allocatedObjects[idx].prevId;
      }
      this->allocatedObjects[this->allocatedObjects[idx].prevId].nextId = this->allocatedObjects[idx].nextId;
      this->allocatedObjects[idx].nextId = this->freeSpace;
      this->allocatedObjects[idx].prevId = END_OF_QUEUE;
      this->freeSpace = idx;
      this->nbAllocatedObjects = this->nbAllocatedObjects - 1;
      this->nbBytesAllocated = this->nbBytesAllocated - object->size;
    }
    else
    {
      /* Error case: No allocated object left to free */
      exit(1);
    }
  }
  else
  {
    /* Error case: request to free a NULL pointer */
    exit(1);
  }
}
