/* Object.h */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Types.h"
#include "Class.h"
#include "Allocator.h"

typedef struct Object Object;

struct Object
{
  unsigned int id;
  Class * class;
  void (*delete)(Object * this);
  Object * (*copy)(Object * this);
  unsigned int refCount;
  unsigned int size;
  Allocator * allocator;
};

PUBLIC Object * Object_new(unsigned int size, Class * class);
PUBLIC void Object_delete(Object * this);
PUBLIC Object * Object_copy(Object * this);
PUBLIC unsigned int Object_isEqual(Object * this, Object * compared);
PUBLIC char * Object_print(Object * this);
PUBLIC Object* Object_getRef(Object* this);

#endif /* _OBJECT_H_ */
