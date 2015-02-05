#ifndef InitialFit_h 
#define InitialFit_h

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "RooAbsReal.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFitResult.h"

class InitialFit {

  public:

    InitialFit(RooRealVar *massVar, RooRealVar *MHvar, int mhLow, int mhHigh, std::vector<int> skipMasses);
    ~InitialFit();

    void buildSumOfGaussians(std::string name, int nGaussians, bool recursive=false, bool forceFracUnity=false);
    void loadPriorConstraints(std::string filename, float constraintValue);
    void saveParamsToFile(std::string filename);
    void saveParamsToFileAtMH(std::string filename, int setMH);
    std::map<int,std::map<std::string,RooRealVar*> > getFitParams();
		void printFitParams();
    void setDatasets(std::map<int,RooDataSet*> data);
    void addDataset(int mh, RooDataSet *data);
    void runFits(int ncpu);
    void plotFits(std::string name);
    void setVerbosity(int v);

    void setFitParams(std::map<int,std::map<std::string,RooRealVar*> >& pars );
  private:

    RooRealVar *mass;
    RooRealVar *MH;
    std::map<int,RooAddPdf*> sumOfGaussians;
    std::map<int,RooDataSet*> datasets; 
    std::map<int,std::map<std::string,RooRealVar*> > fitParams;
    std::map<int,std::map<std::string,RooAbsReal*> > fitUtils;
    std::map<int,std::map<std::string,RooGaussian*> > initialGaussians;
    std::map<int,RooFitResult*> fitResults;
    int mhLow_;
    int mhHigh_;
		std::vector<int> skipMasses_;
    std::vector<int> allMH_;
    std::vector<int> getAllMH();
		bool skipMass(int mh);
    int verbosity_;

};

#endif
