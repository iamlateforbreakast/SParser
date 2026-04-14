/**********************************************//** 
  @file Map.c
 
  @brief A Map class.
  This class provides a container of Objects indexed
  by a string.
**************************************************/

#include "Map.h"
#include "MapNode.h"
#include "List.h"
#include "Class.h"
#include "Object.h"
#include "String2.h"
#include "Memory.h"
#include "Debug.h"
#include "Error.h"

#define DEBUG (0)
#define HTABLE_SIZE (50)

/**********************************************//**
  @private
**************************************************/
PRIVATE unsigned int Map_hash(Map * self, char * s, unsigned int i);
PRIVATE MapNode * Map_findEntry(Map* self, String * s);

/**********************************************//**
  @class Map
**************************************************/
struct Map
{
  Object object;
  MapNode * htable[HTABLE_SIZE];
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
  .f_size = (Sizer)&Map_getSize,
  .classSize = sizeof(Map)
};

/********************************************************//**
  @brief Create a new instance of the class Map.
  @public
  @memberof Map
  @return New Map instance or NULL if failed to allocate.
************************************************************/
PUBLIC Map* Map_new()
{
  Map * self = 0;
  
  self = (Map*)Object_new(sizeof(Map),&mapClass);

  if (OBJECT_IS_INVALID(self)) return 0;

  for (int i = 0; i < HTABLE_SIZE; i++)
  {
    self->htable[i] = 0;
  }

  return self;
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
  Map* self = 0;

  self = (Map*)Object_newFromAllocator(&mapClass, (Allocator*)allocator);

  if (OBJECT_IS_INVALID(self)) return 0;

  for (int i = 0; i < HTABLE_SIZE; i++)
  {
    self->htable[i] = 0;
  }

  return self;
}

/**********************************************//** 
  @brief Delete an instance of the class Map.
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_delete(Map * self)
{
  if (OBJECT_IS_INVALID(self)) return;

  /* De-allocate the specific members */
  for (int i = 0; i < HTABLE_SIZE; i++)
  {
    MapNode * n = self->htable[i];
    if (n!=0)
    {
      MapNode_delete(n);
      self->htable[i] = 0;
    }
  }
  /* De-allocate the base object */
  Object_deallocate(&self->object);
}

/**********************************************//** 
  @brief Copy an instance of the class Map.
  @public
  @memberof Map
  @return Copy of instance of NULL if failed to allocate.
**************************************************/
PUBLIC Map * Map_copy(Map * self)
{
  Map * result = 0;
  
  return result;
}

PUBLIC int Map_comp(Map* self, Map* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Insert an object into a Map instance
  @public
  @memberof Map
  @return 1 is inserted
**************************************************/
PUBLIC unsigned int Map_insert(Map * self, Handle * string, Handle * item)
{
  unsigned int result = 0;
  unsigned int key = 0;
  unsigned int i = 0;
  void * entry =0;
  MapNode * me = 0;
  
  if (OBJECT_IS_INVALID(self)) return 0;

  if (item == 0) return 0;
   
  if (string == 0) return 0;

  /* Check if there is an entry under s */
  if ((me = Map_findEntry(self, (String*)string->object))!=0)
  {
    /* Entry already exists */
    return 0;
  }
  else
  {
    /* Create a new entry */
    key = Map_hash(self,String_getBuffer((String*)string->object), i);
    if (self->htable[key] == 0)
    {
      entry = MapNode_new(string, item);
      self->htable[key] = entry;

      result = 1;
    }
    else
    {
      /* Collision */
      entry = MapNode_new(string, item);
      for (i=0; i<HTABLE_SIZE; i++)
      {
        if (self->htable[(key + i) % HTABLE_SIZE] == 0)
        {
          self->htable[(key + i) % HTABLE_SIZE] = entry;
          break;
        }
      }
      
      result = 1;
    }
  }
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Map
  @return 1 if found
**************************************************/
PUBLIC unsigned int Map_find(Map* self, String* s, void** p)
{
  if (OBJECT_IS_INVALID(self)) return 0;

  unsigned int result = 0;
  MapNode * n = 0;
  
  n = Map_findEntry(self, s);
  
  if (n!=0)
  {
    *p = (MapNode_getItem(n));
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
PRIVATE unsigned int Map_hash(Map * self, char * s, unsigned int length)
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
PRIVATE MapNode * Map_findEntry(Map* self, String * s)
{
  MapNode * result = 0;
  unsigned int key = 0;
  unsigned int i = 0;
  unsigned int isFound = 0;
  MapNode * n = 0;
  
  key = Map_hash(self, String_getBuffer(s), String_getLength(s));
  if (self->htable[key] != 0)
  {
    n = self->htable[key];
    if (String_compare(MapNode_getString(n), s)==0)
    {
      isFound = 1;
      result = n;
    }
    else
    {
      /* Collision */
      for (i=0; i<HTABLE_SIZE; i++)
      {
        n = self->htable[(key + i) % HTABLE_SIZE];
        if (n!=0)
        {
          if (String_compare(MapNode_getString(n), s)==0)
          {
            isFound = 1;
            result = n;
            break;
          }
        }
        else
        {
          break;
        }
      }
    } 
    n = self->htable[key];
  }
  
  return result;
}

/**********************************************//**
  @brief Print a Map instance
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_print(Map * self)
{
  int i = 0;
  MapNode * n = 0;
  int j = 0;
  char * p = 0;
  
  if (OBJECT_IS_INVALID(self)) return;

  for (i=0;i<HTABLE_SIZE;i++)
  {
    if (self->htable[i]!=0)
    {
      n = self->htable[i];
      p = (char*)(String_getBuffer((String*)MapNode_getString(n)));
      Error_new(ERROR_INFO,"Item %d: %s\n", i, p);
    }
    else
    {
      Error_new(ERROR_INFO,"Item %d: void\n", j+1);
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
}
/**********************************************//** 
  @brief Get all the entries in an instance of a Map.
  @public
  @memberof Map
  @return List of map objects
**************************************************/
PUBLIC List * Map_getAll(Map * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;
  
  List * result = 0;
  int i = 0;
  void * pItem = 0;
  MapNode * n = 0;
  
  result = List_new();
  for (i=0; i<HTABLE_SIZE; i++)
  {
    if (this->htable[i]!=0)
    {
      n = this->htable[i];
      pItem =  MapNode_getItem(n);
    }
  }
  
  return result;
}
