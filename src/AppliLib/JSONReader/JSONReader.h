#ifndef _JSONREADER_H_
#define _JSONREADER_H_
#include "String2.h"

typedef enum
{
  JSONNONE,
  JSONOBJECT_START,      /* { */
  JSONOBJECT_END,        /* } */
  JSONARRAY_START,       /* [ */
  JSONARRAY_END,         /* ] */
  JSONKEY,               /* "key" */
  JSONSTRING_VALUE,      /* "value" */
  JSONNUMBER_VALUE,      /* 123, 45.67, -89 */
  JSONBOOLEAN_VALUE,     /* true, false */
  JSONNULL_VALUE,        /* null */
  JSONCOMMA,             /* , */
  JSONCOLON              /* : */
} JsonNode;

typedef struct JsonReader JsonReader;

PUBLIC JsonReader* JsonReader_new(String* s);
PUBLIC void JsonReader_delete(JsonReader* this);
PUBLIC JsonReader* JsonReader_copy(JsonReader* this);
PUBLIC int JsonReader_compare(JsonReader* this, JsonReader* compared);
PUBLIC void JsonReader_print(JsonReader* this);
PUBLIC unsigned int JsonReader_getSize(JsonReader* this);
PUBLIC JsonNode JsonReader_read(JsonReader* this);
PUBLIC String* JsonReader_getContent(JsonReader* this);
PUBLIC int JsonReader_isError(JsonReader* this);
PUBLIC int JsonReader_getLine(JsonReader* this);
PUBLIC int JsonReader_getColumn(JsonReader* this);

#endif /* _JSONREADER_H_ */
