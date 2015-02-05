#ifndef FMTFit_h
#define FMTFit_h

#include "RooRealVar.h"
#include "RooGenericPdf.h"
//#include "RooExponentialSum.h"
//#include "RooPowerLawSum.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"

#include "TFile.h"
#include "TGraph.h"

#include "FMTBase.h"

using namespace std;
using namespace RooFit;

class FMTFit : public FMTBase{

	public:
		FMTFit(TFile *tFile, TFile *outFile);
		FMTFit(TFile *tFile, TFile *outFile, double intLumi, bool is2011, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, int nIncCategories, bool includeVBF, int nVBFCategories, bool includeLEP, int nLEPCategories, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int,vector<double> > > AllBinEdges, bool verbose=false);
		~FMTFit();

		pair<double,double> FitPow(double);
		void redoFit(double);
		void Plot(double);

		bool getblind();
		bool getplot();

		void setblind(bool);
		void setplot(bool);
		void SetNormGraph(TFile *nFile);

	private:
		RooRealVar *r1, *r2, *r3, *f1, *f2;
		RooAbsPdf *fit,*fit2011,*fit2012;
		RooRealVar *nBkgInSigReg;
		RooWorkspace *inWS;
		RooWorkspace *outWS;
		RooRealVar *mass_var;
		RooDataSet *data;
		RooFitResult *fitRes;

		bool blind_;
		bool plot_;
    
		string infilename_;
		string outfilename_;
		//int g_counter;
		TGraph *normGraph;
		bool readFitsFromFile;

    vector<RooAbsPdf*> storage;
};

#endif
