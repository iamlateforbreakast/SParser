/**********************************************//** 
  @file StreamParser.c
  
  @class StreamParser
  
  @brief The StreamParser class TBD
  
  The class StreamParser is process a stream of char and extract:
  - Multiline C comments
  - Single C comments
  - Expand #include
  - Expand macros
**************************************************/

#include "StreamParser.h"

struct StreamParser
{
};

StreamParser* StreamParser_new(/* Filereader fileReader */)
{
}

void StreamParser_delete(StreamParser* this)
{
}

void StreamParser_parse(StreamParser* this /*, FileName* f*/)
{
    yyyparse();
}

char StreamParser_parse2(StreamParser * this)
{
  char c = 0;
  
  /* While char returned by FileReader */
  
  /* Strip single and multiline comments */
  
  /* Detect include files */
  
  /* Detect macros */
  
  /* until char found */
  
  return c;
}

void StreamParser_stripComments(StreamParser * this)
{
}

void StreamParser_detectMacro(StreamParser * this)
{
}

void StreamParser_expandMacro(StreamParser * this)
{
}

