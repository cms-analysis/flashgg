# name of the library
LIBNAME = SimultaneousSignalFit

#Necessary to use shell built-in commands
SHELL=bash

$(shell mkdir -p bin)
$(shell mkdir -p obj)
$(shell mkdir -p lib)
#ifeq ($(shell ls interface | grep Normalization_8TeV.h),)
#$(shell cp ../Macros/Normalization_8TeV.h interface/)
#endif
#ifeq ($(shell ls src | grep Normalization_8TeV.cc),)
#$(shell cp ../Macros/Normalization_8TeV.cc src/)
#endif

# figure out os
UNAME := $(shell uname)
BASEDIR = $(shell pwd)

# make sure inside CMSSW release (this is essential for getting RooSpline1D from combine package)
ifndef CMSSW_BASE
$(error Must be inside a CMSSSW release)
endif


COMBINELIB = $(CMSSW_BASE)/lib/$(SCRAM_ARCH)/libHiggsAnalysisCombinedLimit.so
#COMBINELIB = HiggsAnalysisCombinedLimit
H2GGLOBELIB = ../libLoopAll.so

USERINCLUDES += -I$(ROOTSYS)/include
ifeq ($(UNAME),Darwin)
$(info Found OS Darwin - using mac osx build)
BOOST_PATH=/Applications/boost_1_49_0
USERINCLUDES += -I$(BOOST_PATH)/include/
USERLIBS += -L$(BOOST_PATH)/libs/ -lboost_regex -lboost_program_options
endif
ifeq ($(UNAME),Linux)
$(info Found OS Linux - using linux build)
BOOST_PATH=$(shell scram tool info boost | awk -F= '/BOOST_BASE/ {  print $$2  }')
USERINCLUDES += -I$(BOOST_PATH)/include/
USERINCLUDES += -I $(CMSSW_BASE)/src/ -I $(CMSSW_RELEASE_BASE)/src/
USERLIBS += -L$(BOOST_PATH)/lib/ -lboost_regex -lboost_program_options
USERINCLUDES += -I$(ROOFITSYS)/include
USERLIBS += -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore
#USERLIBS += -L$(PWD)/../ -lLoopAll
#USERLIBSX += -L$(PWD)/../ -lLoopAll
endif

USERLIBS += $(shell root-config --glibs) -lTMVA -lMinuit -lFoam -lRooFit -lRooFitCore -lPyROOT 
CXXFLAGS = -Wall -g -Wl,-rpath=$(BASEDIR)/lib:$(BASEDIR)/../ 
LDFLAGS = -shared -W 

CXX=g++
LD=g++

INCLUDE +=  -I $(BASEDIR)/interface
INCLUDE += $(USERINCLUDES)
CXXFLAGS += $(INCLUDE)

LIBS += $(USERLIBS)
LIBS += -L$(CMSSW_BASE)/lib/$(SCRAM_ARCH)/ -lHiggsAnalysisCombinedLimit

# this is where you  would normaly have the lib, bin and source directories
LIBDIR = $(BASEDIR)/lib
EXEDIR = $(BASEDIR)/bin
MACRODIR = $(BASEDIR)/src
SRCDIR = $(BASEDIR)/src
OBJDIR = $(BASEDIR)/obj
TESTDIR = $(BASEDIR)/test
DOCDIR= $(BASEDIR)/docs
OBJ_EXT=o
TEST_EXT=cpp

ROOT_DICT=RootDict.cxx
ROOT_OBJ=$(subst cxx,$(OBJ_EXT),$(ROOT_DICT))
SRCS=$(wildcard $(BASEDIR)/src/*.cc)
EXES=$(wildcard $(BASEDIR)/test/*.cpp)
HEADERS=$(wildcard $(BASEDIR)/interface/*.h)
OBJS=$(subst $(SRCDIR), $(OBJDIR),$(subst cc,$(OBJ_EXT),$(SRCS)))
BINS=$(subst $(TESTDIR), $(EXEDIR),$(subst .$(TEST_EXT),,$(EXES)))

# special additions
#SRCS+=../Macros/Normlization_8TeV.cc
#HEADERS+=../Macros/Normlization_8TeV.h
#OBJS+=../Macros/Normlization_8TeV.o

all:  $(BINS)

$(EXEDIR)/%: $(TESTDIR)/%.cpp $(LIBDIR)/lib$(LIBNAME).so $(BASEDIR)/interface/*.h  
	@echo Compiling executable $@
	@$(CXX) -o $@ $(CXXFLAGS) $< $(ROOT_DICT) $(ROOTCLASSES) $(LIBS) -L$(LIBDIR) -l $(LIBNAME) $(USERLIBSX)
	#@$(CXX) -o $@ $(CXXFLAGS) $< $(ROOT_DICT) $(ROOTCLASSES) $(LIBS) -L$(LIBDIR) -l $(LIBNAME) $(USERLIBS)


$(OBJDIR)/%.$(OBJ_EXT): $(SRCDIR)/%.cc $(ROOT_DICT) $(COMBINELIB) 
	@echo Making object $@
	@$(CXX) $(CXXFLAGS) -fPIC -c -l $(COMBINELIB) $<  -o $@

$(LIBDIR)/lib$(LIBNAME).so:$(OBJS) $(ROOT_OBJ) $(COMBINELIB)
	@echo Building shared library $@
	@$(LD) $(LDFLAGS) -o $(LIBDIR)/lib$(LIBNAME).so  $^ $(LIBS) #-l $(COMBINELIB)

$(ROOT_DICT): $(SRCS) $(HEADERS)
	@echo Making dictionary $@
	@rootcint -f $@ -c -L$(ROOFITSYS)/lib -I$(ROOFITSYS)/include -L$(CMSSW_BASE)/lib -I$(CMSSW_BASE)/src $(HEADERS)

$(ROOT_OBJ): $(ROOT_DICT)
	@$(CXX) $(CXXFLAGS) -fPIC -c $(ROOT_DICT) -o $(ROOT_OBJ)

lib: $(LIBDIR)/lib$(LIBNAME).so

vars:
	@echo "LIBS: " $(LIBS)
	@echo "CXXFLAGS: " $(CXXFLAGS)
	@echo "Source files: " $(SRCS) 
	@echo "Object files: " $(OBJS)
	@echo "Executables:  " $(TARGETS)

clean:
	rm -rf $(OBJS) $(LIBDIR)/lib$(LIBNAME).so $(BINS) $(ROOT_DICT)


