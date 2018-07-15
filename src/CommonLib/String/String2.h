/* String2.h */

#ifndef _STRING2_H_
#define _STRING2_H_

#include "Types.h"

typedef struct String String;

PUBLIC String * String_new(const char * constString);
PUBLIC void String_delete(String * this);
PUBLIC String * String_copy(String * this);
PUBLIC unsigned int String_getLength(String * this);
PUBLIC char * String_getBuffer(String * this);
PUBLIC void String_setBuffer(String * this, char * buffer);
PUBLIC unsigned int String_isContained(String * this, String * s2);
PUBLIC unsigned int String_prepend(String * this, const char * prefix);
PUBLIC unsigned int String_isEqual(String * this, String * compared);
PUBLIC String * String_subString(String * this, unsigned int idx, unsigned int length);
PUBLIC unsigned int String_matchWildcard(String * this, const char * wildcard);
#endif /* _STRING2_H_ */