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
       lexer/liblexer.a \
       parser/libparser.a \
       io/libio.a \
       lecture/liblecture.a
