#ifndef ProfileMultiplePdfs_h 
#define ProfileMultiplePdfs_h

#include <iostream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TGraph.h"

#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"

using namespace std;
using namespace RooFit;

class ProfileMultiplePdfs {

  public:
    ProfileMultiplePdfs();
    ~ProfileMultiplePdfs();

    void addPdf(RooAbsPdf *pdf, float penaltyTerm=0.);
    void clearPdfs();
    void printPdfs();

    pair<double,map<string,TGraph*> > profileLikelihood(RooAbsData *data, RooRealVar *obs_var, RooRealVar *var, float low, float high, float stepsize);
    map<string,TGraph*> profileLikelihoodEnvelope(RooAbsData *data, RooRealVar *var, float low, float high, float stepsize);

    pair<double,map<string,TGraph*> > computeEnvelope(pair<double,map<string,TGraph*> > minNllCurves, string name, float penalty=0.);

    void plotNominalFits(RooAbsData *data, RooRealVar *var, int binning, string fname="");
    void plot(map<string,TGraph*> minNlls, string fname);
    void print(map<string,TGraph*> minNlls, float low, float high, float stepsize);

    // recommended set
    static pair<double,double> quadInterpMin(TGraph *g, float stepsize=0.001);
    static double quadInterpCrossing(TGraph *g, double crossing, float stepsize=0.001);
    static pair<double,pair<double,double> > getMinAndError(TGraph *graph, float sigma, float stepsize=0.001, bool safemode=true);
    static pair<double,pair<double,double> > getMinAndErrorAsymm(TGraph *graph, float sigma, float stepsize=0.001, bool safemode=true);
		static pair<double,pair<double,double> > getMinAndErrorNoScale(TGraph *graph, float sigma, float stepsize=0.001, bool safemode=true); // this is for nll graphs that dont have their min at 0.
    static vector<double> getMinAndErrorAsymmVec(TGraph *graph, float sigma, float stepsize=0.001, bool safemode=true);
    static vector<double> getMinAndErrorLinearAsymmVec(TGraph *graph, float sigma, float stepsize=0.001, bool safemode=true);
    static pair<double,double> getMinAndErrorSymm(TGraph *graph, float sigma, float stepsize=0.001,bool safemode=true);
		static pair<double,pair<double,double> > getMinAndErrorLinear(TGraph *graph, float sigma, bool safemode);
    // -----------

    static double quadInterpMinimum(TGraph *nll, float stepsize=0.001);
    static double quadInterpMinimumOld(TGraph *nll, float stepsize=0.001);
    static pair<double,double> getGraphMin(TGraph *graph);
    static pair<double,double> interpError(TGraph *nll, float sigma);
    static double interpErrorSymmetric(TGraph *nll, float sigma);
    static double getPull(TGraph *nll, float trueVal=0., float stepsize=0.001, bool safemode=true);

    RooAbsPdf *getBestFitPdf(float var);
    double getGlobalMinNLL();

    

  private:
    
    void addToResultMap(float var, float minNll, RooAbsPdf* pdf);
    void saveValues(map<string,double> &vals, RooArgSet* params);
    void setValues(map<string,double> vals, RooArgSet *params);
    
    map<string,pair<RooAbsPdf*,float> > listOfPdfs;
    map<float,pair<float,RooAbsPdf*> > chosenPdfs;
    RooAbsPdf *bestFitPdf;
    double bestFitVal;
    double bestFitErr;
    double globalMinNLL;

    static TGraph* getMinPoints(TGraph *graph);
    static TGraph* getCrossingPointHigh(TGraph *graph, float crossing);
    static TGraph* getCrossingPointLow(TGraph *graph, float crossing);

    //RooArgList *listOfPdfs;
};
  
#endif
