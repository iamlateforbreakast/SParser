all: main.c ../OptionMgr.c ../OptionMgr.h
	gcc -DUNIT_TEST -g main.c ../OptionMgr.c ../../../CommonLib/libCommon.a ../../libAppli.a -I../../include -I../../../CommonLib/include -o test1

clean:
	rm -rf test1 *.o ../*.o
