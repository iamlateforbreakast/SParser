/* TransUnit.c */
#include "TransUnit.h"
#include "List.h"
#include "Memory.h"
#include "Object.h"

struct Buffer
{
  String * Buffer;
  char * currentPtr;
  char * startPtr;
};

/**********************************************//**
  @class TransUnit
**************************************************/
struct TransUnit
{
  Object object;
  FileDesc* file;
  List* buffers;
  struct Buffer * currentBuffer;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class transUnitClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&TransUnit_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&TransUnit_print,
  .f_size = (Sizer)&TransUnit_getSize
};

/**********************************************//**
  @brief Create a new TransUnit object.
  @public
  @memberof TransUnit
  @return Created TransUnit instance.
**************************************************/
PUBLIC TransUnit * TransUnit_new(FileDesc * file)
{
  TransUnit* this = 0;
  String* newFileContent = 0;

  if (file == 0) return 0;

  this = (TransUnit*)Object_new(sizeof(TransUnit), &transUnitClass);
  
  newFileContent = FileDesc_load(file);
  List_insertHead(this->buffers, newFileContent, 1);
  
  return this;
}

/**********************************************//**
  @brief Delete an instance of a TransUnit object.
  @public
  @memberof TransUnit
**************************************************/
PUBLIC void TransUnit_delete(TransUnit * this)
{
  if (this == 0) return;

  List_delete(this->buffers);
  Object_deallocate(&this->object);
}

PUBLIC void TransUnit_print(TransUnit* this)
{

}

PUBLIC unsigned int TransUnit_getSize(TransUnit* this)
{
  return sizeof(TransUnit);
}

PUBLIC String * TransUnit_getNextBuffer(TransUnit * this)
{
  char* ptr = this->currentBuffer->currentPtr;  //String_getBuffer(this->currentBuffer);
  int isReadyToEmit = 0;

  while (!isReadyToEmit)
  {
    if (Memory_ncmp(this->currentBuffer->currentPtr, "//", 2))
    {
      // Consume until the end of line
      // ptr = ptr + TransUnit_readLineComment(this);
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "/*", 2))
    {
      // Consume until */
      // ptr = ptr + TransUnit_readMultilineComment(this);
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#include", 8))
    {
      // Read file name
      // Open file name
      // Push new buffer
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#define", 7))
    {
      // Consume macro definition
      // TransUnit_readMacroDefinition(this);
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 6))
    {
      // Evaluate condition
    }
    else
    {
      this->currentBuffer->currentPtr++;
    }
  }
  // New String
  return 0;
}
