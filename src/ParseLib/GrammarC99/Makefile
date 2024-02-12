CFLAGS = -g -c -Wall -I../../AppliLib/include -I../include -I../../CommonLib/include

all: grammar

grammar: GrammarC99.y GrammarC99.l
	flex -F -o GrammarC99.lex.c GrammarC99.l
	bison -d -y GrammarC99.y
	gcc -c $(CFLAGS) GrammarC99.lex.c GrammarC99.parse.c GrammarC99.c
	
clean: 
	rm -rf *.o
	rm -rf GrammarC99.lex.c GrammarC99.parse.c GrammarC99.parse.h
