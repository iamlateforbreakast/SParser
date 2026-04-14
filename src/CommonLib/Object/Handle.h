/* Handle.h */

#ifndef HANDLE_H
#define HANDLE_H

#include "Object.h"

typedef struct Handle Handle;

struct Handle
{
  Object *object;
};

Handle * Handle_new();
void Handle_delete(Handle * this);

#endif /* HANDLE_H */
