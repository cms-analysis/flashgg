// A class for running 2D optimizations in S/B (x)

#ifndef Optimizations_h
#define Optimizations_h

#include <ctime>
#include <vector>

#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"
#include "TTree.h"
#include "TText.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TGraph.h"
 
//#include "../interface/Optimizations.h"

using namespace std;

class Optimizations {

    public: 

	Optimizations(TH2F *, TH2F *);
	~Optimizations(){
		delete signalVector1;
		delete backgroundVector1;
	};

	void setMaxBins(int val){maxNumberOfBins=val;};
	void setNdivisions(int val){nNewBins=val;};
	void setThreshold(int val){threshold=val;};
	void setMinBackground(double val){predefMin=val;};
	TH2F *getCategoryMap(){return FinalHist;};
	TH2F *getSignalTarget(){return targetS2d;};
	TH1F *getTargetS1D(){return targetS;};
	TH1F *getTargetB1D(){return targetB;};
	TGraph *getRocCurve(){return roc_curve;};
	TH2F *getBackgroundTarget(){return targetB2d;};
	TGraph *getOptimizationGraph(){
		if (optGr){
		 gROOT->SetBatch(0);
		 optGr->Draw();
		 optGr->SetMarkerStyle(20);
		 optGr->SetMarkerSize(1.0);
	 	 optGr->SetName("Optimization");
	 	 optGr->GetYaxis()->SetTitle("#sigma_{exp}");
		 optGr->GetXaxis()->SetTitle("Number of Bins");	
		}
		return optGr;
	};
	int getFinalNumberOfBins(){return nFinalBins;};

	void smoothHistograms(double bandwidths=0.01,double bandwidthb=0.01, int mode=0); // 1% of data, default linear regression
	void runOptimization();

    protected: 

	void maxSigScan(double *maximumSignificance,int *frozen_counters,int *chosen_counters, int N,int *counters, int movingCounterIndex);
	std::vector<double> significanceOptimizedBinning(TGraph *optGr=0);

	
	double defx(double val){ return TMath::Log2(1+val);};
	//double defx(double val){ return val;};
	double findMaximum(TH2F *);
	double findRMS(TH2F *,double *);
	bool checkMinBkg(double);
	void getIntegralBetweenRanges(double *s,double *b,int l, int h);
	double calculateSigMulti(double *s1, double *b1, int nchannel, bool verb=false);
	
	int maxNumberOfBins;

	double *signalVector1;
	double *backgroundVector1;

	double g_step ;
	bool sweepmode ;
	double SBscale ;

	int n2dbinsX;
	int n2dbinsY;

	TH2F *targetS2d;
	TH2F *targetB2d;
	TH2F *FinalHist;

	TH1F *targetS;
	TH1F *targetB;
	
	TH1F *SBhist;
	TGraph *roc_curve;

	TGraph *optGr;

	int nNewBins;
	int nFinalBins;

        double predefMin;
	double threshold;
	double delta;
		
};

#endif		
