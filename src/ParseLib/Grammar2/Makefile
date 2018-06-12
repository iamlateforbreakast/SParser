all: main

main: Grammar2.lex.c Grammar2.parse.c main.c
	gcc -g main.c Grammar2.lex.c Grammar2.parse.c -lfl -ly -o Grammar2
	
Grammar2.lex.c: Grammar2.l
	flex -o Grammar2.lex.c Grammar2.l

Grammar2.parse.c: Grammar2.y
	bison -dy Grammar2.y

clean: 
	rm -rf *.o *.exe
	rm -rf Grammar2.lex.c Grammar2.parse.c Grammar2.parse.h
