/* TaskMgr.h */
#ifndef _TASKMGR_H_
#define _TASKMGR_H_

#include "Types.h"

typedef struct TaskMgr TaskMgr;

PUBLIC TaskMgr * TaskMgr_new();
PUBLIC void TaskMgr_delete(TaskMgr * this);
PUBLIC void TaskMgr_print(TaskMgr * this);
PUBLIC unsigned int TaskMgr_getSize(TaskMgr * this);
#endif /* _TASKMGR_H_ */
