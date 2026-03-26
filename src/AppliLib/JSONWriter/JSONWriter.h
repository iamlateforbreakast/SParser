#ifndef _JSONWRITER_H_
#define _JSONWRITER_H_
#include "String2.h"

typedef enum
{
  JSON_WRITE_SUCCESS = 0,
  JSON_WRITE_ERROR_BUFFER_FULL = 1,
  JSON_WRITE_ERROR_INVALID_STATE = 2,
  JSON_WRITE_ERROR_INVALID_PARAM = 3,
  JSON_WRITE_ERROR_NESTING_EXCEEDED = 4
} JsonWriteError;

typedef enum
{
  JSON_STATE_ROOT,
  JSON_STATE_OBJECT,
  JSON_STATE_ARRAY,
  JSON_STATE_OBJECT_KEY,
  JSON_STATE_OBJECT_VALUE,
  JSON_STATE_ARRAY_VALUE
} JsonWriteState;

typedef struct JsonWriter JsonWriter;

/* Constructor/Destructor */
PUBLIC JsonWriter* JsonWriter_new(void);
PUBLIC void JsonWriter_delete(JsonWriter* this);
PUBLIC JsonWriter* JsonWriter_copy(JsonWriter* this);
PUBLIC int JsonWriter_compare(JsonWriter* this, JsonWriter* compared);
PUBLIC void JsonWriter_print(JsonWriter* this);
PUBLIC unsigned int JsonWriter_getSize(JsonWriter* this);

/* Core writing operations */
PUBLIC int JsonWriter_startObject(JsonWriter* this);
PUBLIC int JsonWriter_endObject(JsonWriter* this);
PUBLIC int JsonWriter_startArray(JsonWriter* this);
PUBLIC int JsonWriter_endArray(JsonWriter* this);

/* Write key-value pairs (for objects) */
PUBLIC int JsonWriter_writeKey(JsonWriter* this, const char* key);
PUBLIC int JsonWriter_writeStringValue(JsonWriter* this, const char* value);
PUBLIC int JsonWriter_writeNumberValue(JsonWriter* this, double value);
PUBLIC int JsonWriter_writeIntValue(JsonWriter* this, int value);
PUBLIC int JsonWriter_writeBooleanValue(JsonWriter* this, int value);
PUBLIC int JsonWriter_writeNullValue(JsonWriter* this);

/* Write array elements */
PUBLIC int JsonWriter_writeArrayString(JsonWriter* this, const char* value);
PUBLIC int JsonWriter_writeArrayNumber(JsonWriter* this, double value);
PUBLIC int JsonWriter_writeArrayInt(JsonWriter* this, int value);
PUBLIC int JsonWriter_writeArrayBoolean(JsonWriter* this, int value);
PUBLIC int JsonWriter_writeArrayNull(JsonWriter* this);

/* Utility methods */
PUBLIC String* JsonWriter_getResult(JsonWriter* this);
PUBLIC int JsonWriter_getError(JsonWriter* this);
PUBLIC int JsonWriter_getDepth(JsonWriter* this);
PUBLIC void JsonWriter_reset(JsonWriter* this);

#endif /* _JSONWRITER_H_ */
