all: main.c ../OptionMgr.c ../OptionMgr.h
	gcc -DUNIT_TEST -g main.c ../OptionMgr.c \
	../../../CommonLib/libCommon.a ../../FileMgr/*.o \
	-lCommon \
	-I../../include -I../../../CommonLib/include \
	-L../../../CommonLib -L../../AppliLib \
	-o test1

clean:
	rm -rf test1 *.o ../*.o
