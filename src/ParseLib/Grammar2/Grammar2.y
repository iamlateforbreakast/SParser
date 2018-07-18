%{
#include "FileReader.h"
#include "SdbMgr.h"
//void yyerror(int * p, const char *s);
//int yylex(int * p);
%}

%pure-parser
%name-prefix "Grammar2_"
%output "Grammar2.parse.c"
%lex-param {void * scanner} 
%lex-param {FileReader * fr}
%lex-param {SdbMgr * sdbMgr}
%parse-param {void * scanner} 
%parse-param {FileReader * fr}
%parse-param {SdbMgr * sdbMgr}

%token COMMENT CODE

%start translation_unit
%%

comment
  : COMMENT
  ;

code
  : CODE
  ;

node
  : comment
  | code
  ;

translation_unit
  : node
  | translation_unit node
  ;

%%
#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(int * p, char const *s)
{
  fflush(stdout);
  printf("\n%*s\n%*s\n", column, "^", column, s);
}
