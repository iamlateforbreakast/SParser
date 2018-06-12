%{
#include <stdio.h>
#include "Grammar2.parse.h"
#define YY_DECL int Grammar2_lex(YYSTYPE * yylval_param, void * yyscanner)
//#include "Node.h"

//Node * currentNode;

char * node_text = NULL;
int node_max_size_bytes = 500;
int node_text_position = 0;

void count(void * yyscanner);
int start_node_comment(void * yyscanner, int type);
int finish_node_comment(void * yyscanner);
int start_node_code(void * yyscanner);
int finish_node_code(void * yyscanner);
int add_node_commment(void * yyscanner);
%}

%option noyywrap
%option stack
%option reentrant
%option prefix="Grammar2"
%option bison-bridge
%x IN_ML_COMMENT
%x IN_SL_COMMENT
%x IN_CODE

%%
<INITIAL>{
"/*"		   start_node_comment(yyscanner, 1);
"//"           start_node_comment(yyscanner, 2);
.|\n           start_node_code(yyscanner);
<<EOF>>        return(0);
}
<IN_ML_COMMENT>{
"*/"           finish_node_comment(yyscanner); return COMMENT;
.|\n           add_node_comment(yyscanner);
<<EOF>>        finish_node_comment(yyscanner);return 0;
}
<IN_SL_COMMENT>{
\n             finish_node_comment(yyscanner); return COMMENT;
.              add_node_comment(yyscanner);
<<EOF>>        finish_node_comment(yyscanner); yy_push_state(INITIAL, yyscanner);return COMMENT;
}
<IN_CODE>{
"/*"           finish_node_code(yyscanner);start_node_comment(yyscanner, 1);return CODE;
"//"           finish_node_code(yyscanner);start_node_comment(yyscanner, 2);return CODE;
.|\n           add_node_code(yyscanner);
<<EOF>>        finish_node_code(yyscanner);yy_push_state(INITIAL, yyscanner);return CODE;
}

%%

int start_node_comment(void * yyscanner, int type)
{
  int result=0;
  
  if (type==1)
  {
    yy_push_state(IN_ML_COMMENT, yyscanner);
  }
  if (type==2)
  {
    yy_push_state(IN_SL_COMMENT, yyscanner);
  }
  node_text = malloc(node_max_size_bytes);
  
  return result;
}

int add_node_comment(void * yyscanner)
{
  int result = 0;
  char * text = yyget_text(yyscanner);
  
  node_text[node_text_position] = text[0];
  node_text_position++;
  if (node_text_position>(node_max_size_bytes-1)) 
  {
    printf("Node text buffer too small\n");
    exit(0);
  }
  
  return result;
}

int finish_node_comment(void * yyscanner)
{
  int result=0;
  
  yy_push_state(INITIAL, yyscanner);
  node_text[node_text_position] = 0;
  printf("\nComment found: %s\n", node_text);
  node_text_position = 0;
  node_max_size_bytes = 500;
  free(node_text);
  
  
  return COMMENT;
}

int start_node_code(void * yyscanner)
{
  int result = 0;
  char * text = yyget_text(yyscanner);

  yy_push_state(IN_CODE, yyscanner);
  node_text = malloc(node_max_size_bytes);
  node_text[node_text_position] = text[0];
  node_text_position++;
  
  return result;
}

int add_node_code(void * yyscanner)
{
  int result = 0;
  char * text = yyget_text(yyscanner);
  
  node_text[node_text_position] = text[0];
  node_text_position++;
  if (node_text_position>(node_max_size_bytes-1)) 
  {
    printf("Node text buffer too small\n");
    exit(0);
  }
  
  return result;
}

int finish_node_code(void * yyscanner)
{
  int result = 0;
  
  node_text[node_text_position] = 0;
  printf("\nCode found: %s\n", node_text);
  node_text_position = 0;
  node_max_size_bytes = 500;
  free(node_text);
  return CODE;
}

//int yywrap(void)
//{
//  return 1;
//}

int column = 0;

void count(void * yyscanner)
{
  int i;
  char * text = yyget_text(yyscanner);

  for (i = 0; text[i] != '\0'; i++)
    if (text[i] == '\n')
      column = 0;
    else if (text[i] == '\t')
      column += 8 - (column % 8);
    else
      column++;

    /*ECHO;*/
}

