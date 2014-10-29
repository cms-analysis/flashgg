#ifndef LEPTON_SELECTION 
#define LEPTON_SELECTION
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/TTHleptonicTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h" 

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <map>

using namespace std;
using namespace edm;


namespace flashgg {

PtrVector<pat::Muon> selectMuons(const PtrVector<pat::Muon>& muonPointers,Ptr<flashgg::DiPhotonCandidate> dipho, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold, double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold);

PtrVector<pat::Electron> selectElectrons(const PtrVector<pat::Electron>& elecPointers,Ptr<flashgg::DiPhotonCandidate> dipho, double rho, double elecPtThreshold);

PtrVector<pat::Electron> selectElectronsAwayFromTwoPhotons(const PtrVector<pat::Electron>& elecPointers,Ptr<flashgg::DiPhotonCandidate> dipho);

}
#endif
