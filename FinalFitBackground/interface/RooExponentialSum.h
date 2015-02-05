/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooExponentialSum.h,v 1.10 2007/07/12 20:30:49 wouter Exp $
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
#ifndef ROO_EXPONENTIALSUM
#define ROO_EXPONENTIALSUM

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

class RooRealVar;
class RooArgList;

class RooExponentialSum : public RooAbsPdf {
public:
  RooExponentialSum() ;
  RooExponentialSum(const char* name, const char* title, RooAbsReal& x) ;
  RooExponentialSum(const char *name, const char *title,
		 RooAbsReal& _x, const RooArgList& _coefList);

  RooExponentialSum(const RooExponentialSum& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooExponentialSum(*this,newname); }
  virtual ~RooExponentialSum() { delete coefIter; }

  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;

protected:
  RooRealProxy x;
  RooListProxy coefList;
  TIterator* coefIter;

  Double_t evaluate() const;

  //ClassDef(RooExponentialSum,1) // PowerLaw PDF
	// not needed for cmssw 7xx?
};

#endif

