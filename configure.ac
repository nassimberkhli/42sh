AC_INIT([42sh], [1.0], [42sh-paris-73])

AM_INIT_AUTOMAKE([subdir-objects foreign])

AM_SILENT_RULES([yes])

AM_PROG_AR

AC_PROG_RANLIB

AC_PROG_CC

AC_ARG_ENABLE([sanitize],
    [AS_HELP_STRING([--enable-sanitize], [Enable AddressSanitizer flags for testing])],
    [enable_sanitize=$enableval],
    [enable_sanitize=no])

if test "x$enable_sanitize" = "xyes"; then
    SANITIZE_FLAGS="-fsanitize=address -g"
    AC_DEFINE([ENABLE_SANITIZE], [1], [Enable AddressSanitizer])
else
    SANITIZE_FLAGS=""
fi

AX_COMPILER_FLAGS([], [], [], [-Wall -Wextra -Werror -Wvla -pedantic -std=c99 $SANITIZE_FLAGS])

AC_CONFIG_FILES([
                    Makefile
                    src/Makefile
                    src/lexer/Makefile
                    src/parser/Makefile
                    src/io/Makefile
                    src/lecture/Makefile
                    src/builtin/Makefile
					src/variable/Makefile
                    tests/Makefile
                ])

AM_SILENT_RULES([no])

AC_OUTPUT

