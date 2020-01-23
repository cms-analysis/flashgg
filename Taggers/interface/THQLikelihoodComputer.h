#ifndef flashgg_THQLikelihoodComputer_h
#define flashgg_THQLikelihoodComputer_h

#include "TH1.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TMath.h"

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

#define NVARS 14

namespace flashgg {
  class THQLikelihoodComputer 
  {
  public:
    THQLikelihoodComputer(const char* likelihood_inputfile);
    ~THQLikelihoodComputer();
    double evaluate_likelihood(const std::vector<double> &inputvars);
  private:
    TH1F * h_fstatekinematics_sig[NVARS];
    TH1F * h_fstatekinematics_bkg[NVARS];
    const char * likelihood_inputfile_;
  };
}

#endif
