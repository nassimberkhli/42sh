check: unittests
	export VALGRIND_ENABLED=1
	./testsuite.sh

noinst_PROGRAMS = unittests

unittests_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic $(SANITIZE_FLAGS) -lcriterion
unittests_LDFLAGS = $(SANITIZE_FLAGS)

unittests_LDADD = ../src/lexer/liblexer.a ../src/io/libio.a ../src/parser/libparser.a

unittests_CPPFLAGS = -I../src -I../src/lexer -I../src/io

unittests_SOURCES = \
    unittests.c

