#ifndef Normalization_8TeV_h
#define Normalization_8TeV_h

#include <vector>
#include <map>
#include <iostream>

#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TString.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TPython.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"

using namespace std;

class Normalization_8TeV {

  public:
	Normalization_8TeV();

	void Init(int sqrtS);
	
	double GetBR(double);
	// double GetBR(int);
	double GetXsection(double,TString);
	double GetXsection(double);
	// double GetXsection(int);
	double GetNorm(double,TH1F*,double, TH1F*,double);
	double GetMass(int);
	double GetVBFCorrection(double);
	TString GetProcess(int);
	void CheckNorm(double,double,double,TString);
	void FillSignalTypes();
	void PlotExpected(double ,double);	
	void PlotBR(double ,double);	
	void PlotXS(double ,double);	

	TGraph * GetSigmaGraph(TString process);
	TGraph * GetBrGraph();
	
	std::map<int,std::pair<TString,double > > & SignalType() { return SignalTypeMap; }
 private:
	std::map<double,double> BranchingRatioMap;
	std::map<double,double> XSectionMap_ggh;
	std::map<double,double> XSectionMap_vbf;
	std::map<double,double> XSectionMap_vbfold;
	std::map<double,double> XSectionMap_wh;
	std::map<double,double> XSectionMap_zh;
	std::map<double,double> XSectionMap_wzh;
	std::map<double,double> XSectionMap_tth;
  	std::map<double,double> XSectionMap_sm;

	std::map<int,std::pair<TString,double > > SignalTypeMap;
	
	bool is2011_;

};
#endif
