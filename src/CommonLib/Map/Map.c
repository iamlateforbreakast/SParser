/**********************************************//** 
  @file Map.c
 
  @brief A Map class.
  This class provides a container indexed
  by a string.
**************************************************/

#include "Map.h"
#include "List.h"
#include "Object.h"
#include "String2.h"
#include "Memory.h"

#define HTABLE_SIZE (50)

/**********************************************//**
  @private
**************************************************/
PRIVATE unsigned int Map_hash(Map * this, char * s, unsigned int i);

/**********************************************//**
  @private
**************************************************/
typedef struct MapEntry
{
  String * s;
  void * item;
} MapEntry;

/**********************************************//**
  @class Map
**************************************************/
struct Map
{
  Object object;
  List * htable[HTABLE_SIZE];
};

/**********************************************//** 
  @brief Create a new instance of the class Map.
  @public
  @memberof Map
**************************************************/
PUBLIC Map* Map_new()
{
  Map * this = 0;
  unsigned int i = 0;
  
  this = (Map*)Object_new(sizeof(Map),(Destructor)&Map_delete, (Copy_operator)&Map_copy);
  
  for (i=0;i<HTABLE_SIZE;i++)
  {
    this->htable[i] = 0;
  }
  
  return this;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_delete(Map * this)
{
  unsigned int i = 0;
  
  if (this != 0)
  {
    for (i=0;i<HTABLE_SIZE;i++)
    {
      List_delete(this->htable[i]);
    }
  
    Object_delete(&this->object);
  }
}

/**********************************************//** 
  @brief Copy an instance of the class Map.
  @public
  @memberof Map
  @return Copy of instance.
**************************************************/
PUBLIC Map * Map_copy(Map * this)
{
  Map * result = 0;
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Map
  @return TBD
**************************************************/
PUBLIC unsigned int Map_insert(Map * this,String * s, void * p)
{
  unsigned int result = 0;
  unsigned int key = 0;
  unsigned int i = 0;
  MapEntry * entry =0;
  
  /* TODO: Manage duplication */
  for (i=1; (i<=String_getLength(s)) && (result==0); i++)
  {
    key = Map_hash(this,String_getBuffer(s), i);
    if (this->htable[key] == 0)
    {
      this->htable[key] = List_new();
      entry = (MapEntry*)Memory_alloc(sizeof(MapEntry));
      entry->s = s;
      entry->item = p;
      List_insertHead(this->htable[key], entry);
      result = 1;
    }
    else if (i==String_getLength(s)) 
    {
      entry = (MapEntry*)Memory_alloc(sizeof(MapEntry));
      entry->s = s;
      entry->item = p;
      List_insertHead(this->htable[key], entry);
    }
  }
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Map
**************************************************/
PUBLIC unsigned int Map_find(Map* this, String* s, void** p)
{
  unsigned int result = 0;
  unsigned int key = 0;
  unsigned int i = 0;
  unsigned int isFound = 0;
  MapEntry * n = 0;
  
  for (i=1; (i<=String_getLength(s)) && (!isFound); i++)
  {
    key = Map_hash(this, String_getBuffer(s), i);
    if (this->htable[key] != 0)
    {
      while ((n = (MapEntry*)List_getNext(this->htable[key]))!= 0)
      {
        if (String_isEqual(n->s, s))
        {
          *p = n->item;
          isFound = 1;
          result = 1;
          break;
        }
      }
    }
  }
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @private
  @memberof Map
**************************************************/
PRIVATE unsigned int Map_hash(Map * this, char * s, unsigned int length)
{
  unsigned int result = 0;
  unsigned int i = 0;
  
  for (i=0; i<length; i++)
  {
    result = result + s[i];
    result += (result << 10);
    result ^= (result>>6);
  }
  result = result % HTABLE_SIZE;
  /*
          hash += key[i], hash += ( hash << 10 ), hash ^= ( hash >> 6 );
    }
    hash += ( hash << 3 ), hash ^= ( hash >> 11 ), hash += ( hash << 15 );
    */

  return result;
}