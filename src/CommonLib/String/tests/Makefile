COMMONLIB=../../../CommonLib/libCommon.a
COMMONLIB_INCLUDE=-I../../../CommonLib/include
TEST_INCLUDE=-I../../../../tests/unit_tests

all: UT_String_01 UT_String_02

UT_String_01: UT_String_01.c ../String2.c ../String2.h
	gcc -g -DMAIN UT_String_01.c ../String2.c ../../libCommon.a -I../../include -o UT_String_01

UT_String_02: UT_String_02.c ../String2.c ../String2.h ../../../../tests/unit_tests/MyAllocator.c ../../../../tests/unit_tests/MyAllocator.h
	gcc -g UT_String_02.c ../String2.c ../../../../tests/unit_tests/MyAllocator.c \
	../../libCommon.a \
	$(COMMONLIB_INCLUDE) \
	$(TEST_INCLUDE) \
	-o UT_String_02

clean:
	rm -rf UT_String_01 UT_String_02 *.o ../*.o

cleanall: clean
	$(MAKE) -C ../../../CommonLib/ clean

.PHONY: all clean cleanall $(COMMONLIB)
