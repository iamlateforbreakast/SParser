/**********************************************//** 
  @file Map.c
 
  @brief A Map class.
  @details This class provides a container indexed
  by a string.
**************************************************/

#include "Map.h"
#include "List.h"
#include "Object.h"

#define HTABLE_SIZE (50)

PRIVATE unsigned int Map_hash(Map* this, String* s, unsigned int i);

typedef struct MapEntry
{
  String* s;
  void* p;
  List* l;
} MapEntry;

/**********************************************//**
  @class Map
**************************************************/
struct Map
{
  MapEntry htable[HTABLE_SIZE];
};

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof Map
**************************************************/
PUBLIC Map* Map_new()
{
  Map * this = 0;
  
  return this;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_delete(Map* this)
{
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof Map
**************************************************/
PUBLIC unsigned int Map_insert(Map* this,String* s, void* p)
{
  unsigned int result = 0;
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @public
  @memberof Map
**************************************************/
PUBLIC unsigned int Map_find(Map* this, String* s, void** p)
{
  unsigned int result = 0;
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @private
  @memberof Map
**************************************************/
PRIVATE unsigned int Map_hash(Map* this, String* s, unsigned int i)
{
  unsigned int result = 0;
  
  return result;
}
