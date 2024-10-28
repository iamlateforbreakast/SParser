CFLAGS = -ggdb3 -Wall -I../../src/AppliLib/include \
         -I../../src/ParseLib/include \
         -I../../src/CommonLib/include

LDFLAGS =  -L../../src/CommonLib \
           -L../../src/AppliLib \
           -L../../src/ParseLib -lParse -lAppli -lCommon -lsqlite3

TESTS = \
	../../src/CommonLib/BTree/tests/UT_BTree_01.c \
	../../src/CommonLib/List/tests/UT_List_01.c \
	../../src/CommonLib/String/tests/UT_String_01.c

.c.o:
	gcc $(CFLAGS) $< -o $@

all: run_ut_tests

run_ut_tests: main.c $(TESTS)
	gcc $(CFLAGS) main.c TestObject.c $(TESTS) $(LDFLAGS) -o run_UT_tests

clean:
	rm -rf *.o

.PHONY: clean
