#include "flashgg/MicroAODFormats/interface/Photon.h"

using namespace flashgg;

Photon::Photon() {}
Photon::Photon(const pat::Photon& aPhoton ) : pat::Photon(aPhoton) {}
Photon::~Photon() {}


float Photon::computeChargedPFIso(const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers, const edm::Ptr<reco::Vertex>& pvtx, const edm::Ptr<reco::Vertex>& vtx, float dRmax, float dRvetoBarrel, float dRvetoEndcap, float ptMin, float dzMax, float dxyMax){
  float ChargedPFIso = 0.;
  float dRveto = this->isEB() ? dRvetoBarrel : dRvetoEndcap;
  math::XYZVector photon_directionWrtVtx( this->superCluster()->x() - vtx->x(),
                                          this->superCluster()->y() - vtx->y(),
                                          this->superCluster()->z() - vtx->z());
  for(unsigned int i = 0 ; i < pfcandidatePointers.size() ; i++)
  {
    if( fabs(pfcandidatePointers[i]->pdgId()) != 211 ) continue; // only consider charged hadrons
    if( pfcandidatePointers[i]->pt() < ptMin ) continue;
    if( deltaR( pfcandidatePointers[i]->p4(), photon_directionWrtVtx ) >= dRmax ) continue;
    if( deltaR( pfcandidatePointers[i]->p4(), photon_directionWrtVtx ) <= dRveto) continue;
    float dZ = fabs( pfcandidatePointers[i]->dz() - (vtx->z() - pvtx->z()) );
    if( dZ > dzMax ) continue;
    float dxy = 0.;
    if( pfcandidatePointers[i]->pt() > 0.9 ) // track covariance matrix is available, we can get the pseudoTrack
      dxy = (
        - (pfcandidatePointers[i]->pseudoTrack().vx() - vtx->x()) * pfcandidatePointers[i]->py()
        + (pfcandidatePointers[i]->pseudoTrack().vy() - vtx->y()) * pfcandidatePointers[i]->px()
        ) / pfcandidatePointers[i]->pt();
    if( fabs(dxy) > dxyMax) continue;
    ChargedPFIso += pfcandidatePointers[i]->pt();
  }
  return ChargedPFIso;
}

void Photon::setChargedPFIso02(const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers, const edm::Ptr<reco::Vertex>& pvtx, const edm::Ptr<reco::Vertex>& vtx){
  float ChargedPFIso = computeChargedPFIso(pfcandidatePointers, pvtx, vtx, 0.2, 0.02, 0.02, 0.0, 0.2, 0.1);
  if (ChargedPFIso02_.count(vtx)) 
    ChargedPFIso02_[vtx] = ChargedPFIso;
  else
    ChargedPFIso02_.insert(std::make_pair(vtx, ChargedPFIso));
  return;
}

void Photon::setChargedPFIso02(const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers, const edm::PtrVector<reco::Vertex>& vtxPointers){
  // We don't have access to the complete track info but only to d0, dxy wrt primary vertex
  // as well as pseudoTrack for pt > 0.9 GeV
  // Hence: we are passing the primary vertex to recompute at least dz locally
  for(unsigned int i = 0 ; i < vtxPointers.size() ; i++ )
    setChargedPFIso02(pfcandidatePointers, vtxPointers[0], vtxPointers[i]);
  return;
}

