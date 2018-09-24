#!/usr/bin/make -f
 ################################################################
 # $ID: Makefile        Sun Oct 27 13:56:17 2002 CST noclouds $ #
 #                                                              #
 # Description: A general  makefile for program.                #
 #                                                              #
 # Author:      ������  <mhfan@ustc.edu>                        #
 #                                                              #
 # Maintainer:  M.H. Fan  <mhfan@ustc.edu>                      #
 #              Laboratory of Structural Biology                #
 #              School of Life Science                          #
 #              Univ. of Sci.& Tech. of China (USTC)	        #
 #              People's Republic of China (PRC)                #
 #                                                              #
 # CopyRight (c)  2005  M.H.Fan                                 #
 # CopyRight (c)  2003  Meihui Fan                              #
 #   All rights reserved.                                       #
 #                                                              #
 # This file is free software;                                  #
 #   you are free to modify and/or redistribute it  	        #
 #   under the terms of the GNU General Public Licence.         #
 #                                                              #
 # No warranty, no liability; use this at your own risk!        #
 #                                                              #
 # George Foot 2001-02-16 13:56:40			        #
 # email: george.foot@merton.ox.ac.uk			        #
 #                                                              #
 # Copyright (c) 1997 George Foot			        #
 #      All rights reserved.				        #
 #                                                              #
 # No warranty,no liability; you use this at your own risk.     #
 # You are free to modify and distribute this		        #
 #  without giving credit to the original author.	        #
 ################################################################

### Customising

#BOOSTINCDIR	= /usr/src/boost
#BOOSTLIBDIR	= /usr/src/boost/stage/lib
#BOOSTLIBS	= date_time filesystem iostreams program_options regex \
#		  serialization signals thread wserialization \
#		  #python unit_test_framework prg_exec_monitor test_exec_monitor

## don't need to add -I and -l prefix!
INC		 = /usr/include/cairo /usr/include/boost
LIBS 		 = jpeg z mad twin cairo $(CXXLIBS) $(CLIBS)
EXECS 		:= fplayer
SONAME		:=
SRCS		:= # $(wildcard *.cpp)
SUFF		+= # already contained(.c .cpp .cxx .cc .C)
SUBD		:=
SUBMK		:=
DEFS		:= -O3 #-DTEST -DDEBUG -g
#MARCH       := $(shell uname -m)
MARCH       := x86-64 # avoiding error: cc1plus: error: bad value (‘x86_64’) for ‘-march=’ switch


OBJDIR		:= .obj
LDFLAGS		:= -Wl,--as-needed \
		#-L/usr/local/src/cairo-git/twin/.libs \
		#-L$(HOME)/lib -L$(HOME)/lib/mhfan #-L$(BOOSTLIBDIR)
SOFLAGS		:= -D_REENTRANT -fPIC -shared -Wl,-soname,$(SONAME)
CPPFLAGS	:= -MD
CINC		:= $(INC) $(HOME)/include $(HOME)/include/mhfan
CFLAGS		:= $(DEFS) -Wall -Wno-multichar -pipe -pthread \
		    -march=$(MARCH) -mtune=k8 -fomit-frame-pointer \
		    -ffast-math -funroll-loops -fforce-addr -fpermissive \
		    #-finput-charset=GBK -falign-functions=4 \
		    #-Wunused -Wno-implicit
CLIB		:= #l ??? # (e.g.:alleg,stdcx,iostr,etc)
CXXINC		:= $(CINC) $(BOOSTINCDIR)
CXXFLAGS	:= $(CFLAGS)
CXXLIBS		:= $(addprefix boost_, $(addsuffix -gcc-mt-1_33, $(BOOSTLIBS)))
ASFLAGS		:=
LFLAGS		:=
YFLAGS		:= -d
FFLAGS		:=
ARFLAGS		:= crusv	    # for static library

	# Now alter any implicit rules' variables if you like.
CC		:= gcc #-s
CXX		:= g++ #-s
LINKER		:= $(CXX)
LEX		:= flex
YACC		:= bison
AR		:= ar
AS		:= as
FC		:= f77

RM		:= rm -rf

	# You shouldn't need to change anything below this point.
override VPATH 	 = $(subst  ,:,$(SUBD) $(foreach file,$(SRCS),$(dir $(file))))

CFLAGS 		+= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

SUFF		+= .c .cpp .cxx .cc .C
SUBSRC		:= $(filter %$(SUFF),$(foreach dir,$(SUBD), \
			$(wildcard $(dir)/*))) # ???
SRCS		+= $(SUBSRC) $(foreach suff,$(SUFF),$(wildcard *$(suff)))
OBJS   		:= $(addprefix $(OBJDIR)/,$(addsuffix .o, \
			$(basename $(notdir $(SRCS)))))
DEPS		:= $(patsubst %.o,%.d,$(OBJS))
MISS_DEPS	:= $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISS_DEPS_SRCS 	:= $(wildcard $$(patsubst %.d,%.c, $(MISS_DEPS)) \
			$(patsubst %.d,%.cpp,$(MISS_DEPS))) # ???

ifeq (,$(findstring .cpp,$(SRCS)))
    LINKER	:=	$(CC)
endif

ifneq (,$(SUBMK))
    SUBS	:= submake
endif

$(OBJDIR)/%.o	: %.c 	; @mkdir -p $(OBJDIR)
	$(CC)  -o $@  -c $(CFLAGS) $(CPPFLAGS)   $(addprefix -I,$(CINC))    $<
$(OBJDIR)/%.o	: %.cpp ; @mkdir -p $(OBJDIR)
	$(CXX) -o $@  -c $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I,$(CXXINC))  $<
$(OBJDIR)/%.o	: %.cxx ; @mkdir -p $(OBJDIR)
	$(CXX) -o $@  -c $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I,$(CXXINC))  $<
$(OBJDIR)/%.o	: %.cc 	; @mkdir -p $(OBJDIR)
	$(CXX) -o $@  -c $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I,$(CXXINC))  $<
$(OBJDIR)/%.o	: %.C 	; @mkdir -p $(OBJDIR)
	$(CXX) -o $@  -c $(CXXFLAGS) $(CPPFLAGS) $(addprefix -I,$(CXXINC))  $<

#lib%.a		: $(OBJDIR)/%.o

lib%.so		: $(OBJDIR)/%.o
	$(LINKER) -o $@  $(OBJS)  $(SOFLAGS) $(LDFLAGS) $(addprefix -l,$(LIBS))
ifeq (,$(SONAME))
    $(SOFLAGS)	= $(SOFLAGS)$@
endif

%		: $(OBJDIR)/%.o
	$(LINKER) -o $@    $(LDFLAGS)  $(addprefix -l,$(LIBS))    $< #$(OBJS)

.PHONY: all deps objs clean distclean rebuild install strip $(SUBS)

all		: $(SUBS) $(EXECS) ;	@mkdir -p $(OBJDIR)
deps		: $(DEPS)
objs		: $(OBJS)
strip		: ;		strip --strip-all $(EXECS)
rebuild		: distclean all
submake		: ; 		@for dir in $(SUBMK); do $(MAKE) -C $$dir; done
distclean	: ; 		@$(RM) $(OBJDIR) $(EXECS)
ifneq (,$(SUBMK))
	@for dir in $(SUBMK); do $(MAKE) $@ -C $$dir; done
endif
clean		: ;		@$(RM) $(OBJDIR)/*.o
ifneq (,$(SUBMK))
	@for dir in $(SUBMK); do $(MAKE) $@ -C $$dir; done
endif
$(EXECS): %: $(OBJS)
	$(LINKER) -o $@    $($@)    $(LDFLAGS)  $(addprefix -l,$(LIBS)) #$^
install		:

fplayer		:= $(OBJS)
#main		:= $(OBJS)
#main_OBJS	: $(main); #%_OBJS

.SUFFIXES: .tex .dvi .pdf .ps

%.pdf		: %.tex;	pdflatex $<; pdflatex $<
%.dvi		: %.tex;	latex    $<; latex    $<
%.ps		: %.tex;	dvips    $<; dvips    $<
%.bbl		: %.aux;	bibtex   $(subst .aux,,$<)

clean-doc	:
	@find -regex ".*\(chk\|log\|aux\|bbl\|blg\|ilg\|toc\|lof\|lot\|idx" \
		-print0 | xargs -0r rm -f
	@find -regex "ind\|out\|lol\|thm\|hdir\|t2p\|bak\)" \
		-print0 | xargs -0r rm -f

ifneq ($(MISS_DEPS),)
    $(MISS_DEPS): ; 		@$(RM) $(patsubst %.d,%.o,$@)
endif

ifneq (,$(DEPS))
    -include $(DEPS)
endif

# vim:sts=4:ts=8:
