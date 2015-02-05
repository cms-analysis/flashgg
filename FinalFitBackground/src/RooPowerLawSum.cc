/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 * @(#)root/roofit:$Id: RooPowerLawSum.cxx 25185 2008-08-20 14:00:42Z wouter $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
// BEGIN_HTML
// PowerLaw p.d.f
// END_HTML
//

// should impement a function whose integral is defined as x^(p1+p2x+p3x^2+p4x^3+.....)

#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "../interface/RooPowerLawSum.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgList.h"

//ClassImp(RooPowerLawSum)
//;


//_____________________________________________________________________________
RooPowerLawSum::RooPowerLawSum()
{
  // coverity[UNINIT_CTOR]
  coefIter = coefList.createIterator() ;
}

//_____________________________________________________________________________
RooPowerLawSum::RooPowerLawSum(const char *name, const char *title,
			       RooAbsReal& _x, const RooArgList& _coefList) :
  RooAbsPdf(name, title), 
  x("x","Dependent",this,_x),
  coefList("coefList","List of coefficients",this)
{
  coefIter = coefList.createIterator();

  TIterator* _coefIter = _coefList.createIterator();
  RooAbsArg* coef;
  while ((coef = (RooAbsArg*)_coefIter->Next())) {
    if (!dynamic_cast<RooAbsReal*>(coef)){
      cout << "RooPowerLawSum::ctor(" << GetName() << ") ERROR: coefficient " << coef->GetName() 
	   << " is not of type RooAbsReal" << endl ;
      assert(0) ;
    }
    coefList.add(*coef);
  }
  delete _coefIter; 
}

RooPowerLawSum::RooPowerLawSum(const char *name, const char *title,
			       RooAbsReal& _x) :
  RooAbsPdf(name, title), 
  x("x","Dependent",this,_x),
  coefList("coefList","List of coefficients",this)
{
  coefIter = coefList.createIterator();
}

//_____________________________________________________________________________
RooPowerLawSum::RooPowerLawSum(const RooPowerLawSum& other, const char* name) :
  RooAbsPdf(other, name), 
  x("x",this,other.x), 
  coefList("coefList",this,other.coefList)
{
  coefIter = coefList.createIterator();
}

//_____________________________________________________________________________
Double_t RooPowerLawSum::evaluate() const{
 
  // integral form is given by y=x^(p1+p2x+p3x^2+p4x^3+....)
  // i.e y=g(x)^f(x) where g(x)=1 and f(x)=p1+p2x+p3x^2+...
  // dy/dx = dy/dg*dg/dx + dy/df*df/dx
  // dg/dx=1 and df/dx=p2+2*p3x+3*p4x^2+...

  // d/dx [ x^f(x) ] = x^f [df/dx ln(x) + f/x]

  int order=0;
  double p0=-9999.; //avoid uninitialised variables errors in cmssw
  double fx=1.;
  double dfdx=0.;
  coefIter->Reset();
  RooAbsReal *param;
  while ((param = (RooAbsReal*)coefIter->Next())) {
    if (order==0) p0 = param->getVal();
    fx += param->getVal()*pow(x,order);
    dfdx += order*param->getVal()*pow(x,order-1);
    order++;
  }
  double ret = (1./(1.+p0))*pow(x,fx)*(dfdx*log(x)+fx/x);
  //cout << x << " " << fx << " " << dfdx << " " << ret << endl;
  return ret;
}


//_____________________________________________________________________________
Int_t RooPowerLawSum::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const 
{
  if (matchArgs(allVars,analVars,x)) return 1 ;
  return 0 ;
}


//_____________________________________________________________________________
Double_t RooPowerLawSum::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  switch(code) {
  case 1: 
    {
      int order=0;
      double power_xmax=1.;
      double power_xmin=1.;
      double p0=-9999.; //avoid uninitialised variables errors in cmssw
      coefIter->Reset();
      RooAbsReal *param;
      while ((param = (RooAbsReal*)coefIter->Next())) {
        if (order==0) p0 = param->getVal();
        power_xmax += param->getVal()*pow(x.max(rangeName),order);
        power_xmin += param->getVal()*pow(x.min(rangeName),order);
        order++;
      }
      return (1./(1.+p0))*(pow(x.max(rangeName),power_xmax)-pow(x.min(rangeName),power_xmin));
    }
  }
  
  assert(0) ;
  return 0 ;
}


