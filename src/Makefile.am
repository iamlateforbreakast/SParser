# 1. Build the libraries first
# The order matters if AppliLib depends on CommonLib
SUBDIRS = CommonLib ParseLib AppliLib

# 2. Define the final executable
bin_PROGRAMS = sparse

# 3. List the source for the executable
sparse_SOURCES = main.c

# 4. Include paths
# Point to your generated include directory or the library folders
AM_CPPFLAGS = -I$(top_srcdir)/src/CommonLib/include \
              -I$(top_srcdir)/src/AppliLib/include \
              -I$(top_srcdir)/src/ParseLib/include

# 5. Link against your libraries
# Use the relative paths to the .a files created in the subdirectories
sparse_LDADD = \
    AppliLib/libAppli.a \
    ParseLib/libParse.a \
    CommonLib/libCommon.a \
    -lsqlite3
