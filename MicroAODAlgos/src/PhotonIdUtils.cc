#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils() {};
PhotonIdUtils::~PhotonIdUtils() {};

float PhotonIdUtils::pfIsoChgWrtVtx( const flashgg::Photon &photon, 
				  reco::VertexRef vtx,  
				  edm::Handle<reco::PFCandidateCollection> pfHandle,
				  float coneSize, float coneVeto, 
				  float dzMax, float dxyMax ) 
{

  float isovalue = -9;

  math::XYZVector SCdirection( photon.superCluster()->x() - vtx->x(),
			       photon.superCluster()->y() - vtx->y(),
			       photon.superCluster()->z() - vtx->z() 
			       );

  //edm::Handle<reco::PFCandidateCollection> pfHandle;

  const reco::PFCandidateCollection* forIsolation = pfHandle.product();
  
  for( size_t ipf = 0; ipf < forIsolation->size(); ipf++ ) { 
      
    if( (*forIsolation)[ipf].particleId() != reco::PFCandidate::h ) continue;  // to do: write function that computes the isolation for any type
    float dxyTkToVtx = (*forIsolation)[ipf].trackRef()->dxy(vtx->position());
    float dzTkToVtx  = (*forIsolation)[ipf].trackRef()->dz(vtx->position());
    float dRTkToVtx  = deltaR( (*forIsolation)[ipf].momentum().Eta(), (*forIsolation)[ipf].momentum().Phi(),
			       SCdirection.Eta(), SCdirection.Phi() );

    if( dxyTkToVtx > dxyMax ) continue;
    if( dzTkToVtx  > dzMax  ) continue;
    if( dRTkToVtx > coneSize || dRTkToVtx < coneVeto ) continue;

    isovalue += (*forIsolation)[ipf].pt();
  }

  return isovalue;
}


vector<float> PhotonIdUtils::pfIsoChgWrtAllVtx( const flashgg::Photon &photon, 
					     edm::Handle<reco::Vertex> vtxHandle,
					     edm::Handle<reco::PFCandidateCollection> pfHandle, 
					     float coneSize, float coneVeto, 
					     float dzMax, float dxyMax )
{
  vector<float> isovalues;

  

  return isovalues;
}
