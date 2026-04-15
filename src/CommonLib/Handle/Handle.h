/* Handle.h */

#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "Object.h"
#include "Types.h"

#define HANDLE_OWNER (1)
#define HANDLE_NOT_OWNER (0)

typedef struct Handle Handle;

PUBLIC Handle * Handle_new(void *object, int isOwner);
PUBLIC void Handle_delete(Handle * this);
PUBLIC Object * Handle_getObject(Handle * this);
PUBLIC Handle* Handle_copy(Handle* self);

#endif /* HANDLE_H */
