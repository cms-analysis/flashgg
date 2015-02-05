#ifndef Packager_h 
#define Packager_h

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Normalization_8TeV.h"

class Packager {

  public:

    Packager(RooWorkspace *ws, std::vector<std::string> procs, int nCats, int mhLow, int mhHigh, std::vector<int> skipMasses, bool is2011=false, bool skipPlots=false, string outDir="plots",
	     RooWorkspace *mergeWs=0, const std::vector<int>& cats=std::vector<int>() );
    ~Packager();

    void packageOutput();
		void makePlots();
		void makePlot(RooRealVar *mass, RooRealVar *MH, RooAddPdf *pdf, std::map<int,RooDataSet*> data, std::string name);

  private:
    RooWorkspace *outWS, *mergeWS;
    std::vector<std::string> procs_;
    int nCats_;
    std::vector<int> cats_;
    int mhLow_;
    int mhHigh_;
    bool is2011_;
		bool skipPlots_;
    string outDir_;
    int sqrts_;
    std::vector<int> skipMasses_;
    bool skipMass(int mh);
    Normalization_8TeV *normalization;

};
#endif
