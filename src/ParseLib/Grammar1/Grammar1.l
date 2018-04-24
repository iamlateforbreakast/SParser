%{
#include <stdio.h>
#include "Grammar1.parse.h"
#define YY_DECL Grammar1_lex(void)
//#include "Node.h"

//Node * currentNode;

void count(void);
int macro_statement(void);
int ml_comment(void);
int sl_comment(void);
%}

%option prefix="Grammar1"

%%
"/*"			{ return(ml_comment()); }
"//"                    { return(sl_comment()); }
"#"			{ return(macro_statement()); }
%%

int yywrap(void)
{
  return 1;
}

int macro_statement(void)
{
}

int sl_comment(void)
{
}

int ml_comment(void)
{
  char c, prev = 0;

  while ((c = input()) != 0)      /* (EOF maps to 0) */
  {
    if (c == '/' && prev == '*')
      return(COMMENT);
    prev = c;
  }
  error("unterminated comment");
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

