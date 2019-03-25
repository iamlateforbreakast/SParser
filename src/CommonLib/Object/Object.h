/* Object.h */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Types.h"

typedef struct Class Class;
typedef struct Object Object;

struct Object
{
  unsigned int id;
  Class * class;
  void (*delete)(Object * this);
  Object * (*copy)(Object * this);
  unsigned int refCount;
  unsigned int size;
};

typedef struct Object * (*Constructor)();
typedef void (*Destructor)(struct Object *);
typedef struct Object * (*Copy_Operator)(struct Object *);
typedef unsigned int (*Equal_Operator)(struct Object *, struct Object *);
typedef char * (*Printer)(struct Object *);



PUBLIC Object * Object_new(unsigned int size, Class * class);
PUBLIC void Object_delete(Object * this);
PUBLIC Object * Object_copy(Object * this);
PUBLIC Object* Object_getRef(Object* this);

#endif /* _OBJECT_H_ */
