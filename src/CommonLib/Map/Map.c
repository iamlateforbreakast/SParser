/**********************************************//** 
  @file Map.c
 
  @brief A Map class.
  This class provides a container indexed
  by a string.
**************************************************/

#include "Map.h"
#include "MapEntry.h"
#include "List.h"
#include "Class.h"
#include "Object.h"
#include "String2.h"
#include "Memory.h"
#include "Error.h"

#define HTABLE_SIZE (50)

/**********************************************//**
  @private
**************************************************/
PRIVATE unsigned int Map_hash(Map * this, char * s, unsigned int i);
PRIVATE MapEntry * Map_findEntry(Map* this, String * s);

/**********************************************//**
  @class Map
**************************************************/
struct Map
{
  Object object;
  List * htable[HTABLE_SIZE];
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class mapClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&Map_delete,
  .f_copy = (Copy_Operator)&Map_copy,
  .f_comp = (Comp_Operator)&Map_comp,
  .f_print = (Printer)&Map_print,
  .f_size = (Sizer)&Map_getSize
};

/********************************************************//**
  @brief Create a new instance of the class Map.
  @public
  @memberof Map
  @return New Map instance or NULL if failed to allocate.
************************************************************/
PUBLIC Map* Map_new()
{
  Map * this = 0;
  
  this = (Map*)Object_new(sizeof(Map),&mapClass);
  if (this!=0)
  {
    for (int i = 0; i < HTABLE_SIZE; i++)
    {
      this->htable[i] = List_new();
    }
  }

  return this;
}

/**********************************************//**
  @brief Create a new instance of the class Map 
  using a specific allocator.
  @public
  @memberof Map
  @return New Map instance or NULL if failed to allocate.
**************************************************/
PUBLIC Map* Map_newFromAllocator(Allocator * allocator)
{
  Map* this = 0;

  this = (Map*)Object_newFromAllocator(&mapClass, (Allocator*)allocator);
  if (this!=0)
  {
    for (int i = 0; i < HTABLE_SIZE; i++)
    {
      this->htable[i] = List_new();
    }
  }

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Map.
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_delete(Map * this)
{
  if (this != 0)
  {
    if (this->object.refCount == 1)
    {
      /* De-allocate the specific members */
      for (int i = 0; i < HTABLE_SIZE; i++)
      {
        List_delete(this->htable[i]);
      }
      /* De-allocate the base object */
      Object_deallocate(&this->object);
    }
    else if (this->object.refCount > 1)
    {
      this->object.refCount--;
    }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class Map.
  @public
  @memberof Map
  @return Copy of instance of NULL if failed to allocate.
**************************************************/
PUBLIC Map * Map_copy(Map * this)
{
  Map * result = 0;
  
  return result;
}

PUBLIC int Map_comp(Map* this, Map* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Insert an object into a Map instance
  @public
  @memberof Map
  @return 1 is inserted
**************************************************/
PUBLIC unsigned int Map_insert(Map * this,String * s, void * p)
{
  unsigned int result = 0;
  unsigned int key = 0;
  unsigned int i = 0;
  void * entry =0;
  MapEntry * me = 0;
  
  /* Check if there is an entry under s */
  if ((me = Map_findEntry(this, s))!=0)
  {
    /* Replace entry with new entry an free existing entry */
    MapEntry_setString(me, String_getRef(s));
    MapEntry_setItem(me, Object_getRef((Object*)p));
  }
  else
  {
    /* Create a new entry */
    for (i=1; (i<=String_getLength(s)) && (result==0); i++)
    {
      key = Map_hash(this,String_getBuffer(s), i);
      if (this->htable[key] == 0)
      {
        this->htable[key] = List_new();
        entry = MapEntry_new(Object_getRef((Object*)s), Object_getRef((Object*)p));
        List_insertHead(this->htable[key], entry);
        result = 1;
      }
      else if (i==String_getLength(s)) 
      {
        entry = MapEntry_new(Object_getRef((Object*)s), Object_getRef((Object*)p));
        List_insertHead(this->htable[key], entry);
      }
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
  MapEntry * n = 0;
  
  n = Map_findEntry(this, s);
  
  if (n!=0)
  {
    *p = (MapEntry_getItem(n));
    result = 1;
  }
  else
  {
    *p = 0;
    result = 0;
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

/**********************************************//**
  @brief Find a map item based on a key
  @private
  @memberof Map
**************************************************/
PRIVATE MapEntry * Map_findEntry(Map* this, String * s)
{
  MapEntry * result = 0;
  unsigned int key = 0;
  unsigned int i = 0;
  unsigned int isFound = 0;
  MapEntry * n = 0;
  
  for (i=1; (i<=String_getLength(s)) && (!isFound); i++)
  {
    key = Map_hash(this, String_getBuffer(s), i);
    if (this->htable[key] != 0)
    {
      List_resetIterator(this->htable[key]);
      n = (MapEntry*)List_getNext(this->htable[key]);
      while (n!= 0)
      {
        if (String_compare(MapEntry_getString(n), s)==0)
        {
          isFound = 1;
          result = n;
          break;
        }
        n = (MapEntry*)List_getNext(this->htable[key]);
      }
      
    }
  }
  
  return result;
}

/**********************************************//**
  @brief Print a Map instance
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_print(Map * this)
{
  int i = 0;
  MapEntry * n = 0;
  int j = 0;
  char * p = 0;
  
  if (this != 0)
  {
    for (i=0;i<HTABLE_SIZE;i++)
    {
      if (this->htable[i]!=0)
      {
        Error_new(ERROR_INFO,"Map.c: Map[%d] is used with %d elements.\n",i, List_getSize(this->htable[i]));
        for (j=0; j<List_getSize(this->htable[i]);j++)
        {
          List_resetIterator(this->htable[i]);
          n = (MapEntry*)List_getNext(this->htable[i]);
          if  (n!=0)
          {
            p = (char*)(String_getBuffer((String*)MapEntry_getItem(n)));
            Error_new(ERROR_INFO,"Item %d: %x %x %x %x\n", j+1, *p, *(p+1), *(p+2), *(p+3));
          }
          else
          {
            Error_new(ERROR_INFO,"Item %d: void\n", j+1);
          }
          n = (MapEntry*)List_getNext(this->htable[i]);
        }
      }
    }
  }
}

/**********************************************//**
  @brief Provide the size of a Map instance
  @public
  @memberof Map
  @return Size in bytes
**************************************************/
PUBLIC unsigned int Map_getSize(Map* this)
{
  if (this != 0)
  {
    return sizeof(Map);
  }
  else
  {
    return sizeof(Map);
  }
}
/**********************************************//** 
  @brief Get all the entries in an instance of a Map.
  @public
  @memberof Map
  @return List of map objects
**************************************************/
PUBLIC List * Map_getAll(Map * this)
{
  List * result = 0;
  int i = 0;
  void * pItem = 0;
  MapEntry * n = 0;
  
  result = List_new();
  for (i=0; i<HTABLE_SIZE; i++)
  {
    if (this->htable[i]!=0)
    {
      List_resetIterator(this->htable[i]);
      n = (MapEntry*)List_getNext(this->htable[i]);
      while (n!= 0)
      {
        pItem =  MapEntry_getItem(n);
        List_insertHead(result, pItem);
        n = (MapEntry*)List_getNext(this->htable[i]);
      }
    }
  }
  
  return result;
}
