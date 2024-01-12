/* Object.h */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Types.h"
#include "Class.h"
#include "Allocator.h"

typedef struct Object Object;

struct Object
{
  int marker; 
  unsigned int id;
  unsigned int uniqId;
  Class * class;
  void (*delete)(Object * this);
  Object * (*copy)(Object * this);
  unsigned int refCount;
  unsigned int size;
  Allocator * allocator;
};

PUBLIC Object * Object_new(unsigned int size, Class * class);
PUBLIC Object* Object_newFromAllocator(Class* class, Allocator* allocator);
PUBLIC void Object_delete(Object * this);
PUBLIC void Object_deallocate(Object* this);
PUBLIC Object * Object_copy(Object * this);
PUBLIC int Object_comp(Object * this, Object * compared);
PUBLIC char * Object_print(Object * this);
PUBLIC Object* Object_getRef(Object * this);
PUBLIC void Object_deRef(Object * this);

#endif /* _OBJECT_H_ */
