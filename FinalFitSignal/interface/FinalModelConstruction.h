#ifndef FinalModelConstruction_h 
#define FinalModelConstruction_h

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "TGraph.h"

#include "RooAbsReal.h"
#include "RooAddition.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooConstVar.h"
#include "RooWorkspace.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"
#include "Normalization_8TeV.h"

class FinalModelConstruction {

  public:
    
    FinalModelConstruction(RooRealVar *massVar, RooRealVar *MHvar, RooRealVar *intL, int mhLow, int mhHigh, std::string proc, int cat, bool doSecMods, std::string systematicsFileName, std::vector<int> skipMasses, int verbosity, bool isCB=false, bool is2011=false, bool quadraticSigmaSum=false);
    ~FinalModelConstruction();

		void loadSignalSystematics(std::string filename);
		void printSignalSystematics();

    void setSecondaryModelVars(RooRealVar *mh_sm, RooRealVar *deltam, RooAddition *mh_2, RooRealVar *width);

    void buildRvWvPdf(std::string name, int nGrv, int nGwv, bool recusive);
    void buildStdPdf(std::string name, int nGaussians, bool recursive);
    std::vector<RooAddPdf*> buildPdf(std::string name, int nGaussians, bool recursive, std::map<std::string,RooSpline1D*> splines, string add="");
    void getRvFractionFunc(std::string name);
    void setupSystematics();
    void getNormalization();

		RooAbsReal *getMeanWithPhotonSyst(RooAbsReal *dm, string name, bool isMH2=false, bool isMHSM=false);
		RooAbsReal *getSigmaWithPhotonSyst(RooAbsReal *sig_fit, string name);
		RooAbsReal *getRateWithPhotonSyst(string name);
    
		void setRVsplines(std::map<std::string,RooSpline1D*> splines);
    void setWVsplines(std::map<std::string,RooSpline1D*> splines);
    void setSTDsplines(std::map<std::string,RooSpline1D*> splines);

    void setRVdatasets(std::map<int,RooDataSet*> data);
    void setWVdatasets(std::map<int,RooDataSet*> data);
    void setSTDdatasets(std::map<int,RooDataSet*> data);
		void makeSTDdatasets();

		void setHighR9cats(std::string catString);
		void setLowR9cats(std::string catString);

    void plotPdf(std::string outDir);

    void save(RooWorkspace *work);

  private:
    
    RooRealVar *mass;
    RooRealVar *MH;
    RooRealVar *intLumi;
    int mhLow_;
    int mhHigh_;
    std::string proc_;
    int cat_;
    int nIncCats_;
    bool doSecondaryModels;
    bool secondaryModelVarsSet;
    bool isCutBased_;
		bool is2011_;
		int sqrts_;
		bool quadraticSigmaSum_;
		std::vector<int> skipMasses_;
    std::vector<int> allMH_;
    std::vector<int> getAllMH();
		bool skipMass(int mh);
    int verbosity_;
    Normalization_8TeV *norm;

    std::map<std::string,RooSpline1D*> stdSplines;
    std::map<std::string,RooSpline1D*> rvSplines;
    std::map<std::string,RooSpline1D*> wvSplines;
    std::map<int,RooDataSet*> rvDatasets;
    std::map<int,RooDataSet*> wvDatasets;
    std::map<int,RooDataSet*> stdDatasets;
   
	 	// vertex and r9 nuisances
    RooRealVar *vertexNuisance;
		RooRealVar *r9barrelNuisance;
		RooRealVar *r9mixedNuisance;
		std::vector<int> highR9cats;
		std::vector<int> lowR9cats;
    RooSpline1D *rvFracFunc;
    RooSpline1D *rvFracFunc_SM;
    RooSpline1D *rvFracFunc_2;
    RooSpline1D *rvFracFunc_NW;

    RooAddPdf *finalPdf;
    RooAbsReal *finalNorm;
    RooAbsReal *finalNormThisLum;
    RooExtendPdf *extendPdfRel;
    RooExtendPdf *extendPdf;
    // secondary models
    RooAddPdf *finalPdf_SM;
    RooAddPdf *finalPdf_2;
    RooAddPdf *finalPdf_NW;
    RooAbsReal *finalNorm_SM;
    RooAbsReal *finalNorm_2;
    RooAbsReal *finalNorm_NW;

    bool systematicsSet_;
    bool rvFractionSet_;

    RooSpline1D *graphToSpline(string name, TGraph *graph);
    RooSpline1D *graphToSpline(string name, TGraph *graph, RooAbsReal *var);

    std::map<std::string,RooSpline1D*> xsSplines;
    RooSpline1D *brSpline;
    // secondary models
    std::map<std::string,RooSpline1D*> xsSplines_SM;
    RooSpline1D *brSpline_SM;
    std::map<std::string,RooSpline1D*> xsSplines_2;
    RooSpline1D *brSpline_2;
    std::map<std::string,RooSpline1D*> xsSplines_NW;
    RooSpline1D *brSpline_NW;

    RooRealVar *MH_SM;
    RooRealVar *DeltaM;
    RooAddition *MH_2;
    RooRealVar *higgsDecayWidth;

		// photon systematic stuff
		std::vector<std::string> photonCatScales;
		std::vector<std::string> photonCatScalesCorr;
		std::vector<std::string> photonCatSmears;
		std::vector<std::string> photonCatSmearsCorr;
		std::vector<std::string> globalScales;
		std::vector<std::string> globalScalesCorr;
		// these are required to know specific options about further scaling 
		std::map<std::string,std::vector<std::pair<int,float> > > globalScalesOpts;
		std::map<std::string,std::vector<std::pair<int,float> > > globalScalesCorrOpts;
		std::vector<std::string> systematicsList;
		std::vector<float> systematicsCorr;
		std::vector<int> systematicsIdx;
		
		std::vector<std::string> photonCats;
		std::map<std::string,std::map<int,std::map<std::string,double> > > meanSysts;
		std::map<std::string,std::map<int,std::map<std::string,double> > > sigmaSysts;
		std::map<std::string,std::map<int,std::map<std::string,double> > > rateSysts;

		std::map<string,RooAbsReal*> photonSystematics;
		std::map<string,RooConstVar*> photonSystematicConsts;

		// utility funcs
		void addToSystematicsList(std::vector<std::string> systs);
		void addToSystematicsList(vector<string>::iterator begin, vector<string>::iterator end);
		
		bool isGlobalSyst(std::string name);
		bool isPerCatSyst(std::string name);
		bool isHighR9cat();
		bool isLowR9cat();
		float getRequiredAddtionalGlobalScaleFactor(std::string syst);
		void stripSpace(std::string &line);
		void printVec(std::vector<std::string> vec);
		void printSystMap(std::map<std::string,std::map<int,std::map<std::string,double> > > &theMap);
		void addToSystMap(std::map<std::string,std::map<int,std::map<std::string,double> > > &theMap, string proc, int diphotonCat, string phoSystName, double var);

};

#endif

