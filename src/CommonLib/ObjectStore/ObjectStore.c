/* ObjectStore.c */

#include "ObjectStore.h"
#include "Memory.h"

typedef struct PoolInfo
{
  Pool * ptr;
  Pool * prevPool;
  Pool * nextPool; 
} PoolInfo;

struct ObjectStore
{
  PoolInfo * poolList;
};

PUBLIC void ObjectStore_delete(ObjectStore * this)
{
}

PUBLIC ObjectStore * ObjectStore_getRef()
{
}

PUBLIC Pool * ObjectStore_createPool(ObjectStore * this)
{
}

PUBLIC void ObjectStore_deletePool(ObjectStore * this, Pool * pool)
{
}

#endif /* _OBJECTSTORE_H_ */
