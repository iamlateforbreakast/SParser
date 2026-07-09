#include "Malloc.h"
#include "Types.h"
#include "Debug.h"

#include <stdlib.h>

struct Malloc
{
  Allocator allocator;
};

/**********************************************//**
  @private
**************************************************/
PRIVATE Malloc mem_alloc = {
    {
        0,                 /* init / new */
        Malloc_allocate,   /* allocate */
        Malloc_deallocate, /* deallocate */
        Malloc_report,     /* report */
        Malloc_destroy,    /* destroy / delete */
        0                  /* nbAllocatedObjects */
    }
};

/******************************************************//**
  @brief Get a reference to the Malloc allocator.
  @public
  @memberof Allocator
  @return A pointer to the Malloc allocator instance.
*******************************************************/
PUBLIC Malloc * Malloc_getRef()
{
  return &mem_alloc;
}

/********************************************************//**
  @brief Delete an instance of the class Malloc.
  @public
  @memberof Allocator
  @return None
************************************************************/
PUBLIC void Malloc_delete(Allocator * this)
{
  (void)this;
}

/********************************************************//**
  @brief Allocate memory using the Malloc allocator.
  @public
  @memberof Allocator
  @param size The size of the memory block to allocate.
  @return A pointer to the allocated memory or NULL if allocation failed.
************************************************************/
PUBLIC void * Malloc_allocate(Allocator * this, unsigned int size)
{
  this->nbAllocatedObjects++;
  return (void*)malloc(size);
}

/********************************************************//**
  @brief Deallocate memory using the Malloc allocator.
  @public
  @memberof Allocator
  @param ptr A pointer to the memory block to deallocate.
  @return None
************************************************************/
PUBLIC void Malloc_deallocate(Allocator * this, void * ptr)
{
  free(ptr);
  this->nbAllocatedObjects--;
}

/********************************************************//**
  @brief Reallocate memory using the Malloc allocator.
  @public
  @memberof Allocator
  @param ptr A pointer to the memory block to reallocate.
  @param size The new size of the memory block.
  @return A pointer to the reallocated memory or NULL if reallocation failed.
************************************************************/
PUBLIC void * Malloc_reallocate(Allocator * this, void * ptr, unsigned int size)
{
  return (void*)realloc(ptr, size);
}

/******************************************************//**
  @brief Report the number of allocated objects using the Malloc allocator.
  @public
  @memberof Allocator
  @return The number of currently allocated objects.
*******************************************************/
PUBLIC unsigned int Malloc_report(Allocator * this)
{
  return this->nbAllocatedObjects;
  //PRINT(("Nb allocated objects: %d\n", ((Malloc*)this)->nbAllocatedObjects));
  //PRINT(("Max nb allocated objects: %d\n", this->maxNbObjectAllocated));
  //PRINT(("Nb alloc request: %d\n", this->allocRequestId));
  //PRINT(("Nb free requests: %d\n", this->freeRequestId));
}
