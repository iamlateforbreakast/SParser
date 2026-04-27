/* Arena.c */

#include "Arena.h"
#include "Types.h"
#include "Debug.h"

/* Arena configuration constants */
#define ARENA_SIZE 65536  /* 64KB default arena size */
#define ARENA_ALIGNMENT 8 /* 8-byte alignment for allocations */

/* Helper macro to align pointers */
#define ALIGN_UP(ptr, alignment) \
  ((void*)(((unsigned long)(ptr) + (alignment) - 1) & ~((alignment) - 1)))

struct Arena
{
  Allocator allocator;
  char * buffer;           /* Start of the arena buffer */
  char * current;          /* Current allocation pointer */
  unsigned int totalSize;  /* Total size of the arena */
  unsigned int usedSize;   /* Number of bytes currently used */
  unsigned int maxUsed;    /* Peak usage */
};

/*
 * @brief Create an Arena allocator.
 * @details Initializes and returns an Arena allocator instance.
 *          The arena is initialized with a fixed-size buffer for fast, efficient
 *          allocation. All allocations are carved from this single contiguous block.
 * @public
 * @return Pointer to the Arena instance
 */
PUBLIC Arena * Arena_new()
{
  Arena * mem_arena = (Arena*)malloc(sizeof(Arena));

  if (mem_arena == 0) return 0;

  /* Initialize the arena on first access */
  mem_arena->buffer = (char*)malloc(ARENA_SIZE);
  mem_arena->current = mem_arena->buffer;
  mem_arena->totalSize = ARENA_SIZE;
  mem_arena->usedSize = 0;
  mem_arena->maxUsed = 0;
    
  /* Set up the allocator function pointers */
  mem_arena->allocator.new = 0;
  mem_arena->allocator.allocate = Arena_allocate;
  mem_arena->allocator.deallocate = Arena_deallocate;
  mem_arena->allocator.report = Arena_report;
  mem_arena->allocator.delete = Arena_delete;
  mem_arena->allocator.nbAllocatedObjects = 0;

}

/*
 * @brief Delete the Arena allocator and free its resources.
 * @details Frees the underlying buffer allocated for the arena.
 *          Since arena allocators do not track individual objects,
 *          all memory is released at once when the arena is deleted.
 * @public
 * @param this Pointer to the Allocator interface (cast from Arena*)
 */
PUBLIC void Arena_delete(Allocator * this)
{
  Arena * arena = (Arena*)this;
  
  if (OBJECT_IS_INVALID(arena)) return;

  if (arena->buffer != 0)
  {
    free(arena->buffer);
    arena->buffer = 0;
    arena->current = 0;
    arena->totalSize = 0;
    arena->usedSize = 0;
    arena->maxUsed = 0;
  }
}

/*
 * @brief Allocate memory from the arena.
 * @details Carves out a chunk of memory from the arena's contiguous buffer.
 *          The allocation pointer is advanced by the requested size (with alignment).
 *          Allocations are extremely fast since no metadata or fragmentation concerns exist.
 *          If the arena runs out of space, returns NULL.
 * @public
 * @param this Pointer to the Allocator interface (cast from Arena*)
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if allocation fails
 */
PUBLIC void * Arena_allocate(Allocator * this, unsigned int size)
{
  Arena * arena = (Arena*)this;
  void * ptr = 0;
  unsigned int alignedSize = 0;

  /* Align the requested size to ARENA_ALIGNMENT boundary */
  alignedSize = ((size + ARENA_ALIGNMENT - 1) / ARENA_ALIGNMENT) * ARENA_ALIGNMENT;

  /* Check if there is enough space in the arena */
  if ((arena->usedSize + alignedSize) <= arena->totalSize)
  {
    ptr = (void*)arena->current;
    arena->current += alignedSize;
    arena->usedSize += alignedSize;
    
    /* Track peak usage */
    if (arena->usedSize > arena->maxUsed)
    {
      arena->maxUsed = arena->usedSize;
    }
    
    /* Increment the object counter */
    arena->allocator.nbAllocatedObjects++;
  }
  else
  {
    /* Arena exhausted, allocation failed */
    PRINT(("Arena exhausted: requested %u bytes, available %u bytes\n", 
           alignedSize, arena->totalSize - arena->usedSize));
    ptr = 0;
  }

  return ptr;
}

/*
 * @brief Deallocate memory from the arena.
 * @details Arena allocators typically do not support individual deallocation.
 *          This is a no-op function for compatibility with the Allocator interface.
 *          Memory is only reclaimed when the entire arena is deleted.
 *          However, we decrement the object counter for tracking purposes.
 * @public
 * @param this Pointer to the Allocator interface (cast from Arena*)
 * @param ptr Pointer to memory to deallocate (ignored)
 */
PUBLIC void Arena_deallocate(Allocator * this, void * ptr)
{
  Arena * arena = (Arena*)this;
  
  /* Arena allocators do not support individual deallocation */
  /* Just decrement the object counter for statistics */
  if (arena->allocator.nbAllocatedObjects > 0)
  {
    arena->allocator.nbAllocatedObjects--;
  }
  
  /* Note: ptr is ignored - individual memory chunks cannot be freed from an arena */
  (void)ptr;  /* Suppress unused parameter warning */
}

/*
 * @brief Report statistics about the arena allocator.
 * @details Returns the number of currently allocated objects in the arena.
 *          Additional statistics can be printed for debugging purposes.
 * @public
 * @param this Pointer to the Allocator interface (cast from Arena*)
 * @return Number of allocated objects
 */
PUBLIC unsigned int Arena_report(Allocator * this)
{
  Arena * arena = (Arena*)this;
  
  PRINT(("Arena Usage Report:\n"));
  PRINT(("  Total size: %u bytes\n", arena->totalSize));
  PRINT(("  Used size: %u bytes\n", arena->usedSize));
  PRINT(("  Peak used: %u bytes\n", arena->maxUsed));
  PRINT(("  Available: %u bytes\n", arena->totalSize - arena->usedSize));
  PRINT(("  Allocated objects: %d\n", arena->allocator.nbAllocatedObjects));
  
  return arena->allocator.nbAllocatedObjects;
}