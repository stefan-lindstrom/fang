# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



SHELL = /bin/bash

srcdir = .
top_srcdir = .

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/libstrfunc
pkglibdir = $(libdir)/libstrfunc
pkgincludedir = $(includedir)/libstrfunc
top_builddir = .

ACLOCAL = ${SHELL} /home/fang/git/fang/libstrfunc-7.4.6/missing --run aclocal
AUTOCONF = @echo autoconf not available
AUTOMAKE = ${SHELL} /home/fang/git/fang/libstrfunc-7.4.6/missing --run automake
AUTOHEADER = @echo autoheader not available

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = 
build_triplet = x86_64-unknown-linux-gnu
host_alias = 
host_triplet = x86_64-unknown-linux-gnu
target_alias = 
target_triplet = x86_64-unknown-linux-gnu
AMTAR = ${SHELL} /home/fang/git/fang/libstrfunc-7.4.6/missing --run tar
AR = /usr/bin/ar
AS = @AS@
AWK = mawk
CC = gcc
CONFIGURE_DEPENDS = configure.in aclocal.m4
CPP = gcc -E
DEPDIR = .deps
DLLTOOL = @DLLTOOL@
ECHO = echo
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
MAINT = #
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = libstrfunc
PATH = /usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games:/usr/ucb:/usr/ccs/bin
RANLIB = ranlib
STRIP = strip
VERSION = 7
YACC = yacc
am__include = include
am__quote = 
install_sh = /home/fang/git/fang/libstrfunc-7.4.6/install-sh

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
subdir = .
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = config.h strfunc.h getopt.h
CONFIG_CLEAN_FILES =
LTLIBRARIES = $(lib_LTLIBRARIES)

libstrfunc_la_LIBADD =
am_libstrfunc_la_OBJECTS = cfg.lo cfglex.lo cfgread.lo cgifunc.lo \
	datetime.lo filepos.lo format.lo join.lo lang.lo netconv.lo \
	param.lo replace.lo safealloc.lo sbuf.lo sed.lo simplechar.lo \
	split.lo strfunc.lo svectpair.lo
libstrfunc_la_OBJECTS = $(am_libstrfunc_la_OBJECTS)

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I. -I. -I.
CPPFLAGS = 
LDFLAGS = 
LIBS = 
depcomp = $(SHELL) $(top_srcdir)/depcomp
DEP_FILES = $(DEPDIR)/cfg.Plo $(DEPDIR)/cfglex.Plo \
	$(DEPDIR)/cfgread.Plo $(DEPDIR)/cgifunc.Plo \
	$(DEPDIR)/datetime.Plo $(DEPDIR)/filepos.Plo \
	$(DEPDIR)/format.Plo $(DEPDIR)/join.Plo \
	$(DEPDIR)/lang.Plo $(DEPDIR)/netconv.Plo \
	$(DEPDIR)/param.Plo $(DEPDIR)/replace.Plo \
	$(DEPDIR)/safealloc.Plo $(DEPDIR)/sbuf.Plo \
	$(DEPDIR)/sed.Plo $(DEPDIR)/simplechar.Plo \
	$(DEPDIR)/split.Plo $(DEPDIR)/strfunc.Plo \
	$(DEPDIR)/svectpair.Plo
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) \
	$(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
CFLAGS = -g -O2
YACCCOMPILE = $(YACC) $(YFLAGS) $(AM_YFLAGS)
LTYACCCOMPILE = $(LIBTOOL) --mode=compile $(YACC) $(YFLAGS) $(AM_YFLAGS)
DIST_SOURCES = $(libstrfunc_la_SOURCES)
HEADERS = $(include_HEADERS)


RECURSIVE_TARGETS = info-recursive dvi-recursive install-info-recursive \
	uninstall-info-recursive all-recursive install-data-recursive \
	install-exec-recursive installdirs-recursive install-recursive \
	uninstall-recursive check-recursive installcheck-recursive
DIST_COMMON = README $(include_HEADERS) ./stamp-h1.in ./stamp-h2.in \
	./stamp-h3.in AUTHORS COPYING ChangeLog INSTALL Makefile.am \
	Makefile.in NEWS TODO acconfig.h aclocal.m4 cfg.c cfg.h \
	config.guess config.h.in config.sub configure configure.in \
	depcomp getopt.h.in install-sh ltconfig ltmain.sh missing \
	mkinstalldirs strfunc.h.in
DIST_SUBDIRS = $(SUBDIRS)
SOURCES = $(libstrfunc_la_SOURCES)

all: $(BUILT_SOURCES) config.h strfunc.h getopt.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

.SUFFIXES:
.SUFFIXES: .c .lo .o .obj .y

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool
$(srcdir)/Makefile.in: # Makefile.am  $(top_srcdir)/configure.in $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --gnu  Makefile
Makefile: # $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$@ $(SHELL) ./config.status

$(top_builddir)/config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: # $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

$(ACLOCAL_M4): # configure.in 
	cd $(srcdir) && $(ACLOCAL) $(ACLOCAL_AMFLAGS)
config.h: stamp-h1
	@if test ! -f $@; then \
		rm -f stamp-h1; \
		$(MAKE) stamp-h1; \
	else :; fi
stamp-h1: $(srcdir)/config.h.in $(top_builddir)/config.status
	@rm -f stamp-h1 stamp-h1T
	@echo timestamp > stamp-h1T 2> /dev/null
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@mv stamp-h1T stamp-h1
$(srcdir)/config.h.in: # $(srcdir)/./stamp-h1.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/./stamp-h1.in; \
		$(MAKE) $(srcdir)/./stamp-h1.in; \
	else :; fi
$(srcdir)/./stamp-h1.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) $(top_srcdir)/acconfig.h
	@rm -f $(srcdir)/./stamp-h1.in $(srcdir)/./stamp-h1.inT
	@echo timestamp > $(srcdir)/./stamp-h1.inT 2> /dev/null
	cd $(top_srcdir) && $(AUTOHEADER)
	@mv $(srcdir)/./stamp-h1.inT $(srcdir)/./stamp-h1.in
strfunc.h: stamp-h2
	@if test ! -f $@; then \
		rm -f stamp-h2; \
		$(MAKE) stamp-h2; \
	else :; fi
stamp-h2: $(srcdir)/strfunc.h.in $(top_builddir)/config.status
	@rm -f stamp-h2 stamp-h2T
	@echo timestamp > stamp-h2T 2> /dev/null
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=strfunc.h \
	     $(SHELL) ./config.status
	@mv stamp-h2T stamp-h2
$(srcdir)/strfunc.h.in: # $(srcdir)/./stamp-h2.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/./stamp-h2.in; \
		$(MAKE) $(srcdir)/./stamp-h2.in; \
	else :; fi
$(srcdir)/./stamp-h2.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) $(top_srcdir)/acconfig.h
	@rm -f $(srcdir)/./stamp-h2.in $(srcdir)/./stamp-h2.inT
	@echo timestamp > $(srcdir)/./stamp-h2.inT 2> /dev/null
	cd $(top_srcdir) && $(AUTOHEADER)
	@mv $(srcdir)/./stamp-h2.inT $(srcdir)/./stamp-h2.in
getopt.h: stamp-h3
	@if test ! -f $@; then \
		rm -f stamp-h3; \
		$(MAKE) stamp-h3; \
	else :; fi
stamp-h3: $(srcdir)/getopt.h.in $(top_builddir)/config.status
	@rm -f stamp-h3 stamp-h3T
	@echo timestamp > stamp-h3T 2> /dev/null
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=getopt.h \
	     $(SHELL) ./config.status
	@mv stamp-h3T stamp-h3
$(srcdir)/getopt.h.in: # $(srcdir)/./stamp-h3.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/./stamp-h3.in; \
		$(MAKE) $(srcdir)/./stamp-h3.in; \
	else :; fi
$(srcdir)/./stamp-h3.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) $(top_srcdir)/acconfig.h
	@rm -f $(srcdir)/./stamp-h3.in $(srcdir)/./stamp-h3.inT
	@echo timestamp > $(srcdir)/./stamp-h3.inT 2> /dev/null
	cd $(top_srcdir) && $(AUTOHEADER)
	@mv $(srcdir)/./stamp-h3.inT $(srcdir)/./stamp-h3.in

distclean-hdr:
	-rm -f config.h strfunc.h getopt.h
install-libLTLIBRARIES: $(lib_LTLIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(LIBTOOL) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(libdir)/$$p"; \
	    $(LIBTOOL) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLTLIBRARIES:
	@$(NORMAL_UNINSTALL)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  echo " $(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(libdir)/$$p"; \
	  $(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(libdir)/$$p; \
	done

clean-libLTLIBRARIES:
	-test -z "$(lib_LTLIBRARIES)" || rm -f $(lib_LTLIBRARIES)
cfg.h: cfg.c
libstrfunc.la: $(libstrfunc_la_OBJECTS) $(libstrfunc_la_DEPENDENCIES) 
	$(LINK) -rpath $(libdir) $(libstrfunc_la_LDFLAGS) $(libstrfunc_la_OBJECTS) $(libstrfunc_la_LIBADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/cfg.Plo
include $(DEPDIR)/cfglex.Plo
include $(DEPDIR)/cfgread.Plo
include $(DEPDIR)/cgifunc.Plo
include $(DEPDIR)/datetime.Plo
include $(DEPDIR)/filepos.Plo
include $(DEPDIR)/format.Plo
include $(DEPDIR)/join.Plo
include $(DEPDIR)/lang.Plo
include $(DEPDIR)/netconv.Plo
include $(DEPDIR)/param.Plo
include $(DEPDIR)/replace.Plo
include $(DEPDIR)/safealloc.Plo
include $(DEPDIR)/sbuf.Plo
include $(DEPDIR)/sed.Plo
include $(DEPDIR)/simplechar.Plo
include $(DEPDIR)/split.Plo
include $(DEPDIR)/strfunc.Plo
include $(DEPDIR)/svectpair.Plo

distclean-depend:
	-rm -rf $(DEPDIR)

.c.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(COMPILE) -c `test -f $< || echo '$(srcdir)/'`$<

.c.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(COMPILE) -c `cygpath -w $<`

.c.lo:
	source='$<' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CCDEPMODE) $(depcomp) \
	$(LTCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<
CCDEPMODE = depmode=gcc3

.y.c:
	$(YACCCOMPILE) $< && mv y.tab.c $@
	if test -f y.tab.h; then \
	  if cmp -s y.tab.h $*.h; then \
	    rm -f y.tab.h; \
	  else \
	    mv y.tab.h $*.h; \
	  fi; \
	fi
uninstall-info-am:
install-includeHEADERS: $(include_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(includedir)
	@list='$(include_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_HEADER) $$d$$p $(DESTDIR)$(includedir)/$$f"; \
	  $(INSTALL_HEADER) $$d$$p $(DESTDIR)$(includedir)/$$f; \
	done

uninstall-includeHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(include_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(includedir)/$$f"; \
	  rm -f $(DESTDIR)$(includedir)/$$f; \
	done

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.
$(RECURSIVE_TARGETS):
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	rev=''; for subdir in $$list; do \
	  if test "$$subdir" = "."; then :; else \
	    rev="$$subdir $$rev"; \
	  fi; \
	done; \
	rev="$$rev ."; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) config.h.in strfunc.h.in getopt.h.in $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
	  fi; \
	done; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in strfunc.h.in getopt.h.in$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags config.h.in strfunc.h.in getopt.h.in $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = .
# Avoid unsightly `./'.
distdir = $(PACKAGE)-$(VERSION)

GZIP_ENV = --best

distdir: $(DISTFILES)
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)
	mkdir $(distdir)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    (cd $$subdir && \
	      $(MAKE) $(AM_MAKEFLAGS) \
	        top_distdir="$(top_distdir)" \
	        distdir=../$(distdir)/$$subdir \
	        distdir) \
	      || exit 1; \
	  fi; \
	done
	-find $(distdir) -type d ! -perm -777 -exec chmod a+rwx {} \; -o \
	  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -444 -exec $(SHELL) $(install_sh) -c -m a+r {} {} \; \
	|| chmod -R a+r $(distdir)
dist: distdir
	$(AMTAR) chof - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-chmod -R a+w $(distdir) > /dev/null 2>&1; rm -rf $(distdir)
	GZIP=$(GZIP_ENV) gunzip -c $(distdir).tar.gz | $(AMTAR) xf -
	chmod -R a-w $(distdir); chmod a+w $(distdir)
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	chmod a-w $(distdir)
	dc_install_base=`CDPATH=: && cd $(distdir)/=inst && pwd` \
	  && cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && (test `find $$dc_install_base -type f -print | wc -l` -le 1 \
	     || (echo "Error: files left after uninstall" 1>&2; \
	         exit 1) ) \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && $(MAKE) $(AM_MAKEFLAGS) distclean \
	  && rm -f $(distdir).tar.gz \
	  && (test `find . -type f -print | wc -l` -eq 0 \
	     || (echo "Error: files left after distclean" 1>&2; \
	         exit 1) )
	-chmod -R a+w $(distdir) > /dev/null 2>&1; rm -rf $(distdir)
	@echo "$(distdir).tar.gz is ready for distribution" | \
	  sed 'h;s/./=/g;p;x;p;x'
check-am: all-am
check: check-recursive
all-am: Makefile $(LTLIBRARIES) $(HEADERS) config.h strfunc.h getopt.h
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs) $(DESTDIR)$(libdir) $(DESTDIR)$(includedir)

install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
	-test -z "cfg.h$(BUILT_SOURCES)" || rm -f cfg.h $(BUILT_SOURCES)
clean: clean-recursive

clean-am: clean-generic clean-libLTLIBRARIES clean-libtool \
	mostlyclean-am

dist-all: distdir
	$(AMTAR) chof - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)
distclean: distclean-recursive
	-rm -f config.status config.cache config.log
distclean-am: clean-am distclean-compile distclean-depend \
	distclean-generic distclean-hdr distclean-libtool \
	distclean-tags

dvi: dvi-recursive

dvi-am:

info: info-recursive

info-am:

install-data-am: install-includeHEADERS

install-exec-am: install-libLTLIBRARIES

install-info: install-info-recursive

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-recursive

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

uninstall-am: uninstall-includeHEADERS uninstall-info-am \
	uninstall-libLTLIBRARIES

uninstall-info: uninstall-info-recursive

.PHONY: $(RECURSIVE_TARGETS) GTAGS all all-am check check-am clean \
	clean-generic clean-libLTLIBRARIES clean-libtool \
	clean-recursive dist dist-all distcheck distclean \
	distclean-compile distclean-depend distclean-generic \
	distclean-hdr distclean-libtool distclean-recursive \
	distclean-tags distdir dvi dvi-am dvi-recursive info info-am \
	info-recursive install install-am install-data install-data-am \
	install-data-recursive install-exec install-exec-am \
	install-exec-recursive install-includeHEADERS install-info \
	install-info-am install-info-recursive install-libLTLIBRARIES \
	install-man install-recursive install-strip installcheck \
	installcheck-am installdirs installdirs-am \
	installdirs-recursive maintainer-clean maintainer-clean-generic \
	maintainer-clean-recursive mostlyclean mostlyclean-compile \
	mostlyclean-generic mostlyclean-libtool mostlyclean-recursive \
	tags tags-recursive uninstall uninstall-am \
	uninstall-includeHEADERS uninstall-info-am \
	uninstall-info-recursive uninstall-libLTLIBRARIES \
	uninstall-recursive


.PHONY: lines
lines:
	wc `find . -type f \( -name "*.c" -or -name "*.h" -or -name "*.in" -or -name "*.html" -or -name "*.conf" -or -name "*.3" \) -print`
# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
