%{
#include "Grammar2.h"
void yyerror(void * scanner, Grammar2 * grammar, char const *s);
//int yylex(void * yylval_param, void * yyscanner, SdbMgr * sdbMgr, FileReader * fileReader);
int yylex(void * yylval_param, void * yyscanner, Grammar2 * grammar);
%}

%pure-parser
%name-prefix "Grammar2_"
%output "Grammar2.parse.c"
%lex-param {void * scanner}
%lex-param {Grammar2 * grammar}
%parse-param {void * scanner} 
%parse-param {Grammar2 * grammar}

%union {
  String * text;
}

%token COMMENT 
%token CODE
%token END_OF_UNIT

%start translation_unit
%%

comment
  : COMMENT { Grammar2_addComment(grammar); }
  ;

code
  : CODE { Grammar2_addCodeNode(grammar); }
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

void yyerror(void * scanner, Grammar2 * grammar, char const *s)
{
  fflush(stdout);
  printf("\n%*s\n%*s\n", column, "^", column, s);
}
