#!/bin/sh

echo "/* User Types */" > UserTypes.h
echo "#ifndef _USERTYPES_" >> UserTypes.h
echo "#define _USERTYPES_" >> UserTypes.h
echo "#include \"Class.h\"" >> UserTypes.h
echo "Class * userTypes[] =" >> UserTypes.h
echo "{" >> UserTypes.h
#enum = "enum {\n"
class = "grep -R --include="*.c" "DECLARE_CLASS(" ../../ | awk -F'[()]' '{print "  " $2 }'"
echo $class
echo "  &listClass,"  >> UserTypes.h
echo "  &stringClass"  >> UserTypes.h
echo "};" >> UserTypes.h

echo "#endif /* UserTypes.h */" >> UserTypes.h
