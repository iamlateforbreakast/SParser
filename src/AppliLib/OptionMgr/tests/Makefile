COMMONLIB=../../../CommonLib/libCommon.a
COMMONLIB_INCLUDE=-I../../../CommonLib/include
APPLILIB=../../../AppliLib/libAppli.a
APPLILIB_INCLUDE=-I../../../AppliLib/include

all: FileDesc FileMgr $(COMMONLIB) UT_OptionMgr_01

$(COMMONLIB):
	$(MAKE) -C ../../../CommonLib/

FileDesc: ../../FileMgr/FileDesc.c ../../FileMgr/FileDesc.h
	gcc -c -g ../../FileMgr/FileDesc.c \
	$(COMMONLIB_INCLUDE) \
	-I../../FileMgr -o FileDesc.o

FileMgr: ../../FileMgr/FileMgr.c ../../FileMgr/FileMgr.h
	gcc -c -g ../../FileMgr/FileMgr.c \
	$(COMMONLIB_INCLUDE) \
	-I../../FileMgr -o FileMgr.o

UT_OptionMgr_01: UT_OptionMgr_01.c ../OptionMgr.c ../OptionMgr.h $(COMMONLIB)
	gcc -DUNIT_TEST -DMAIN -g UT_OptionMgr_01.c ../OptionMgr.c \
	FileMgr.o FileDesc.o \
	$(COMMONLIB) \
	-I../../../CommonLib/include \
	-I../../include \
	-o UT_OptionMgr_01

clean:
	rm -rf UT_OptionMgr_01 *.o ../*.o

cleanall: clean
	$(MAKE) -C ../../../CommonLib/ clean

.PHONY: all clean cleanall $(COMMONLIB)
