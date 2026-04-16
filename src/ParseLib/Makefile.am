
BUILT_SOURCES = $(srcdir)/include/.headers-copied \
	Grammar2/Grammar2.lex.c \
	Grammar2/Grammar2.parse.c \
	Grammar2/Grammar2.parse.h

AM_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)/../AppliLib/include -I$(srcdir)/../CommonLib/include

# Configure flex and bison
AM_LFLAGS = -F
AM_YFLAGS = -dy

# list of headers
pkginclude_HEADERS = \
    Configuration/Configuration.h \
    Configuration/Product.h \
    FileReader/FileReader.h \
    Grammar/Grammar.h \
    Grammar2/Grammar2.h \
    GrammarC99/GrammarC99.h \
    SParse/SParse.h \
    TransUnit/TransUnit.h \
    HTTPServer/HTTPServer.h \
    Ast/Ast.h

# List the headers you want to "publish" to the local include folder
PUBLIC_HDRS = $(pkginclude_HEADERS)

# This creates a 'built-include' directory and symlinks/copies files into it
$(srcdir)/include/.headers-copied:
	@mkdir -p $(srcdir)/include
	@for hdr in $(PUBLIC_HDRS); do \
	    cp -f $(srcdir)/$$hdr $(srcdir)/include/ ; \
	done
	@touch $@

# Clean up the generated directory
clean-local:
	rm -rf $(srcdir)/include \
    	Grammar2/Grammar2.lex.c \
	Grammar2/Grammar2.parse.c \
	Grammar2/Grammar2.parse.h
    
# 1. Define the library name
lib_LIBRARIES = libParse.a

# 2. List every .c and .h file required to build it
libParse_a_SOURCES = \
    Configuration/Configuration.c \
    Configuration/Product.c \
    FileReader/FileReader.c \
    Grammar2/Grammar2.c \
    Grammar2/Grammar2.lex.c \
    Grammar2/Grammar2.parse.c \
    SParse/SParse.c \
    TransUnit/TransUnit.c \
    HTTPServer/HTTPServer.c \
    Ast/Ast.c




