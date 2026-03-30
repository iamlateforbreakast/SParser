/**********************************************//**
  @file CharBuffer.c

  @brief Implementation of the CharBuffer class.

  @details CharBuffer wraps a dynamically sized char array behind the
  Object lifecycle protocol. The internal buffer is separate from the
  Object footprint and is managed directly via Memory_alloc /
  Memory_free so that it can be resized independently of the Object
  slot. When the buffer is full, capacity is doubled until the append
  succeeds or Memory_alloc fails.

  The class registers itself with the framework via a static Class
  descriptor and supports both construction paths:
  - CharBuffer_new(): uses the ObjectMgr pool (Object_new).
  - CharBuffer_newFromAllocator(): uses the ObjectStore path
    (Object_newFromAllocator) with a caller-supplied Allocator.

  Invariant: this->data is always non-null for any valid CharBuffer,
  this->length < this->capacity, and this->data[this->length] == '\0'.
**************************************************/

#include "CharBuffer.h"
#include "Object.h"
#include "Class.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#include <string.h>

/**********************************************//**
  @private Internal structure
**************************************************/
struct CharBuffer
{
  Object    object;       /**< Must be first - base class */
  char *    data;         /**< Heap-allocated character buffer */
  unsigned int length;    /**< Number of chars written (excluding NUL) */
  unsigned int capacity;  /**< Total allocated bytes including NUL slot */
};

/*-------- Forward declarations of Object protocol functions ----------*/
PRIVATE void        CharBuffer_destroy(CharBuffer * this);
PRIVATE CharBuffer* CharBuffer_copyOp(CharBuffer * this);
PRIVATE int         CharBuffer_compareOp(CharBuffer * this, CharBuffer * compared);
PRIVATE char*       CharBuffer_printOp(CharBuffer * this);
PRIVATE unsigned int CharBuffer_size();

/*-------- Class descriptor -------------------------------------------*/
PRIVATE Class charBufferClass =
{
  .f_new    = 0,
  .f_delete = (Destructor)     &CharBuffer_destroy,
  .f_copy   = (Copy_Operator)  &CharBuffer_copyOp,
  .f_comp   = (Comp_Operator)  &CharBuffer_compareOp,
  .f_print  = (Printer)        &CharBuffer_printOp,
  .f_size   = (Sizer)          &CharBuffer_size
};

/**********************************************//**
  @brief Return size of the CharBuffer struct.
  @private
**************************************************/
PRIVATE unsigned int CharBuffer_size()
{
  return sizeof(CharBuffer);
}

/**********************************************//**
  @brief Allocate and initialise the internal char buffer.
  @private
  @return 1 on success, 0 on allocation failure.
**************************************************/
PRIVATE int CharBuffer_initBuffer(CharBuffer * this)
{
  this->data = (char*)Memory_alloc(CHARBUFFER_DEFAULT_SIZE);
  if (this->data == 0) return 0;

  this->data[0]  = '\0';
  this->length   = 0;
  this->capacity = CHARBUFFER_DEFAULT_SIZE;

  return 1;
}

/**********************************************//**
  @brief Double the internal buffer capacity.
  @private
  @return 1 on success, 0 on allocation failure.
**************************************************/
PRIVATE int CharBuffer_grow(CharBuffer * this)
{
  unsigned int newCapacity = this->capacity * 2;
  char * newData = (char*)Memory_alloc(newCapacity);

  if (newData == 0)
  {
    Error_new(ERROR_NORMAL, "CharBuffer_grow: failed to allocate %u bytes\n", newCapacity);
    return 0;
  }

  memcpy(newData, this->data, this->length + 1);
  Memory_free(this->data, this->capacity);

  this->data     = newData;
  this->capacity = newCapacity;

  return 1;
}

/**********************************************//**
  @brief Shared post-construction initialisation.
  @private
  @return 1 on success, 0 on failure.
**************************************************/
PRIVATE int CharBuffer_init(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;
  if (!CharBuffer_initBuffer(this))
  {
    Error_new(ERROR_NORMAL, "CharBuffer_init: internal buffer allocation failed\n");
    return 0;
  }
  return 1;
}

/*-------- Lifecycle --------------------------------------------------*/

/**********************************************//**
  @brief Create a CharBuffer using the ObjectMgr pool.
  @public
  @memberof CharBuffer
  @return New instance, or 0 on failure.
**************************************************/
PUBLIC CharBuffer * CharBuffer_new()
{
  CharBuffer * this = (CharBuffer*)Object_new(sizeof(CharBuffer), &charBufferClass);

  if (!CharBuffer_init(this)) return 0;

  return this;
}

/**********************************************//**
  @brief Create a CharBuffer using a caller-supplied Allocator.
  @public
  @memberof CharBuffer
  @param[in] allocator Allocator registered with the ObjectStore.
  @return New instance, or 0 on failure.
**************************************************/
PUBLIC CharBuffer * CharBuffer_newFromAllocator(Allocator * allocator)
{
  if (allocator == 0) return 0;

  CharBuffer * this = (CharBuffer*)Object_newFromAllocator(&charBufferClass, allocator);

  if (!CharBuffer_init(this)) return 0;

  return this;
}

/**********************************************//**
  @brief Release the internal buffer. Called by the Object layer.
  @private
  @memberof CharBuffer
**************************************************/
PRIVATE void CharBuffer_destroy(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return;

  if (this->data != 0)
  {
    Memory_free(this->data, this->capacity);
    this->data     = 0;
    this->length   = 0;
    this->capacity = 0;
  }

  Object_deallocate((Object*)this);
}

/**********************************************//**
  @brief Public delete — invokes destructor then deallocates.
  @public
  @memberof CharBuffer
**************************************************/
PUBLIC void CharBuffer_delete(CharBuffer * this)
{
  Object_delete((Object*)this);
}

/*-------- Buffer operations ------------------------------------------*/

/**********************************************//**
  @brief Append a single character to the buffer.
  @public
  @memberof CharBuffer
  @param[in] c Character to append.
  @return 1 on success, 0 on failure.
**************************************************/
PUBLIC int CharBuffer_appendChar(CharBuffer * this, char c)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  /* Grow if no room for char + NUL */
  if (this->length + 1 >= this->capacity)
  {
    if (!CharBuffer_grow(this)) return 0;
  }

  this->data[this->length]     = c;
  this->data[this->length + 1] = '\0';
  this->length++;

  return 1;
}

/**********************************************//**
  @brief Append a null-terminated string to the buffer.
  @public
  @memberof CharBuffer
  @param[in] str String to append.
  @return 1 on success, 0 on failure.
**************************************************/
PUBLIC int CharBuffer_appendString(CharBuffer * this, const char * str)
{
  if (OBJECT_IS_INVALID(this)) return 0;
  if (str == 0) return 0;

  unsigned int strLen = (unsigned int)strlen(str);

  /* Grow until there is room for str + NUL */
  while (this->length + strLen >= this->capacity)
  {
    if (!CharBuffer_grow(this)) return 0;
  }

  memcpy(this->data + this->length, str, strLen + 1);
  this->length += strLen;

  return 1;
}

/**********************************************//**
  @brief Reset the buffer to empty without releasing capacity.
  @public
  @memberof CharBuffer
**************************************************/
PUBLIC void CharBuffer_clear(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->data[0] = '\0';
  this->length  = 0;
}

/**********************************************//**
  @brief Return the number of characters written (excluding NUL).
  @public
  @memberof CharBuffer
**************************************************/
PUBLIC unsigned int CharBuffer_getLength(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  return this->length;
}

/**********************************************//**
  @brief Return the current allocated capacity in bytes.
  @public
  @memberof CharBuffer
**************************************************/
PUBLIC unsigned int CharBuffer_getCapacity(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  return this->capacity;
}

/**********************************************//**
  @brief Return a read-only pointer to the buffer contents.
  @public
  @memberof CharBuffer
**************************************************/
PUBLIC const char * CharBuffer_getData(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  return this->data;
}

/*-------- Object protocol --------------------------------------------*/

/**********************************************//**
  @brief Produce a deep copy of a CharBuffer.
  @private
**************************************************/
PRIVATE CharBuffer * CharBuffer_copyOp(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  CharBuffer * copy = CharBuffer_new();
  if (copy == 0) return 0;

  if (!CharBuffer_appendString(copy, this->data))
  {
    CharBuffer_delete(copy);
    return 0;
  }

  return copy;
}

PUBLIC CharBuffer * CharBuffer_copy(CharBuffer * this)
{
  return CharBuffer_copyOp(this);
}

/**********************************************//**
  @brief Lexicographic comparison of two CharBuffers.
  @private
**************************************************/
PRIVATE int CharBuffer_compareOp(CharBuffer * this, CharBuffer * compared)
{
  if (OBJECT_IS_INVALID(this))     return -1;
  if (OBJECT_IS_INVALID(compared)) return  1;

  return strcmp(this->data, compared->data);
}

PUBLIC int CharBuffer_compare(CharBuffer * this, CharBuffer * compared)
{
  return CharBuffer_compareOp(this, compared);
}

/**********************************************//**
  @brief Return a pointer to the buffer contents for printing.
  @details Returns the internal data pointer directly — caller must
  not free or modify it.
  @private
**************************************************/
PRIVATE char * CharBuffer_printOp(CharBuffer * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  return this->data;
}

PUBLIC char * CharBuffer_print(CharBuffer * this)
{
  return CharBuffer_printOp(this);
}

/**********************************************//**
  @brief Return the class descriptor for CharBuffer.
  @public
**************************************************/
PUBLIC Class * CharBuffer_getClass()
{
  return &charBufferClass;
}