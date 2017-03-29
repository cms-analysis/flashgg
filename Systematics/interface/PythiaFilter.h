#ifndef PYTHIAFILTER_h
#define PYTHIAFILTER_h

// -*- C++ -*-
//
// Package:    PythiaFilter
// Class:      PythiaFilter
// 
/* 
 Description: filter events based on the Pythia ProcessID and the Pt_hat
 Implementation: inherits from generic EDFilter
     
*/
//
// Original Author:  Martina Malberti
//         Created:  Tue Jan 12 22:57:54 CET 2016
//
// Cut-band-based by Seth Zenz


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

//
// class decleration
//

using namespace std;
using namespace edm;

class PythiaFilter : public edm::EDFilter {
 public:
  explicit PythiaFilter(const edm::ParameterSet&);
  ~PythiaFilter();


  virtual bool filter(edm::Event&, const edm::EventSetup&);
 private:
  // ----------member data ---------------------------      
  EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
  EDGetTokenT<vector<pat::PackedGenParticle> > packedGenParticleToken_;
  bool usePacked_;

};
#endif
