#include "XmlReader.h"
#include "Debug.h"

PRIVATE FILE * logChannel;

PRIVATE int UT_XmlReader_01_step1()
{
  int isPassed = 1;

  return isPassed;
}

int main()
{
  int isPassed = 1;

  logChannel = Debug_openChannel("UT_List_01.log");
  
  isPassed = isPassed && UT_XmlReader_01_step1();

  Debug_closeChannel(logChannel);

  return isPassed;
}
