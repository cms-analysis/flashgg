#ifndef TTHGGFILTER_h
#define TTHGGFILTER_h

// -*- C++ -*-
//
// Package:    ttHGGFilter
// Class:      ttHGGFilter
// 
/* 
 Description: filter events based on the Pythia ProcessID and the Pt_hat
 Implementation: inherits from generic EDFilter
     
*/
//
// Original Author:  Martina Malberti
//         Created:  Tue Jan 12 22:57:54 CET 2016
//
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

//
// class decleration
//

using namespace std;
using namespace edm;

class ttHGGFilter : public edm::EDFilter {
 public:
  explicit ttHGGFilter(const edm::ParameterSet&);
  ~ttHGGFilter();


  virtual bool filter(edm::Event&, const edm::EventSetup&);
 private:
  // ----------member data ---------------------------      
  EDGetTokenT<View<reco::GenParticle> > genParticleToken_;

};
#endif
