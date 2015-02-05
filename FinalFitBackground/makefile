# name of the library
LIBNAME = BackgroundProfileFitting

#Necessary to use shell built-in commands
SHELL=bash

# figure out os
UNAME := $(shell uname)
BASEDIR = $(shell pwd)

$(shell mkdir -p bin)
$(shell mkdir -p lib)
$(shell mkdir -p obj)

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
USERLIBS += -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore -lRooStats
USERLIBS += -L$(CMSSW_BASE)/lib 
endif

USERLIBS += $(shell root-config --glibs) -lTMVA -lMinuit -lFoam -lRooFit -lRooFitCore -lMathMore -lRooStats 
CXXFLAGS = -Wall -g -Wl,-rpath=$(BASEDIR)/lib 
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
INCDIR = $(BASEDIR)/interface
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

all:  $(BINS)

$(EXEDIR)/%: $(TESTDIR)/%.cpp $(LIBDIR)/lib$(LIBNAME).so $(BASEDIR)/interface/*.h  
	@echo Compiling executable $@
	@$(CXX) -o $@ $(CXXFLAGS) $< $(ROOT_DICT) $(ROOTCLASSES) $(LIBS) -L$(LIBDIR) -l$(LIBNAME) $(USERLIBSX)

$(OBJDIR)/%.$(OBJ_EXT): $(SRCDIR)/%.cc $(ROOT_DICT) 
	@echo Making object $@
	@$(CXX) $(CXXFLAGS) -fPIC -c $<  -o $@

$(LIBDIR)/lib$(LIBNAME).so: $(OBJS) $(ROOT_OBJ)
	@echo Building shared library $@
	@$(LD) $(LDFLAGS) -o $(LIBDIR)/lib$(LIBNAME).so $^ $(LIBS)
	
$(ROOT_DICT): $(SRCS) $(HEADERS)
	@echo Making dictionary $@
	@rootcint -f $@ -c -L$(ROOFITSYS)/lib -I$(ROOFITSYS)/include $(HEADERS)
	@$(CXX) $(CXXFLAGS) -fPIC -c $(ROOT_DICT) -o $(ROOT_OBJ)


lib: $(LIBDIR)/lib$(LIBNAME).so

vars:
	@echo "LIBS: " $(LIBS)
	@echo "CXXFLAGS: " $(CXXFLAGS)
	@echo "Header files: " $(HEADERS)
	@echo "Source files: " $(SRCS) 
	@echo "Root H files: " $(ROOTHEADERS)
	@echo "Root S files: " $(ROOTCLASSES) 
	@echo "Object files: " $(OBJS)
	@echo "Executables:  " $(TARGETS)

clean:
	@echo "Cleaning: " 
	@echo "  " $(OBJS)
	@echo "  " $(LIBDIR)/lib$(LIBNAME).so
	@echo "  " $(BINS)
	@echo "  " $(ROOT_DICT)
	@rm -rf $(OBJS) $(LIBDIR)/lib$(LIBNAME).so $(BINS) $(ROOT_DICT)


