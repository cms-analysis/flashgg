#ifndef doublehreweighter
#define doublehreweighter 

#include <iostream>
#include <vector>
#include <algorithm>

#include "TH2F.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "CfgManager.h"
#include "CfgManagerT.h"


using namespace std;

class DoubleHReweighter
{
  public:
    DoubleHReweighter( CfgManager conf );
    ~DoubleHReweighter();
    float getWeight( int targetNode, float gen_mHH, float gen_cosTheta);
    float getWeight(float kl, float kt, float gen_mHH, float gen_cosTheta);
    float getCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2);
    float getXSratio(float kl, float kt);
    float getXSratio(float kl, float kt, float c2, float cg, float c2g);
  private:
    float functionGF(float kl, float kt, float c2, float cg, float c2g, vector<float> A);
    pair<int,int> find2DBin(TH2* h, float x, float y);

    string weightsFile_;  // file with prepared histograms needed for reweighting
    const unsigned int NUM_benchmarks = 15;  // number of becnhmarks for reweighting 12 +1 SM = 13, box=14, fake2017SM=15 
    const unsigned int numberSMbenchmark = 13;  // index of SM benchmark 
    const unsigned int numberBoxbenchmark = 14;  // index of SM benchmark 
    const unsigned int numberFakebenchmark = 15;  // index of SM benchmark 
    std::vector<TH2F*> hists_params_;   // histograms with weights for 15 parameters
    TH2F* hist_SM_;  // histogram for SM point
    TH2F* hist_inputMix_;  // histogram for input mix of nodes
    unsigned int NCOEFFSA_;// number of needed coefficient to describe the phase space 
    vector<float> A_13TeV_SM_;
    TFile* f_weights_ ;
    map<string,vector<float> > benchmarks_map_ ;

};

DoubleHReweighter::DoubleHReweighter( CfgManager conf ) :
    weightsFile_(conf.GetOpt<string> ("DoubleHReweight.weightsFile")),
    NCOEFFSA_(conf.GetOpt<int>( "DoubleHReweight.NCOEFFSA" )),
    A_13TeV_SM_(conf.GetOpt<vector<float> >("DoubleHReweight.A_13TeV_SM"))
    {
        benchmarks_map_["kl"]  = conf.GetOpt<vector<float> > ("DoubleHReweight.kl");
        benchmarks_map_["kt"]  = conf.GetOpt<vector<float> > ("DoubleHReweight.kt");
        benchmarks_map_["c2"]  = conf.GetOpt<vector<float> > ("DoubleHReweight.c2");
        benchmarks_map_["cg"]  = conf.GetOpt<vector<float> > ("DoubleHReweight.cg");
        benchmarks_map_["c2g"] = conf.GetOpt<vector<float> > ("DoubleHReweight.c2g");
        
        f_weights_ = new TFile(weightsFile_.c_str(), "READ");
        for (unsigned n=0; n<NCOEFFSA_; n++)
        {
            hists_params_.push_back((TH2F*)f_weights_->Get(Form("h_A%i",n)));
            if (!(hists_params_[n])) 
                cout<<"ERROR: The file "<<weightsFile_<<" provided for reweighting full grid does not contain the expected histogram number : "<<n<<endl;
        }
        hist_SM_ = (TH2F*)f_weights_->Get("h_SM");
        if (!(hist_SM_)) 
            cout<<"ERROR: The file "<<weightsFile_<<" provided for reweighting benchmarks does not contain the expected SM histogram"<<endl;
        hist_inputMix_ = (TH2F*)f_weights_->Get("allHHNodeMap2D");
        if (!(hist_inputMix_)) 
            cout<<"ERROR: The file "<<weightsFile_<<" provided for reweighting benchmarks does not contain the expected input histogram for mix of nodes."<<endl;
    }   
    
DoubleHReweighter::~DoubleHReweighter()
{
  delete f_weights_;
}
// return bin in 2D isto wihtout under/over flow (e.g. if ibin > ibinmax , ibin = ibinmax)
pair<int,int> DoubleHReweighter::find2DBin(TH2* h, float x, float y)
{
    int ibinx = h->GetXaxis()->FindBin(x);
    int ibiny = h->GetYaxis()->FindBin(y);
    if (ibinx <= 0) ibinx = 1;
    if (ibinx > h->GetNbinsX()) ibinx = h->GetNbinsX();
    if (ibiny <= 0) ibiny = 1;
    if (ibiny > h->GetNbinsY()) ibiny = h->GetNbinsY();
    
    return make_pair(ibinx, ibiny);
}

float DoubleHReweighter::getXSratio(float kl, float kt)
{
  return getXSratio(kl,kt,0,0,0);
}

float DoubleHReweighter::getXSratio(float kl, float kt, float c2, float cg, float c2g)
{
  return functionGF(kl,kt,c2,cg,c2g,A_13TeV_SM_);
}

float DoubleHReweighter::functionGF(float kl, float kt, float c2, float cg, float c2g, vector<float> A)
{
    return ( A[0]*pow(kt,4) + A[1]*pow(c2,2) + (A[2]*pow(kt,2) + A[3]*pow(cg,2))*pow(kl,2) + A[4]*pow(c2g,2) + ( A[5]*c2 + A[6]*kt*kl )*pow(kt,2) + (A[7]*kt*kl + A[8]*cg*kl )*c2 + A[9]*c2*c2g + (A[10]*cg*kl + A[11]*c2g)*pow(kt,2)+ (A[12]*kl*cg + A[13]*c2g )*kt*kl + A[14]*cg*c2g*kl );
}


float DoubleHReweighter::getWeight(int targetNode, float gen_mHH, float gen_cosTheta)
{
    float w = 0.;
    pair<int,int> bins = find2DBin(hist_inputMix_, gen_mHH, fabs(gen_cosTheta));
    float denom = hist_inputMix_->GetBinContent(bins.first, bins.second);
    if (denom == 0) 
        return 0;
    
    float nEvSM = hist_SM_->GetBinContent(bins.first, bins.second);
    vector<float> Acoeffs;
    for (unsigned int ic = 0; ic < NCOEFFSA_; ++ic)
        Acoeffs.push_back((hists_params_[ic])->GetBinContent(bins.first, bins.second));

    float kl  = benchmarks_map_["kl"][targetNode];  
    float kt  = benchmarks_map_["kt"][targetNode];  
    float c2  = benchmarks_map_["c2"][targetNode];  
    float cg  = benchmarks_map_["cg"][targetNode];  
    float c2g = benchmarks_map_["c2g"][targetNode];  
    float effBSM = nEvSM * functionGF(kl,kt,c2,cg,c2g,Acoeffs)/functionGF(kl,kt,c2,cg,c2g,A_13TeV_SM_);
    
    if (effBSM/denom < 0)
            return 0;
    
    w = (effBSM/denom) ;

    return w;
}

float DoubleHReweighter::getWeight(float kl, float kt, float gen_mHH, float gen_cosTheta)
{
    float w = 0.;
    pair<int,int> bins = find2DBin(hist_inputMix_, gen_mHH, fabs(gen_cosTheta));
    float denom = hist_inputMix_->GetBinContent(bins.first, bins.second);
    if (denom == 0) 
        return 0;
    float nEvSM = hist_SM_->GetBinContent(bins.first, bins.second);
    vector<float> Acoeffs;
    for (unsigned int ic = 0; ic < NCOEFFSA_; ++ic)
        Acoeffs.push_back((hists_params_[ic])->GetBinContent(bins.first, bins.second));

    float effBSM = nEvSM * functionGF(kl,kt,0,0,0,Acoeffs)/functionGF(kl,kt,0,0,0,A_13TeV_SM_);
    w = (effBSM/denom) ;

    return w;

}

float DoubleHReweighter::getCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2)
{
    // cos theta star angle in the Collins Soper frame
    TLorentzVector hh = h1 + h2;
    h1.Boost(-hh.BoostVector());                     
    return h1.CosTheta();
}
#endif
