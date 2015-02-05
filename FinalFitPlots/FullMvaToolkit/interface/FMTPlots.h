#ifndef FMTPlots_h
#define FMTPlots_h

#include <iostream>

#include "TH1F.h"

#include "FMTBase.h"
#include "../../Normalization_8TeV.h"

using namespace std;

class FMTPlots : public FMTBase {

  public:
    FMTPlots(string);
    FMTPlots(string, bool, double, bool, int, int, double, double, double, int, double, double, int, int, int, double, double, int, bool, int, bool, int, vector<string>, bool, vector<map<int,vector<double> > >, bool blind=false,bool verbose=false);
    ~FMTPlots();
  
    void Setup();
    TH1F *linearBin(TH1F*);
    void plotAll(double);
    void makePlots(TH1F*,TH1F*,TH1F*,TH1F*,vector<TH1F*>,vector<TH1F*>,vector<TH1F*>,vector<TH1F*>,vector<pair<TH1F*,TH1F*> >,TH1F*,TH1F*,double);

    void plotOutput(TH1F*,TH1F*,TH1F*,TH1F*,TH1F*,double);
    void plotSidebands(TH1F*, vector<TH1F*>, vector<TH1F*>, TH1F*, vector<TH1F*>, vector<TH1F*>,double);
    void plotSystFracs(TH1F*, vector<pair<TH1F*,TH1F*> >,double);
    void plotInterpolation(TH1F*, TH1F*, TH1F*, double);
    void makeNormPlot();
    void makeSignalNormPlot();

		void plotByMH(string, double, TH1F*, TH1F*, TH1F*);
		pair<pair<double,double>,pair<double,double> > getBandSB(string,int);
		vector<double> getNormErrs(string);
		double quadInterp(double, double, double, double, double, double, double);

  private:
    
    TFile *tFile;
    bool blind_;
		bool runSB_;
    bool verbose_;
    Normalization_8TeV *normalizer;
};

#endif
