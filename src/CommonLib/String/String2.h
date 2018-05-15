/* String2.h */

#ifndef _STRING2_H_
#define _STRING2_H_

#include "Types.h"

typedef struct String String;

PUBLIC String * String_new();
PUBLIC void String_delete(String * this);
PUBLIC String * String_copy(String * this);

#endif /* _STRING2_H_ */