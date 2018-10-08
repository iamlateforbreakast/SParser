%{
#include "FileReader.h"
#include "SdbMgr.h"
//void yyerror(int * p, const char *s);
void yyerror(void * scanner, SdbMgr * sdbMgr, FileReader * fileReader, char const *s);
int yylex(void * yylval_param, void * yyscanner, SdbMgr * sdbMgr, FileReader * fileReader);

%}

%pure-parser
%name-prefix "Grammar2_"
%output "Grammar2.parse.c"
%lex-param {void * scanner} 
%lex-param {SdbMgr * sdbMgr}
%lex-param {FileReader * fr}
%parse-param {void * scanner} 
%parse-param {SdbMgr * sdbMgr}
%parse-param {FileReader * fr}

%union {
  String * text;
}

%token COMMENT 
%token CODE
%token INCLUDE

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

void yyerror(void * scanner, SdbMgr * sdbMgr, FileReader * fileReader, char const *s)
{
  fflush(stdout);
  printf("\n%*s\n%*s\n", column, "^", column, s);
}
