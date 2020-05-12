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

    namespace LeptonSelection2018 {
        std::vector<edm::Ptr<flashgg::Electron> > selectElectrons( const std::vector<edm::Ptr<flashgg::Electron> > Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, std::vector<double> EleEtaCuts, double ElePhotonDrCut, double ElePhotonZMassCut, double DeltaRTrkEle, bool debug_, int id = 2);
        std::vector<edm::Ptr<flashgg::Muon    > > selectMuons( const std::vector<edm::Ptr<flashgg::Muon> > Muons, Ptr<flashgg::DiPhotonCandidate> dipho, const std::vector<edm::Ptr<reco::Vertex>> vtx, double MuonPtCut, double MuonEtaCut, double MuonMiniIsoCut, double MuonPhotonDrCut, bool debug_, int id = 2);
    }


    std::vector<edm::Ptr<flashgg::Muon> > selectMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold );

    std::vector<edm::Ptr<flashgg::Muon> > selectAllMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, 
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold);

    std::vector<edm::Ptr<flashgg::Muon> > selectMuonsSum16( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muMiniIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold );

    std::vector<edm::Ptr<flashgg::Muon> > selectAllMuonsSum16( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, 
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muMiniIsoSumRelThreshold );

    std::vector<edm::Ptr<Electron> > selectStdAllElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
                                                      const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,
                                                            vector<double> EtaCuts , bool useMVARecipe, bool useLooseID,
                                                            double rho, bool isData);

    std::vector<edm::Ptr<Electron> > selectStdElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
                                                         const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,  vector<double> EtaCuts,
                                                         bool useMVARecipe, bool useLooseID,
                                                         double deltaRPhoElectronThreshold, double DeltaRTrkElec, double deltaMassElectronZThreshold,
                                                         double rho, bool isData);

    std::vector<edm::Ptr<Electron> > selectAllElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
                                                         const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double ElectronPtThreshold ,  double TransverseImpactParam,
                                                         double LongitudinalImpactParam, vector<double> NonTrigMVAThreshold, vector<double> NonTrigMVAEtaCuts,
                                                         double IsoThreshold, double NumOfMissingHitsThreshold, vector<double> EtaCuts );
    
    std::vector<edm::Ptr<Electron> > selectElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
                                                      const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double ElectronPtThreshold ,  double TransverseImpactParam,
                                                      double LongitudinalImpactParam, vector<double> NonTrigMVAThreshold, vector<double> NonTrigMVAEtaCuts, double IsoThreshold,
                                                      double NumOfMissingHitsThreshold, vector<double> EtaCuts , double deltaRPhoElectronThreshold, double DeltaRTrkElec,
                                                      double deltaMassElectronZThreshold );

    std::vector<edm::Ptr<Electron> > selectAllElectronsSum16( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
                                                              const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers,  double ElectronPtThreshold,
                                                              vector<double> EtaCuts , bool useMVARecipe, bool useLooseID,
                                                              double elMiniIsoEBThreshold, double elMiniIsoEEThreshold,
                                                              double TransverseImpactParam_EB, double LongitudinalImpactParam_EB, double TransverseImpactParam_EE, double LongitudinalImpactParam_EE,
                                                              double rho, bool isData);

    std::vector<edm::Ptr<Electron> > selectElectronsSum16( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
                                                           const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,  vector<double> EtaCuts,
                                                           double deltaRPhoElectronThreshold, double DeltaRTrkElec, double deltaMassElectronZThreshold, 
                                                           double elMiniIsoEBThreshold, double elMiniIsoEEThreshold,
                                                           double TransverseImpactParam_EB, double LongitudinalImpactParam_EB, double TransverseImpactParam_EE, double LongitudinalImpactParam_EE,
                                                           double rho, bool isData);
    

    std::vector<edm::Ptr<Electron> > selectElectronsNotMatchingPhotons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, const Ptr<flashgg::DiPhotonCandidate> dipho );
    Ptr<reco::Vertex> chooseElectronVertex( Ptr<flashgg::Electron> &elec, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers );
    
    vector<bool> EgammaIDs(const edm::Ptr<flashgg::Electron> &elec, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double rho );
    bool phoVeto(   const Ptr<flashgg::Electron> electron , const Ptr<flashgg::DiPhotonCandidate> diphoton, float deltaRPhoElectronThreshold, float DeltaRTrkElec, float deltaMassElectronZThreshold);
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

