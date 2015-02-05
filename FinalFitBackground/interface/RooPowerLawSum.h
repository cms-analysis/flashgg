/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooPowerLawSum.h,v 1.10 2007/07/12 20:30:49 wouter Exp $
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
#ifndef ROO_POWERLAWSUM
#define ROO_POWERLAWSUM

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

class RooRealVar;
class RooArgList;

class RooPowerLawSum : public RooAbsPdf {
public:
  RooPowerLawSum() ;
  RooPowerLawSum(const char* name, const char* title, RooAbsReal& x) ;
  RooPowerLawSum(const char *name, const char *title,
		 RooAbsReal& _x, const RooArgList& _coefList);

  RooPowerLawSum(const RooPowerLawSum& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooPowerLawSum(*this,newname); }
  virtual ~RooPowerLawSum() { delete coefIter; }

  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;

protected:
  RooRealProxy x;
  RooListProxy coefList;
  TIterator* coefIter;

  Double_t evaluate() const;

//  ClassDef(RooPowerLawSum,1) // PowerLaw PDF
//  not neded for cmssw 7xx ?
};

#endif

