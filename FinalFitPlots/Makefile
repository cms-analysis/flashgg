include ../Makefile.arch

ROOFIT_BASE=$(ROOFITSYS)
LDFLAGS+=-L$(ROOFIT_BASE)/lib $(ROOTLIBS) -lRooFitCore -lRooFit -lTMVA
CXXFLAGS+=-I$(ROOFIT_BASE)/include  -I$(CMSSW_BASE)/src

CXXFLAGS+=-I$(shell pwd)
