all: UT_XmlReader_01.c ../XmlReader.c ../XmlReader.h ../XmlElement.h ../XmlAttribute.h
	gcc -DUNIT_TEST -DMAIN -g UT_XmlReader_01.c ../XmlReader.c ../../../CommonLib/libCommon.a \
	-I.. -I../../include -I../../../CommonLib/include -o UT_XmlReader_01

clean:
	rm -rf UT_XmlReader_01 *.o ../*.o
