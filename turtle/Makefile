# ----------------------------------------------------------------------------
# Build libturtlebinning.so
# Created 27 Feb 2013 HBP & Sezen Sekmen
#         30 May 2015 HBP - standardize structure (src, lib, include)
#         08 Mar 2023 HBP - make compatible with conda and rename
#                           library to turtlebinning to avoid conflict with
#                           existing "turtle" Python module.
# ----------------------------------------------------------------------------
ifndef ROOTSYS
	$(error *** Please set up Root)
endif

ifndef TURTLE_PREFIX
ifdef CONDA_PREFIX
TURTLE_PREFIX := $(CONDA_PREFIX)
endif
endif

PYVER	:= $(shell python --version | cut -d' ' -f2)
PY1	:= $(shell echo "$(PYVER)"  | cut -d. -f1)
PY2	:= $(shell echo "$(PYVER)"  | cut -d. -f2)
PYTHONLIB	:= python$(PY1).$(PY2)
#say := $(shell echo "$(PYTHONLIB)" >& 2)
#$(error bye)

ROOFIT	:= $(ROOTSYS)
# ----------------------------------------------------------------------------
NAME	:= turtlebinning
incdir	:= include
srcdir	:= src
libdir	:= lib

# create lib directory if one does not exist
$(shell mkdir -p lib)

# get lists of sources

SRCS	:=  	$(srcdir)/Turtle.cc

CINTSRCS:= $(wildcard $(srcdir)/*_dict.cc)

OTHERSRCS:= $(filter-out $(CINTSRCS) $(SRCS),$(wildcard $(srcdir)/*.cc))

# list of dictionaries to be created
DICTIONARIES	:= $(SRCS:.cc=_dict.cc)

# get list of objects
OBJECTS		:= $(SRCS:.cc=.o) $(OTHERSRCS:.cc=.o) $(DICTIONARIES:.cc=.o)
#say := $(shell echo "DICTIONARIES:     $(DICTIONARIES)" >& 2)
#say := $(shell echo "" >& 2)
#say := $(shell echo "SRCS: $(SRCS)" >& 2)
#say := $(shell echo "OBJECTS: $(OBJECTS)" >& 2)
#$(error bye)
# ----------------------------------------------------------------------------
ROOTCINT	:= rootcint
# check for clang++, otherwise use g++
COMPILER	:= $(shell which clang++)
ifneq ($(COMPILER),)
CXX		:= clang++
LD		:= clang++
else
CXX		:= g++
LD		:= g++
endif

CPPFLAGS	:= -I. -I$(incdir)
CXXFLAGS	:= $(shell root-config --cflags) -fPIC
LDFLAGS		:= -g
# ----------------------------------------------------------------------------
# which operating system?
OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	LDFLAGS += -dynamiclib
	LDEXT	:= .dylib
else
	LDFLAGS	+= -shared
	LDEXT	:= .so
endif
LDFLAGS += $(shell root-config --ldflags)
LIBS	+= $(shell root-config --libs)
LIBRARY	:= $(libdir)/lib$(NAME)$(LDEXT)
# ----------------------------------------------------------------------------
all: $(LIBRARY)


ifdef TURTLE_PREFIX
install:
	cp $(incdir)/Turtle.h $(TURTLE_PREFIX)/include
	cp $(libdir)/lib$(NAME)$(LDEXT) $(TURTLE_PREFIX)/lib
	find $(libdir) -name "*.pcm" -exec cp {} $(TURTLE_PREFIX)/lib \;
	cp $(NAME).py $(TURTLE_PREFIX)/lib/$(PYTHONLIB)/site-packages
endif


$(LIBRARY)	: $(OBJECTS)
	@echo ""
	@echo "=> Linking shared library $@"
	$(LD) $(LDFLAGS) $^ $(LIBS)  -o $@

$(OBJECTS)	: %.o	: 	%.cc
	@echo ""
	@echo "=> Compiling $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(DICTIONARIES)	: $(srcdir)/%_dict.cc	: $(incdir)/%.h
	@echo ""
	@echo "=> Building dictionary $@"
	$(ROOTCINT) -f $@ -c $(CPPFLAGS) $^
	find $(srcdir) -name "*.pcm" -exec mv {} $(libdir) \;

tidy:
	rm -rf $(srcdir)/*_dict*.* $(srcdir)/*.o 

clean:
	rm -rf $(libdir)/* $(srcdir)/*_dict*.* $(srcdir)/*.o


ifdef TURTLE_PREFIX
uninstall:
	rm -rf $(TURTLE_PREFIX)/lib/*$(NAME)*
	rm -rf $(TURTLE_PREFIX)/include/Turtle.h
	rm -rf $(TURTLE_PREFIX)/lib/$(PYTHONLIB)/site-packages/$(NAME).py
endif

