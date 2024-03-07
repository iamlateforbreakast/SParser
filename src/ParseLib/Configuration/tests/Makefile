all: UT_Configuration_01.c ../Configuration.c ../Configuration.h
	gcc -DUNIT_TEST -g UT_Configuration_01.c ../Configuration.c \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-L../../../CommonLib -L../../../AppliLib -L../.. \
	-lParse -lAppli -lCommon \
	-o UT_Configuration_01

clean:
	rm -rf UT_Configuration_01 *.o ../*.o
