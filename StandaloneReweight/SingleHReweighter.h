#ifndef SingleHreweighter
#define SingleHreweighter

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "CfgManager.h"
#include "CfgManagerT.h"
#include "TString.h"
using namespace std;

namespace SingleHReweighter 
{
  float tthformula (float kl, float kt, float C1, float EWK, float dZH)
  { 
    return (kt*kt+(kl-1)*C1/EWK) / ((1-(kl*kl-1)*dZH)); 
  }

    class SingleHReweighter
    {

    public:
        SingleHReweighter(CfgManager conf, const string &process);
        ~SingleHReweighter(){};
        float getWeight(const float &pTH, const float &kl, const float &kt);
    private:
        int FindBin(const float &pTH);
        string process_;
        float dZH_;
        vector<float> pTHbins_;
        vector<float> C1_process_pTH_map_;  // C1_process_pTH_map [#pTHbin]  
        vector<float> EWK_process_pTH_map_; // EWK_process_pTH_map [#pTHbin]  
	float (*weightformula_)(float,float,float,float,float);
    };

    SingleHReweighter::SingleHReweighter(CfgManager conf, const string &process) :
        process_(process)
    {
        dZH_     = conf.GetOpt<float> ("SingleHReweight.dZH");
        pTHbins_ = conf.GetOpt<vector<float> > (Form("SingleHReweight.%s.pTHbins",process.c_str()));
        C1_process_pTH_map_ = conf.GetOpt<vector<float> > (Form("SingleHReweight.%s.C1",process.c_str()));
        EWK_process_pTH_map_ = conf.GetOpt<vector<float> > (Form("SingleHReweight.%s.EWK",process.c_str()));
	if(process=="tth")
	  weightformula_=tthformula;
	  
    }   

    int SingleHReweighter::FindBin(const float &pTH)
    {
        //to do
        return 0;
    }
    
    float SingleHReweighter::getWeight(const float &pTH, const float &kl, const float &kt)
    {
        float w = 0.;
        int pTHbin = FindBin(pTH);
        float C1 = C1_process_pTH_map_.at(pTHbin);
        double EWK = EWK_process_pTH_map_.at(pTHbin);
        //w = (kt*kt+(kl-1)*C1/EWK) / ((1-(kl*kl-1)*dZH_));
	w = (*weightformula_)(kl, kt, C1, EWK, dZH_);
        return w;
    }

    
    static string IdentifyProcess( TString treename )
    {
        string process = "";

        if(treename.Contains("hh201")) 
            if(process != "")
                cout<<"IdentifyProcess --> WARNING: ambigous treename"<<endl;
            else
                process="hh";

        if(treename.Contains("tth201")) 
            if(process != "")
                cout<<"IdentifyProcess --> WARNING: ambigous treename"<<endl;
            else
                process="tth";

        if(treename.Contains("ggh201")) 
            if(process != "")
                cout<<"IdentifyProcess --> WARNING: ambigous treename"<<endl;
            else
                process="ggh";

        if(treename.Contains("qqh201")) 
            if(process != "")
                cout<<"IdentifyProcess --> WARNING: ambigous treename"<<endl;
            else
                process="qqh";

        if(treename.Contains("vh201")) 
            if(process != "")
                cout<<"IdentifyProcess --> WARNING: ambigous treename"<<endl;
            else
                process="vh";

        if(process == "")
            cout<<"ERROR: process not found"<<endl;
        return process;
    }

}

#endif
