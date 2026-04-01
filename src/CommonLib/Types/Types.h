/* Types.h */

#ifndef _TYPES_H_
#define _TYPES_H_

#define PUBLIC (extern)

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

/* C89 INLINE implementation */
#ifndef INLINE
#  if defined(__GNUC__)
#    define INLINE static __inline__
#  elif defined(_MSC_VER)
#    define INLINE static __inline
#  else
#    define INLINE static
#  endif
#endif

#include "UserTypes.h"
#endif /* _TYPES_H_ */
