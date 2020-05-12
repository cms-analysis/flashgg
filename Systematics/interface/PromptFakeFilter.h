#ifndef PromptFakeFILTER_h
#define PromptFakeFILTER_h

// -*- C++ -*-
//
// Package:    PromptFakeFilter
// Class:      PromptFakeFilter
// 
/* 
Requires fake-fake or prompt-fake diphoton
*/
// S Zenz


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

//
// class decleration
//

using namespace std;
using namespace edm;

class PromptFakeFilter : public edm::EDFilter {
 public:
  explicit PromptFakeFilter(const edm::ParameterSet&);
  ~PromptFakeFilter();


  virtual bool filter(edm::Event&, const edm::EventSetup&);
 private:
  // ----------member data ---------------------------      
  EDGetTokenT<View<flashgg::DiPhotonTagBase> > tagToken_;
  bool fakeFake_;
  bool promptFake_;
  bool acceptBoth_;

};
#endif
