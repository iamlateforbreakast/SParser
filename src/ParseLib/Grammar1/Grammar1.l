%{
#include <stdio.h>
#include "Grammar1.parse.h"
#define YY_DECL Grammar1_lex(void)
#define MEMORY_BLOCK_BYTES (500)
//#include "Node.h"

//Node * currentNode;

char * node_text = NULL;
int node_max_size_bytes = MEMORY_BLOCK_BYTES;
int node_text_position = 0;

void count(void);
int start_node_comment(int type);
int finish_node_comment();
int start_node_code();
int finish_node_code();
int add_node_commment();
%}

%option prefix="Grammar1"
%x IN_ML_COMMENT
%x IN_SL_COMMENT
%x IN_CODE

%%
<INITIAL>{
"/*"		   start_node_comment(1);
"//"           start_node_comment(2);
.|\n           start_node_code();
<<EOF>>        return(0);
}
<IN_ML_COMMENT>{
"*/"           finish_node_comment(); return COMMENT;
.|\n           add_node_comment();
<<EOF>>        finish_node_comment();return 0;
}
<IN_SL_COMMENT>{
\n             finish_node_comment(); return COMMENT;
.              add_node_comment();
<<EOF>>        finish_node_comment(); BEGIN(INITIAL);return COMMENT;
}
<IN_CODE>{
"/*"           finish_node_code();start_node_comment(1);return CODE;
"//"           finish_node_code();start_node_comment(2);return CODE;
.|\n           add_node_code();
<<EOF>>        finish_node_code();BEGIN(INITIAL);return CODE;
}

%%

int start_node_comment(int type)
{
  int result=0;
  
  if (type==1)
  {
    BEGIN(IN_ML_COMMENT);
  }
  if (type==2)
  {
    BEGIN(IN_SL_COMMENT);
  }
  node_text = malloc(node_max_size_bytes);
  
  return result;
}

int add_node_comment()
{
  int result = 0;
  char * temp = 0;
  
  node_text[node_text_position] = yytext[0];
  node_text_position++;
  if (node_text_position>(node_max_size_bytes-1)) 
  {
    printf("Node text buffer too small\n");
	node_max_size_bytes+=MEMORY_BLOCK_BYTES ;
    temp = (char *)realloc(node_text, node_max_size_bytes);
    node_text = temp;
  }
  
  return result;
}

int finish_node_comment()
{
  int result=0;
  
  BEGIN(INITIAL);
  node_text[node_text_position] = 0;
  printf("\nComment found: %s\n", node_text);
  node_text_position = 0;
  node_max_size_bytes = MEMORY_BLOCK_BYTES ;
  free(node_text);
  
  
  return COMMENT;
}

int start_node_code()
{
  int result = 0;
  
  BEGIN(IN_CODE);
  node_text = malloc(node_max_size_bytes);
  node_text[node_text_position] = yytext[0];
  node_text_position++;
  
  return result;
}

int add_node_code()
{
  int result = 0;
  char * temp = 0;
  
  node_text[node_text_position] = yytext[0];
  node_text_position++;
  if (node_text_position>(node_max_size_bytes-1)) 
  {
    printf("Node text buffer too small\n");
    node_max_size_bytes+=MEMORY_BLOCK_BYTES ;
    temp = (char *)realloc(node_text, node_max_size_bytes);
    node_text = temp;
  }
  
  return result;
}

int finish_node_code()
{
  int result = 0;
  
  node_text[node_text_position] = 0;
  printf("\nCode found: %s\n", node_text);
  node_text_position = 0;
  node_max_size_bytes = MEMORY_BLOCK_BYTES ;
  free(node_text);
  return CODE;
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

