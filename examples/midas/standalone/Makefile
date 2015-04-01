CXX           := g++

PWDST := $(shell pwd)

ifdef ROOTSYS
ROOTCONFIG := $(ROOTSYS)/bin/root-config
ROOTCINT   := $(ROOTSYS)/bin/rootcint
else
ROOTCONFIG := root-config
ROOTCINT   := rootcint
endif

# directory where mxml.c and strlcpy.c resides
CXXFLAGS      := -g -O -Wall -fexceptions -I../include -I$(ROMESYS)/include $(shell $(ROOTCONFIG) --cflags)
LIBS          := $(shell $(ROOTCONFIG) --libs) -Wl,-rpath=$(PWDST)/../obj  -Wl,-rpath=$(ROMESYS)

ROMELIB = -L../obj -lmidanalyzer
ifeq ($(LIBROME), yes)
ROMELIB += -L$(ROMESYS) -lrome
endif

all: analyzer

analyzer: main.cc
	$(CXX) $(CXXFLAGS) $< -o analyzer $(LIBS) $(ROMELIB)

obj:
	@if [ ! -d  obj ] ; then \
		echo "Making directory obj" ; \
		mkdir obj; \
	fi;

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -MT $@ -c $< -o $@

clean:
	$(RM) obj/*.o obj/*.d *~
