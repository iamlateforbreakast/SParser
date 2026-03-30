/**********************************************//**
  @file CharBuffer.h

  @brief Interface for the CharBuffer class.

  @details CharBuffer manages a heap-backed buffer of characters that
  grows automatically using a doubling strategy when capacity is
  exceeded. It integrates with the Object base class and supports
  both the ObjectMgr and ObjectStore allocation paths.
**************************************************/

#ifndef CHARBUFFER_H
#define CHARBUFFER_H

#include "Object.h"
#include "Class.h"
#include "Allocator.h"

#define CHARBUFFER_DEFAULT_SIZE (64U)

/**********************************************//**
  @class CharBuffer
**************************************************/
typedef struct CharBuffer CharBuffer;

/*-------- Lifecycle --------------------------------------------------*/

PUBLIC CharBuffer * CharBuffer_new();
PUBLIC CharBuffer * CharBuffer_newFromAllocator(Allocator * allocator);
PUBLIC void         CharBuffer_delete(CharBuffer * this);

/*-------- Buffer operations ------------------------------------------*/

PUBLIC int          CharBuffer_appendChar(CharBuffer * this, char c);
PUBLIC int          CharBuffer_appendString(CharBuffer * this, const char * str);
PUBLIC void         CharBuffer_clear(CharBuffer * this);
PUBLIC unsigned int CharBuffer_getLength(CharBuffer * this);
PUBLIC unsigned int CharBuffer_getCapacity(CharBuffer * this);
PUBLIC const char * CharBuffer_getData(CharBuffer * this);

/*-------- Object protocol --------------------------------------------*/

PUBLIC CharBuffer * CharBuffer_copy(CharBuffer * this);
PUBLIC int          CharBuffer_compare(CharBuffer * this, CharBuffer * compared);
PUBLIC char *       CharBuffer_print(CharBuffer * this);
PUBLIC Class *      CharBuffer_getClass();

#endif /* CHARBUFFER_H */