/* MapEntry.h */

#ifndef _MAPENTRY_H_
#define _MAPENTRY_H_

typedef struct MapEntry MapEntry;

MapEntry * MapEntry_new();
void MapEntry_delete(MapEntry * this);
MapEntry * MapEntry_copy(MapEntry * this);

#endif /* _MAPENTRY_H_ */
