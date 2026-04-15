/* MapNode.h */

#ifndef MAPNODE_H
#define MAPNODE_H

#include "Object.h"
#include "Class.h"
#include "Handle.h"

typedef struct MapNode MapNode;

PRIVATE MapNode * MapNode_new(Handle * string, Handle * object);
PRIVATE void MapNode_delete(MapNode * self);
PRIVATE int MapNode_getSize(MapNode * self);

/**********************************************//**
  @private Class Description
**************************************************/
struct MapNode
{
  Object object;
  Handle * string;
  Handle * item;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class mapNodeClass =
{
  .f_new = (Constructor)&MapNode_new,
  .f_delete = (Destructor)&MapNode_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0,
  .f_size = (Sizer)&MapNode_getSize,
  .classSize = sizeof(MapNode)
};

/********** */
PRIVATE MapNode * MapNode_new(Handle * string, Handle * object)
{
  /* Parameters string and object are guaranteed to be valid */

  MapNode * self = 0;

  self = (MapNode*)Object_new(sizeof(MapNode),&mapNodeClass);

  self->string = string;
  self->item = object;

  return self;
}

PRIVATE void MapNode_delete(MapNode * self)
{
  /* Parameter self is guaranteed to be valid */

  Handle_delete(self->string);
  Handle_delete(self->item);

  Object_delete(&self->object);
}

PRIVATE int MapNode_getSize(MapNode * self)
{
  return sizeof(MapNode);
}

PRIVATE String * MapNode_getString(MapNode * self)
{
  return (String*)Handle_getObject(self->string);
}

PRIVATE Object * MapNode_getItem(MapNode * self)
{
  return Handle_getObject(self->item);
}

#endif /* MAPNODE_H */
