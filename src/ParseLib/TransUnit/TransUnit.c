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

#define IS_MACRO_LETTER(C) ((((C)>='A') && ((C)<='Z')) || (((C)>='a') && ((C)<='z')) || (((C)>='0') && ((C)<='9')) || ((C)=='_'))
/* Size of the output buffer in bytes */
#define OUTPUT_BUFFER_SIZE (20000)

/**********************************************//**
  @class TransUnit
**************************************************/
struct TransUnit
{
  Object object;
  FileDesc* file;
  FileMgr* fm;
  List* buffers;
  MacroStore* store;
  Buffer* currentBuffer;
  int nbCharRead;
  char* outputBuffer;
  int outputBufferSize;
  int nbCharWritten;
  TransUnitState state;
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
PRIVATE void TransUnit_consumeString(TransUnit* this);
PRIVATE void TransUnit_readMacroDefinition(TransUnit* this);
PRIVATE void TransUnit_checkMacro(TransUnit* this, int checkForTrue);
PRIVATE int TransUnit_pushNewBuffer(TransUnit* this, String* content);
PRIVATE int TransUnit_popBuffer(TransUnit* this);
PRIVATE int TransUnit_expandMacro(TransUnit* this);
PRIVATE String* TransUnit_expandString(String* s, MacroStore* localMacroStore);

/**********************************************//**
  @brief Create a new TransUnit object.
  @public
  @memberof TransUnit
  @return Created TransUnit instance.
**************************************************/
PUBLIC TransUnit* TransUnit_new(FileDesc* file, FileMgr* fileMgr)
{
  TransUnit* this = 0;
  String* newFileContent = 0;

  if (file == 0)
  {
    Error_new(ERROR_NORMAL, "TransUnit: File not found\n");
    return 0;
  }

  TRACE(("TransUnit_new: Processing %s\n", String_getBuffer(FileDesc_getFullName(file))));
  this = (TransUnit*)Object_new(sizeof(TransUnit), &transUnitClass);

  /* TODO: Use INVALID check */
  if (this == 0) return 0;

  this->file = file;
  this->fm = fileMgr;

  this->buffers = List_new();

  /* Create new buffer */
  String* content = FileDesc_load(file);
  Buffer* buffer = Buffer_new(content);

  List_insertHead(this->buffers, buffer, 0);
  this->currentBuffer = buffer;

  this->nbCharRead = 0;
  this->store = MacroStore_new();
  this->outputBufferSize = 0;
  this->outputBuffer = 0;
  this->nbCharWritten = 0;
  this->state = NOT_STARTED;

  return this;
}

/**********************************************//**
  @brief Delete an instance of a TransUnit object.
  @public
  @memberof TransUnit
**************************************************/
PUBLIC void TransUnit_delete(TransUnit* this)
{
  /* TODO: Use INVALID check */
  if (this == 0) return;

  /* De-allocate the specific members */
  Buffer* buffer = 0;
  while ((buffer = (Buffer*)List_removeTail(this->buffers)) != 0)
  {
    Buffer_delete(buffer);
  }
  List_delete(this->buffers);
  MacroStore_delete(this->store);

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
  if (this == 0) return 0;

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
  if (this->state==COMPLETED) return 0;

  char* ptr = this->currentBuffer->currentPtr;  //String_getBuffer(this->currentBuffer);
  int isReadingContent = 0;
  //int start = this->currentBuffer->nbCharRead;

  /* Reset output buffer */
  this->outputBufferSize = OUTPUT_BUFFER_SIZE;
  this->outputBuffer = Memory_alloc(this->outputBufferSize);
  Memory_set(this->outputBuffer, 0, this->outputBufferSize);
  this->nbCharWritten = 0;

  while (1)
  {
    while ((this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
    {
      if (Memory_ncmp(this->currentBuffer->currentPtr, "//", 2))
      {
        TransUnit_consumeLineComment(this);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "/*", 2))
      {
        TransUnit_consumeMultilineComment(this);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#include", 8))
      {
        TransUnit_consumeInclude(this);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#define", 7))
      {
        TransUnit_readMacroDefinition(this);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 7))
      {
        this->currentBuffer->currentPtr += 7;
        this->currentBuffer->nbCharRead += 7;
        // Check macro is not defined
        TransUnit_checkMacro(this, 0);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifdef", 6))
      {
        this->currentBuffer->currentPtr += 6;
        this->currentBuffer->nbCharRead += 6;
        // Check macro is defined
        TransUnit_checkMacro(this, 1);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#undef", 6))
      {
        this->currentBuffer->currentPtr += 6;
        this->currentBuffer->nbCharRead += 6;
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#if", 2))
      {
        this->currentBuffer->currentPtr += 2;
        this->currentBuffer->nbCharRead += 2;
        //start = this->currentBuffer->nbCharRead;
      }
      /*else if (Memory_ncmp(this->currentBuffer->currentPtr, "#else", 4))
      {
        Error
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#endif", 5))
      {
        Error
      }*/
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "#error", 5))
      {
        this->currentBuffer->currentPtr += 5;
        this->currentBuffer->nbCharRead += 5;
        //start = this->currentBuffer->nbCharRead;
      }
      else if (0) //nothing to read
      {
        //unstack
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "\"", 1))
      {
        TransUnit_consumeString(this);
        //start = this->currentBuffer->nbCharRead;
      }
      else if (Memory_ncmp(this->currentBuffer->currentPtr, "\r\n", 2))
      {
        this->currentBuffer->currentPtr += 2;
        this->currentBuffer->nbCharRead += 2;
        /* TODO: Update line being processed in Buffer */
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
      else if (!TransUnit_expandMacro(this))
      {
        this->outputBuffer[this->nbCharWritten] = *(this->currentBuffer->currentPtr);
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
        this->nbCharWritten++; // Need to check max value

        if (this->nbCharWritten >= OUTPUT_BUFFER_SIZE) Error_new(ERROR_FATAL, "Output BUffer is overflowing.\n");


      }
    }
    int isStackNotEmpty = TransUnit_popBuffer(this);

    if (!isStackNotEmpty)
    {
      TRACE(("TransUnit.c: Lastbuffer was processed.\n"));
      TRACE(("TransUnit.c: Total number of chr written: %d\n", this->nbCharWritten));
      String * s = String_new(0);
      String_setBuffer(s, this->outputBuffer, 1);
      this->outputBuffer = 0;
      this->state = COMPLETED;
      return s;
    }
  }

  TRACE(("TransUnit_getNextBuffer: Run out of string\n"));

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
  String_delete(lineComment);
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

  while ((*this->currentBuffer->currentPtr != '\"') &&
         (*this->currentBuffer->currentPtr != '<') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  start = this->currentBuffer->nbCharRead;

  this->currentBuffer->currentPtr++;
  this->currentBuffer->nbCharRead++;
  while ((*this->currentBuffer->currentPtr != '\"') &&
         (*this->currentBuffer->currentPtr != '>') && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
  }
  this->currentBuffer->currentPtr++;
  this->currentBuffer->nbCharRead++;

  fileName = String_subString(this->currentBuffer->string, start + 1, this->currentBuffer->nbCharRead - start - 2);

  PRINT(("TransUnit_consumeInclude: %s\n", String_getBuffer(fileName)));

  /* Load the new buffer using the file manager */
  FileDesc* fileDesc = FileMgr_isManaged(this->fm, fileName);
  
  if (fileDesc != 0)
  {
    String* buffer = FileDesc_load(fileDesc);

    TransUnit_pushNewBuffer(this, buffer);
  }
  else
  {
    Error_new(ERROR_NORMAL, "TransUnit_consumeInclude: file not found %s.\n", String_getBuffer(fileName));
  }
  String_delete(fileName);
} 

/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
PRIVATE void TransUnit_consumeString(TransUnit* this)
{
  /* Read the first " and copy to output buffer */
  this->outputBuffer[this->nbCharWritten] = *(this->currentBuffer->currentPtr);
  this->currentBuffer->currentPtr ++;
  this->currentBuffer->nbCharRead ++;
  this->nbCharWritten++;

  /* Need to check for EOL which causes an error */
  while ((*this->currentBuffer->currentPtr != '\"') &&
         (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    this->outputBuffer[this->nbCharWritten] = *(this->currentBuffer->currentPtr);
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
    this->nbCharWritten++;
  }

  /* Read the last " and copy to output buffer */
  this->outputBuffer[this->nbCharWritten] = *(this->currentBuffer->currentPtr);
  this->currentBuffer->currentPtr++;
  this->currentBuffer->nbCharRead++;
  this->nbCharWritten++;
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
  List* parameters = 0;

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


  String* macroName = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  //String_print(macroName);

  /* Consume parameters if any */
  if (*this->currentBuffer->currentPtr == '(')
  {
    this->currentBuffer->currentPtr++;
    this->currentBuffer->nbCharRead++;
    
    while (*this->currentBuffer->currentPtr != ')')
    {
      start = this->currentBuffer->nbCharRead;
      paramLength = 0;
      while ((*this->currentBuffer->currentPtr != ',') && (*this->currentBuffer->currentPtr != ')'))
      {
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
        paramLength++;
      }
      if (parameters == 0)
      {
        parameters = List_new();
      }

      parameter = String_subString(this->currentBuffer->string, start, paramLength); // StringBuffer_readback(this->currentBuffer, paramLength);
      TRACE(("Macro argument %s\n", String_getBuffer(parameter)));

      paramLength = 0;

      List_insertHead(parameters, parameter, 1);

      if (*this->currentBuffer->currentPtr == ',')
      {
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
      }
    }

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
  String* macroBody = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  //String_print(macroBody);

  MacroDefinition* macroDefinition = MacroDefinition_new(parameters, macroBody);
  //Map_insert(this->macros, macroName, macroDefinition, 1);
  MacroStore_insertName(this->store, macroName, macroDefinition);

  TRACE(("Defined macro: %s\n", String_getBuffer(macroName)));
  TRACE(("Nb param: %d\n", List_getNbNodes(macroDefinition->parameters)));
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
 
  String* macroName = String_subString(this->currentBuffer->string, start, this->currentBuffer->nbCharRead - start);
  //TRACE(("TransUnit_checkMacro: read macro name %s\n", String_getBuffer(macroName)));
  MacroDefinition* macro = 0;
  start = this->currentBuffer->nbCharRead;
  int nestingLevel = 0;
  int isLookingForEndif = 0;

  /* Check if macro is defined or not defined */
  //int firstBlockActive = (Map_find(this->macros, macroName, &macro) == checkForTrue);
  int firstBlockActive = (MacroStore_isDefName(this->store, macroName) == checkForTrue);
  int isFinished = 0;
  int startBlock = -1;  
  int endBlock = -1;
  String_delete(macroName);

  if (firstBlockActive) startBlock = this->currentBuffer->nbCharRead;

  while ((!isFinished) && (this->currentBuffer->nbCharRead < (int)String_getLength(this->currentBuffer->string)))
  {
    if (Memory_ncmp(this->currentBuffer->currentPtr, "#ifndef", 7))
    {
      nestingLevel++;
      this->currentBuffer->currentPtr += 7;
      this->currentBuffer->nbCharRead += 7;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#endif", 6))
    {
      if (nestingLevel == 0)
      {
        if (firstBlockActive)
        {
          if (endBlock == -1) // There is no #else
          {
            endBlock = this->currentBuffer->nbCharRead;
          }
        }
        else
        {
          if (startBlock != -1) // There was an else
          {
            endBlock = this->currentBuffer->nbCharRead;
          }
        }
        isFinished = 1;
      }
      else
      {
        nestingLevel--;
      }
      this->currentBuffer->currentPtr += 6;
      this->currentBuffer->nbCharRead += 6;
    }
    else if (Memory_ncmp(this->currentBuffer->currentPtr, "#else", 5))
    {
      if (nestingLevel == 0)
      {
        if (firstBlockActive)
        {
          endBlock = this->currentBuffer->nbCharRead;
        }
        else
        {
          startBlock = this->currentBuffer->nbCharRead + 5;
          endBlock = -1;
        }

      }
      this->currentBuffer->currentPtr += 5;
      this->currentBuffer->nbCharRead += 5;
    }
    else
    {
      this->currentBuffer->currentPtr++;
      this->currentBuffer->nbCharRead++;
    }
  }

  if (endBlock != -1) // Need to stack a buffer
  {
    String* buffer = String_subString(this->currentBuffer->string, startBlock, endBlock - startBlock);
    TransUnit_pushNewBuffer(this, buffer);
  }
  //TRACE(("TransUnit_checkMacro: BUffer extracted %s\n", String_getBuffer(buffer)));
  //PRINT(("TransUnit_checkMacro: outbuffer %s\n", this->outputBuffer));
}

/**********************************************//**
  @brief TBC.
  @private
  @memberof TransUnit
**************************************************/
PRIVATE int TransUnit_pushNewBuffer(TransUnit* this, String* content)
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
  TRACE(("TransUnit_popBuffer: Buffer %d\n", List_getNbNodes(this->buffers)));

  if (List_getNbNodes(this->buffers) == 0) return 0;

  Buffer* buffer = (Buffer*)List_removeHead(this->buffers);
  Buffer_delete(buffer);

  buffer = (Buffer*)List_getHead(this->buffers);
  this->currentBuffer = buffer;

  return List_getNbNodes(this->buffers);
}

/**********************************************//**
  @brief Attempts to expand a macro at current pointer position
  @private
  @memberof TransUnit
  @return 0 if not possible to expand macro.
**************************************************/
PRIVATE int TransUnit_expandMacro(TransUnit* this)
{
  int length = 1;
  MacroDefinition *macroDefinition;
  enum MacroEvalName status;

  status = MacroStore_evalName(this->store, this->currentBuffer->currentPtr, length, &macroDefinition);
  while (status ==  E_POSSIBLE_MACRO)
  {
    length++;
    status = MacroStore_evalName(this->store, this->currentBuffer->currentPtr, length, &macroDefinition);
  }

  if (status == E_NOT_MACRO)
  {
    return 0;
  }

  /* Consume macro name */
  this->currentBuffer->currentPtr+=length;
  this->currentBuffer->nbCharRead+=length;

  /* Expansion buffer */
  char* macroExpansionBuffer = Memory_alloc(4096);
  Memory_set(macroExpansionBuffer, 0, 4096);

  int bracketCount = 0;
  int isArgParseComplete = 0;
  int start = 0;
  List * argNames = macroDefinition->parameters;
  String* argValue;
  MacroStore* localMacroStore = 0;
  String* inStr = String_newByRef(this->currentBuffer->currentPtr);

  List_resetIterator(macroDefinition->parameters);

  if (*this->currentBuffer->currentPtr == '(')
  {
    /* Parse arguments and create a local macro Store */
    localMacroStore = MacroStore_new();
    while (!isArgParseComplete) // while E_POSSIBLE_MACRO
    {
      char nextChar = *this->currentBuffer->currentPtr;
      if (nextChar == '(')
      {
        if (bracketCount == 0)
        {
          start = 1;
          length = 0;
          bracketCount = 1;
        }
        else
        {
          bracketCount++;
          length++;
        }
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
      }
      else if ((nextChar == ',')&&(bracketCount==0))
      {
        argValue = String_subString(inStr, start, length);
        String_print(argValue);
        /* Insert argument in macroStore */
        MacroDefinition * m = MacroDefinition_new(0,argValue);
        String * arg = (String*)List_getNext(argNames);
        MacroStore_insertName(localMacroStore, arg, m);
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
        start += (length + 1);
        length = 0;
      }
      else if (nextChar == ')')
      {
        if (bracketCount == 1)
        {
          argValue = String_subString(inStr, start, length);
          String_print(argValue);
          /* Insert argument in macroStore */
          MacroDefinition* m = MacroDefinition_new(0, argValue);
          String* arg = (String*)List_getNext(argNames);
          MacroStore_insertName(localMacroStore, arg, m);
          isArgParseComplete = 1;
        }
        else bracketCount--;
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
        length++;
      }
      else
      {
        this->currentBuffer->currentPtr++;
        this->currentBuffer->nbCharRead++;
        length++;
      }
    }
    String_delete(inStr);
  }

  TRACE(("Macro expansion: %s\n", String_getBuffer(macroDefinition->body)));
  

  if (macroDefinition->parameters)
  {
    TRACE(("Macro parameters: %d\n", List_getNbNodes(macroDefinition->parameters)));
    String* expandedString = TransUnit_expandString(macroDefinition->body, localMacroStore);
    TransUnit_pushNewBuffer(this, expandedString);
    MacroStore_delete(localMacroStore);
    Memory_free(macroExpansionBuffer, 4096);
  }
  else
  {
    Memory_copy(macroExpansionBuffer,
      String_getBuffer(macroDefinition->body),
      String_getLength(macroDefinition->body));
    String* macroExpansion = String_new(macroExpansionBuffer);
    TransUnit_pushNewBuffer(this, macroExpansion);
  }
  //Memory_free(macroExpansionBuffer, 4096);
  return 1;
}

PRIVATE String * TransUnit_expandString(String* s, MacroStore* localMacroStore)
{
  String* result = 0;
  int bufferSize = 2048;
  char* expandBuffer = Memory_alloc(bufferSize);
  char* readPtr = String_getBuffer(s);
  char* writePtr = expandBuffer;
  int length = 1;
  MacroDefinition* body = 0;
  enum MacroEvalName status = 0;
  int stringify = 0;
  int concat = 0;

  PRINT(("TransUnit_expandString:\n"));
  PRINT(("  Input buffer: %s\n", readPtr));
  Memory_set(expandBuffer, 0, bufferSize);
  while (readPtr < String_getBuffer(s) + String_getLength(s))
  {
    /* Check if we need to stringigy or concatenate */
    if (*readPtr == '#')
      if (*(readPtr + 1) == '#')
      {
        concat = 1;
        readPtr += 2;
      }
      else
      {
        stringify = 1;
        readPtr++;
        *writePtr = '"';
        writePtr++;
      }

    status = MacroStore_evalName(localMacroStore, readPtr, length, &body);
    while (status == E_POSSIBLE_MACRO)
    {
      length++;
      status = MacroStore_evalName(localMacroStore, readPtr, length, &body);
    }

    if (status == E_NOT_MACRO)
    {
      Memory_copy(writePtr, readPtr, length);
      readPtr++;
      writePtr += length;
    }
    else if (status == E_DEFINED_MACRO)
    {
      PRINT((" Argument: %s\n", String_getBuffer(body->body)));
      Memory_copy(writePtr, String_getBuffer(body->body), String_getLength(body->body));
      readPtr += length;
      writePtr += String_getLength(body->body);
      if (stringify)
      {
        *writePtr = '"';
        writePtr++;
        stringify = 0;
      }
    }
    else
    {
      /* Error */
    }
  }

  PRINT(("Expanded Buffer %s\n:", expandBuffer));

  result = String_new(0);
  String_setBuffer(result, expandBuffer, 1);
  return result;
}
