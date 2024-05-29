all: UT_GrammarC99_01

UT_GrammarC99_01: UT_GrammarC99_01.c
	gcc -g UT_GrammarC99_01.c ../GrammarC99.c \
	../GrammarC99.lex.c ../GrammarC99.parse.c \
	-L../../../CommonLib \
	-L../../../AppliLib \
	-L../../../ParseLib \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-lParse -lAppli -lCommon \
	-o UT_Grammar_01

clean:
	rm -rf UT_GrammarC99_01 *.o ../*.o
