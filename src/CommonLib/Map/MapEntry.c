/**********************************************//** 
   @file Map.c
		     
   @brief A support class for theMap class.
**************************************************/
#include "MapEntry.h"
#include "Object.h"
#include "String2.h"

struct MapEntry
{
  Object object;
  String * s;
  void * item;
};

MapEntry * MapEntry_new(String * s, void * item)
{
  MapEntry * this = 0;

  this = (MapEntry*)Object_new(sizeof(MapEntry),(Destructor)&MapEntry_delete,(Copy_operator)&MapEntry_copy);
  this->s = s;
  this->item = item;

  return this;
}

void MapEntry_delete(MapEntry * this)
{
  String_delete(this->s);
  Object_delete(&this->object);
}

MapEntry * MapEntry_copy(MapEntry * this)
{
  MapEntry * copy = 0;

  return copy;
}

String * MapEntry_getString(MapEntry * this)
{
  return this->s;
}

void * MapEntry_getItem(MapEntry * this)
{
  return this->item;
}
