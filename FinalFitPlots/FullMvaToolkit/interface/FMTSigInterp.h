#ifndef FMTSigInterp_h
#define FMTSigInterp_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "FMTBase.h"
#include "TGraph.h"
#include "TF1.h"

#include "../../Normalization_8TeV.h"

using namespace std;

class FMTSigInterp : public FMTBase {

  public:
    FMTSigInterp(string);
    FMTSigInterp(string, double, bool, bool, bool, int, int, double, double, double, int, double, double, int, int, int, double, double, int, bool, int, bool, int, vector<string>, bool, vector<map<int,vector<double> > >, bool blind=false,bool verbose=false);
    ~FMTSigInterp();

    TH1F *Interpolate(double,TH1F*,double,TH1F*,double,  bool smoothEff=false, std::string prod="nothing");

    void runInterpolation();

  private:
    TFile *tFile;
    Normalization_8TeV *normalizer;
   // bool diagnose_;
   // bool blind_;
	
    void makeEfficiencyGraphs();
    std::map<std::string,TGraph*> efficiencyGraphs;
};

#endif
