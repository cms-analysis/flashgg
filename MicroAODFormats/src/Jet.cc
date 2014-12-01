#include "flashgg/MicroAODFormats/interface/Jet.h"

using namespace flashgg;

Jet::Jet() : pat::Jet() {
  puJetId_.clear();
}

Jet::Jet(const pat::Jet& aJet ) : pat::Jet(aJet) {
}

Jet::~Jet() {}

void Jet::setPuJetId(const edm::Ptr<reco::Vertex> vtx, const PileupJetIdentifier & id) {

  // Prepare new identifier that zeroes non-relevant variables.
  // This will help CMSSW with compression, apparently
  PileupJetIdentifier zeroed_id;

  // We keep these three
  zeroed_id.RMS(id.RMS());
  zeroed_id.betaStar(id.RMS());
  zeroed_id.idFlag(id.idFlag());

  // Set all the others to zero
  zeroed_id.jetEta(0.);
  zeroed_id.jetPt(0.);
  zeroed_id.nCharged   (0.);
  zeroed_id.nNeutrals(0.);
  zeroed_id.dZ         (0.);  
  zeroed_id.nParticles (0.);  
  zeroed_id.dRMean     (0.);  
  zeroed_id.frac01    (0.);  
  zeroed_id.frac02    (0.);  
  zeroed_id.frac03    (0.);  
  zeroed_id.frac04    (0.);  
  zeroed_id.frac05   (0.);  
  zeroed_id.frac06   (0.);  
  zeroed_id.frac07   (0.);  
  zeroed_id.beta       (0.);  
  zeroed_id.betaStar   (0.);  
  zeroed_id.betaClassic   (0.);  
  zeroed_id.betaStarClassic   (0.);  
  zeroed_id.ptD        (0.);
  zeroed_id.nvtx   (0.);  
  zeroed_id.mva      (0.);
  zeroed_id.jetPhi   (0.);
  zeroed_id.jetM     (0.);
  zeroed_id.chgEMfrac  (0.);
  zeroed_id.neuEMfrac  (0.);
  zeroed_id.chgHadrfrac(0.);
  zeroed_id.neuHadrfrac(0.);
  zeroed_id.d0         (0.);   
  zeroed_id.leadPt     (0.);  
  zeroed_id.leadEta    (0.);  
  zeroed_id.leadPhi    (0.);  
  zeroed_id.secondPt   (0.);  
  zeroed_id.secondEta  (0.);  
  zeroed_id.secondPhi  (0.);  
  zeroed_id.leadNeutPt (0.);  
  zeroed_id.leadNeutEta(0.);  
  zeroed_id.leadNeutPhi(0.);  
  zeroed_id.leadEmPt   (0.);  
  zeroed_id.leadEmEta  (0.);  
  zeroed_id.leadEmPhi  (0.);  
  zeroed_id.leadChPt   (0.);  
  zeroed_id.leadChEta  (0.);  
  zeroed_id.leadChPhi  (0.);  
  zeroed_id.dRLeadCent (0.);  
  zeroed_id.dRLead2nd  (0.);  
  zeroed_id.dRMeanNeut (0.);  
  zeroed_id.dRMeanEm   (0.);  
  zeroed_id.dRMeanCh   (0.);  
  zeroed_id.ptMean     (0.);
  zeroed_id.ptRMS      (0.);
  zeroed_id.pt2A       (0.);
  zeroed_id.ptDCh      (0.);
  zeroed_id.ptDNe      (0.);
  zeroed_id.sumPt      (0.);
  zeroed_id.sumChPt    (0.);
  zeroed_id.sumNePt    (0.);
  zeroed_id.leadFrac    (0.);  
  zeroed_id.secondFrac  (0.);  
  zeroed_id.thirdFrac   (0.);  
  zeroed_id.fourthFrac  (0.);  
  zeroed_id.leadChFrac    (0.);  
  zeroed_id.secondChFrac  (0.);  
  zeroed_id.thirdChFrac   (0.);  
  zeroed_id.fourthChFrac  (0.);  
  zeroed_id.leadNeutFrac    (0.);  
  zeroed_id.secondNeutFrac  (0.);  
  zeroed_id.thirdNeutFrac   (0.);  
  zeroed_id.fourthNeutFrac  (0.);  
  zeroed_id.leadEmFrac    (0.);  
  zeroed_id.secondEmFrac  (0.);  
  zeroed_id.thirdEmFrac   (0.);  
  zeroed_id.fourthEmFrac  (0.);  
  zeroed_id.jetW  (0.);  
  zeroed_id.etaW  (0.);  
  zeroed_id.phiW  (0.);  
  zeroed_id.majW  (0.);  
  zeroed_id.minW  (0.);  
  zeroed_id.chFrac01    (0.);  
  zeroed_id.chFrac02    (0.);  
  zeroed_id.chFrac03    (0.);  
  zeroed_id.chFrac04    (0.);  
  zeroed_id.chFrac05   (0.);  
  zeroed_id.chFrac06   (0.);  
  zeroed_id.chFrac07   (0.);  
  zeroed_id.neutFrac01    (0.);  
  zeroed_id.neutFrac02    (0.);  
  zeroed_id.neutFrac03    (0.);  
  zeroed_id.neutFrac04    (0.);  
  zeroed_id.neutFrac05   (0.);  
  zeroed_id.neutFrac06   (0.);  
  zeroed_id.neutFrac07   (0.);  
  zeroed_id.emFrac01    (0.);  
  zeroed_id.emFrac02    (0.);  
  zeroed_id.emFrac03    (0.);  
  zeroed_id.emFrac04    (0.);  
  zeroed_id.emFrac05   (0.);  
  zeroed_id.emFrac06   (0.);  
  zeroed_id.emFrac07   (0.);  

  puJetId_.insert(std::make_pair(vtx,zeroed_id));
}

bool Jet::passesPuJetId(const edm::Ptr<reco::Vertex> vtx, PileupJetIdentifier::Id level) const {
  assert (puJetId_.count(vtx));
  return PileupJetIdentifier::passJetId(puJetId_.at(vtx).idFlag(),level);
}

float Jet::RMS(const edm::Ptr<reco::Vertex> vtx) const {
  assert (puJetId_.count(vtx));
  return puJetId_.at(vtx).RMS();
}

float Jet::betaStar(const edm::Ptr<reco::Vertex> vtx) const {
  assert (puJetId_.count(vtx));
  return puJetId_.at(vtx).betaStar();
}

bool Jet::passesPuJetId(const edm::Ptr<DiPhotonCandidate> dipho, PileupJetIdentifier::Id level) const {
  return passesPuJetId(dipho->getVertex());
}

float Jet::RMS(const edm::Ptr<DiPhotonCandidate> dipho) const {
  return RMS(dipho->getVertex());
}

float Jet::betaStar(const edm::Ptr<DiPhotonCandidate> dipho) const {
  return betaStar(dipho->getVertex());
}

