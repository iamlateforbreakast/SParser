/* Types.h */

#ifndef _TYPES_H_
#define _TYPES_H_

#define PUBLIC

#define DECLARE_CLASS(x)

#ifndef UNIT_TEST
  #define PRIVATE static
#else
  #define PRIVATE
#endif

#ifndef WIN32
#define INVALID_MEMORY (0x00000000)
#else
#define INVALID_MEMORY (0xffffffcd)
#endif
union mem_align
{
  void * a;
  long int b;
  long long c;
};

#define MEM_ALIGN (sizeof(union mem_align))

#include "UserTypes.h"
#endif /* _TYPES_H_ */
