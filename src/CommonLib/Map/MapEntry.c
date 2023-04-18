/**********************************************//** 
   @file Map.c
		     
   @brief A support class for theMap class.
**************************************************/
#include "MapEntry.h"
#include "Class.h"
#include "Object.h"
#include "String2.h"

struct MapEntry
{
  Object object;
  String * s;
  void * item;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class mapEntryClass = 
{
  .f_new = 0,
  .f_delete = (Destructor)&MapEntry_delete,
  .f_copy = (Copy_Operator)&MapEntry_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0
};

PUBLIC MapEntry * MapEntry_new(String * s, void * item)
{
  MapEntry * this = 0;

  this = (MapEntry*)Object_new(sizeof(MapEntry),&mapEntryClass);
  this->s = s;
  this->item = item;

  return this;
}

PUBLIC void MapEntry_delete(MapEntry * this)
{
  String_delete(this->s);
  if (((Object*)this->item)->delete!=0)
  {
    ((Object*)this->item)->delete(this->item);
  }
  Object_delete(&this->object);
}

PUBLIC MapEntry * MapEntry_copy(MapEntry * this)
{
  MapEntry * copy = 0;

  return copy;
}

PUBLIC String * MapEntry_getString(MapEntry * this)
{
  return this->s;
}

PUBLIC void MapEntry_setString(MapEntry * this, String * s)
{
  String_delete(this->s);
  this->s = s;
}

PUBLIC void * MapEntry_getItem(MapEntry * this)
{
  return this->item;
}

PUBLIC void MapEntry_setItem(MapEntry * this, void * item)
{
  if (((Object*)this->item)->delete!=0)
  {
    ((Object*)this->item)->delete(this->item);
  }
  this->item = item;
}
