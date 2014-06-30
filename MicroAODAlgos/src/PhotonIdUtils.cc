#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils() {};
PhotonIdUtils::~PhotonIdUtils() {};

float PhotonIdUtils::pfIsoChgWrtVtx( const edm::Ptr<flashgg::Photon>& photon, 
				     edm::Ptr<reco::Vertex> vtx,  
				     const edm::PtrVector<pat::PackedCandidate>& pfcandidates,
				     float coneSize, float coneVeto, 
				     float dzMax, float dxyMax ) 
{

  float isovalue = 0;

  math::XYZVector SCdirection( photon->superCluster()->x() - vtx->x(),
			       photon->superCluster()->y() - vtx->y(),
			       photon->superCluster()->z() - vtx->z() 
			       );

  //edm::Handle<reco::PFCandidateCollection> pfHandle;

  //const reco::PFCandidateCollection* forIsolation = pfHandle.product();
  //const edm::PtrVector<pat::PackedCandidate>& forIsolation  = pfHandle->ptrVector();  

  for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 
      
    //if( forIsolation[ipf]->particleId() != pat::PackedCandidate::h ) continue;  // to do: write function that computes the isolation for any type
    float dxyTkToVtx = pfcandidates[ipf]->dxy(vtx->position());
    float dzTkToVtx  = pfcandidates[ipf]->dz(vtx->position());
    float dRTkToVtx  = deltaR( pfcandidates[ipf]->momentum().Eta(), pfcandidates[ipf]->momentum().Phi(),
			       SCdirection.Eta(), SCdirection.Phi() );

    if( dxyTkToVtx > dxyMax ) continue;
    if( dzTkToVtx  > dzMax  ) continue;
    if( dRTkToVtx > coneSize || dRTkToVtx < coneVeto ) continue;


    isovalue += pfcandidates[ipf]->pt();
  }

  return isovalue;
}


vector<float> PhotonIdUtils::pfIsoChgWrtAllVtx( const edm::Ptr<flashgg::Photon>& photon, 
						edm::Handle<reco::Vertex> vtxHandle,
						const edm::PtrVector<pat::PackedCandidate>& pfcandidates, 
					     	float coneSize, float coneVeto, 
						float dzMax, float dxyMax )
{
  vector<float> isovalues;

  

  return isovalues;
}
