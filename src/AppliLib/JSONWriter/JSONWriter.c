/* JSONWriter.c */
#include "JSONWriter.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE (4096)      /* Output buffer size */
#define MAX_DEPTH (64)          /* Maximum nesting depth */

/**********************************************//**
  @class JsonWriter
**************************************************/
struct JsonWriter
{
  Object object;
  char * buffer;
  int bufferUse;
  int bufferSize;
  JsonWriteError lastError;
  int depth;
  JsonWriteState stateStack[MAX_DEPTH];
  int needsComma[MAX_DEPTH];  /* Track if next element needs comma separator */
  int isFirstElement[MAX_DEPTH];
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class jsonWriterClass = 
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&JsonWriter_delete,
  .f_copy = (Copy_Operator)&JsonWriter_copy,
  .f_comp = (Comp_Operator)&JsonWriter_compare,
  .f_print = (Printer)&JsonWriter_print,
  .f_size = (Sizer)&JsonWriter_getSize
};

/**********************************************//**
  Private method declarations
**************************************************/
PRIVATE int JsonWriter_appendChar(JsonWriter* this, char c);
PRIVATE int JsonWriter_appendString(JsonWriter* this, const char* str);
PRIVATE int JsonWriter_appendEscapedString(JsonWriter* this, const char* str);
PRIVATE int JsonWriter_ensureCapacity(JsonWriter* this, int needed);
PRIVATE int JsonWriter_validateState(JsonWriter* this, JsonWriteState expected);
PRIVATE int JsonWriter_handleComma(JsonWriter* this);
PRIVATE int JsonWriter_writeQuotedString(JsonWriter* this, const char* str);

/**********************************************//** 
  @brief Create a new instance of the class JsonWriter.
  @public
  @memberof JsonWriter
  @return New instance.
**************************************************/
PUBLIC JsonWriter* JsonWriter_new(void)
{
  JsonWriter* this = 0;

  this = (JsonWriter*)Object_new(sizeof(JsonWriter), &jsonWriterClass);

  if (OBJECT_IS_INVALID(this)) return 0;
  
  this->buffer = (char*)Memory_alloc(BUFFER_SIZE);
  if (this->buffer == 0)
  {
    Object_deallocate(&this->object);
    return 0;
  }

  this->bufferSize = BUFFER_SIZE;
  this->bufferUse = 0;
  this->lastError = JSON_WRITE_SUCCESS;
  this->depth = 0;
  this->buffer[0] = 0;

  /* Initialize state stack */
  this->stateStack[0] = JSON_STATE_ROOT;
  this->needsComma[0] = 0;
  this->isFirstElement[0] = 1;

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class JsonWriter.
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC void JsonWriter_delete(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  Memory_free(this->buffer, BUFFER_SIZE);
  
  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

/**********************************************//** 
  @brief Copy an instance of the class JsonWriter.
  @public
  @memberof JsonWriter
  @return Copy of instance
**************************************************/
PUBLIC JsonWriter* JsonWriter_copy(JsonWriter* this)
{
  return 0;
}

/**********************************************//** 
  @brief Compare an instance of the class JsonWriter.
  @public
  @memberof JsonWriter
  @return always 0
**************************************************/
PUBLIC int JsonWriter_compare(JsonWriter* this, JsonWriter* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Print an instance of the class JsonWriter.
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC void JsonWriter_print(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return;
  printf("%s", this->buffer);
}

/**********************************************//** 
  @brief Provide the size of the class or an instance
  @public
  @memberof JsonWriter
  @return Size in byte
**************************************************/
PUBLIC unsigned int JsonWriter_getSize(JsonWriter* this)
{
  return sizeof(JsonWriter);
}

/**********************************************//** 
  @brief Get current error state
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_getError(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return JSON_WRITE_ERROR_INVALID_PARAM;
  return this->lastError;
}

/**********************************************//** 
  @brief Get current nesting depth
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_getDepth(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return -1;
  return this->depth;
}

/**********************************************//** 
  @brief Get the JSON output as String
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC String* JsonWriter_getResult(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return 0;
  return String_new(this->buffer);
}

/**********************************************//** 
  @brief Reset writer state
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC void JsonWriter_reset(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->bufferUse = 0;
  this->lastError = JSON_WRITE_SUCCESS;
  this->depth = 0;
  this->buffer[0] = 0;

  this->stateStack[0] = JSON_STATE_ROOT;
  this->needsComma[0] = 0;
  this->isFirstElement[0] = 1;
}

/*============== Object Operations ==============*/

/**********************************************//** 
  @brief Start a new JSON object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_startObject(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);
  
  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (JsonWriter_appendChar(this, '{') != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (this->depth >= MAX_DEPTH - 1)
    return (this->lastError = JSON_WRITE_ERROR_NESTING_EXCEEDED);

  this->depth++;
  this->stateStack[this->depth] = JSON_STATE_OBJECT;
  this->needsComma[this->depth] = 0;
  this->isFirstElement[this->depth] = 1;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief End the current JSON object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_endObject(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);
  
  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_OBJECT)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  if (JsonWriter_appendChar(this, '}') != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->depth--;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/*============== Array Operations ==============*/

/**********************************************//** 
  @brief Start a new JSON array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_startArray(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);
  
  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (JsonWriter_appendChar(this, '[') != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (this->depth >= MAX_DEPTH - 1)
    return (this->lastError = JSON_WRITE_ERROR_NESTING_EXCEEDED);

  this->depth++;
  this->stateStack[this->depth] = JSON_STATE_ARRAY;
  this->needsComma[this->depth] = 0;
  this->isFirstElement[this->depth] = 1;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief End the current JSON array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_endArray(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this)) return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);
  
  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_ARRAY)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  if (JsonWriter_appendChar(this, ']') != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->depth--;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/*============== Object Key-Value Operations ==============*/

/**********************************************//** 
  @brief Write an object key
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeKey(JsonWriter* this, const char* key)
{
  if (OBJECT_IS_INVALID(this) || key == 0)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);
  
  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_OBJECT)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (JsonWriter_writeQuotedString(this, key) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (JsonWriter_appendChar(this, ':') != JSON_WRITE_SUCCESS)
    return this->lastError;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/*============== Object Value Operations ==============*/

/**********************************************//** 
  @brief Write a string value in object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeStringValue(JsonWriter* this, const char* value)
{
  if (OBJECT_IS_INVALID(this) || value == 0)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  return JsonWriter_writeQuotedString(this, value);
}

/**********************************************//** 
  @brief Write a double value in object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeNumberValue(JsonWriter* this, double value)
{
  char buffer[32];
  
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  snprintf(buffer, sizeof(buffer), "%.15g", value);
  
  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (JsonWriter_appendString(this, buffer) != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write an integer value in object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeIntValue(JsonWriter* this, int value)
{
  char buffer[16];
  
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  snprintf(buffer, sizeof(buffer), "%d", value);

  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (JsonWriter_appendString(this, buffer) != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write a boolean value in object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeBooleanValue(JsonWriter* this, int value)
{
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (value)
  {
    if (JsonWriter_appendString(this, "true") != JSON_WRITE_SUCCESS)
      return this->lastError;
  }
  else
  {
    if (JsonWriter_appendString(this, "false") != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write a null value in object
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeNullValue(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (JsonWriter_appendString(this, "null") != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/*============== Array Value Operations ==============*/

/**********************************************//** 
  @brief Write a string value in array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeArrayString(JsonWriter* this, const char* value)
{
  if (OBJECT_IS_INVALID(this) || value == 0)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_ARRAY)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (JsonWriter_writeQuotedString(this, value) != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write a number value in array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeArrayNumber(JsonWriter* this, double value)
{
  char buffer[32];
  
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_ARRAY)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  snprintf(buffer, sizeof(buffer), "%.15g", value);

  if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (JsonWriter_appendString(this, buffer) != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write an integer value in array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeArrayInt(JsonWriter* this, int value)
{
  char buffer[16];
  
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_ARRAY)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  snprintf(buffer, sizeof(buffer), "%d", value);

  if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (JsonWriter_appendString(this, buffer) != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write a boolean value in array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeArrayBoolean(JsonWriter* this, int value)
{
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_ARRAY)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (value)
  {
    if (JsonWriter_appendString(this, "true") != JSON_WRITE_SUCCESS)
      return this->lastError;
  }
  else
  {
    if (JsonWriter_appendString(this, "false") != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/**********************************************//** 
  @brief Write a null value in array
  @public
  @memberof JsonWriter
**************************************************/
PUBLIC int JsonWriter_writeArrayNull(JsonWriter* this)
{
  if (OBJECT_IS_INVALID(this))
    return (this->lastError = JSON_WRITE_ERROR_INVALID_PARAM);

  if (this->depth <= 0 || this->stateStack[this->depth] != JSON_STATE_ARRAY)
    return (this->lastError = JSON_WRITE_ERROR_INVALID_STATE);

  if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
    return this->lastError;

  if (JsonWriter_appendString(this, "null") != JSON_WRITE_SUCCESS)
    return this->lastError;

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}

/*============== Private Helper Methods ==============*/

/**********************************************//** 
  @brief Append single character to buffer
  @private
**************************************************/
PRIVATE int JsonWriter_appendChar(JsonWriter* this, char c)
{
  if (JsonWriter_ensureCapacity(this, 1) != JSON_WRITE_SUCCESS)
    return JSON_WRITE_ERROR_BUFFER_FULL;

  this->buffer[this->bufferUse++] = c;
  this->buffer[this->bufferUse] = 0;

  return JSON_WRITE_SUCCESS;
}

/**********************************************//** 
  @brief Append string to buffer
  @private
**************************************************/
PRIVATE int JsonWriter_appendString(JsonWriter* this, const char* str)
{
  int len = strlen(str);

  if (JsonWriter_ensureCapacity(this, len) != JSON_WRITE_SUCCESS)
    return JSON_WRITE_ERROR_BUFFER_FULL;

  strcpy(this->buffer + this->bufferUse, str);
  this->bufferUse += len;

  return JSON_WRITE_SUCCESS;
}

/**********************************************//** 
  @brief Append escaped string to buffer
  @private
**************************************************/
PRIVATE int JsonWriter_appendEscapedString(JsonWriter* this, const char* str)
{
  int len = 0;
  const char* p = str;

  /* First, calculate required length */
  while (*p)
  {
    if (*p == '"' || *p == '\\' || *p == '\b' || *p == '\f' || 
        *p == '\n' || *p == '\r' || *p == '\t')
    {
      len += 2;
    }
    else if ((unsigned char)*p < 32)
    {
      len += 6; /* \uXXXX format */
    }
    else
    {
      len += 1;
    }
    p++;
  }

  if (JsonWriter_ensureCapacity(this, len) != JSON_WRITE_SUCCESS)
    return JSON_WRITE_ERROR_BUFFER_FULL;

  /* Now append escaped characters */
  p = str;
  while (*p)
  {
    if (*p == '"')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = '"';
    }
    else if (*p == '\\')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = '\\';
    }
    else if (*p == '\b')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = 'b';
    }
    else if (*p == '\f')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = 'f';
    }
    else if (*p == '\n')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = 'n';
    }
    else if (*p == '\r')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = 'r';
    }
    else if (*p == '\t')
    {
      this->buffer[this->bufferUse++] = '\\';
      this->buffer[this->bufferUse++] = 't';
    }
    else if ((unsigned char)*p < 32)
    {
      sprintf(this->buffer + this->bufferUse, "\\u%04x", (unsigned char)*p);
      this->bufferUse += 6;
    }
    else
    {
      this->buffer[this->bufferUse++] = *p;
    }
    p++;
  }

  this->buffer[this->bufferUse] = 0;
  return JSON_WRITE_SUCCESS;
}

/**********************************************//** 
  @brief Ensure buffer has enough capacity
  @private
**************************************************/
PRIVATE int JsonWriter_ensureCapacity(JsonWriter* this, int needed)
{
  if (this->bufferUse + needed >= this->bufferSize)
  {
    return JSON_WRITE_ERROR_BUFFER_FULL;
  }
  return JSON_WRITE_SUCCESS;
}

/**********************************************//** 
  @brief Handle comma insertion between elements
  @private
**************************************************/
PRIVATE int JsonWriter_handleComma(JsonWriter* this)
{
  if (!this->isFirstElement[this->depth])
  {
    if (JsonWriter_appendChar(this, ',') != JSON_WRITE_SUCCESS)
      return JSON_WRITE_ERROR_BUFFER_FULL;
  }
  this->isFirstElement[this->depth] = 0;
  return JSON_WRITE_SUCCESS;
}

/**********************************************//** 
  @brief Write a quoted string value
  @private
**************************************************/
PRIVATE int JsonWriter_writeQuotedString(JsonWriter* this, const char* str)
{
  if (this->depth > 0)
  {
    if (JsonWriter_handleComma(this) != JSON_WRITE_SUCCESS)
      return this->lastError;
  }

  if (JsonWriter_appendChar(this, '"') != JSON_WRITE_SUCCESS)
    return (this->lastError = JSON_WRITE_ERROR_BUFFER_FULL);

  if (JsonWriter_appendEscapedString(this, str) != JSON_WRITE_SUCCESS)
    return (this->lastError = JSON_WRITE_ERROR_BUFFER_FULL);

  if (JsonWriter_appendChar(this, '"') != JSON_WRITE_SUCCESS)
    return (this->lastError = JSON_WRITE_ERROR_BUFFER_FULL);

  this->needsComma[this->depth] = 1;
  this->isFirstElement[this->depth] = 0;

  return (this->lastError = JSON_WRITE_SUCCESS);
}
