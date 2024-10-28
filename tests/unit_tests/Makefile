CFLAGS = -ggdb3 -Wall -I../../src/AppliLib/include \
         -I../../src/ParseLib/include \
         -I../../src/CommonLib/include

LDFLAGS =  -L../../src/CommonLib \
           -L../../src/AppliLib \
           -L../../src/ParseLib -lParse -lAppli -lCommon -lsqlite3

TESTS = \
	../../src/CommonLib/BTree/tests/UT_BTree_01.c \
	../../src/CommonLib/Debug/tests/UT_Debug_01.c \
	../../src/CommonLib/Error/tests/UT_Error_01.c \
	../../src/CommonLib/FileIo/tests/UT_FileIo_01.c \
	../../src/CommonLib/List/tests/UT_List_01.c \
	../../src/CommonLib/List/tests/UT_List_02.c \
	../../src/CommonLib/Map/tests/UT_Map_01.c \
	../../src/CommonLib/SkipList/tests/UT_SkipList_01.c \
	../../src/CommonLib/String/tests/UT_String_01.c

STUBS = \
	TestObject.c \
	MyAllocator.c \
	Words1000.c

.c.o:
	gcc $(CFLAGS) $< -o $@

all: run_ut_tests

run_ut_tests: main.c $(TESTS) $(STUBS)
	gcc $(CFLAGS) main.c $(TESTS) $(STUBS) $(LDFLAGS) -o run_UT_tests

clean:
	rm -rf *.o

.PHONY: clean
