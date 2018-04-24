all: main

main: Grammar1.lex.c Grammar1.parse.c main.c
	gcc main.c Grammar1.lex.c Grammar1.parse.c -lfl -ly -o Grammar1
	
Grammar1.lex.c: Grammar1.l
	flex -oGrammar1.lex.c Grammar1.l

Grammar1.parse.c: Grammar1.y
	bison -dy Grammar1.y
