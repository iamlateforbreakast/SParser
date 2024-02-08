all: UT_TransUnit_01.c ../TransUnit.c ../TransUnit.h
	gcc -DUNIT_TEST -g UT_TransUnit_01.c ../TransUnit.c \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-L ../../../CommonLib -L ../../../AppliLib -L ../.. \
	-lCommon -lAppli -lParse \
	-o UT_TransUnit_01

clean:
	rm -rf UT_TransUnit_01 *.o ../*.o
