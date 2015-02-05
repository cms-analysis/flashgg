UNAME := $(shell uname)

#------------------------------------------------------------------------------
# setup for BOOST (take the CMSSW boost if defined)
#------------------------------------------------------------------------------
ifeq ($(UNAME), Linux)
BOOST_DIR=$(shell scram tool tag boost BOOST_BASE)
PYTHON_BASE=$(shell scram tool tag python PYTHON_BASE)
endif

CXXFLAGS += -I$(BOOST_DIR)/include
LDFLAGS += -L$(BOOST_DIR)/lib


ifeq ($(UNAME), Darwin)
CXXFLAGS += -I/opt/local//Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
# TODO: do we need the -L ?
LDLIBS += -L/opt/local//Library/Frameworks/Python.framework/Versions/2.7/lib -lpython2.7

  LDLIBS += -lboost_regex-mt
else
  LDLIBS += -lboost_regex 

endif

ifeq ($(UNAME), Linux)
CXXFLAGS += -I$(PYTHON_BASE)/include/python2.6
LDLIBS +=  -L$(PYTHON_BASE)/lib -lpython2.6
endif

#------------------------------------------------------------------------------

ALL_EXECUTABLES = fitSignal

all:		$(ALL_EXECUTABLES) 


fitSignal:	fitSignal.o ParametricSignalModelConfig.o utils.o \
			fitting/FittingStep.o \
			fitting/Fitter.o \
			fitting/FitFunction.o \
			interpolation/SignalModelParameterInterpolator.o \
			interpolation/SignalModelParameterInterpolatorMap.o \
			DataSetList.o \
			AbstractSignalModel.o \
			OutputRedirector.o \
			ReportMaker.o \
			PythonConfigUtils.o \
			NullSystematics.o \
			InputDataSetInfo.o \
			XsectUtil.o \
			DataSetTable.o \
			NormalizationComparison.o 
			g++ -L$$ROOFITSYS/lib -L$(BOOST_DIR)/lib -lRooFit -lRooFitCore -lFoam -lMinuit -lMathMore -g  $^ -o $@ $$($$ROOTSYS/bin/root-config --glibs --cflags) -I$$ROOFITSYS/include -I$(BOOST_DIR)/include -lXMLParser $(LDLIBS) 

# COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

COMPILE.cc = $(CXX) $(CXXFLAGS) -g $$($$ROOTSYS/bin/root-config --cflags) -I$$ROOFITSYS/include -I$$BOOST_DIR/include -Wall -c 

COMPILE.C = $(COMPILE.cc)

# %.o: 	%.cc
#		g++ -L$$ROOFITSYS/lib -lRooFit -lRooFitCore -lFoam -lMinuit -lMathMore -g -c $^ $$($$ROOTSYS/bin/root-config --glibs --cflags) -I$$ROOFITSYS/include -I$$BOOST_DIR/include -Wall



clean:
		-rm -f *.o fitting/*.o interpolation/*.o systematics/*.o 
		-rm -f *.so
		-rm -f *~
		-rm -f $(ALL_EXECUTABLES)
