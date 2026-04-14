COMMONLIB=../../libCommon.a
COMMONLIB_INCLUDE=../../include
TESTLIB_INCLUDE=../../../../tests/unit_tests
TESTLIB=../../../../tests/unit_tests/libTest.a

all: $(COMMONLIB) UT_Map_01 UT_Map_02

$(COMMONLIB):
	$(MAKE) -C ../../../CommonLib/
	
UT_Map_01: UT_Map_01.c ../Map.c ../Map.h ../MapNode.h ../../../../tests/unit_tests/Words1000.h
	gcc -g -DMAIN UT_Map_01.c \
	../Map.c \
	../MapEntry.c \
	../../../../tests/unit_tests/TestObject.c \
	../../../../tests/unit_tests/Words1000.c \
	../../libCommon.a \
	-I../../include \
	-I$(TEST_INCLUDE) \
	-o UT_Map_01

UT_Map_02: UT_Map_02.c ../Map.c ../Map.h ../MapNode.h ../../../../tests/unit_tests/MyAllocator.c ../../../../tests/unit_tests/MyAllocator.h
	gcc -g -DMAIN \
	UT_Map_02.c \
	../../../../tests/unit_tests/MyAllocator.c \
	../Map.c ../MapEntry.c \
	../../libCommon.a \
	-I../../include \
	-I$(TEST_INCLUDE) \
	-o UT_Map_02

clean:
	rm -rf UT_Map_01 UT_Map_02 *.o ../*.o

cleanall: clean
	$(MAKE) -C ../../../CommonLib/ clean

.PHONY: all clean cleanall $(COMMONLIB)
