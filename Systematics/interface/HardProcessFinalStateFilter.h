#ifndef HARDPROCESSFINALSTATEFILTER_h
#define HARDPROCESSFINALSTATEFILTER_h

// -*- C++ -*-
//
// Package:    HardProcessFinalStateFilter
// Class:      HardProcessFinalStateFilter
// 

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//
// class decleration
//

using namespace std;
using namespace edm;

class HardProcessFinalStateFilter : public edm::EDFilter {
 public:
  explicit HardProcessFinalStateFilter(const edm::ParameterSet&);
  ~HardProcessFinalStateFilter();

  virtual void endJob() override;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
 private:
  // ----------member data ---------------------------      
  EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
  EDGetTokenT<GenEventInfoProduct> genInfoToken_;

  int npass,nfail,ntot;
  float wpass,wfail,wtot;

};
#endif
