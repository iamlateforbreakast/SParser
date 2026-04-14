/* Handle.h */

#ifndef HANDLE_H
#define HANDLE_H

#include "Object.h"

#define OWNER (1)
#define NOT_OWNER (0)

typedef struct Handle Handle;

struct Handle
{
  Object *object;
  int isOwner;
};

/**********************************************//** 
  @brief Create a new instance of the class Handle.
  @public
  @memberof Object
  @return New Handle instance or NULL if failed to allocate.
**************************************************/
PUBLIC Handle * Handle_new(Object *object, int isOwner)
{
  Handle * result = 0;

  if (OBJECT_IS_INVALID(object)) return 0;

  result = (Handle*)Memory_alloc(sizeof(Handle));

  if (result == 0) return 0;

  result->object = object;
  result->isOwner = isOwner;

  return result;
}

/**********************************************//** 
  @brief Delete an instance of the class Handle.
  @public
  @memberof Object
**************************************************/
PUBLIC void Handle_delete(Handle * this)
{
  if (this != 0)
  {
    if (this->isOwner && this->object != 0)
    {
      Object_delete(this->object);
    }
    Memory_dealloc(this);
  }
}

#endif /* HANDLE_H */
