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
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
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


// 2018 Single Lepton selection

    std::vector<edm::Ptr<flashgg::Electron> > selectElectrons( const std::vector<edm::Ptr<flashgg::Electron> > Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, std::vector<double> EleEtaCuts, double ElePhotonDrCut, double ElePhotonZMassCut, double DeltaRTrkEle, bool debug_, int id = 2);
    
    std::vector<edm::Ptr<flashgg::Muon    > > selectMuons( const std::vector<edm::Ptr<flashgg::Muon> > Muons, Ptr<flashgg::DiPhotonCandidate> dipho, const std::vector<edm::Ptr<reco::Vertex>> vtx, double MuonPtCut, double MuonEtaCut, double MuonMiniIsoCut, double MuonPhotonDrCut, bool debug_, int id = 2);
    

// DiLeptonic selection    

    std::vector<pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Muon>>> selectDiMuons(const std::vector<edm::Ptr<flashgg::Muon>> Muons, Ptr<flashgg::DiPhotonCandidate> dipho, double MuonEtaCut, double MuonPtCut, double MuonPhotonDrCut, double MuonsDrCut, double MuonsZMassCut, double LeadingLeptonPtCut);

    std::vector<pair<edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Electron>>> selectDiElectrons(const std::vector<edm::Ptr<flashgg::Electron>> Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, std::vector<double> EleEtaCuts, double ElePhotonDrCut, double ElesDrCut, double ElePhotonZMassCut, double ElesZMassCut, double LeadingLeptonPtCut);

    std::vector<pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Electron>>> selectEleMuon(const std::vector<edm::Ptr<flashgg::Muon>> Muons, const std::vector<edm::Ptr<flashgg::Electron>> Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, double MuonPtCut, std::vector<double> EleEtaCuts, double MuonEtaCut, double MuonPhotonDrCut, double ElePhotonDrCut, double EleMuDrCut, double LeadingPtLeptonCut);



}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

