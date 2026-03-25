COMMONLIB = ../../../CommonLib/libCommon.a
COMMONLIB_INCLUDE = ../../../CommonLib/include
APPLILIB = ../../../AppliLib/libAppli.a
APPLILIB_INCLUDE = ../../../AppliLib/include

all: $(COMMONLIB) $(APPLILIB) UT_HTTPServer_01

$(COMMONLIB):
	$(MAKE) -C ../../../CommonLib/

$(APPLILIB):
	$(MAKE) -C ../../../AppliLib/

UT_HTTPServer_01: UT_HTTPServer_01.c ../HTTPServer.c ../HTTPServer.h ../HTTPRequest.h ../HTTPResponse.h
	gcc -g UT_HTTPServer_01.c ../HTTPServer.c \
	$(APPLILIB) \
	$(COMMONLIB) \
	-I../../include \
	-I../../../AppliLib/include \
	-I../../../CommonLib/include \
	-o UT_HTTPServer_01

run: UT_HTTPServer_01.c
	./UT_HTTPServer_01

clean:
	rm -rf UT_HTTPServer_01 *.o ../*.o

cleanall: clean
	$(MAKE) -C ../../../CommonLib/ clean
	$(MAKE) -C ../../../AppliLib/ clean
