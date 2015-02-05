#ifndef FMTRebin_h
#define FMTRebin_h

#include "FMTFit.h"
#include "FMTBase.h"

using namespace std;

class FMTRebin : public FMTBase {
	
	public:
    FMTRebin();
		FMTRebin(TFile *tF, TFile *oF);
		FMTRebin(TFile *tF, TFile *oF, double intLumi, bool is2011, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, int nIncCategories, bool includeVBF, int nVBFCategories, bool includeLEP, int nLEPCategories, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int,vector<double> > > AllBinEdges, bool verbose=false);
		~FMTRebin();

		// rebinning methods
    vector<double> optimizedReverseBinning(TH1F*,int,bool,bool);
    double calculateSigMulti(double*,double*,int);
    void histogramSmoothing(TH1F*,int);
    void histogramSmoothingFit(TH1F*);
    TH1F* rebinBinnedDataset(string,TH1F*,vector<double>,int);
    void maxSigScan(double*,int*,int*,TH1F*,TH1F*,int,int*,int);
    vector<double> significanceOptimizedBinning(TH1F*,TH1F*,int,int mass=125);
    vector<double> soverBOptimizedBinning(TH1F*,TH1F*,int,double);
    
		// histogram methods
    void mergeHistograms(string,TH1F*,TH1F*);
    void mergeHistograms(string,TH1F*);
    void makeSignalOutputHistogram(string,string,int);
    void makeOutputHistogram(string,string,int);
		
		TH1F *sumMultiBinnedDatasets(string,vector<TH1F*>,double,bool);
    TH1F *getCombBackground(int mass,int cat, double bkgInSigThisMass);
    TH1F *getCombSignal(int mass,int cat);
    void rebinBackground(double, int);
    void rebinSignal(int, int);
		void executeRebinning(int);

		int getg_step();
		int getsweepmode();
		double *getsignalVector1();
		double *getbackgroundVector1();
    bool getcatByHand();
		bool getjustRebin();

		void setg_step(int);
		void setsweepmode();
		void setsignalVector1(double*);
		void setbackgroundVector1(double*);
    void setcatByHand(bool);
		void setjustRebin(bool);

		FMTFit *fitter;

	private:
		int g_step;
		int sweepmode;
		double *signalVector1;
		double *backgroundVector1;
		TFile *tFile;
		TFile *outFile;

		bool justRebin_;
    bool catByHand_;

};

#endif
