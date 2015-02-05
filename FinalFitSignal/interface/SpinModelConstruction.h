#ifndef SpinModelConstruction_h 
#define SpinModelConstruction_h

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"

#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"
#include "Normalization_8TeV.h"

class SpinModelConstruction {

	public:
		
		SpinModelConstruction(RooRealVar *massVar, RooRealVar *MHvar, RooRealVar *intL, int mh, std::string proc, int cat, int sqrts, std::string systematicsFileName, double scaleGravGG, double scaleGravQQ, int verbosity=0, bool quadraticSigmaSum=false);
		~SpinModelConstruction();

		void setRVfitParams(std::map<int,std::map<std::string,RooRealVar*> > fP);
		void setWVfitParams(std::map<int,std::map<std::string,RooRealVar*> > fP);
    
		void setRVdatasets(std::map<int,RooDataSet*> data);
    void setWVdatasets(std::map<int,RooDataSet*> data);
    void setSTDdatasets(std::map<int,RooDataSet*> data);
		void makeSTDdatasets();

		RooAddPdf *buildPdf(std::string name, int nGaussians, bool recusive, std::map<int,std::map<std::string,RooRealVar*> > params, std::string add);
    void buildRvWvPdf(std::string name, int nGrv, int nGwv, bool recusive);
    
    double getRvFractionFunc();
    void setupSystematics();
		void getNormalization();
    
		void loadSignalSystematics(std::string filename);
		void setHighR9cats(std::string catString);
		void setLowR9cats(std::string catString);

		RooAbsReal *getMeanWithPhotonSyst(RooAbsReal *dm, std::string name);
		RooAbsReal *getSigmaWithPhotonSyst(RooAbsReal *sig_fit, std::string name);
		RooAbsReal *getRateWithPhotonSyst(std::string name);
    
		void plotPdf(std::string outDir, int mhLow, int mhHigh);
    void save(RooWorkspace *work);

	private:

    RooRealVar *mass;
    RooRealVar *MH;
    RooRealVar *intLumi;
    int mh_;
    std::string proc_;
    int cat_;
		int sqrts_;
		double scaleGravGG_;
		double scaleGravQQ_;
		int verbosity_;
		bool quadraticSigmaSum_;
		bool systematicsSet_;
    Normalization_8TeV *norm;

    RooSpline1D *graphToSpline(string name, TGraph *graph);
    RooSpline1D *graphToSpline(string name, TGraph *graph, RooAbsReal *var);
    std::map<std::string,RooSpline1D*> xsSplines;
    RooSpline1D *brSpline;

		std::map<int,std::map<std::string,RooRealVar*> > fitParamsRV;
		std::map<int,std::map<std::string,RooRealVar*> > fitParamsWV;
    std::map<int,RooDataSet*> rvDatasets;
    std::map<int,RooDataSet*> wvDatasets;
    std::map<int,RooDataSet*> stdDatasets;
    
    RooRealVar *vertexNuisance;
		RooRealVar *r9barrelNuisance;
		RooRealVar *r9mixedNuisance;
		std::vector<int> highR9cats;
		std::vector<int> lowR9cats;
		
		RooAddPdf *finalPdf;
    RooAbsReal *finalNorm;
    RooAbsReal *finalNormThisLum;
    RooExtendPdf *extendPdfRel;
    RooExtendPdf *extendPdf;
		
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

		std::vector<std::string> photonCats;
		std::map<std::string,std::map<int,std::map<std::string,double> > > meanSysts;
		std::map<std::string,std::map<int,std::map<std::string,double> > > sigmaSysts;
		std::map<std::string,std::map<int,std::map<std::string,double> > > rateSysts;
		std::map<std::string,RooRealVar*> photonSystematics;
		std::map<std::string,RooConstVar*> photonSystematicConsts;
		
		// utility funcs
		void addToSystematicsList(std::vector<std::string> systs);
		bool isGlobalSyst(std::string name);
		bool isPerCatSyst(std::string name);
		bool isHighR9cat();
		bool isLowR9cat();
		float getRequiredAddtionalGlobalScaleFactor(std::string syst);
		void stripSpace(std::string &line);
		void printVec(std::vector<std::string> vec);
		void printSystMap(std::map<std::string,std::map<int,std::map<std::string,double> > > &theMap);
		void addToSystMap(std::map<std::string,std::map<int,std::map<std::string,double> > > &theMap, std::string proc, int diphotonCat, std::string phoSystName, double var);
		void printMap(std::map<std::string,RooRealVar*> m);
};

#endif
