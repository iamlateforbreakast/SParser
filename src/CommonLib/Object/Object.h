/* Object.h */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Types.h"

typedef struct Object Object;

typedef void (*Destructor)(struct Object *);
typedef struct Object * (*Copy_operator)(struct Object *);

struct Object
{
  unsigned int id;
  void (*delete)(Object * this);
  Object * (*copy)(Object * this);
  unsigned int refCount;
  unsigned int size;
};

PUBLIC Object * Object_new(unsigned int size, void (*f_delete)(Object*), Object * (*f_copy)(Object*));
PUBLIC void Object_delete(Object * this);
PUBLIC Object * Object_copy(Object * this);
PUBLIC Object* Object_getRef(Object* this);

#endif /* _OBJECT_H_ */
