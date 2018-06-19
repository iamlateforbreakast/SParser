/* SdbRequest.h */
#ifndef _SDBREQUEST_H_
#define _SDBREQUEST_H_

#include "Types.h"

typedef struct SdbRequest SdbRequest;

PUBLIC SdbRequest * SdbRequest_new();
PUBLIC void SdbRequest_delete(SdbRequest * this);
PUBLIC SdbRequest * SdbRequest_copy(SdbRequest * this);

#endif /* _SDBREQUEST_H_ */