all: UT_GrammarC99_01

UT_GrammarC99_01: UT_GrammarC99_01.c
	gcc -g UT_GrammarC99_01.c ../GrammarC99.c \
	../GrammarC99.lex.c ../GrammarC99.parse.c \
	../../../CommonLib/libCommon.a \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-o UT_Grammar_01

clean:
	rm -rf UT_GrammarC99_01 *.o ../*.o
