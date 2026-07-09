COMMONLIB=../../libCommon.a
COMMONLIB_INCLUDE=../../include
TESTLIB_INCLUDE=../../../../tests/unit_tests
TESTLIB=../../../../tests/unit_tests/libTest.a

# Coverage flags
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

all: $(COMMONLIB) UT_Map_01 UT_Map_02 UT_Full

$(COMMONLIB):
	$(MAKE) -C ../../../CommonLib/
	
UT_Map_01: UT_Map_01.c ../Map.c ../Map.h ../MapNode.h ../../../../tests/unit_tests/Words1000.h
	gcc -g $(COVERAGE_FLAGS) -DMAIN UT_Map_01.c \
	../Map.c \
	../MapEntry.c \
	../../../../tests/unit_tests/TestObject.c \
	../../../../tests/unit_tests/Words1000.c \
	../../libCommon.a \
	-I../../include \
	-I$(TESTLIB_INCLUDE) \
	-o UT_Map_01

UT_Map_02: UT_Map_02.c ../Map.c ../Map.h ../MapNode.h ../../../../tests/unit_tests/MyAllocator.c ../../../../tests/unit_tests/MyAllocator.h
	gcc -g $(COVERAGE_FLAGS) -DMAIN \
	UT_Map_02.c \
	../../../../tests/unit_tests/MyAllocator.c \
	../Map.c ../MapEntry.c \
	../../../../tests/unit_tests/TestObject.c \
	../../../../tests/unit_tests/Words1000.c \
	../../libCommon.a \
	-I../../include \
	-I$(TESTLIB_INCLUDE) \
	-o UT_Map_02

UT_Full: UT_Full.c ../Map.c ../Map.h ../MapNode.h ../../../../tests/unit_tests/MyAllocator.c ../../../../tests/unit_tests/MyAllocator.h
	gcc -g $(COVERAGE_FLAGS) -DMAIN \
	UT_Full.c \
	../../../../tests/unit_tests/MyAllocator.c \
	../Map.c ../MapEntry.c \
	../../../../tests/unit_tests/TestObject.c \
	../../../../tests/unit_tests/Words1000.c \
	../../libCommon.a \
	-I../../include \
	-I$(TESTLIB_INCLUDE) \
	-o UT_Full

run-tests: UT_Map_01 UT_Map_02
	./UT_Map_01
	./UT_Map_02

coverage: run-tests
	gcov ../Map.c
	gcov ../MapEntry.c
	@echo "Coverage reports generated: *.gcov files"

clean:
	rm -rf UT_Map_01 UT_Map_02 *.o ../*.o *.gcda *.gcno *.gcov

cleanall: clean
	$(MAKE) -C ../../../CommonLib/ clean

.PHONY: all clean cleanall coverage run-tests $(COMMONLIB)
