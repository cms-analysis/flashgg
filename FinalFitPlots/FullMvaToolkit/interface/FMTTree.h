#ifndef FMTTree_h
#define FMTTree_h
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "TH1F.h"
#include "TH2F.h"

#include "TMVA/Reader.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "FMTFit.h"
#include "FMTBase.h"

using namespace std;

class FMTTree : public FMTBase {
	
	public:
    FMTTree(string,string);
    FMTTree(string,string,string,string,double,bool,int, int, double, double, double, int, double, double, int, int, int, double, double, int, bool, int, bool, int, vector<string>, bool, vector<map<int,vector<double> > >, bool isCutBased=false, bool useSidebandBDT=true, bool verbose=false);

    ~FMTTree();
		
		void Setup(string,string);
		void addTreeToMap(map<string,TTree*>&, string, string label="0");
		map<string,TTree*> getSignalTrees(string option="all");
		map<string,TTree*> getDataTrees();
		map<string,TTree*> getBackgroundTrees();

		void doCrossCheck(vector<pair<int,map<string,TTree*> > >, int mH=125);
		void printTrees(vector<pair<int,map<string,TTree*> > >);

		void initVariables();
		void setBranchVariables(TTree*);
		int icCat(int);

		void FillHist(string, int, double);
		void FillSigHist(string, double);
		void FillSystHist(string, double);

		string getProc(string);
		int getMH(string);
		void FillMassDatasets();

		float tmvaGetVal(float,float);
		float getCategoryMapVal(float,float);
    float tmvaGetValCutBased(float);
		void run(string option="all");

		void setdirname(string);

   private:
    float mass_;
    float bdtoutput_;
    float weight_;
    int category_;
    double lead_eta_;
    double sublead_eta_;
    float lead_eta_float_;
    float sublead_eta_float_;
    float lead_r9_;
    float sublead_r9_;
    vector<pair<double,double> > massSyst_;
    vector<pair<double,double> > bdtoutputSyst_;
    vector<pair<double,double> > weightSyst_;
    vector<pair<int,int> > categorySyst_;
		
		map<string,TH1F*> th1fs_;
		
		TFile *inFile_;
		TFile *outFile_;
		RooWorkspace *ws_;
		RooRealVar *massVar_;
		RooDataSet *dataSet_;
    vector<RooDataSet*> catDataSets_;
		TMVA::Reader *tmvaReader_;
		TH2F *categoryMap;		
		TH1F *binedgeMap;		
		
		string dirname_;
		string bdtname_;
		bool crossCheck_;
		
    float deltaMOverM_;
		float diphotonBDT_;

};

#endif
