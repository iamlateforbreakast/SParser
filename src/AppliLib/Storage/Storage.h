/* Storage.h */
#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Types.h"

typedef struct Storage Storage;

PUBLIC Storage * Storage_new();
PUBLIC void Storage_delete(Storage * this);
// "SELECT * FROM Include_Nodes WHERE Name='%s'
// "SELECT * FROM Nodes WHERE NodeId=%d;
PUBLIC void Storage_select();
// INSERT INTO Include_Nodes (NodeId, Name, EntryNode)
// INSERT INTO Code_Nodes (NodeId, Code)
//   "INSERT INTO Comment_Nodes (NodeId, Comment) "
PUBLIC void Storage_insert();
// UPDATE Include_Nodes SET EntryNode = %d WHERE NodeId = %d;

// UPDATE Nodes SET NodeNext = %d WHERE NodeId = %d;
PUBLIC void Storage_update();

/* "CREATE TABLE Nodes ("
   "NodeId integer PRIMARY_KEY,"
   "NodeType integer NOT NULL,"
   "NodePtr integer NOT NULL,"
   "NodeNext integer,"
   "NodePrev integer"
   ");");*/
PUBLIC void Storage_create();
// "DROP TABLE "
PUBLIC void STorage_drop();
#endif /* _STORAGE_H_ */

