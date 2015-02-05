#include "RooMsgService.h"

#include "../interface/LinearInterp.h"

using namespace std;
using namespace RooFit;

LinearInterp::LinearInterp(RooRealVar *MHvar, int mhLow, int mhHigh, map<int,map<string,RooRealVar*> > fitParamVals, bool doSecMods, vector<int> skipMasses):
  MH(MHvar),
  mhLow_(mhLow),
  mhHigh_(mhHigh),
  fitParams(fitParamVals),
  doSecondaryModels(doSecMods),
  secondaryModelVarsSet(false),
	skipMasses_(skipMasses),
  verbosity_(0)
{
  allMH_ = getAllMH();
}

LinearInterp::~LinearInterp(){}

bool LinearInterp::skipMass(int mh){
	for (vector<int>::iterator it=skipMasses_.begin(); it!=skipMasses_.end(); it++) {
		if (*it==mh) return true;
	}
	return false;
}

vector<int> LinearInterp::getAllMH(){
  vector<int> result;
  for (int m=mhLow_; m<=mhHigh_; m+=5){
		if (skipMass(m)) continue;
    if (verbosity_>=1) cout << "LinearInterp - Adding mass: " << m << endl;
    result.push_back(m);
  }
  return result;
}

void LinearInterp::setSecondaryModelVars(RooRealVar *mh_sm, RooRealVar *deltam, RooAddition *mh_2, RooRealVar *width){
  MH_SM = mh_sm;
  DeltaM = deltam;
  MH_2 = mh_2;
  higgsDecayWidth = width;
  secondaryModelVarsSet=true;
}

void LinearInterp::interpolate(int nGaussians){
 
  for (int g=0; g<nGaussians; g++) {
    vector<double> xValues;
    vector<double> dmValues;
    vector<double> sigmaValues;
    for (unsigned int i=0; i<allMH_.size(); i++){
      int mh = allMH_[i];
      xValues.push_back(double(mh));
      dmValues.push_back(fitParams[mh][Form("dm_mh%d_g%d",mh,g)]->getVal());
      sigmaValues.push_back(fitParams[mh][Form("sigma_mh%d_g%d",mh,g)]->getVal());
    }
    assert(xValues.size()==dmValues.size());
    assert(xValues.size()==sigmaValues.size());
    RooSpline1D *dmSpline = new RooSpline1D(Form("dm_g%d",g),Form("dm_g%d",g),*MH,xValues.size(),&(xValues[0]),&(dmValues[0]),"LINEAR");
    RooSpline1D *sigmaSpline = new RooSpline1D(Form("sigma_g%d",g),Form("sigma_g%d",g),*MH,xValues.size(),&(xValues[0]),&(sigmaValues[0]),"LINEAR");
    splines.insert(pair<string,RooSpline1D*>(dmSpline->GetName(),dmSpline));
    splines.insert(pair<string,RooSpline1D*>(sigmaSpline->GetName(),sigmaSpline));
    // add secondary models as well
    if (doSecondaryModels){
      assert(secondaryModelVarsSet);
      // sm higgs as background
      RooSpline1D *dmSplineSM = new RooSpline1D(Form("dm_g%d_SM",g),Form("dm_g%d_SM",g),*MH_SM,xValues.size(),&(xValues[0]),&(dmValues[0]),"LINEAR");
      RooSpline1D *sigmaSplineSM = new RooSpline1D(Form("sigma_g%d_SM",g),Form("sigma_g%d_SM",g),*MH_SM,xValues.size(),&(xValues[0]),&(sigmaValues[0]),"LINEAR");
      splines.insert(pair<string,RooSpline1D*>(dmSplineSM->GetName(),dmSplineSM));
      splines.insert(pair<string,RooSpline1D*>(sigmaSplineSM->GetName(),sigmaSplineSM));
      // second degen higgs
      RooSpline1D *dmSpline2 = new RooSpline1D(Form("dm_g%d_2",g),Form("dm_g%d_2",g),*MH_2,xValues.size(),&(xValues[0]),&(dmValues[0]),"LINEAR");
      RooSpline1D *sigmaSpline2 = new RooSpline1D(Form("sigma_g%d_2",g),Form("sigma_g%d_2",g),*MH_2,xValues.size(),&(xValues[0]),&(sigmaValues[0]),"LINEAR");
      splines.insert(pair<string,RooSpline1D*>(dmSpline2->GetName(),dmSpline2));
      splines.insert(pair<string,RooSpline1D*>(sigmaSpline2->GetName(),sigmaSpline2));
    }
    if (g<nGaussians-1){
      vector<double> fracValues;
      for (unsigned int i=0; i<allMH_.size(); i++){
        int mh = allMH_[i];
        fracValues.push_back(fitParams[mh][Form("frac_mh%d_g%d",mh,g)]->getVal());
      }
      assert(xValues.size()==fracValues.size());
      RooSpline1D *fracSpline = new RooSpline1D(Form("frac_g%d",g),Form("frac_g%d",g),*MH,xValues.size(),&(xValues[0]),&(fracValues[0]),"LINEAR");
      splines.insert(pair<string,RooSpline1D*>(fracSpline->GetName(),fracSpline));
      // add secondary models as well
      if (doSecondaryModels){
        assert(secondaryModelVarsSet);
        // sm higgs as background
        RooSpline1D *fracSplineSM = new RooSpline1D(Form("frac_g%d_SM",g),Form("frac_g%d_SM",g),*MH,xValues.size(),&(xValues[0]),&(fracValues[0]),"LINEAR");
        splines.insert(pair<string,RooSpline1D*>(fracSplineSM->GetName(),fracSplineSM));
        // second degen higgs
        RooSpline1D *fracSpline2 = new RooSpline1D(Form("frac_g%d_2",g),Form("frac_g%d_2",g),*MH,xValues.size(),&(xValues[0]),&(fracValues[0]),"LINEAR");
        splines.insert(pair<string,RooSpline1D*>(fracSpline2->GetName(),fracSpline2));
      }
    }
  }
}

map<string,RooSpline1D*> LinearInterp::getSplines(){
  return splines;
}

void LinearInterp::setVerbosity(int v){
  if (v<2) {
    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
    RooMsgService::instance().setSilentMode(true);
  }
  verbosity_=v;
}

