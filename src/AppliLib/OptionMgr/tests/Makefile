all: UT_OptionMgr_01.c ../OptionMgr.c ../OptionMgr.h
	gcc -DUNIT_TEST -g UT_OptionMgr_01.c ../OptionMgr.c \
	../../../CommonLib/libCommon.a ../../libAppli.a \
	-lCommon -lAppli \
	-I../../include -I../../../CommonLib/include \
	-L../../../CommonLib -L../../../AppliLib \
	-o UT_OptionMgr_01

clean:
	rm -rf UT_OptionMgr_01 *.o ../*.o
