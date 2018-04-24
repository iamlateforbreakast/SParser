%{
void yyerror(int * p, const char *s);
int yylex(int * p);
%}

%name-prefix "Grammar1_"
%output "Grammar1.parse.c"
%lex-param {int * p}
%parse-param {int * p}

%token COMMENT STATEMENT

%start translation_unit
%%

comment
  : COMMENT
  ;

statement
  : STATEMENT
  ;

node
  : comment
  | statement
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
