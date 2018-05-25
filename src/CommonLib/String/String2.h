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

#endif /* _STRING2_H_ */