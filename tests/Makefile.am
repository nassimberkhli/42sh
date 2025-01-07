check: unittests
	export VALGRIND_ENABLED=1
	./testsuite.sh
	./unittests

noinst_PROGRAMS = unittests

unittests_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -lcriterion

unittests_SOURCES = \
    unittests.c
