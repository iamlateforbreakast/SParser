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
int read_include(void * yyscanner, Grammar2 * grammar);
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
                  printf("<INITIAL> EOF\n");
				  yyterminate();
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
				  printf("<IN_ML_COMMENT> EOF\n");
				  yyterminate();
				}
.|\n           Grammar2_addToBuffer(grammar, yyget_text(yyscanner));
}

<IN_SL_COMMENT>{
\n             {
                  //Grammar2_addComment(grammar); 
				  yy_pop_state(yyscanner); 
				  return COMMENT;
				}
<<EOF>> {
                  printf("<IN_SL_COMMENT> EOF\n"); 
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
                  printf("\n");
				  if (get_buffer_top(yyscanner) == 0)
				  {
				    printf("Terminate\n");
					yy_push_state(INITIAL, yyscanner);
					return CODE;
				  }
				  else
				  {
                    Grammar2_returnToFile(grammar);
				    pop_buffer(yyscanner);
		            printf("<CODE> EOF\n");
				    return CODE;
				  }
				}
.               {
                  //printf("c=%x\n", *yytext);
				  Grammar2_addToBuffer(grammar, yyget_text(yyscanner));
			    }
}

<IN_INCLUDE>{
[ \t]*
<<EOF>> {
                  printf("EOF6\n"); yyterminate();
				}
[^ \t\n]*  	{
				   read_include(yyscanner, grammar);
				   yy_pop_state(yyscanner);
				}
\n		printf("CR!\n");
}

%%
int read_include(void * yyscanner, Grammar2 * grammar)
{
  char * c = 0;
  String * fileName = String_new(0);
  
  //print_state_stack(yyscanner);
  YY_BUFFER_STATE curBuf = get_current_buffer(yyscanner);
  char * text = yyget_text(yyscanner);
  printf("File name =%s\n", text);
  unsigned int l = Memory_len(text);
  printf("Length =%d\n", l);
  char * buffer = Memory_alloc(l);
  Memory_copy(buffer, &text[1], l-2);
  buffer[l-2] = 0;
  
  String_setBuffer(fileName, buffer);
  printf("INCLUDE %s!!!\n", buffer);
  /* G2_changeFile */
  //c = FileReader_addFile(Grammar2_getFileReader(grammar), fileName);
  c = Grammar2_processNewFile(grammar, fileName);
  if (c!=0)
  {
    push_buffer(curBuf,yyscanner);
    yy_scan_string(c, yyscanner);
  }
  else
  {
    Error_new(ERROR_NORMAL,"Cannot find %s\n", fileName);
  }
  String_delete(fileName);
  
  return 0;
}

#if 0
void print_state_stack(void * yyscanner)
{

}
#endif

void push_buffer(YY_BUFFER_STATE new_buffer, void * yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  yypush_buffer_state(new_buffer,yyscanner);
  printf("PUSH State Stack Pointer = %d\n", yyg->yy_start_stack_ptr);
  printf("PUSH State Stack Top = %d\n", yyg->yy_buffer_stack_top);
  printf("PUSH State Stack Max. = %d\n", yyg->yy_buffer_stack_max);
}

void pop_buffer(void * yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  yypop_buffer_state(yyscanner);
  printf("POP State Stack Pointer = %d\n", yyg->yy_start_stack_ptr);
  printf("POP State Stack Top = %d\n", yyg->yy_buffer_stack_top);
  printf("POP State Stack Max. = %d\n", yyg->yy_buffer_stack_max);
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

