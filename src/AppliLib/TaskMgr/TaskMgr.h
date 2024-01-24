/* TaskMgr.h */
#ifndef _TASKMGR_H_
#define _TASKMGR_H_

#include "Types.h"

typedef struct TaskMgr TaskMgr;

PUBLIC TaskMgr * TaskMgr_new();
PUBLIC void TaskMgr_delete(TaskMgr * this);

// create Async task(function)
// create Sync task(function)
#endif /* _TASKMGR_H_ */
