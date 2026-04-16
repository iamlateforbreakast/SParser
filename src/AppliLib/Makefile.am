
BUILT_SOURCES = copy-headers

AM_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)/../CommonLib/include

# list of headers
pkginclude_HEADERS = \
    FileMgr/FileMgr.h \
    FileMgr/FileDesc.h \
    OptionMgr/OptionMgr.h \
    SdbMgr/SdbMgr.h \
    TaskMgr/TaskMgr.h \
    TimeMgr/TimeMgr.h \
    XmlReader/XmlReader.h

# List the headers you want to "publish" to the local include folder
PUBLIC_HDRS = $(pkginclude_HEADERS)

# This creates a 'built-include' directory and symlinks/copies files into it
copy-headers:
	@mkdir -p $(srcdir)/include
	@for hdr in $(PUBLIC_HDRS); do \
	    cp -f $(srcdir)/$$hdr $(srcdir)/include/ ; \
	done

# Clean up the generated directory
clean-local:
	rm -rf $(srcdir)/include
    
# 1. Define the library name
noinst_LIBRARIES = libAppli.a

# 2. List every .c and .h file required to build it
libAppli_a_SOURCES = \
    FileMgr/FileMgr.c \
    FileMgr/FileDesc.c \
    OptionMgr/OptionMgr.c \
    SdbMgr/SdbMgr.c \
    TaskMgr/TaskMgr.c \
    TimeMgr/TimeMgr.c \
    XmlReader/XmlReader.c

.PHONY: copy-headers




