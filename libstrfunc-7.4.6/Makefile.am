
SUBDIRS = . man bin test

AM_YFLAGS = -p sfcfg -d

include_HEADERS = strfunc.h

lib_LTLIBRARIES = libstrfunc.la

libstrfunc_la_LDFLAGS = -version-info $(VERSION)

libstrfunc_la_SOURCES = \
	b64.h \
	cfg.y cfglex.c cfgread.c cgifunc.c \
	datetime.c \
	filepos.c \
	format.c \
	join.c \
	lang.c \
	netconv.c \
	param.c param.h \
	replace.c \
	safealloc.c \
	sbuf.c \
	sed-int.h sed.c simplechar.c split.c \
	strfunc.c strfunc.h svectpair.c \
	getopt.h

BUILT_SOURCES = strfunc.h getopt.h

# this is needed because of a bug in Automake
# related to processing of secondary ./configure's and 'make dist'
EXTRA_DIST = config.h.in strfunc.h.in getopt.h.in

.PHONY: lines
lines:
	wc `find . -type f \( -name "*.c" -or -name "*.h" -or -name "*.in" -or -name "*.html" -or -name "*.conf" -or -name "*.3" \) -print`
