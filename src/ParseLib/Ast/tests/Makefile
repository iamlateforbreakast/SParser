all: UT_Ast_01.c ../Ast.c ../Ast.h
	gcc -DUNIT_TEST -DMAIN -g UT_Ast_01.c ../Ast.c \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-L../../../CommonLib -L../../../AppliLib -L../.. \
	-lParse -lAppli -lCommon \
	-o UT_Ast_01

clean:
	rm -rf UT_Ast_01 *.o ../*.o
