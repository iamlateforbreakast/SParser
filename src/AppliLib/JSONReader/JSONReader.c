/* JSONReader.c */
#include "JSONReader.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#define BUFFER_SIZE (512) /* In bytes */

#define IS_JSON_KEY_CHAR(C) ((((C)>='A') && ((C)<='Z')) || (((C)>='a') && ((C)<='z')) \
|| ((C)=='_') || ((C)=='-'))

#define IS_DIGIT(C) (((C)>='0') && ((C)<='9'))

#define IS_WHITESPACE(C) (((C)==' ') || ((C)=='\t') || ((C)=='\n') || ((C)=='\r'))

/**********************************************//**
  @class JsonReader
**************************************************/
struct JsonReader
{
  Object object;
  char * content;
  char * readPtr;
  int nbCharRead;
  int length;
  int line;
  int col;
  int contentUse;
  int isError;
  JsonNode node;
  int depth;  /* Track nesting depth */
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class jsonReaderClass = 
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&JsonReader_delete,
  .f_copy = (Copy_Operator)&JsonReader_copy,
  .f_comp = (Comp_Operator)&JsonReader_compare,
  .f_print = (Printer)&JsonReader_print,
  .f_size = (Sizer)&JsonReader_getSize
};

/**********************************************//**
  Private method declarations
**************************************************/
PRIVATE int JsonReader_consumeWhitespace(JsonReader* this);
PRIVATE int JsonReader_consumeString(JsonReader* this);
PRIVATE int JsonReader_consumeNumber(JsonReader* this);
PRIVATE int JsonReader_consumeBoolean(JsonReader* this);
PRIVATE int JsonReader_consumeNull(JsonReader* this);
PRIVATE int JsonReader_consumeOneChar(JsonReader* this);

/**********************************************//** 
  @brief Create a new instance of the class JsonReader.
  @public
  @memberof JsonReader
  @return New instance.
**************************************************/
PUBLIC JsonReader* JsonReader_new(String* string)
{
  JsonReader* this = 0;

  if (OBJECT_IS_INVALID(string)) return 0;

  this = (JsonReader*)Object_new(sizeof(JsonReader), &jsonReaderClass);

  if (OBJECT_IS_INVALID(this)) return 0;
  
  this->readPtr = String_getBuffer(string);
  this->length = String_getLength(string);
  this->nbCharRead = 0;
  this->node = JSONNONE;
  this->content = (char*)Memory_alloc(BUFFER_SIZE);
  this->contentUse = 0;
  this->line = 1;
  this->col = 1;
  this->isError = 0;
  this->depth = 0;

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class JsonReader.
  @public
  @memberof JsonReader
**************************************************/
PUBLIC void JsonReader_delete(JsonReader* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->readPtr = 0;
  this->node = JSONNONE;

  Memory_free(this->content, BUFFER_SIZE);
  
  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

/**********************************************//** 
  @brief Copy an instance of the class JsonReader.
  @public
  @memberof JsonReader
  @return Copy of instance
**************************************************/
PUBLIC JsonReader* JsonReader_copy(JsonReader* this)
{
  return 0;
}

/**********************************************//** 
  @brief Compare an instance of the class JsonReader.
  @public
  @memberof JsonReader
  @return always 0
**************************************************/
PUBLIC int JsonReader_compare(JsonReader* this, JsonReader* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Print an instance of the class JsonReader.
  @public
  @memberof JsonReader
**************************************************/
PUBLIC void JsonReader_print(JsonReader* this)
{
  /* Implementation optional */
}

/**********************************************//** 
  @brief Provide the size of the class or an instance
  @public
  @memberof JsonReader
  @return Size in byte
**************************************************/
PUBLIC unsigned int JsonReader_getSize(JsonReader* this)
{
  return sizeof(JsonReader);
}

/**********************************************//** 
  @brief Get line number
  @public
  @memberof JsonReader
**************************************************/
PUBLIC int JsonReader_getLine(JsonReader* this)
{
  if (OBJECT_IS_INVALID(this)) return -1;
  return this->line;
}

/**********************************************//** 
  @brief Get column number
  @public
  @memberof JsonReader
**************************************************/
PUBLIC int JsonReader_getColumn(JsonReader* this)
{
  if (OBJECT_IS_INVALID(this)) return -1;
  return this->col;
}

/**********************************************//** 
  @brief Check if error occurred
  @public
  @memberof JsonReader
**************************************************/
PUBLIC int JsonReader_isError(JsonReader* this)
{
  if (OBJECT_IS_INVALID(this)) return 1;
  return this->isError;
}

/**********************************************//** 
  @brief Read the next JSON node
  @public
  @memberof JsonReader
  @return JsonNode type
**************************************************/
PUBLIC JsonNode JsonReader_read(JsonReader * this)
{
  this->isError = 0;
  this->node = JSONNONE;
  this->contentUse = 0;

  /* Skip whitespace first */
  JsonReader_consumeWhitespace(this);

  /* Check if we've reached end of stream */
  if (this->nbCharRead >= this->length)
  {
    return JSONNONE;
  }

  char current = *this->readPtr;

  /* Parse JSON tokens */
  if (current == '{')
  {
    JsonReader_consumeOneChar(this);
    this->depth++;
    this->node = JSONOBJECT_START;
  }
  else if (current == '}')
  {
    JsonReader_consumeOneChar(this);
    this->depth--;
    this->node = JSONOBJECT_END;
  }
  else if (current == '[')
  {
    JsonReader_consumeOneChar(this);
    this->depth++;
    this->node = JSONARRAY_START;
  }
  else if (current == ']')
  {
    JsonReader_consumeOneChar(this);
    this->depth--;
    this->node = JSONARRAY_END;
  }
  else if (current == ':')
  {
    JsonReader_consumeOneChar(this);
    this->node = JSONCOLON;
  }
  else if (current == ',')
  {
    JsonReader_consumeOneChar(this);
    this->node = JSONCOMMA;
  }
  else if (current == '"')
  {
    /* String value - could be key or value */
    this->isError = JsonReader_consumeString(this);
    if (!this->isError)
    {
      /* Peek ahead to determine if this is a key or value */
      JsonReader_consumeWhitespace(this);
      if (this->nbCharRead < this->length && *this->readPtr == ':')
      {
        this->node = JSONKEY;
      }
      else
      {
        this->node = JSONSTRING_VALUE;
      }
    }
  }
  else if (current == 't' || current == 'f')
  {
    this->isError = JsonReader_consumeBoolean(this);
    if (!this->isError)
    {
      this->node = JSONBOOLEAN_VALUE;
    }
  }
  else if (current == 'n')
  {
    this->isError = JsonReader_consumeNull(this);
    if (!this->isError)
    {
      this->node = JSONNULL_VALUE;
    }
  }
  else if (current == '-' || IS_DIGIT(current))
  {
    this->isError = JsonReader_consumeNumber(this);
    if (!this->isError)
    {
      this->node = JSONNUMBER_VALUE;
    }
  }
  else
  {
    /* Unexpected character */
    this->isError = 1;
    this->node = JSONNONE;
  }

  return this->node;
}

/**********************************************//** 
  @brief Provide the content of the current node.
  @public
  @memberof JsonReader
  @return String with content
**************************************************/
PUBLIC String* JsonReader_getContent(JsonReader* this)
{
  String* content = String_new(this->content);
  
  this->content[0] = 0;
  this->contentUse = 0;

  return content;
}

/**********************************************//** 
  @brief Consume whitespace characters
  @private
**************************************************/
PRIVATE int JsonReader_consumeWhitespace(JsonReader* this)
{
  while (this->nbCharRead < this->length && IS_WHITESPACE(*this->readPtr))
  {
    JsonReader_consumeOneChar(this);
  }
  return 0;
}

/**********************************************//** 
  @brief Consume a quoted string
  @private
**************************************************/
PRIVATE int JsonReader_consumeString(JsonReader* this)
{
  /* Skip opening quote */
  if (*this->readPtr != '"') return 1;
  JsonReader_consumeOneChar(this);

  /* Read string content */
  while (this->nbCharRead < this->length)
  {
    if (*this->readPtr == '"')
    {
      /* End of string */
      this->content[this->contentUse] = 0;
      JsonReader_consumeOneChar(this);
      return 0;
    }
    else if (*this->readPtr == '\\')
    {
      /* Escape sequence */
      JsonReader_consumeOneChar(this);
      if (this->nbCharRead < this->length)
      {
        if (this->contentUse < BUFFER_SIZE - 1)
        {
          this->content[this->contentUse++] = *this->readPtr;
        }
        JsonReader_consumeOneChar(this);
      }
      else
      {
        return 1; /* Error: unterminated escape sequence */
      }
    }
    else
    {
      if (this->contentUse < BUFFER_SIZE - 1)
      {
        this->content[this->contentUse++] = *this->readPtr;
      }
      else
      {
        return 1; /* Error: string too long */
      }
      JsonReader_consumeOneChar(this);
    }
  }

  return 1; /* Error: unterminated string */
}

/**********************************************//** 
  @brief Consume a number (integer or float)
  @private
**************************************************/
PRIVATE int JsonReader_consumeNumber(JsonReader* this)
{
  /* Optional minus sign */
  if (*this->readPtr == '-')
  {
    if (this->contentUse < BUFFER_SIZE - 1)
    {
      this->content[this->contentUse++] = *this->readPtr;
    }
    JsonReader_consumeOneChar(this);
  }

  /* Integer part */
  if (this->nbCharRead >= this->length || !IS_DIGIT(*this->readPtr))
  {
    return 1; /* Error: invalid number format */
  }

  while (this->nbCharRead < this->length && IS_DIGIT(*this->readPtr))
  {
    if (this->contentUse < BUFFER_SIZE - 1)
    {
      this->content[this->contentUse++] = *this->readPtr;
    }
    JsonReader_consumeOneChar(this);
  }

  /* Decimal part (optional) */
  if (this->nbCharRead < this->length && *this->readPtr == '.')
  {
    if (this->contentUse < BUFFER_SIZE - 1)
    {
      this->content[this->contentUse++] = *this->readPtr;
    }
    JsonReader_consumeOneChar(this);

    if (this->nbCharRead >= this->length || !IS_DIGIT(*this->readPtr))
    {
      return 1; /* Error: invalid decimal format */
    }

    while (this->nbCharRead < this->length && IS_DIGIT(*this->readPtr))
    {
      if (this->contentUse < BUFFER_SIZE - 1)
      {
        this->content[this->contentUse++] = *this->readPtr;
      }
      JsonReader_consumeOneChar(this);
    }
  }

  /* Exponent part (optional) */
  if (this->nbCharRead < this->length && (*this->readPtr == 'e' || *this->readPtr == 'E'))
  {
    if (this->contentUse < BUFFER_SIZE - 1)
    {
      this->content[this->contentUse++] = *this->readPtr;
    }
    JsonReader_consumeOneChar(this);

    if (this->nbCharRead < this->length && (*this->readPtr == '+' || *this->readPtr == '-'))
    {
      if (this->contentUse < BUFFER_SIZE - 1)
      {
        this->content[this->contentUse++] = *this->readPtr;
      }
      JsonReader_consumeOneChar(this);
    }

    if (this->nbCharRead >= this->length || !IS_DIGIT(*this->readPtr))
    {
      return 1; /* Error: invalid exponent format */
    }

    while (this->nbCharRead < this->length && IS_DIGIT(*this->readPtr))
    {
      if (this->contentUse < BUFFER_SIZE - 1)
      {
        this->content[this->contentUse++] = *this->readPtr;
      }
      JsonReader_consumeOneChar(this);
    }
  }

  this->content[this->contentUse] = 0;
  return 0; /* Success */
}

/**********************************************//** 
  @brief Consume boolean value (true or false)
  @private
**************************************************/
PRIVATE int JsonReader_consumeBoolean(JsonReader* this)
{
  if (Memory_ncmp(this->readPtr, "true", 4))
  {
    if (this->contentUse + 4 < BUFFER_SIZE)
    {
      Memory_copy(this->content + this->contentUse, "true", 4);
      this->contentUse += 4;
      this->nbCharRead += 4;
      this->readPtr += 4;
      this->col += 4;
      this->content[this->contentUse] = 0;
      return 0;
    }
  }
  else if (Memory_ncmp(this->readPtr, "false", 5))
  {
    if (this->contentUse + 5 < BUFFER_SIZE)
    {
      Memory_copy(this->content + this->contentUse, "false", 5);
      this->contentUse += 5;
      this->nbCharRead += 5;
      this->readPtr += 5;
      this->col += 5;
      this->content[this->contentUse] = 0;
      return 0;
    }
  }

  return 1; /* Error: invalid boolean */
}

/**********************************************//** 
  @brief Consume null value
  @private
**************************************************/
PRIVATE int JsonReader_consumeNull(JsonReader* this)
{
  if (Memory_ncmp(this->readPtr, "null", 4))
  {
    if (this->contentUse + 4 < BUFFER_SIZE)
    {
      Memory_copy(this->content + this->contentUse, "null", 4);
      this->contentUse += 4;
      this->nbCharRead += 4;
      this->readPtr += 4;
      this->col += 4;
      this->content[this->contentUse] = 0;
      return 0;
    }
  }

  return 1; /* Error: invalid null */
}

/**********************************************//** 
  @brief Consume one character and update position
  @private
**************************************************/
PRIVATE int JsonReader_consumeOneChar(JsonReader* this)
{
  if (*this->readPtr == '\n')
  {
    this->col = 1;
    this->line++;
  }
  else
  {
    this->col++;
  }
  this->nbCharRead++;
  this->readPtr++;
  return 0;
}
