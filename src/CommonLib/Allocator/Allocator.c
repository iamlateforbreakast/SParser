/**********************************************//**
  @file Allocator.c

  @brief A base Allocator class.
**************************************************/

#include "Allocator.h"

#define ALLOCATOR_IS_INVALID(a) ((a) == 0)

/********************************************************//**
  @brief Allocate a block of memory through an Allocator.

  Dispatches to the Allocator's `allocate` function pointer
  and, on success, increments the Allocator's bookkeeping
  counter so Allocator_new()'s report()-style consumers can
  track how many live blocks are outstanding.

  @public
  @memberof Allocator
  @param this The allocator to allocate from.
  @param size Number of bytes requested.
  @return Pointer to the allocated memory, or NULL on failure
          (or if `this` / its allocate hook is invalid).
************************************************************/
void * Allocator_allocate(Allocator * this, unsigned int size)
{
  void * ptr = 0;

  if (ALLOCATOR_IS_INVALID(this)) return 0;
  if (this->allocate == 0) return 0;

  ptr = this->allocate(this, size);

  if (ptr != 0)
  {
    this->nbAllocatedObjects++;
  }

  return ptr;
}

/********************************************************//**
  @brief Release a block of memory previously obtained from
  Allocator_allocate() (or Allocator_reallocate()).

  Safe to call with a NULL ptr (no-op), matching the usual
  free()-style contract.

  @public
  @memberof Allocator
  @param this The allocator that owns ptr.
  @param ptr Pointer to release. May be NULL.
************************************************************/
void Allocator_deallocate(Allocator * this, void * ptr)
{
  if (ALLOCATOR_IS_INVALID(this)) return;
  if (this->deallocate == 0) return;
  if (ptr == 0) return;

  this->deallocate(this, ptr);

  if (this->nbAllocatedObjects > 0)
  {
    this->nbAllocatedObjects--;
  }
}

/********************************************************//**
  @brief Resize a block of memory previously obtained from
  this Allocator.

  Follows realloc() semantics: passing a NULL ptr behaves
  like a fresh allocation, and passing a 0 size behaves like
  a deallocation. The Allocator's live-block counter is kept
  consistent with both of those cases.

  @public
  @memberof Allocator
  @param this The allocator that owns ptr.
  @param ptr Pointer to resize, or NULL for a fresh allocation.
  @param size New size in bytes, or 0 to free ptr.
  @return Pointer to the (possibly moved) block, or NULL if
          the block was freed or the resize failed.
************************************************************/
void * Allocator_reallocate(Allocator * this, void * ptr, unsigned int size)
{
  void * result = 0;

  if (ALLOCATOR_IS_INVALID(this)) return 0;
  if (this->reallocate == 0) return 0;

  result = this->reallocate(this, ptr, size);

  if (ptr == 0 && result != 0)
  {
    /* Acted as a fresh allocation */
    this->nbAllocatedObjects++;
  }
  else if (size == 0 && ptr != 0 && result == 0)
  {
    /* Acted as a deallocation */
    if (this->nbAllocatedObjects > 0)
    {
      this->nbAllocatedObjects--;
    }
  }

  return result;
}

/********************************************************//**
  @brief Delete an Allocator instance.

  Gives the Allocator's `delete` hook a chance to release any
  allocator-specific state (e.g. a pool's backing storage),
  then releases the Allocator struct itself.

  @public
  @memberof Allocator
  @param this The allocator to delete. Safe to call with NULL.
************************************************************/
void Allocator_delete(Allocator * this)
{
  if (ALLOCATOR_IS_INVALID(this)) return;

  if (this->delete != 0)
  {
    this->delete(this);
  }

  this->delete(this);
}