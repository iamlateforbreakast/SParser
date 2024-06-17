/**********************************************//**
  @file TransUnit.c
  @brief This file implements a class that extract C code.
  It removes the comments, 
  expands the macros, 
  parses the include files
**************************************************/
#include "TransUnit.h"
#include "MacroDefinition.h"
#include "MacroStore.h"
#include "Buffer.h"
#include "List.h"
#include "Map.h"
#include "String2.h"
#include "Memory.h"
#include "Error.h"
#include "Object.h"
#include "Debug.h"

#define DEBUG (0)
#define IS_MACRO_LETTER(C) ((((C)>='A') && ((C)<='Z')) || ((C)=='_'))

/**********************************************//**
  @class TransUnit
**************************************************/
struct TransUnit
{
  Object object;
  FileDesc* file;
  FileMgr * fm;
  List* buffers;
  Map* macros;
  MacroStore* store;
  struct Buffer* currentBuffer;
  int nbCharRead;
  char * outputBuffer;
  int outputBufferSize;
  int nbCharWritten;
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
PRIVATE int TransUnit_pushNewBuffer(TransUnit* this, String* content);
PRIVATE int TransUnit_popBuffer(TransUnit* this);

/**********************************************//**
  @brief Create a new TransUnit object.
  @public
  @memberof TransUnit
  @return Created TransUnit instance.
**************************************************/
PUBLIC TransUnit* TransUnit_new(FileDesc* file, FileMgr * fileMgr)
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
  this->fm = fileMgr;
  this->buffers = List_new();

  /* Create new buffer */
  String * content = FileDesc_load(file);
  Buffer* buffer = Buffer_new(content);

  List_insertHead(this->buffers, buffer, 0);
  this->currentBuffer = buffer;
  this->nbCharRead = 0;
  this->store = MacroStore_new();
  this->outputBufferSize = 8000;
  this->outputBuffer = Memory_alloc(this->outputBufferSize);
  this->nbCharWritten = 0;

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
  Buffer* buffer = 0;
  while ((buffer = (Buffer*)List_removeTail(this->buffers)) != 0)
  {
    Buffer_delete(buffer);
  }
  List_delete(this->buffers);
  Map_delete(this->macros);
  MacroStore_delete(this->store);
  Memory_free(this->outputBuffer, this->outputBufferSize);

  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Print a new TransUnit object.
  @public
  @memberof TransUnit
**************************************************/
PUBLIC void TransUnit_print(TransUnit* this)
{

}

/**********************************************//**
  @brief Returns the size a new TransUnit object.
  @public
  @memberof TransUnit
  @return Size in bytes.
**************************************************/
PUBLIC unsigned int TransUnit_getSize(TransUnit* this)
{
  if (this == 0) return sizeof(TransUnit);

  return sizeof(TransUnit);
}

/**********************************************//**
  @brief Returns the filename  a new TransUnit object.
  @public
  @memberof TransUnit
  @return Filename
**************************************************/
PUBLIC char* TransUnit_getName(TransUnit* this)
{
  return String_getBuffer(FileDesc_getName(this->file));
}

/**********************************************//**
  @brief Returns the buffer of  a new TransUnit object.
  @public
  @memberof TransUnit
  @return Buffer
**************************************************/
PUBLIC String* TransUnit_getNextBuffer(TransUnit* this)
{
  char* ptr = this->currentBuffer->currentPtr;  //String_getBuffer(this->currentBuffer);
  int isFinished = 0;
  int isReadingContent = 0;
  int start = this->currentBuffer->nbCharRead;

  /* Reset output buffer */
  Memory_set(this->outputBuffer, 0, this->outputBufferSize);
  this->nbCharWritten = 0;

  while (!isFinished)
  {
    while ((this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    if (Memory_ncmp(this->currentBuffer->currentPtr, "//", 2))
    {
      if (isReadingContent)
      {
        String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
        return newString;
      }
      // Consume until the end of line
      // Comment is discarded
      TransUnit_consumeLineComment(this);
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "/*", 2))
    {
      if (isReadingContent)
      {
        String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
        return newString;
      }
      // Consume until */
      // Comment is discarded
      TransUnit_consumeMultilineComment(this);
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#include", 8))
    {
      if (isReadingContent)
      {
        String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
        return newString;
      }
      // Consume include
      TransUnit_consumeInclude(this);
      start = this->currentBuffer->nbCharRead;
        TRACE(("#include: start = %d\n", start));
        //isReadyToEmit = 1;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#define", 7))
    {
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      // Consume macro definition
      TransUnit_readMacroDefinition(this);
      start = this->currentBuffer->nbCharRead;
        TRACE(("#define: start = %d\n", start));
    }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 7))
    {
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      // Evaluate condition
        this->currentBuffer->currentPtr += 7;
        this->currentBuffer->nbCharRead += 7;
      // Check macro is not defined
      TransUnit_checkMacro(this, 0);
      start = this->currentBuffer->nbCharRead;
        TRACE(("#ifndef: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifdef", 6))
    {
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      this->currentBuffer->currentPtr += 6;
      this->currentBuffer->nbCharRead += 6;
      // Check macro is defined
      TransUnit_checkMacro(this, 1);
      start = this->currentBuffer->nbCharRead;
        TRACE(("#ifdef: start = %d\n", start));
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#undef", 6))
    {
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      this->currentBuffer->currentPtr += 6;
      this->currentBuffer->nbCharRead += 6;
      start = this->currentBuffer->nbCharRead;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#if", 2))
    {
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      this->currentBuffer->currentPtr += 2;
      this->currentBuffer->nbCharRead += 2;
      start = this->currentBuffer->nbCharRead;
        TRACE(("#if: start = %d\n", start));
    }
      /*else if (Memory_ncmp(this->currentBuffer->currentPtr, "#else", 4))
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
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      this->currentBuffer->currentPtr += 5;
      this->currentBuffer->nbCharRead += 5;
      start = this->currentBuffer->nbCharRead;
      }*/
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#error", 5))
    {
        if (isReadingContent)
        {
          String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
          return newString;
        }
      this->currentBuffer->currentPtr += 5;
      this->currentBuffer->nbCharRead += 5;
      start = this->currentBuffer->nbCharRead;
    }
    else if (0) //nothing to read
    {
      //unstack
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "\n", 1))
    {
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "\t", 1))
    {
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
    }
    else
    {
      this->outputBuffer[this->nbCharWritten] = *(this->currentBuffer->currentPtr);
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
      this->nbCharWritten++;
        /* MacroStore_checkName(this->currentBuffer->currentMacroPtr, length)
        if 1 length ++
        if 0 currentMacroPtr = currentPtr, length = 1*/
      isReadingContent = 1;
    }
  }
    TRACE(("TransUnit_getNextBuffer: start=%d nbCharRead=%d\n", start, this->currentBuffer->nbCharRead));
    String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
    TRACE(("TransUnit_getNextBuffer: %s\n", String_getBuffer(newString)));
    int a = TransUnit_popBuffer(this);
    if (!a) PRINT(("Lastbuffer\n"));
    return newString;
  }
  TRACE(("TransUnit_getNextBuffer: Run out of string\n"));
  //String* newString = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  return 0;
}

/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
PRIVATE void TransUnit_consumeLineComment(TransUnit* this)
{
  const char* eol = "\n";
  
  int start = this->currentBuffer->nbCharRead;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "\n", 1) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
#if DEBUG
  String* lineComment = 0;
  lineComment = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(lineComment);
  String_delete(linComment);
#endif
}

/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
PRIVATE void TransUnit_consumeMultilineComment(TransUnit* this)
{
  int start = this->currentBuffer->nbCharRead;

  while (!Memory_ncmp(this->currentBuffer->currentPtr, "*/", 2) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  this->currentBuffer->currentPtr += 2;
  this->currentBuffer->nbCharRead += 2;
#if DEBUG
  String* multiLineComment = 0;
  multiLineComment = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  String_print(multiLineComment);
  String_delete(multiLineComment);
#endif
}

/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
PRIVATE void TransUnit_consumeInclude(TransUnit* this)
{
  String* fileName = 0;
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

  fileName = String_subString(this->currentBuffer->string, start + 1, this->currentBuffer->nbCharRead - start - 2);

  PRINT(("TransUnit_consumeInclude: %s\n", String_getBuffer(fileName)));

  /* Load the new buffer using the file manager */
  FileDesc * fileDesc = FileMgr_isManaged(this->fm, fileName);
  if (fileDesc != 0)
  {
    String * buffer = FileDesc_load(fileDesc);

    TransUnit_pushNewBuffer(this, buffer);
  }
  else
  {
    Error_new(ERROR_FATAL, "TransUnit_consumeInclude: file not found %s.\n", String_getBuffer(fileName));
  }
}

/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
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

  MacroDefinition* macroDefinition = MacroDefinition_new(0, macroBody);
  //Map_insert(this->macros, macroName, macroDefinition, 1);
  MacroStore_insertName(this->store, macroName, macroDefinition);
  String_delete(macroName);
}

/**********************************************//**
  @brief Check if a macro is defined.
  @private
  @memberof TransUnit
**************************************************/
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
  TRACE(("TransUnit_checkMacro: read macro name %s\n", String_getBuffer(macroName)));
  MacroDefinition* macro = 0;
  start = this->currentBuffer->nbCharRead;
  int nestingLevel = 0;
  int isLookingForEndif = 0;
  /* Check if macro is defined or not defined */
  //int firstBlockActive = (Map_find(this->macros, macroName, &macro) == checkForTrue);
  int firstBlockActive = (MacroStore_isDefName(this->store, macroName) == checkForTrue);
  int isFinished = 0;
  int startBlock = this->currentBuffer->nbCharRead;
  int endBlock = -1;
  int copyChar = 1;
  String_delete(macroName);
  while ((!isFinished) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 7))
    {
      nestingLevel++;
    } 
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#endif", 6))
    {
      if (nestingLevel == 0)
      {
        //String* buffer = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
        if (firstBlockActive)
        {
        isFinished = 1;
        }
        else
        {
          isFinished = 1;
          endBlock = this->currentBuffer->nbCharRead;
          copyChar = 0;
        }
        this->currentBuffer->currentPtr += 6;
        this->currentBuffer->nbCharRead += 6;
      }
      else nestingLevel--;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#else", 5))
    {
      if (nestingLevel == 0)
      {
        if (firstBlockActive)
        {
          endBlock = this->currentBuffer->nbCharRead;
          copyChar = 0;
        }
        else
        {
          startBlock = this->currentBuffer->nbCharRead + 5;
          endBlock = -1;
          copyChar = 1;
        }
        this->currentBuffer->currentPtr += 5;
        this->currentBuffer->nbCharRead += 5;
      }
    }
    else
    {
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
      if (copyChar)
      {
        this->outputBuffer[this->nbCharWritten] = *(this->currentBuffer->currentPtr);
        this->nbCharWritten++;
      }
    }
  }

  String* buffer = String_subString(this->currentBuffer->string, startBlock, endBlock - startBlock);
  TransUnit_pushNewBuffer(this, buffer);

  //TRACE(("TransUnit_checkMacro: BUffer extracted %s\n", String_getBuffer(buffer)));
  PRINT(("TransUnit_checkMacro: outbuffer %s\n", this->outputBuffer));
}
/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
PRIVATE int TransUnit_pushNewBuffer(TransUnit* this, String * content)
{
  Buffer* buffer = Buffer_new(content);
  List_insertHead(this->buffers, buffer, 0);
  this->currentBuffer = buffer;
  TRACE(("TransuNit_pushNewBuffer: Buffer %d\n", List_getNbNodes(this->buffers)));
  return 0;
}
/**********************************************//**
  @brief Unstack a buffer to parse.
  @private
  @memberof TransUnit
  @return 0 if no buffer left, 1 if still at least a buffer.
**************************************************/
PRIVATE int TransUnit_popBuffer(TransUnit* this)
{
  if (List_getNbNodes(this->buffers) == 0) return 0;
  Buffer* buffer = (Buffer*)List_removeHead(this->buffers);
  Buffer_delete(buffer);
  buffer = (Buffer*)List_getHead(this->buffers);
  this->currentBuffer  = buffer;
  TRACE(("TransUnit_popBuffer: Buffer %d\n", List_getNbNodes(this->buffers)));
  return 1;
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
