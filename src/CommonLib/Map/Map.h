/* Map.h */

#ifndef _MAP_H_
#define _MAP_H_

#include "Types.h"
#include "String2.h"
#include "List.h"
#include "Allocator.h"

typedef struct Map Map;

PUBLIC Map * Map_new();
PUBLIC Map* Map_newFromAllocator(Allocator * allocator);
PUBLIC void Map_delete(Map * this);
PUBLIC Map * Map_copy(Map * this);
PUBLIC int Map_comp(Map* this, Map* compared);
PUBLIC unsigned int Map_insert(Map * this,String* s, void * p);
PUBLIC unsigned int Map_find(Map * this, String* s, void ** p);
PUBLIC void Map_print(Map * this);
PUBLIC unsigned int Map_getSize(Map * this);
PUBLIC List * Map_getAll(Map * this);

#endif /* _MAP_H_ */
