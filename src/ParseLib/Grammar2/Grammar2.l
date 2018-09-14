%{
#include <stdio.h>
#include "FileReader.h"
#include "SdbMgr.h"
#include "SdbRequest.h"
#include "Grammar2.parse.h"
#define YY_DECL int Grammar2_lex(YYSTYPE * yylval_param, void * yyscanner, SdbMgr * sdbMgr)
//#include "Node.h"

//Node * currentNode;
#define MAX_BUFFER_SIZE  (16384)
char buffer[MAX_BUFFER_SIZE];
char * node_text = NULL;
int node_text_position = 0;

void count(void * yyscanner);
int start_node_comment(void * yyscanner, int type);
int finish_node_comment(void * yyscanner, SdbMgr * sdbMgr);
int start_node_code(void * yyscanner);
int finish_node_code(void * yyscanner, SdbMgr *sdbMgr);
int add_node_comment(void * yyscanner);
int add_node_code(void * yyscanner);
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
"*/"           finish_node_comment(yyscanner, sdbMgr); return COMMENT;
.|\n           add_node_comment(yyscanner);
<<EOF>>        finish_node_comment(yyscanner, sdbMgr);return 0;
}
<IN_SL_COMMENT>{
\n             finish_node_comment(yyscanner, sdbMgr); return COMMENT;
.              add_node_comment(yyscanner);
<<EOF>>        finish_node_comment(yyscanner, sdbMgr); yy_push_state(INITIAL, yyscanner);return COMMENT;
}
<IN_CODE>{
"/*"           finish_node_code(yyscanner, sdbMgr);start_node_comment(yyscanner, 1);return CODE;
"//"           finish_node_code(yyscanner, sdbMgr);start_node_comment(yyscanner, 2);return CODE;
.|\n           add_node_code(yyscanner);
<<EOF>>        finish_node_code(yyscanner, sdbMgr);yy_push_state(INITIAL, yyscanner);return CODE;
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
  //node_text = malloc(node_max_size_bytes);
  
  return result;
}

int add_node_comment(void * yyscanner)
{
  int result = 0;
  char * text = yyget_text(yyscanner);
 
  buffer[node_text_position] = text[0];
  node_text_position++;
  if (node_text_position>(MAX_BUFFER_SIZE-1)) 
  {
    printf("Node text buffer too small\n");
    exit(0);
  }
  
  return result;
}

int finish_node_comment(void * yyscanner, SdbMgr * sdbMgr)
{
    SdbRequest * insertCommentNode = 0;
  
  insertCommentNode = SdbRequest_new(
  "INSERT INTO Comment_Nodes (Comment) "
  "VALUES ('%s');"
  );
  
  yy_push_state(INITIAL, yyscanner);
  buffer[node_text_position] = 0;
  printf("\nComment found: %s\n", buffer);
  node_text_position = 0;
  
  SdbRequest_execute(insertCommentNode, buffer);
  SdbRequest_delete(insertCommentNode);
  
  return COMMENT;
}

int start_node_code(void * yyscanner)
{
  int result = 0;
  char * text = yyget_text(yyscanner);

  yy_push_state(IN_CODE, yyscanner);
  buffer[node_text_position] = text[0];
  node_text_position++;
  
  return result;
}

int add_node_code(void * yyscanner)
{
  int result = 0;
  char * text = yyget_text(yyscanner);
  
  buffer[node_text_position] = text[0];
  node_text_position++;
  if (node_text_position>(MAX_BUFFER_SIZE-1)) 
  {
    printf("Node text buffer too small\n");
    exit(0);
  }
  
  return result;
}

int finish_node_code(void * yyscanner, SdbMgr * sdbMgr)
{
  SdbRequest * insertCodeNode = 0;
  
  insertCodeNode = SdbRequest_new(
  "INSERT INTO Code_Nodes (Code) "
  "VALUES ('%s');"
  );
  
  buffer[node_text_position] = 0;
  node_text_position++;
  printf("\nCode found: %s\n", buffer);
  node_text_position = 0;
  
  SdbRequest_execute(insertCodeNode, buffer);
  SdbRequest_delete(insertCodeNode);
  
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

