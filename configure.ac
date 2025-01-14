# Init the 42sh project
AC_INIT([42sh], [1.0], [42sh-paris-73])

# Setup automake
AM_INIT_AUTOMAKE([subdir-objects] [foreign])

# Pretty display of makefile rules
AM_SILENT_RULES([yes])

# Enable ar for Makefile
AM_PROG_AR

# Check if a ranlib is available
AC_PROG_RANLIB

# Check if a compiler is available for C
AC_PROG_CC

# Add optional sanitization flags
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

AC_SUBST(SANITIZE_FLAGS)

# List Makefile in subdirectories
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

