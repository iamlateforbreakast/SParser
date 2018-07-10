/* MapEntry.h */

#ifndef _MAPENTRY_H_
#define _MAPENTRY_H_

#include "Types.h"
#include "String2.h"

typedef struct MapEntry MapEntry;

PUBLIC MapEntry * MapEntry_new();
PUBLIC void MapEntry_delete(MapEntry * this);
PUBLIC MapEntry * MapEntry_copy(MapEntry * this);
PUBLIC String * MapEntry_getString(MapEntry * this);
PUBLIC void * MapEntry_getItem(MapEntry * this);
PUBLIC void MapEntry_setString(MapEntry * this, String * s);
PUBLIC void MapEntry_setItem(MapEntry * this, void * item);

#endif /* _MAPENTRY_H_ */
