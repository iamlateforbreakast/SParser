/* String2.h */

#ifndef _STRING2_H_
#define _STRING2_H_

#include "Types.h"
#include "List.h"

typedef struct String String;

PUBLIC String * String_new(const char * constString);
PUBLIC String * String_newByRef(const char * constString);
PUBLIC void String_delete(String * this);
PUBLIC String * String_copy(String * this);
PUBLIC String * String_getRef(String * this);
PUBLIC unsigned int String_getLength(String * this);
PUBLIC char * String_getBuffer(String * this);
PUBLIC void String_setBuffer(String* this, char* buffer, int isOwned);
PUBLIC unsigned int String_isContained(String * this, String * s2);
PUBLIC unsigned int String_prepend(String * this, const char * prefix);
PUBLIC unsigned int String_append(String* this, const char* postfix);
PUBLIC int String_compare(String * this, String * compared);
PUBLIC String * String_subString(String * this, unsigned int idx, unsigned int length);
PUBLIC unsigned int String_matchWildcard(String * this, const char * wildcard);
PUBLIC int String_toInt(String* this);
PUBLIC List* String_splitToken(String* this, const char* separator);
PUBLIC void String_stealBuffer(String* this, String* s);
PUBLIC unsigned int String_getSize(String* this);
PUBLIC void String_print(String* this);
#endif /* _STRING2_H_ */
