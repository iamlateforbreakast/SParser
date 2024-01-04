/* MapEntry.h */

#ifndef _MAPENTRY_H_
#define _MAPENTRY_H_

#include "Types.h"
#include "String2.h"
#include "Allocator.h"

typedef struct MapEntry MapEntry;

PUBLIC MapEntry * MapEntry_new(String * s, void * p, int isOwner);
PUBLIC MapEntry * MapEntry_newFromAllocator(Allocator * allocator, String *s, void * p, int isOwner);
PUBLIC void MapEntry_delete(MapEntry * this);
PUBLIC MapEntry * MapEntry_copy(MapEntry * this);
PUBLIC unsigned int MapEntry_getSize(MapEntry * this);
PUBLIC String * MapEntry_getString(MapEntry * this);
PUBLIC void * MapEntry_getItem(MapEntry * this);
PUBLIC void MapEntry_setString(MapEntry * this, String * s);
PUBLIC void MapEntry_setItem(MapEntry * this, void * item);

#endif /* _MAPENTRY_H_ */
