CC = g++
ROOFITBASE=$(ROOFITSYS)
ROOFITLIBS=$(ROOFITBASE)/lib
ROOFITINCS=$(ROOFITBASE)/include
BOOSTBASE=$(CMSSW_DATA_PATH)/../external/boost/1.47.0
BOOSTLIBS=$(BOOSTBASE)/lib
BOOSTINCS=$(BOOSTBASE)/include

DEPENDENTS = `root-config --cflags --libs` -L$(ROOFITLIBS) -I$(ROOFITINCS) -lTMVA -lMinuit -lFoam -lRooFit -lRooFitCore -I$(BOOSTINCS) -L$(BOOSTLIBS) -lboost_program_options 
FILES_TRAIN = SidebandMVATraining.cc 
OUT_EXE_TRAIN = runSidebandMVATraining.exe

all: Train

Train: $(FILES_TRAIN)
	$(CC) -g -O2 -o $(OUT_EXE_TRAIN) $(FILES_TRAIN) $(DEPENDENTS)

clean:
	rm $(OUT_EXE)
