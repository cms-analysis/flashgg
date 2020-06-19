#ifndef flashgg_XYMETCorrection_h
#define flashgg_XYMETCorrection_h

#include <stdio.h>
#include<iostream>
#include"TMath.h"
namespace flashgg {

  class XYMETCorrection
  {
  public:
    XYMETCorrection();
    ~XYMETCorrection();

    std::pair<double,double> METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int run, unsigned int year, bool isMC, int npv);
  };
}
#endif
