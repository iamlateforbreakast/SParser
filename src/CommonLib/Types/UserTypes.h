/* User Types */
#ifndef _USERTYPES_
#define _USERTYPES_
#include "Class.h"
extern Class listClass;
extern Class stringClass;

enum ClassId
{
  ListClass,
  StringClass,
  NB_CLASSES
};

/*Class * userTypes[] =
{
  &listClass,
  &stringClass,
};*/
#endif /* UserTypes.h */
