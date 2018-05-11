/* Map.h */

#ifndef _MAP_H_
#define _MAP_H_

#include "List.h"
#include "String2.h"
#include "Types.h"

typedef struct Map Map;

PUBLIC Map* Map_new();
PUBLIC void Map_delete(Map* this);
PUBLIC unsigned int Map_insert(Map* this,String* s, void* p);
PUBLIC unsigned int Map_find(Map* this, String* s, void** p);
#endif
