#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>
#include "TMath.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "../interface/RooGaussBern2D.h"
#include "RooRealVar.h"
#include "RooArgList.h"

//ClassImp(RooGaussBern2D)

RooGaussBern2D::RooGaussBern2D(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _y, const RooArgList& _polParamsMean, const RooArgList& _polParamsSigma) :
  RooAbsPdf(name,title),
  x("x","DependentX",this,_x),
  y("y","DependentY",this,_y),
  polParamsMean("polParamsMean","Mean coefficients",this),
  polParamsSigma("polParamsSigma","Sigma coefficients",this)
{
  TIterator* meanIter = _polParamsMean.createIterator();
  RooAbsArg* mean;
  while ((mean = (RooAbsArg*)meanIter->Next())) {
    if (!dynamic_cast<RooAbsReal*>(mean)) {
      cout << "RooGaussBern2D::ctor(" << GetName() << ") ERROR: coefficient " << mean->GetName() 
	   << " is not of type RooAbsReal" << endl ;
      assert(0) ;
    }
    polParamsMean.add(*mean) ;
  }
  TIterator* sigmaIter = _polParamsSigma.createIterator();
  RooAbsArg* sigma;
  while ((sigma = (RooAbsArg*)sigmaIter->Next())) {
    if (!dynamic_cast<RooAbsReal*>(sigma)) {
      cout << "RooGaussBern2D::ctor(" << GetName() << ") ERROR: coefficient " << sigma->GetName() 
	   << " is not of type RooAbsReal" << endl ;
      assert(0) ;
    }
    polParamsSigma.add(*sigma) ;
  }
  delete meanIter;
  delete sigmaIter;
}

RooGaussBern2D::RooGaussBern2D(const RooGaussBern2D& other, const char* name) :
  RooAbsPdf(other, name),
  x("x",this,other.x),
  y("y",this,other.y),
  polParamsMean("polParamsMean",this,other.polParamsMean),
  polParamsSigma("polParamsSigma",this,other.polParamsSigma)
{
}

Double_t RooGaussBern2D::evalBernstein(const char* type) const {

  RooListProxy coefList;
  if (strcmp(type,"mean")==0) {
    coefList = polParamsMean;
  }
  else if (strcmp(type,"sigma")==0){
    coefList = polParamsSigma;
  }
  else {
    cout << "ERROR -- eval type must be mean or sigma not " << type << endl;
    assert(0);
  }
  // run in y direction
  // Bernstein polynomial implementation from K. Cranmer (RooBernstein)
  Double_t ymin = y.min();
  Double_t yval = (y - ymin) / (y.max() - ymin); // rescale to [0,1]
  Int_t degree = coefList.getSize()-1; // n+1 polys of degree n
  RooFIter iter = coefList.fwdIterator();

  if(degree == 0) {

    return ((RooAbsReal *)iter.next())->getVal();

  } else if(degree == 1) {

    Double_t a0 = ((RooAbsReal *)iter.next())->getVal(); // c0
    Double_t a1 = ((RooAbsReal *)iter.next())->getVal(); // c1 - c0
    return a1 * yval + a0;

  } else if(degree == 2) {

    Double_t a0 = ((RooAbsReal *)iter.next())->getVal(); // c0
    Double_t a1 = 2 * (((RooAbsReal *)iter.next())->getVal() - a0); // 2 * (c1 - c0)
    Double_t a2 = ((RooAbsReal *)iter.next())->getVal() - a1 - a0; // c0 - 2 * c1 + c2
    return (a2 * yval + a1) * x + a0;

  } else if(degree > 2) {

    Double_t t = yval;
    Double_t s = 1 - yval;

    Double_t result = ((RooAbsReal *)iter.next())->getVal() * s;    
    for(Int_t i = 1; i < degree; i++) {
      result = (result + t * TMath::Binomial(degree, i) * ((RooAbsReal *)iter.next())->getVal()) * s;
      t *= yval;
    }
    result += t * ((RooAbsReal *)iter.next())->getVal(); 

    return result;
  }

  // in case list of arguments passed is empty
  return TMath::SignalingNaN();
}

Double_t RooGaussBern2D::evaluate() const {
  
  // Gaussian implementation from D. Kirby (RooGaussian)
  Double_t arg = x - evalBernstein("mean");
  Double_t sig = evalBernstein("sigma");
  Double_t ret =exp(-0.5*arg*arg/(sig*sig)) ;
  return ret;
}

Double_t RooGaussBern2D::analyticalIntegralBernstein(const char* type, const char* rangeName) const {
  
  RooListProxy coefList;
  if (strcmp(type,"mean")==0) {
    coefList = polParamsMean;
  }
  else if (strcmp(type,"sigma")==0){
    coefList = polParamsSigma;
  }
  else {
    cout << "ERROR -- eval type must be mean or sigma not " << type << endl;
    assert(0);
  }

  // Bernstein polynomial implementation from K. Cranmer (RooBernstein)
  Double_t ymin = y.min(rangeName); Double_t ymax = y.max(rangeName);
  Int_t degree= coefList.getSize()-1; // n+1 polys of degree n
  Double_t norm(0) ;

  RooFIter iter = coefList.fwdIterator() ;
  Double_t temp=0;
  for (int i=0; i<=degree; ++i){
    // for each of the i Bernstein basis polynomials
    // represent it in the 'power basis' (the naive polynomial basis)
    // where the integral is straight forward.
    temp = 0;
    for (int j=i; j<=degree; ++j){ // power basis≈ß
      temp += pow(-1.,j-i) * TMath::Binomial(degree, j) * TMath::Binomial(j,i) / (j+1);
    }
    temp *= ((RooAbsReal*)iter.next())->getVal(); // include coeff
    norm += temp; // add this basis's contribution to total
  }
  norm *= ymax-ymin;
  return norm;
}

/*
Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const {
  if (matchArgs(allVars,analVars,x)) return 1;
  return 0;
}

Double_t RooGaussBern2D::analyticalIntegral(Int_t code, const char* rangeName=0) const {
  
  assert(code==1);
  // Implementation from D. Kirby (RooGaussian)
  static const Double_t root2 = sqrt(2.) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t mean = analyticalIntegralBernstein("mean",rangeName);
  Double_t sigma = analyticalIntegralBernstein("sigma",rangeName);
  Double_t xscale = root2*sigma;
  Double_t ret = 0;
  if(code==1){  
    ret = rootPiBy2*sigma*(RooMath::erf((x.max(rangeName)-mean)/xscale)-RooMath::erf((x.min(rangeName)-mean)/xscale));
//     cout << "Int_gauss_dx(mean=" << mean << ",sigma=" << sigma << ", xmin=" << x.min(rangeName) << ", xmax=" << x.max(rangeName) << ")=" << ret << endl ;
  } else{
    cout << "Error in RooGaussian::analyticalIntegral" << endl;
  }
  return ret ;

}
*/
