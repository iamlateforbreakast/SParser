%{
#include <stdio.h>
#include "Grammar1.parse.h"
#define YY_DECL Grammar1_lex(void)
//#include "Node.h"

//Node * currentNode;

void count(void);
int start_node_comment();
int finish_node_comment();
int start_node_code();
int finish_node_code();
%}

%option prefix="Grammar1"
%x IN_ML_COMMENT
%x IN_SL_COMMENT
%x IN_CODE

%%
<INITIAL>{
"/*"			start_node_comment();
"//"           BEGIN(IN_SL_COMMENT);
.               BEGIN(IN_CODE);
}
<IN_ML_COMMENT>{
"*/"           finish_node_comment();
.|\n            ;
}
<IN_SL_COMMENT>{
\n          BEGIN(INITIAL);
.            ;
}
<IN_CODE>{
"/*"          BEGIN(IN_ML_COMMENT); yyless(2);
"//"           BEGIN(IN_SL_COMMENT);
}
[//t/n]        ;

%%

int start_node_comment()
{
  int result=0;
  
  BEGIN(IN_ML_COMMENT);
  
  return result;
}

int finish_node_comment()
{
  int result=0;
  
  BEGIN(INITIAL);
  
  return result;
}

int start_node_code()
{
  int result = 0;
  
  return result;
}

int finish_node_code()
{
  int result = 0;
  
  return result;
}

int yywrap(void)
{
  return 1;
}

int column = 0;

void count(void)
{
  int i;

  for (i = 0; yytext[i] != '\0'; i++)
    if (yytext[i] == '\n')
      column = 0;
    else if (yytext[i] == '\t')
      column += 8 - (column % 8);
    else
      column++;

    ECHO;
}

