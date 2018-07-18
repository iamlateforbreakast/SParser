
#include "Grammar2.h"
#include "Object.h"
#include "Grammar2.parse.h"

struct Grammar2
{
  Object object;
  void * scanner;
  SdbMgr * sdbMgr;
  FileReader * reader;
};

Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr)
{
  Grammar2 * this = 0;

  this = (Grammar2*)Object_new(sizeof(Grammar2),(Destructor)&Grammar2_delete, (Copy_operator)&Grammar2_copy);
  this->reader = fr;
  this->sdbMgr = sdbMgr;
  Grammar2lex_init(&this->scanner);

  return this;
}

void Grammar2_delete(Grammar2 * this)
{
  if (this!=0)
  {
     if (this->object.refCount==1)
     {
       Grammar2lex_destroy(this->scanner);
       Object_delete(&this->object);
     }
     else if (this->object.refCount>1)
     {
       this->object.refCount--;
     }
  }
}

PUBLIC Grammar2 * Grammar2_copy(Grammar2 * this)
{
  Grammar2 * copy = 0;

  return copy;
}

PUBLIC void Grammar2_process(Grammar2 * this)
{
  Grammar2_scan_string(FileReader_getBuffer(this->reader), this->scanner);
  Grammar2set_in(FileReader_getBuffer(this->reader), this->scanner);
  Grammar2_parse(this->scanner, this->reader, this->sdbMgr);
}
