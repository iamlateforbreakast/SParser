/* Handle.c */

#include "Handle.h"
#include "Object.h"
#include "Memory.h"

struct Handle
{
  Object* object;
  int isOwner;
};

/**********************************************//**
  @brief Create a new instance of the class Handle.
  @public
  @memberof Object
  @return New Handle instance or NULL if failed to allocate.
**************************************************/
PUBLIC Handle* Handle_new(void* object, int isOwner)
{
  Handle* result = 0;

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
PUBLIC void Handle_delete(Handle* this)
{
  if (this != 0)
  {
    if (this->isOwner && (OBJECT_IS_VALID(this->object)))
    {
      Object_delete(this->object);
    }
    Memory_free(this, sizeof(Handle));
  }
}

PUBLIC Handle* Handle_copy(Handle* self)
{
  Handle * copy = (Handle*)Memory_alloc(sizeof(Handle));

  if (copy == 0) return 0;

  copy->object = Object_copy(self->object);
  copy->isOwner = self->isOwner;
}

PUBLIC Object* Handle_getObject(Handle* self)
{
  return self->object;
}
