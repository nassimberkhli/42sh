lib_LIBRARIES = libvariable.a

libvariable_a_SOURCES =  \
  variable.c \
  variable.h \
  hash_map.h \
  hash_map.c \
  hash.c

libvariable_a_CPPFLAGS = -I$(top_srcdir)/src
libvariable_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic $(SANITIZE_FLAGS)

noinst_LIBRARIES = libvariable.a

