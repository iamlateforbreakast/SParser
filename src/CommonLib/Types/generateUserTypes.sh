#!/bin/sh

echo "/* User Types */" > UserTypes.h
echo "#ifndef _USERTYPES_" >> UserTypes.h
echo "#define _USERTYPES_" >> UserTypes.h
echo "#include \"Class.h\"" >> UserTypes.h
#enum = "enum {\n"
#class="$(grep -R --include='*.c' 'DECLARE_CLASS(' ../../ | awk -F'[()]' '{print '  ' $2 }')"
class="$(grep -R --include='*.c' 'DECLARE_CLASS(' ../../ | awk -F'[()]' '{print $2}')"
for i in $class
do
  echo $i | awk -F' ' '{print "extern Class " tolower(substr($1,1,1)) substr($1,2) "Class;"}' >> UserTypes.h
done
echo >> UserTypes.h
echo "enum ClassId" >> UserTypes.h
echo "{" >> UserTypes.h
for i in $class
do
  echo $i | awk -F' ' '{print "  " $1 ","}' >> UserTypes.h
done
echo "  NB_CLASSES" >> UserTypes.h
echo "};" >> UserTypes.h
echo >> UserTypes.h
echo "Class * userTypes[] =" >> UserTypes.h
echo "{" >> UserTypes.h
for i in $class
do
  echo $i | awk -F' ' '{print "  &" tolower(substr($1,1,1)) substr($1,2) "Class,"}' >> UserTypes.h
done
#echo "  &listClass,"  >> UserTypes.h
#echo "  &stringClass"  >> UserTypes.h
echo "};" >> UserTypes.h

echo "#endif /* UserTypes.h */" >> UserTypes.h
