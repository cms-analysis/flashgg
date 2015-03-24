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
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <map>

using namespace std;
using namespace edm;


namespace flashgg {

	std::vector<edm::Ptr<pat::Muon> > selectMuons(const edm::Handle<edm::View<pat::Muon> >& muonPointers,Ptr<flashgg::DiPhotonCandidate> dipho, const edm::Handle<edm::View<reco::Vertex> >& vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold, double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold);

	std::vector<edm::Ptr<Electron> > selectElectrons(const edm::Handle<edm::View<flashgg::Electron> >& ElectronPointers, const edm::Handle<edm::View<reco::Vertex> >& vertexPointers,double ElectronPtThreshold ,double DeltaRTrkElec ,double TransverseImpactParam,double LongitudinalImpactParam, double NonTrigMVAThreshold, double IsoThreshold, double NumOfMissingHitsThreshold, vector<double> EtaCuts);

	Ptr<reco::Vertex> chooseElectronVertex(Ptr<flashgg::Electron> & elec,const PtrVector<reco::Vertex> & vertexPopinters);

}

#endif
