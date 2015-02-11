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
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/Electron.h"
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

	PtrVector<pat::Muon> selectMuons(const PtrVector<pat::Muon>& muonPointers,Ptr<flashgg::DiPhotonCandidate> dipho, const PtrVector<reco::Vertex>& vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold, double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold);

	PtrVector<Electron> selectElectrons(const PtrVector<flashgg::Electron>& ElectronPointers, const PtrVector<reco::Vertex>& vertexPointers,double ElectronPtThreshold ,double DeltaRTrkElec ,double TransverseImpactParam,double LongitudinalImpactParam, double NonTrigMVAThreshold, double IsoThreshold, double NumOfMissingHitsThreshold, vector<double> EtaCuts);

	Ptr<reco::Vertex> ChooseElectronVertex(Ptr<flashgg::Electron> & elec,const PtrVector<reco::Vertex> & vertexPopinters);

}

#endif
