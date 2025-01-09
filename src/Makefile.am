SUBDIRS = lexer \
          parser \
          io \
          lecture

bin_PROGRAMS = 42sh

42sh_SOURCES = main.c

nobase_include_HEADERS = main.h

42sh_CPPFLAGS = -I%D%

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD =  \
       $(top_builddir)/src/parser/libparser.a \
       $(top_builddir)/src/lexer/liblexer.a \
       $(top_builddir)/src/io/libio.a \
       $(top_builddir)/src/lecture/liblecture.a

