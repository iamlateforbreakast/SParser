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
#define INITIAL_HTABLE_SIZE (50)
#define LOAD_FACTOR_THRESHOLD (75)

/**********************************************//**
  @private
**************************************************/
PRIVATE unsigned int Map_hash(Map * self, char * s, unsigned int i);
PRIVATE MapNode * Map_findEntry(Map* self, String * s);
PRIVATE int Map_resize(Map* self);

/**********************************************//**
  @class Map
**************************************************/
struct Map
{
  Object object;
  MapNode ** htable;
  int capacity;
  int count;
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

  self->capacity = INITIAL_HTABLE_SIZE;
  self->count = 0;

  self->htable = (MapNode**)Memory_alloc(sizeof(MapNode*) * self->capacity);
  if (self->htable == 0)
  {
    Map_delete(self);
    return 0;
  }

  for (int i = 0; i < self->capacity; i++)
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
PUBLIC Map* Map_newFromAllocator(Allocator* allocator)
{
  Map* self = 0;

  self = (Map*)Object_newFromAllocator(&mapClass, (Allocator*)allocator);

  if (OBJECT_IS_INVALID(self)) return 0;

  self->capacity = INITIAL_HTABLE_SIZE;
  self->count = 0;

  for (int i = 0; i < self->capacity; i++)
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
PUBLIC void Map_delete(Map* self)
{
  if (OBJECT_IS_INVALID(self)) return;

  /* De-allocate the specific members */
  for (int i = 0; i < self->capacity; i++)
  {
    MapNode* n = self->htable[i];
    if (n != 0)
    {
      MapNode_delete(n);
      self->htable[i] = 0;
    }
  }
  Memory_free(self->htable, sizeof(MapNode*) * self->capacity);

  /* De-allocate the base object */
  Object_deallocate(&self->object);
}

/**********************************************//**
  @brief Copy an instance of the class Map.
  @public
  @memberof Map
  @return Copy of instance or NULL if failed to allocate.
**************************************************/
PUBLIC Map* Map_copy(Map* self)
{
  if (OBJECT_IS_INVALID(self)) return 0;

  Map* copy = Map_new();

  if (OBJECT_IS_INVALID(copy)) return 0;

  for (int i = 0; i < self->capacity; i++)
  {
    /* Only copy slots that contain actual data */
    if (self->htable[i] != 0)
    {
      Handle * hKey = MapNode_getString(self->htable[i]);
      Handle * hItem = MapNode_getItem(self->htable[i]);

      if (Map_insert(copy, hKey, hItem) == 0)
      {
        /* Clean up and fail if an insertion fails (e.g., out of memory) */
        Map_delete(copy);
        return 0;
      }
    }
  }

  return copy;
}

/**********************************************//**
  @brief Compare an instance of the class Map.
  @public
  @memberof Map
  @return 0 if equal, 1 if different
**************************************************/
PUBLIC int Map_comp(Map* self, Map* compared)
{
  /* 1. Basic pointer and size check */
  if (self == compared) return 0;
  if (self->count != compared->count) return 1;

  /* 2. Iterate through self and verify every entry exists in 'compared' */
  for (int i = 0; i < self->capacity; i++)
  {
    /* Skip empty or tombstone slots */
    if (self->htable[i] != 0)
    {
      String* key = MapNode_getString(self->htable[i]);
      void* value = MapNode_getValue(self->htable[i]);

      /* Look up this key in the other map */
      void* comparedValue = Map_get(compared, key);

      /*
       * If key doesn't exist in 'compared' or values don't match,
       * maps are not equal.
       */
      if (comparedValue == 0 || !Value_isEqual(value, comparedValue))
      {
        return 1;
      }
    }
  }

  /* All keys and values matched */
  return 0;
}

/**********************************************//**
  @brief Insert an object into a Map instance
  @public
  @memberof Map
  @return 1 is inserted
**************************************************/
PUBLIC unsigned int Map_insert(Map* self, Handle* string, Handle* item)
{
  unsigned int result = 0;
  unsigned int key = 0;
  int i = 0;
  void* entry = 0;
  MapNode* me = 0;

  if (OBJECT_IS_INVALID(self)) return 0;

  if (item == 0) return 0;

  if (string == 0) return 0;

  if ((self->count * 100) > LOAD_FACTOR_THRESHOLD * self->capacity)
  {
    Map_resize(self);
  }
  /* Check if there is an entry under s */
  if ((me = Map_findEntry(self, (String*)Handle_getObject(string)))!=0)
  {
    /* Failed: Entry already exists */
    return 0;
  }
  else
  {
    /* Create a new entry */
    key = Map_hash(self,String_getBuffer((String*)Handle_getObject(string)), String_getLength((String*)Handle_getObject(string)));
    if (self->htable[key] == 0)
    {
      entry = MapNode_new(string, item);
      self->htable[key] = entry;
      self->count++;
      result = 1; /* Success: Inserted */
    }
    else
    {
      /* Collision */
      entry = MapNode_new(string, item);
      for (i=0; i<self->capacity; i++)
      {
        if (self->htable[(key + i) % self->capacity] == 0)
        {
          self->htable[(key + i) % self->capacity] = entry;
          self->count++;
          result = 1; /* success: Inserted */
          break;
        }
      }
      /* Failed: No space left to insert */
    }
  }
  
  return result;
}

/**********************************************//**
  @brief Retrieve an item from the Map by its key.
  @public
  @memberof Map
  @param s The string key to search for.
  @param[out] p Pointer to a pointer where the found item will be stored.
               Set to NULL if the key is not found.
  @return 1 if the key was found, 0 otherwise (or if the Map is invalid).
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
  @brief Print a Map instance
  @public
  @memberof Map
**************************************************/
PUBLIC void Map_print(Map* self)
{
  int i = 0;
  MapNode* n = 0;
  int j = 0;
  char* p = 0;

  if (OBJECT_IS_INVALID(self)) return;

  for (i = 0; i < self->capacity; i++)
  {
    if (self->htable[i] != 0)
    {
      n = self->htable[i];
      p = (char*)(String_getBuffer((String*)MapNode_getString(n)));
      Error_new(ERROR_INFO, "Item %d: %s\n", i, p);
    }
    else
    {
      Error_new(ERROR_INFO, "Item %d: void\n", j + 1);
    }
  }
}

/**********************************************//**
  @brief Provide the size of a Map instance
  @public
  @memberof Map
  @return Size in bytes
**************************************************/
PUBLIC unsigned int Map_getSize(Map* self)
{
  if (self == 0) return 0;

  return sizeof(Map) + (self->count * sizeof(MapNode));
}

/**********************************************//**
  @brief Get all the entries in an instance of a Map.
  @public
  @memberof Map
  @return List of map objects
**************************************************/
PUBLIC List* Map_getAll(Map* self)
{
  if (OBJECT_IS_INVALID(self)) return 0;

  List* result = 0;
  int i = 0;
  void* pItem = 0;
  MapNode* n = 0;

  result = List_new();
  for (i = 0; i < self->capacity; i++)
  {
    if (self->htable[i] != 0)
    {
      n = self->htable[i];
      pItem = MapNode_getItem(n);
      List_insertHead(result, pItem, 0);
    }
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
  result = result % self->capacity;
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
  int i = 0;
  unsigned int isFound = 0;
  MapNode * n = 0;
  
  unsigned int key = String_hash(s) % self->capacity;
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
      for (i = 1; i<self->capacity; i++)
      {
        n = self->htable[(key + i) % self->capacity];
        if (n!=0)
        {
          if (String_compare(MapNode_getString(n), s)==0)
          {
            isFound = 1;
            result = n;
            break;
          }
        }
      }
    } 
  }
  
  return result;
}

/**********************************************//**
  @brief Increases the map's capacity by doubling it.

  Allocates a new hash table and re-hashes all existing entries
  using open addressing to resolve collisions in the new space.

  @private
  @memberof Map
  @return 1 if successful, 0 if memory allocation failed.
**************************************************/
PRIVATE int Map_resize(Map* self)
{
  /* self is guaranteed to be valid */
  
  int newCapacity = self->capacity * 2;
  MapNode** newHtable = (MapNode**)Memory_alloc(sizeof(MapNode*) * newCapacity);

  if (newHtable == 0) return 0;

  Memory_set(newHtable, 0, sizeof(MapNode*) * newCapacity);

  for (int i = 0; i < self->capacity; i++)
  {
    if (self->htable[i] != 0)
    {
      String* key = MapNode_getString(self->htable[i]);
      unsigned int hash = String_hash(key) % newCapacity;

      /* Collision case */
      if (newHtable[hash] != 0)
      {
        for (int j = 1; j < newCapacity; j++)
        {
          if (self->htable[(hash + j) % newCapacity] != 0)
          {
            hash = (hash + j) % newCapacity;
            break;
          }
        }
      }
      newHtable[hash] = self->htable[i];
      self->htable[i] = 0;
    }
  }
  Memory_free(self->htable, sizeof(MapNode*) * self->capacity);
  self->htable = newHtable;
  self->capacity = newCapacity;

  return 1;
}
