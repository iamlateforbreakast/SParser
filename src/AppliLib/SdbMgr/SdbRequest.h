/* SdbRequest.h */
#ifndef _SDBREQUEST_H_
#define _SDBREQUEST_H_

#include "Types.h"
#include "List.h"

typedef struct SdbRequest SdbRequest;

PUBLIC SdbRequest * SdbRequest_new(const char * fmt);
PUBLIC void SdbRequest_delete(SdbRequest * this);
PUBLIC SdbRequest * SdbRequest_copy(SdbRequest * this);
PUBLIC unsigned int SdbRequest_getSize(SdbRequest * this);
PUBLIC void SdbRequest_execute(SdbRequest * this, ...);
PUBLIC unsigned int SdbRequest_getNbResult(SdbRequest * this);
PUBLIC List * SdbRequest_getResults(SdbRequest * this);

#endif /* _SDBREQUEST_H_ */
