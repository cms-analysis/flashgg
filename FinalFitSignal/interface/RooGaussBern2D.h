#ifndef ROO_GAUSSBERN2D
#define ROO_GAUSSBERN2D

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

class RooRealVar;
class RooAbsReal;
class RooArgList;

class RooGaussBern2D : public RooAbsPdf {
public:
  RooGaussBern2D() {} ;
  RooGaussBern2D(const char *name, const char *title,
		 RooAbsReal& _x, RooAbsReal& _y, const RooArgList& _polParamsMean, const RooArgList& _polParamsSigma);
  RooGaussBern2D(const RooGaussBern2D& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooGaussBern2D(*this,newname); }
  inline virtual ~RooGaussBern2D() { }

  //Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  //Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;

protected:
  RooRealProxy x;
  RooRealProxy y;
  RooListProxy polParamsMean;
  RooListProxy polParamsSigma;
  
  Double_t analyticalIntegralBernstein(const char* type, const char* rangeName=0) const;
  Double_t analyticalIntegralGaussian(const char* rangeName=0);

  Double_t evalBernstein(const char* type) const;
  Double_t evaluate() const;

  ClassDef(RooGaussBern2D,1)

};



#endif

