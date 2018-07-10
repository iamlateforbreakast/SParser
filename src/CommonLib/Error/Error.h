/* Error.h */

#include "Types.h"

typedef enum
{
  ERROR_DBG,
  ERROR_INFO,
  ERROR_NORMAL,
  ERROR_FATAL
} ErrorSeverity;

PUBLIC void Error_new(ErrorSeverity severity, char * msg, ...);
