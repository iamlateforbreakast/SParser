/* TransUnit.c */
#include "TransUnit.h"
#include "List.h"
#include "Map.h"
#include "String2.h"
#include "Memory.h"
#include "Error.h"
#include "Object.h"

//#define IS_MACRO_LETTER(C) (((C=>'A') && (C<='Z')) || (C=='_'))
#define IS_MACRO_LETTER(C) (((C>'A') && (C<'Z')) || (C=='_'))

struct Buffer
{
  String * string;
  char * currentPtr;
  char* startPtr;
};

struct MacroDefinition
{
  Object object;
  String* name;
  String* body;
  List* parameters;
} MacroDefinition;

/**********************************************//**
  @class TransUnit
**************************************************/
struct TransUnit
{
  Object object;
  FileDesc * file;
  List * buffers;
  Map* macros;
  struct Buffer * currentBuffer;
  int nbCharRead;
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

PRIVATE void TransUnit_consumeLineComment(TransUnit* this);
PRIVATE void TransUnit_consumeMultilineComment(TransUnit* this);
PRIVATE void TransUnit_consumeInclude(TransUnit* this);
PRIVATE void TransUnit_readMacroDefinition(TransUnit* this);

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

  if (file == 0)
  {
    Error_new(ERROR_NORMAL, "TransUnit: File not found\n");
    return 0;
  }

  this = (TransUnit*)Object_new(sizeof(TransUnit), &transUnitClass);
  
  if (this == 0) return 0;

  this->file = file;
  this->buffers = List_new();

  struct Buffer* buffer = Memory_alloc(sizeof(struct Buffer));
  buffer->string = FileDesc_load(file);
  buffer->startPtr = String_getBuffer(buffer->string);
  buffer->currentPtr = buffer->startPtr;

  List_insertHead(this->buffers, buffer, 0);
  this->currentBuffer = buffer;
  this->nbCharRead = 0;
  this->macros = Map_new();

return this;
}

/**********************************************//**
  @brief Delete an instance of a TransUnit object.
  @public
  @memberof TransUnit
**************************************************/
PUBLIC void TransUnit_delete(TransUnit* this)
{
  if (this == 0) return;

  /* De-allocate the specific members */
  struct Buffer* buffer = 0;
  while ((buffer = List_removeTail(this->buffers)) != 0)
  {
    //String_delete(buffer->string);
    buffer->startPtr = 0;
    buffer->currentPtr = 0;
    Memory_free(buffer, sizeof(struct Buffer));
  }
  List_delete(this->buffers);
  Map_delete(this->macros);
  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

PUBLIC void TransUnit_print(TransUnit* this)
{

}

PUBLIC unsigned int TransUnit_getSize(TransUnit* this)
{
  if (this == 0) return sizeof(TransUnit);

  return sizeof(TransUnit);
}

PUBLIC char* TransUnit_getName(TransUnit* this)
{
  return String_getBuffer(FileDesc_getName(this->file));
}

PUBLIC String* TransUnit_getNextBuffer(TransUnit* this)
{
  char* ptr = this->currentBuffer->currentPtr;  //String_getBuffer(this->currentBuffer);
  int isReadyToEmit = 0;
  int start = this->currentBuffer->currentPtr;

  while ((!isReadyToEmit) && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    if (Memory_ncmp(this->currentBuffer->currentPtr, "//", 2))
    {
      // Consume until the end of line
      // ptr = ptr + TransUnit_readLineComment(this);
      TransUnit_consumeLineComment(this);
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "/*", 2))
    {
      // Consume until */
      // ptr = ptr + TransUnit_readMultilineComment(this);
      TransUnit_consumeMultilineComment(this);
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#include", 8))
    {
      // Read file name
      // Open file name
      // Push new buffer
      TransUnit_consumeInclude(this);
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#define", 7))
    {
      // Consume macro definition
      TransUnit_readMacroDefinition(this);
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 6))
    {
      // Evaluate condition
      this->currentBuffer->currentPtr += 6;
      this->nbCharRead += 6;
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifdef", 5))
    {
      this->currentBuffer->currentPtr += 5;
      this->nbCharRead += 5;
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#undef", 6))
    {
      this->currentBuffer->currentPtr += 6;
      this->nbCharRead += 6;
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#if", 2))
    {
      this->currentBuffer->currentPtr += 2;
      this->nbCharRead += 2;
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#else", 4))
    {
      this->currentBuffer->currentPtr += 4;
      this->nbCharRead += 4;
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#endif", 5))
    {
      this->currentBuffer->currentPtr+=5;
      this->nbCharRead+=5;
      start = this->currentBuffer->currentPtr;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#error", 5))
    {
      this->currentBuffer->currentPtr += 5;
      this->nbCharRead += 5;
      start = this->currentBuffer->currentPtr;
    }
    else if (0) //nothing to read
    {
      //unstack
    }
    else
    {
      this->currentBuffer->currentPtr++;
      this->nbCharRead++;
    }
  }
  // New String
  String * newString = String_subString(this->currentBuffer->string, 0, this->nbCharRead++);


  return newString;
}

PRIVATE void TransUnit_consumeLineComment(TransUnit* this)
{
  const char* eol = "\n";
  String* lineComment = 0;
  int start = this->nbCharRead;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\n", 1) && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  this->nbCharRead = this->nbCharRead;
  lineComment = String_subString(this->currentBuffer->string, start, this->nbCharRead - start);
  String_print(lineComment);
}

PRIVATE void TransUnit_consumeMultilineComment(TransUnit* this)
{
  String* multiLineComment = 0;
  int start = this->nbCharRead;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "*/", 2) && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  this->currentBuffer->currentPtr+=2;
  this->nbCharRead+=2;
  multiLineComment = String_subString(this->currentBuffer->string, start, this->nbCharRead - start);
  String_print(multiLineComment);
}

PRIVATE void TransUnit_consumeInclude(TransUnit* this)
{
  String* include = 0;
  int start = this->nbCharRead;
  int isStarted = 0;

  this->currentBuffer->currentPtr+=8;
  this->nbCharRead+=8;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\"", 1) && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  start = this->nbCharRead;

  this->currentBuffer->currentPtr++;
  this->nbCharRead++;
  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\"", 1) && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  this->currentBuffer->currentPtr++;
  this->nbCharRead++;

  include = String_subString(this->currentBuffer->string, start, this->nbCharRead - start);
  String_print(include);
}

PRIVATE void TransUnit_readMacroDefinition(TransUnit* this)
{
 
  struct MacroDefinition* macroDefinition = 0;
  String* parameter = 0;
  unsigned int paramLength = 0;

  this->currentBuffer->currentPtr += 7;
  this->nbCharRead += 7;

  /* Consume spaces */
  while ((*this->currentBuffer->currentPtr == ' ') && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  int start = this->nbCharRead;
  /* Consume macro name */
  while (IS_MACRO_LETTER(*this->currentBuffer->currentPtr))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }

  macroDefinition = Memory_alloc(sizeof(MacroDefinition));
  macroDefinition->name = String_subString(this->currentBuffer->string, start, this->nbCharRead - start);
  macroDefinition->parameters = 0;
  macroDefinition->body = 0;
  String_print(macroDefinition->name);

  /* Consume parameters if any */
  if (*this->currentBuffer->currentPtr == '(')
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
    start = this->currentBuffer->currentPtr;
    while ((*this->currentBuffer->currentPtr != ')') && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
    {
      this->currentBuffer->currentPtr++;
      this->nbCharRead++;
    }
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  
  /* Consume macro body */
  while ((*this->currentBuffer->currentPtr == ' ') && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  start = this->nbCharRead;
  while ((*this->currentBuffer->currentPtr != '\n') && (this->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->nbCharRead++;
  }
  macroDefinition->body = String_subString(this->currentBuffer->string, start, this->nbCharRead - start);
  String_print(macroDefinition->body);
}

  /* Consume macro param */
  /*if (c == '(')
  {
    c = StringProcessor_readChar(this, 1);
    c = StringProcessor_readChar(this, 0);
    while (c != ')')
    {
      while ((c != ',') && (c != ')'))
      {
        paramLength++;
        c = StringProcessor_readChar(this, 1);
        c = StringProcessor_readChar(this, 0);
      }
      if (macroDefinition->parameters == 0)
      {
        macroDefinition->parameters = List_new();
      }
      parameter = StringBuffer_readback(this->currentBuffer, paramLength);
      paramLength = 0;

      List_insert(macroDefinition->parameters, parameter);
      if (c == ',')
      {
        c = StringProcessor_readChar(this, 1);
        c = StringProcessor_readChar(this, 0);
      }
    }

    c = StringProcessor_readChar(this, 1);
  }

  if ((c != 10) && (c != 13))
  {
    c = StringProcessor_readChar(this, 0);
    while (c == 32)
    {
      c = StringProcessor_readChar(this, 1);
      c = StringProcessor_readChar(this, 0);
    }
    result = 0;

    while ((c != 10) && (c != 13))
    {
      result++;
      c = StringProcessor_readChar(this, 1);
      c = StringProcessor_readChar(this, 0);
    }
    //printf("Read define: result=%d\n", result);
    macroDefinition->body = StringBuffer_readback(this->currentBuffer, result);
    String_print(macroDefinition->body, "#define: ");
  }
  if (!Map_insert(this->macros, macroDefinition->name, (void*)macroDefinition))
  {
    String_print(macroDefinition->name, "StringProcessor.c: Could not store macro ");
  }
  if (Map_find(this->macros, macroDefinition->name, 0))
  {
    String_print(macroDefinition->name, "StringProcessor.c: Found the macro again->");
  }
}*/
