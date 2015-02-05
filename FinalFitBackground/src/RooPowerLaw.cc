/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 * @(#)root/roofit:$Id: RooPowerLaw.cxx 25185 2008-08-20 14:00:42Z wouter $
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

#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "../interface/RooPowerLaw.h"
#include "RooRealVar.h"

//ClassImp(RooPowerLaw)


//_____________________________________________________________________________
RooPowerLaw::RooPowerLaw(const char *name, const char *title,
			       RooAbsReal& _x, RooAbsReal& _c) :
  RooAbsPdf(name, title), 
  x("x","Dependent",this,_x),
  c("c","Power",this,_c)
{
}


//_____________________________________________________________________________
RooPowerLaw::RooPowerLaw(const RooPowerLaw& other, const char* name) :
  RooAbsPdf(other, name), x("x",this,other.x), c("c",this,other.c)
{
}


//_____________________________________________________________________________
Double_t RooPowerLaw::evaluate() const{
  //cout << "pow(x=" << x << ",c=" << c << ")=" << pow(x,c) << endl ;
  return pow(x,c);
}


//_____________________________________________________________________________
Int_t RooPowerLaw::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const 
{
  if (matchArgs(allVars,analVars,x)) return 1 ;
  return 0 ;
}


//_____________________________________________________________________________
Double_t RooPowerLaw::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  switch(code) {
  case 1: 
    {
      Double_t ret(0) ;
      if(c == 0.0) {
	ret = (x.max(rangeName) - x.min(rangeName));
      } else {
	ret =  ( pow(x.max(rangeName),c+1) - pow(x.min(rangeName),c+1) )/(c+1); 
      }

      //cout << "Int_pow_dx(c=" << c << ", xmin=" << x.min(rangeName) << ", xmax=" << x.max(rangeName) << ")=" << ret << endl ;
      return ret ;
    }
  }
  
  assert(0) ;
  return 0 ;
}

