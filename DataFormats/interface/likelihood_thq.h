#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLorentzVector.h"

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <TChain.h>
#include "TMath.h"
#include "TGraphErrors.h"
using namespace std;
using namespace edm;

namespace flashgg{
class LikelihoodClass {
public:
//    LikelihoodClass();
    ~LikelihoodClass();
    double evaluate_likelihood(std::vector<double> inputvars , const char * likelihood_inputfile);
    const char * likelihood_inputfile_;
//    TFile *file_inputdistributions;
private:
    TH1F * h_fstatekinematics_sig[14];
    TH1F * h_fstatekinematics_bkg[14];
    TFile * file_inputdistributions;
//    const char * likelihood_inputfile_;
};

double LikelihoodClass::evaluate_likelihood(std::vector<double> inputvars, const char * likelihood_inputfile ) {
    likelihood_inputfile_ = likelihood_inputfile;
    file_inputdistributions=new TFile( likelihood_inputfile_ , "READ");
    h_fstatekinematics_sig[0]= (TH1F*) file_inputdistributions->Get("thq_n_jets");
    h_fstatekinematics_sig[1]= (TH1F*) file_inputdistributions->Get("thq_n_centraljets");
    h_fstatekinematics_sig[2]= (TH1F*) file_inputdistributions->Get("thq_muon1_ch");
    h_fstatekinematics_sig[3]= (TH1F*) file_inputdistributions->Get("thq_fwdjet1_eta");
    h_fstatekinematics_sig[4]= (TH1F*) file_inputdistributions->Get("thq_deta_muonfwdjet");
    h_fstatekinematics_sig[5]= (TH1F*) file_inputdistributions->Get("thq_dr_leadphofwdjet");
    h_fstatekinematics_sig[6]= (TH1F*) file_inputdistributions->Get("thq_dr_subleadphofwdjet");
    h_fstatekinematics_sig[7]= (TH1F*) file_inputdistributions->Get("thq_bjet1_pt");
    h_fstatekinematics_sig[8]= (TH1F*) file_inputdistributions->Get("top_mt11_thq");
    h_fstatekinematics_sig[9]= (TH1F*) file_inputdistributions->Get("thq_dipho_pt");
    h_fstatekinematics_sig[10]= (TH1F*) file_inputdistributions->Get("thq_n_bjets");
    h_fstatekinematics_sig[11]= (TH1F*) file_inputdistributions->Get("thq_dr_tHchainfwdjet");
    h_fstatekinematics_sig[12]= (TH1F*) file_inputdistributions->Get("thq_dr_leptonbjet");
    h_fstatekinematics_sig[13]= (TH1F*) file_inputdistributions->Get("thq_dr_leptonfwdjet");
    
	
    h_fstatekinematics_bkg[0]= (TH1F*) file_inputdistributions->Get("tth_n_jets");
    h_fstatekinematics_bkg[1]= (TH1F*) file_inputdistributions->Get("tth_n_centraljets");
    h_fstatekinematics_bkg[2]= (TH1F*) file_inputdistributions->Get("tth_muon1_ch");
    h_fstatekinematics_bkg[3]= (TH1F*) file_inputdistributions->Get("tth_fwdjet1_eta");
    h_fstatekinematics_bkg[4]= (TH1F*) file_inputdistributions->Get("tth_deta_muonfwdjet");
    h_fstatekinematics_bkg[5]= (TH1F*) file_inputdistributions->Get("tth_dr_leadphofwdjet");
    h_fstatekinematics_bkg[6]= (TH1F*) file_inputdistributions->Get("tth_dr_subleadphofwdjet");
    h_fstatekinematics_bkg[7]= (TH1F*) file_inputdistributions->Get("tth_bjet1_pt");
    h_fstatekinematics_bkg[8]= (TH1F*) file_inputdistributions->Get("top_mt11_tth");
    h_fstatekinematics_bkg[9]= (TH1F*) file_inputdistributions->Get("tth_dipho_pt");
    h_fstatekinematics_bkg[10]= (TH1F*) file_inputdistributions->Get("tth_n_bjets");
    h_fstatekinematics_bkg[11]= (TH1F*) file_inputdistributions->Get("tth_dr_tHchainfwdjet");
    h_fstatekinematics_bkg[12]= (TH1F*) file_inputdistributions->Get("tth_dr_leptonbjet");
    h_fstatekinematics_bkg[13]= (TH1F*) file_inputdistributions->Get("tth_dr_leptonfwdjet");
    


//    file_inputdistributions->Close();

    if(inputvars.size()!=14) {
        std::cout<<"<LikelihoodClass::evaluate_likelihood>: inputvars.size() is "<<inputvars.size()<< " and it is expected to be 8!"<<std::endl;
        std::cout<<"<LikelihoodClass::evaluate_likelihood>: is returning a value of -10.!"<<std::endl;
        return -10.;
    }
    double p_signal=1.;
    double p_background=1.;
    for(unsigned int ielement=0; ielement<inputvars.size(); ielement++) {
        int bin_hsignal= (Int_t) h_fstatekinematics_sig[ielement]->FindBin(inputvars.at(ielement));
        int bin_hbackground= (Int_t) h_fstatekinematics_bkg[ielement]->FindBin(inputvars[ielement]);
        p_signal = (p_signal*h_fstatekinematics_sig[ielement]->GetBinContent(bin_hsignal));
        p_background = (p_background*h_fstatekinematics_bkg[ielement]->GetBinContent(bin_hbackground));
//    cout<<"inputvars.at(ielement)"<<inputvars.at(ielement)<<endl;
        }
	file_inputdistributions->Close();
          double lhood_val_den = (p_signal+p_background);
        return (lhood_val_den != 0. ? (p_signal/lhood_val_den) : -11.);
//	file_inputdistributions->Close();
                };


LikelihoodClass::~LikelihoodClass() {
//    file_inputdistributions->Close();
    
	delete h_fstatekinematics_sig[0];
	delete h_fstatekinematics_sig[1];
	delete h_fstatekinematics_sig[2];
        delete h_fstatekinematics_sig[3];
	delete h_fstatekinematics_sig[4];
        delete h_fstatekinematics_sig[5];
        delete h_fstatekinematics_sig[6];
        delete h_fstatekinematics_sig[7];
	delete h_fstatekinematics_sig[8];
	delete h_fstatekinematics_sig[9];
        delete h_fstatekinematics_sig[10];
        delete h_fstatekinematics_sig[11];
        delete h_fstatekinematics_sig[12];
        delete h_fstatekinematics_sig[13];


	delete h_fstatekinematics_bkg[0];
        delete h_fstatekinematics_bkg[1];
        delete h_fstatekinematics_bkg[2];
        delete h_fstatekinematics_bkg[3];
        delete h_fstatekinematics_bkg[4];
        delete h_fstatekinematics_bkg[5];
        delete h_fstatekinematics_bkg[6];
        delete h_fstatekinematics_bkg[7];
        delete h_fstatekinematics_bkg[8];
        delete h_fstatekinematics_bkg[9];
        delete h_fstatekinematics_bkg[10];
        delete h_fstatekinematics_bkg[11];
        delete h_fstatekinematics_bkg[12];
        delete h_fstatekinematics_bkg[13];

}


}
