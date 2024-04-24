/* TransUnit.c */
#include "TransUnit.h"
#include "MacroDefinition.h"
#include "List.h"
#include "Map.h"
#include "String2.h"
#include "Memory.h"
#include "Error.h"
#include "Object.h"
#include "Debug.h"
//#define IS_MACRO_LETTER(C) (((C=>'A') && (C<='Z')) || (C=='_'))
#define IS_MACRO_LETTER(C) (((C>'A') && (C<'Z')) || (C=='_'))

struct Buffer
{
  String* string;
  char* currentPtr;
  char* startPtr;
  int nbCharRead;
};

/**********************************************//**
  @class TransUnit
**************************************************/
struct TransUnit
{
  Object object;
  FileDesc* file;
  List* buffers;
  Map* macros;
  struct Buffer* currentBuffer;
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
PRIVATE void TransUnit_checkMacro(TransUnit* this, int checkForTrue);
PRIVATE void TransUnit_pushNewBuffer(TransUnit* this, String* content);

/**********************************************//**
  @brief Create a new TransUnit object.
  @public
  @memberof TransUnit
  @return Created TransUnit instance.
**************************************************/
PUBLIC TransUnit* TransUnit_new(FileDesc* file)
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
  buffer->nbCharRead = 0;

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
  int isReadingContent = 0;
  int start = this->currentBuffer->nbCharRead;

  while ((!isReadyToEmit) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    if (Memory_ncmp(this->currentBuffer->currentPtr, "//", 2))
    {
      // Consume until the end of line
      // Comment is discarded
      TransUnit_consumeLineComment(this);
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "/*", 2))
    {
      // Consume until */
      // Comment is discarded
      TransUnit_consumeMultilineComment(this);
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#include", 8))
    {
      // Consume include
      TransUnit_consumeInclude(this);
      start = this->currentBuffer->nbCharRead;
      PRINT(("#include: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#define", 7))
    {
      // Consume macro definition
      TransUnit_readMacroDefinition(this);
      start = this->currentBuffer->nbCharRead;
      PRINT(("#define: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 6))
    {
      // Evaluate condition
      this->currentBuffer->currentPtr += 6;
      this->currentBuffer->nbCharRead += 6;
      TransUnit_checkMacro(this, 0);
      start = this->currentBuffer->nbCharRead;
      PRINT(("#ifndef: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifdef", 6))
    {
      this->currentBuffer->currentPtr += 6;
      this->currentBuffer->nbCharRead += 6;
      TransUnit_checkMacro(this, 1);
      start = this->currentBuffer->nbCharRead;
      PRINT(("#ifdef: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#undef", 6))
    {
      this->currentBuffer->currentPtr += 6;
      this->currentBuffer->nbCharRead += 6;
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#if", 2))
    {
      this->currentBuffer->currentPtr += 2;
      this->currentBuffer->nbCharRead += 2;
      start = this->currentBuffer->nbCharRead;
      PRINT(("#if: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#else", 4))
    {
      if (isReadingContent)
      {
        String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);

        return newString;
      }
      this->currentBuffer->currentPtr += 4;
      this->currentBuffer->nbCharRead += 4;
      start = this->currentBuffer->nbCharRead;
      PRINT(("#else: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#endif", 5))
    {
      this->currentBuffer->currentPtr += 5;
      this->currentBuffer->nbCharRead += 5;
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#error", 5))
    {
      this->currentBuffer->currentPtr += 5;
      this->currentBuffer->nbCharRead += 5;
      start = this->currentBuffer->nbCharRead;
    }
    else if (0) //nothing to read
    {
      //unstack
    }
    else
    {
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
      isReadingContent = 1;
    }
  }
  // New String
  String* newString = String_subString(this->currentBuffer->string, 0, this->currentBuffer->nbCharRead++);


  return newString;
}

PRIVATE void TransUnit_consumeLineComment(TransUnit* this)
{
  const char* eol = "\n";
  String* lineComment = 0;
  int start = this->currentBuffer->nbCharRead;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\n", 1) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  //this->nbCharRead = this->nbCharRead;
  lineComment = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(lineComment);
}

PRIVATE void TransUnit_consumeMultilineComment(TransUnit* this)
{
  String* multiLineComment = 0;
  int start = this->currentBuffer->nbCharRead;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "*/", 2) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  this->currentBuffer->currentPtr += 2;
  this->currentBuffer->nbCharRead += 2;
  multiLineComment = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(multiLineComment);
}

PRIVATE void TransUnit_consumeInclude(TransUnit* this)
{
  String* include = 0;
  int start = this->currentBuffer->nbCharRead;
  int isStarted = 0;

  this->currentBuffer->currentPtr += 8;
  this->currentBuffer->nbCharRead += 8;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\"", 1) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  start = this->currentBuffer->nbCharRead;

  this->currentBuffer->currentPtr++;
  this->currentBuffer->nbCharRead++;
  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\"", 1) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  this->currentBuffer->currentPtr++;
  this->currentBuffer->nbCharRead++;

  include = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(include);
}

PRIVATE void TransUnit_readMacroDefinition(TransUnit* this)
{

  String* parameter = 0;
  unsigned int paramLength = 0;

  this->currentBuffer->currentPtr += 7;
  this->currentBuffer->nbCharRead += 7;

  /* Consume spaces */
  while ((*this->currentBuffer->currentPtr == ' ') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  int start = this->currentBuffer->nbCharRead;
  /* Consume macro name */
  while (IS_MACRO_LETTER(*this->currentBuffer->currentPtr))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }

  
  String * macroName = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(macroName);

  /* Consume parameters if any */
  if (*this->currentBuffer->currentPtr == '(')
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
    start = this->currentBuffer->nbCharRead;
    while ((*this->currentBuffer->currentPtr != ')') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
    {
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
    }
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }

  /* Consume macro body */
  while ((*this->currentBuffer->currentPtr == ' ') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  start = this->currentBuffer->nbCharRead;
  while ((*this->currentBuffer->currentPtr != '\n') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  String * macroBody = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(macroBody);

  MacroDefinition* macroDefinition = MacroDefinition_new();
  Map_insert(this->macros, macroName, macroDefinition, 1);
}

PRIVATE void TransUnit_checkMacro(TransUnit* this, int checkForTrue)
{
  /* Consume spaces */
  while ((*this->currentBuffer->currentPtr == ' ') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  int start = this->currentBuffer->nbCharRead;
  /* Consume macro name */
  while (IS_MACRO_LETTER(*this->currentBuffer->currentPtr))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  String * macroName = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  //if (Map_get(this->macros, macroName))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
}
PRIVATE void TransUnit_pushNewBuffer(TransUnit* this, String * content)
{
  struct Buffer* buffer = Memory_alloc(sizeof(struct Buffer));
  buffer->string = content;
  buffer->startPtr = String_getBuffer(buffer->string);
  buffer->currentPtr = buffer->startPtr;
  buffer->nbCharRead = 0;
  List_insertHead(this->buffers, buffer, 0);
  this->currentBuffer = buffer;
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
