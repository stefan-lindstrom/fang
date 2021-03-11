# Generated automatically from Makefile.in by configure.
# CircleMUD 3.0 Makefile.in - Makefile template used by 'configure'
#

# C compiler to use
CC = gcc

# Any special flags you want to pass to the compiler
MYFLAGS = -std=gnu90 -I/usr/include/libxml2

#flags for profiling (see hacker.doc for more information)
# Uncomment this if no dmallocinf is to be done. Comment it out if debugging
#PROFILE=-DDMALLOC_DISABLE

LUAINCDIR = -I/home/fang/git/fang/lua-5.0/include
LUALIBDIR = -L/home/fang/git/fang/lua-5.0/lib
LUALIBS   = -llualib -llua

#DMALLOCLIBDIR = /home/fang/dmalloc/lib
#DMALLOCINCDIR = /home/fang/dmalloc/include
#DMALLOCLIBS   = -ldmalloc -ldmalloclp

XMLLIBS   = -lz -lxml

#GLIBINCDIR = `glib-config --cflags glib`
GLIBINCDIR = -I/usr/local/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/local/lib64/glib-2.0/include/

#GLIBLIBS = `glib-config --libs glib`
GLIBLIBS = -L/usr/local/include/glib-2.0/ -lglib-2.0

STRLIBDIR=-L/home/fang/git/fang/libstrfunc-7.4.6/.libs
STRINCDIR=-I/home/fang/git/fang/libstrfunc-7.4.6
STRLIB=-lstrfunc

INCDIRS = $(LUAINCDIR) $(GLIBINCDIR) $(STRINCDIR)
LIBDIRS = $(LUALIBDIR) $(STRLIBDIR)
LIBS    = $(LUALIBS) $(XMLLIBS) -lcrypt -lm $(GLIBLIBS) $(STRLIB)

##############################################################################
# Do Not Modify Anything Below This Line (unless you know what you're doing) #
##############################################################################

BINDIR = ../bin

CFLAGS = -g $(MYFLAGS) $(PROFILE) $(INCDIRS)

OBJFILES = comm.o color.o act.comm.o act.guild.o act.informative.o act.movement.o \
        act.item.o act.offensive.o act.other.o act.social.o act.wizard.o  \
	act.wizard2.o ban.o boards.o class.o config.o constants.o db.o fight.o graph.o \
	handler.o interpreter.o limits.o magic.o mail.o mobact.o mobprog.o \
	modify.o portstone.o prefight.o xmlBoards.o newbiegoto.o \
	objprog.o olc.o shop.o spec_assign.o spec_procs.o \
	spell_parser.o spells.o utils.o weather.o redit.o oedit.o zedit.o medit.o \
	sedit.o random.o craft.o alias.o aedit.o mcl.o tree.o \
	bitvector.o strutil.o skimming.o hotboot.o xmlobjsave.o dll.o \
	xmlRooms.o namedspec.o xmlZones.o cards.o skills.o xmlObjects.o \
	xmlMobiles.o guild_parser.o guild.o hedit.o chess.o threadboard.o \
	xmlcharsave.o dg_comm.o dg_db_scripts.o dg_event.o dg_handler.o \
        dg_misc.o dg_mobcmd.o dg_objcmd.o dg_olc.o dg_scripts.o dg_triggers.o \
        dg_wldcmd.o xml.o pedit.o stoneedit.o retainer.o dynvars.o messages.o \
        msgedit.o


%.d: %.c
	$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< | \
	sed '\''s/$*\\.o[ ]*/& $@/g'\'' > $@'

default: circle

-include $(OBJFILES:.o=.d)

all:
	$(MAKE) $(BINDIR)/circle
	$(MAKE) utils

utils:
	(cd util; $(SHELL) Configure; $(MAKE) all)


circle:
	$(MAKE) $(BINDIR)/circle

$(BINDIR)/circle : $(OBJFILES)
	$(CC) -o $(BINDIR)/circle $(PROFILE) $(OBJFILES) $(LIBDIRS) $(LIBS)


clean:
	rm -f *.o *.d
	(cd util; $(MAKE) clean)

distclean:
	rm -f *.o *.d *~
	rm -f \
	  $(BINDIR)/circle
	(cd util; $(MAKE) distclean)
