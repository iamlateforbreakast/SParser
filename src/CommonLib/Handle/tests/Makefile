COMMONLIB=../../libCommon.a
COMMONLIB_INCLUDE=../../include
TESTLIB_INCLUDE=../../../../tests/unit_tests
TESTLIB=../../../../tests/unit_tests/libTest.a

# Coverage flags
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

all: $(COMMONLIB) UT_Handle_01

$(COMMONLIB):
	$(MAKE) -C ../../../CommonLib/
	
UT_Handle_01: UT_Handle_01.c ../Handle.c ../Handle.h ../../../../tests/unit_tests/Words1000.h
	gcc -g $(COVERAGE_FLAGS) -DMAIN UT_Handle_01.c \
	../Handle.c \
	../../../../tests/unit_tests/TestObject.c \
	../../libCommon.a \
	-I../../include \
	-I$(TESTLIB_INCLUDE) \
	-o UT_Handle_01

run-tests: UT_Handle_01
	./UT_Handle_01

coverage: run-tests
	gcov ../Handle.c
	@echo "Coverage reports generated: *.gcov files"

clean:
	rm -rf UT_Handle_01 *.o ../*.o *.gcda *.gcno *.gcov

cleanall: clean
	$(MAKE) -C ../../../CommonLib/ clean

.PHONY: all clean cleanall coverage run-tests $(COMMONLIB)
