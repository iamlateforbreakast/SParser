D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			([Ee][+-]?{D}+)
P                       ([Pp][+-]?{D}+)
FS			(f|F|l|L)
IS                      ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U))

%{
#include <stdio.h>
#include "GrammarC99.h"
#include "Error.h"
#include "GrammarC99.parse.h"
#define YYSTYPE GRAMMARC99_STYPE
#define YY_DECL int GrammarC99_lex(YYSTYPE * yylval_param, void * yyscanner, GrammarC99 * grammar)
#define YY_NO_INPUT

/* TODO: Define YY_USER_INIT */
/* TODO: Define YY_USER_ACTION */
void comment(void * yyscanner);
int check_type(void * yyscanner);
void count(void * yyscanner);
extern void yyerror(char const *s);
%}

%option noyywrap
%option stack
%option reentrant
%option prefix="GrammarC99"
%option bison-bridge

%%
"/*"			{ comment(yyscanner); }
"//"[^\n]*              { /* consume //-comment */ }


"auto"			{ count(yyscanner); return(AUTO); }
"_Bool"			{ count(yyscanner); return(BOOL); }
"break"			{ count(yyscanner); return(BREAK); }
"case"			{ count(yyscanner); return(CASE); }
"char"			{ count(yyscanner); return(CHAR); }
"_Complex"		{ count(yyscanner); return(COMPLEX); }
"const"			{ count(yyscanner); return(CONST); }
"continue"		{ count(yyscanner); return(CONTINUE); }
"default"		{ count(yyscanner); return(DEFAULT); }
"do"			{ count(yyscanner); return(DO); }
"double"		{ count(yyscanner); return(DOUBLE); }
"else"			{ count(yyscanner); return(ELSE); }
"enum"			{ count(yyscanner); return(ENUM); }
"extern"		{ count(yyscanner); return(EXTERN); }
"float"			{ count(yyscanner); return(FLOAT); }
"for"			{ count(yyscanner); return(FOR); }
"goto"			{ count(yyscanner); return(GOTO); }
"if"			{ count(yyscanner); return(IF); }
"_Imaginary"		{ count(yyscanner); return(IMAGINARY); }
"inline"		{ count(yyscanner); return(INLINE); }
"int"			{ count(yyscanner); return(INT); }
"long"			{ count(yyscanner); return(LONG); }
"register"		{ count(yyscanner); return(REGISTER); }
"restrict"		{ count(yyscanner); return(RESTRICT); }
"return"		{ count(yyscanner); return(RETURN); }
"short"			{ count(yyscanner); return(SHORT); }
"signed"		{ count(yyscanner); return(SIGNED); }
"sizeof"		{ count(yyscanner); return(SIZEOF); }
"static"		{ count(yyscanner); return(STATIC); }
"struct"		{ count(yyscanner); return(STRUCT); }
"switch"		{ count(yyscanner); return(SWITCH); }
"typedef"		{ count(yyscanner); return(TYPEDEF); }
"union"			{ count(yyscanner); return(UNION); }
"unsigned"		{ count(yyscanner); return(UNSIGNED); }
"void"			{ count(yyscanner); return(VOID); }
"volatile"		{ count(yyscanner); return(VOLATILE); }
"while"			{ count(yyscanner); return(WHILE); }

{L}({L}|{D})*		{ count(yyscanner); yylval->sval=strdup(yytext); return(check_type(yyscanner)); }

0[xX]{H}+{IS}?		{ count(yyscanner); yylval->sval = "CONST"; return(CONSTANT); }
0[0-7]*{IS}?		{ count(yyscanner); return(CONSTANT); }
[1-9]{D}*{IS}?		{ count(yyscanner); return(CONSTANT); }
L?'(\\.|[^\\'\n])+'	{ count(yyscanner); return(CONSTANT); }

{D}+{E}{FS}?		{ count(yyscanner); return(CONSTANT); }
{D}*"."{D}+{E}?{FS}?	{ count(yyscanner); return(CONSTANT); }
{D}+"."{D}*{E}?{FS}?	{ count(yyscanner); return(CONSTANT); }
0[xX]{H}+{P}{FS}?	{ count(yyscanner); return(CONSTANT); }
0[xX]{H}*"."{H}+{P}{FS}?     { count(yyscanner); return(CONSTANT); }
0[xX]{H}+"."{H}*{P}{FS}?     { count(yyscanner); return(CONSTANT); }


L?\"(\\.|[^\\"\n])*\"	{ count(yyscanner); yylval->sval = "STR"; return(STRING_LITERAL); }

"..."			{ count(yyscanner); return(ELLIPSIS); }
">>="			{ count(yyscanner); return(RIGHT_ASSIGN); }
"<<="			{ count(yyscanner); return(LEFT_ASSIGN); }
"+="			{ count(yyscanner); return(ADD_ASSIGN); }
"-="			{ count(yyscanner); return(SUB_ASSIGN); }
"*="			{ count(yyscanner); return(MUL_ASSIGN); }
"/="			{ count(yyscanner); return(DIV_ASSIGN); }
"%="			{ count(yyscanner); return(MOD_ASSIGN); }
"&="			{ count(yyscanner); return(AND_ASSIGN); }
"^="			{ count(yyscanner); return(XOR_ASSIGN); }
"|="			{ count(yyscanner); return(OR_ASSIGN); }
">>"			{ count(yyscanner); return(RIGHT_OP); }
"<<"			{ count(yyscanner); return(LEFT_OP); }
"++"			{ count(yyscanner); return(INC_OP); }
"--"			{ count(yyscanner); return(DEC_OP); }
"->"			{ count(yyscanner); return(PTR_OP); }
"&&"			{ count(yyscanner); return(AND_OP); }
"||"			{ count(yyscanner); return(OR_OP); }
"<="			{ count(yyscanner); return(LE_OP); }
">="			{ count(yyscanner); return(GE_OP); }
"=="			{ count(yyscanner); return(EQ_OP); }
"!="			{ count(yyscanner); return(NE_OP); }
";"			{ count(yyscanner); return(';'); }
("{"|"<%")		{ count(yyscanner); return('{'); }
("}"|"%>")		{ count(yyscanner); return('}'); }
","			{ count(yyscanner); return(','); }
":"			{ count(yyscanner); return(':'); }
"="			{ count(yyscanner); return('='); }
"("			{ count(yyscanner); return('('); }
")"			{ count(yyscanner); return(')'); }
("["|"<:")		{ count(yyscanner); return('['); }
("]"|":>")		{ count(yyscanner); return(']'); }
"."			{ count(yyscanner); return('.'); }
"&"			{ count(yyscanner); return('&'); }
"!"			{ count(yyscanner); return('!'); }
"~"			{ count(yyscanner); return('~'); }
"-"			{ count(yyscanner); return('-'); }
"+"			{ count(yyscanner); return('+'); }
"*"			{ count(yyscanner); return('*'); }
"/"			{ count(yyscanner); return('/'); }
"%"			{ count(yyscanner); return('%'); }
"<"			{ count(yyscanner); return('<'); }
">"			{ count(yyscanner); return('>'); }
"^"			{ count(yyscanner); return('^'); }
"|"			{ count(yyscanner); return('|'); }
"?"			{ count(yyscanner); return('?'); }

[ \t\v\n\f]		{ count(yyscanner); }
.			{ /* Add code to complain about unmatched characters */ }

%%

/*int yywrap(void)
{
	return 1;
}*/


void comment(void * yyscanner)
{
	/*char c, prev = 0;
  
	while ((c = input()) != 0)      (EOF maps to 0) 
		{
		if (c == '/' && prev == '*')
			return;
		prev = c;
	}
	yyerror("unterminated comment");*/
}


int column = 0;

void count(void * yyscanner)
{
	/*int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;*/
}


int check_type(void * yyscanner)
{
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return TYPE_NAME;
*
*	return IDENTIFIER;
*/

/*
*	it actually will only return IDENTIFIER
*/
    //printf("\nIdentifier seen %s\n", yytext);
    //Declarator_setName(yytext);
	return IDENTIFIER;
}

//void scanString(const char * buffer)
//{
//  yy_scan_string(buffer);
//}
