PWDST := $(shell pwd)

ifdef ROOTSYS
ROOTCONFIG := $(ROOTSYS)/bin/root-config
ROOTCINT   := $(ROOTSYS)/bin/rootcint
else
ROOTCONFIG := root-config
ROOTCINT   := rootcint
endif


CXXFLAGS      := -g -O -Wall -fexceptions -I../include -I$(ROMESYS) -I$(ROMESYS)/include $(shell $(ROOTCONFIG) --cflags)
LIBS          := $(shell $(ROOTCONFIG) --libs) -lz

# Enable here to read compressed MIDAS files (.xz)
CXXFLAGS      += -DHAVE_XZ
LIBS          += -llzma

# Enable here to read compressed MIDAS files (.bz2)
CXXFLAGS      += -DHAVE_BZ2
LIBS          += -lbz2


CXX           := g++

# Local ROOT version
ROOT_VERSION := $(shell $(ROOTCONFIG) --version)
ROOT_MAJOR := $(shell $(ROOTCONFIG) --version 2>&1 | cut -d'.' -f1)
ROOT_MINOR := $(shell $(ROOTCONFIG) --version 2>&1 | cut -d'/' -f1 | cut -d'.' -f2)
ROOT_PATCH := $(shell $(ROOTCONFIG) --version 2>&1 | cut -d'/' -f2)

ifeq ($(shell expr $(ROOT_MAJOR) \< 6), 1)
  DICT_HEADER_SUF := .h
else
  DICT_HEADER_SUF := _rdict.pcm
endif

DICTIONARIES = ROMEMidasDict$(DICT_HEADER_SUF) 
ROMEMidasDictHeaders := $(ROMESYS)/include/ROMEString.h \
                        $(ROMESYS)/include/ROMEStrArray.h

ROMEObj := obj/ROMEMidasFile.o \
           obj/ROMEPrint.o \
           obj/ROMEString.o \
           obj/ROMEStrArray.o \
           obj/ROMEMidasDict.o

all: obj analyzer

analyzer: main.cc $(ROMEObj)
	$(CXX) $(CXXFLAGS) $< $(ROMEObj) -o $@ $(LIBS)

obj:
	@if [ ! -d  obj ] ; then \
		echo "Making directory obj" ; \
		mkdir obj; \
	fi;

obj/ROME%.o: $(ROMESYS)/src/ROME%.cpp $(ROMESYS)/include/ROME%.h
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -MT $@ -c $< -o $@

obj/ROME%Dict.o: ROME%Dict.cpp ROME%Dict$(DICT_HEADER_SUF)
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -MT $@ -c $< -o $@

ROMEMidasDict$(DICT_HEADER_SUF) ROMEMidasDict.cpp: $(ROMEMidasDictHeaders) Makefile
	$(ROOTCINT) -f ROMEMidasDict.cpp -c -p $(patsubst -stdlib=%,,$(patsubst -std=%,,$(CXXFLAGS))) $(ROMEMidasDictHeaders) ROMEMidasLinkDef.h

clean:
	$(RM) obj/*.o obj/*.d *~ ROMEMidasDict.cpp ROMEMidasDict$(DICT_HEADER_SUF)
