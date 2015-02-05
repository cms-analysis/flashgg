UNAME := $(shell uname)

#------------------------------------------------------------------------------
# where to find the 'framework'
FRAMEWORKDIR=../../framework


CXXFLAGS += -I$(FRAMEWORKDIR)


#------------------------------------------------------------------------------
# setup for BOOST (take the CMSSW boost if defined)
#------------------------------------------------------------------------------
ifeq ($(UNAME), Linux)
BOOST_DIR=$(shell scram tool tag boost BOOST_BASE)
PYTHON_BASE=$(shell scram tool tag python PYTHON_BASE)
endif

CXXFLAGS += -I$(BOOST_DIR)/include
LDFLAGS += -L$(BOOST_DIR)/lib

CXXFLAGS += -fPIC


ifeq ($(UNAME), Darwin)
# needed to allow for undefined symbols at linking time
# when building the shared object. See http://stackoverflow.com/a/5201180/288875
CXXFLAGS += -I/opt/local//Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
CXXFLAGS += -undefined dynamic_lookup

  LDLIBS += -lboost_regex-mt
else
  LDLIBS += -lboost_regex 

endif

ifeq ($(UNAME), Linux)
CXXFLAGS += -I$(PYTHON_BASE)/include/python2.6
LDLIBS +=  -L$(PYTHON_BASE)/lib -lpython2.6
endif


#------------------------------------------------------------------------------

ALL_EXECUTABLES = ICHEP2012LikeModel.so

all:		$(ALL_EXECUTABLES)


ICHEP2012LikeModel.so:	SignalFitModel.o \
			SystematicsModel.o \
			FitParameterSystematics.o \
			SignalModel.o \
			Config.o
			g++ -shared $(CXXFLAGS) -L$$ROOFITSYS/lib -L$(BOOST_DIR)/lib -lRooFit -lRooFitCore -lFoam -lMinuit -lMathMore -g  $^ -o $@ $$($$ROOTSYS/bin/root-config --glibs --cflags) -I$$ROOFITSYS/include -I$(BOOST_DIR)/include 

# COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

COMPILE.cc = $(CXX) $(CXXFLAGS) -g $$($$ROOTSYS/bin/root-config --cflags) -I$$ROOFITSYS/include -I$$BOOST_DIR/include -Wall -c 

COMPILE.C = $(COMPILE.cc)

clean:
		-rm -f *.o 
		-rm -f *.so
		-rm -f *~
		-rm -f $(ALL_EXECUTABLES)
