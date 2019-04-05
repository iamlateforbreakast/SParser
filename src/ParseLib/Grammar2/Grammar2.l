%{
#include <stdio.h>
#include "Grammar2.h"
#include "SdbRequest.h"
#include "Memory.h"
#include "Error.h"

#include "Grammar2.parse.h"
#define YY_DECL int Grammar2_lex(YYSTYPE * yylval_param, void * yyscanner, Grammar2 * grammar)
#define YY_NO_INPUT

/* TODO: Define YY_USER_INIT */
/* TODO: Define YY_USER_ACTION */

void count(void * yyscanner);
unsigned int read_include(void * yyscanner, Grammar2 * grammar);
int end_of_file(void * yyscanner);
YY_BUFFER_STATE get_current_buffer(void * yyscanner);
void pop_buffer(void * yyscanner);
void push_buffer(YY_BUFFER_STATE new_buffer, void * yyscanner);
int get_buffer_top(void * yyscanner);
%}

%option noyywrap
%option stack
%option reentrant
%option prefix="Grammar2"
%option bison-bridge

/* Defintions of states */
%x IN_ML_COMMENT
%x IN_SL_COMMENT
%x IN_CODE
%x IN_INCLUDE

/* Definitions */

/* Rules */
%%
<INITIAL>{
<<EOF>> {
                  Error_new(ERROR_DBG,"<INITIAL> EOF\n");
                  if (get_buffer_top(yyscanner) == 0)
				  {
				    Error_new(ERROR_DBG, "Terminate\n");		
					yyterminate();
				  }
				  else
				  {
                    Grammar2_returnToFile(grammar);
				    pop_buffer(yyscanner);
				  }
				}
.               {
                  yyless(yyleng-1);
				  yy_push_state(IN_CODE, yyscanner);
				}  
}

<IN_ML_COMMENT>{
"*/"           {
				  yy_pop_state(yyscanner); 
				  return COMMENT;
				}
<<EOF>> {
				  Error_new(ERROR_DBG,"<IN_ML_COMMENT> EOF\n");
				  yyterminate();
				}
.|\n           Grammar2_addToBuffer(grammar, yyget_text(yyscanner));
}

<IN_SL_COMMENT>{
\n             {
				  yy_pop_state(yyscanner); 
                  
				  return COMMENT;
				}
<<EOF>> {
                  Error_new(ERROR_DBG,"<IN_SL_COMMENT> EOF\n"); 
				  yy_pop_state(yyscanner); 
				  return COMMENT;
				}
.              Grammar2_addToBuffer(grammar, yyget_text(yyscanner));
}

<IN_CODE>{
"#include" { 
				  yy_push_state(IN_INCLUDE,yyscanner);
				  return CODE;
				}
"/*"           {
				  yy_push_state(IN_ML_COMMENT, yyscanner);
				  return CODE;
				}
"//"           {
				  yy_push_state(IN_SL_COMMENT, yyscanner);
				  return CODE;
				}
<<EOF>> {
                  Error_new(ERROR_DBG,"<CODE> EOF\n");
                  yy_push_state(INITIAL, yyscanner);
				  return CODE;
				}
.               {
                  //printf("c=%x\n", *yytext);
				  Grammar2_addToBuffer(grammar, yyget_text(yyscanner));
			    }
}

<IN_INCLUDE>{
[ \t]*
<<EOF>> {
                  Error_new(ERROR_DBG, "EOF6\n"); yyterminate();
				}
[^ \t\n]*  	{
				   read_include(yyscanner, grammar);
				   yy_pop_state(yyscanner);
				}
\n		//printf("CR!\n");
}

%%
unsigned int read_include(void * yyscanner, Grammar2 * grammar)
{
  char * c = 0;
  String * fileName = 0; //String_new(0);
  unsigned int i = 0;
  unsigned int result = 0;

  YY_BUFFER_STATE curBuf = get_current_buffer(yyscanner);
  char * text = yyget_text(yyscanner);
  i = Memory_len(text);
  
  // TODO: in some cases on windows system the string text contains tes.h/r/n instead of test.h/n
  Error_new(ERROR_DBG, "Gramar2.lex.c: Include file name =%s\n", text);
  //Error_new(ERROR_DBG,"Length =%d\n", i);
  
  char * buffer = Memory_alloc(i-1);
  Memory_set(buffer, 0, i-1);
  Memory_copy(buffer, &text[1], i-2);
  
  fileName = String_new(0);
  String_setBuffer(fileName, buffer);
  //Error_new(ERROR_DBG,"INCLUDE %s!!!\n", buffer);

  c = Grammar2_processNewFile(grammar, fileName);
  if (c!=0)
  {
    push_buffer(curBuf,yyscanner);
    yy_scan_string(c, yyscanner);
    result = 1;
  }
  else
  {
    Error_new(ERROR_NORMAL,"Grammar2.lex.c: Cannot find %s\n", String_getBuffer(fileName));
    result = 0;
  }
  String_delete(fileName);
  
  return result;
}

void push_buffer(YY_BUFFER_STATE new_buffer, void * yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  yypush_buffer_state(new_buffer,yyscanner);
  Error_new(ERROR_DBG, "PUSH State Stack Pointer = %d\n", yyg->yy_start_stack_ptr);
  Error_new(ERROR_DBG, "PUSH State Stack Top = %d\n", yyg->yy_buffer_stack_top);
  Error_new(ERROR_DBG, "PUSH State Stack Max. = %d\n", yyg->yy_buffer_stack_max);
}

void pop_buffer(void * yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  yypop_buffer_state(yyscanner);
  Error_new(ERROR_DBG, "POP State Stack Pointer = %d\n", yyg->yy_start_stack_ptr);
  Error_new(ERROR_DBG, "POP State Stack Top = %d\n", yyg->yy_buffer_stack_top);
  Error_new(ERROR_DBG, "POP State Stack Max. = %d\n", yyg->yy_buffer_stack_max);
}

YY_BUFFER_STATE get_current_buffer(void * yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  
  if (yyg->yy_buffer_stack)
    return yyg->yy_buffer_stack[yyg->yy_buffer_stack_top];
  else
    return NULL;
}

int get_buffer_top(void * yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  
  return yyg->yy_buffer_stack_top;
}
	
int end_of_file(void * yyscanner)
{
  (void)yyunput;
  (void)yy_top_state;
  return 0;
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

