#ifndef LinearInterp_h 
#define LinearInterp_h

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooAddition.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"

class LinearInterp {

  public:
    
    LinearInterp(RooRealVar *MHvar, int mhLow, int mhHigh, std::map<int,std::map<std::string,RooRealVar*> > fitParamVals, bool doSecMods, std::vector<int> skipMasses);
    ~LinearInterp();

    void setSecondaryModelVars(RooRealVar *mh_sm, RooRealVar *deltam, RooAddition *mh_2, RooRealVar *width);
    void interpolate(int nGaussians);
    std::map<std::string,RooSpline1D*> getSplines();
    void setVerbosity(int v);

  private:
    RooRealVar *MH;
    int mhLow_;
    int mhHigh_;
    std::map<int,std::map<std::string,RooRealVar*> > fitParams;
    bool doSecondaryModels;
    bool secondaryModelVarsSet;
		std::vector<int> skipMasses_;
    int verbosity_;
    std::vector<int> allMH_;
    std::vector<int> getAllMH();
		bool skipMass(int mh);
    std::map<std::string,RooSpline1D*> splines;
    
    RooRealVar *MH_SM;
    RooRealVar *DeltaM;
    RooAddition *MH_2;
    RooRealVar *higgsDecayWidth;
};

#endif
