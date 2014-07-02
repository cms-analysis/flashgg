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

  
  for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 
      
    if( fabs(pfcandidates[ipf]->pdgId()) != 211 ) continue;     // equivalent to particleType::h
    float dxyTkToVtx = pfcandidates[ipf]->dxy(vtx->position());
    float dzTkToVtx  = pfcandidates[ipf]->dz(vtx->position());
    float dRTkToVtx  = deltaR( pfcandidates[ipf]->momentum().Eta(), pfcandidates[ipf]->momentum().Phi(),
			       SCdirection.Eta(), SCdirection.Phi() );

    /*
    std::cout << "through pseudoTrack->vz() " <<  pfcandidates[ipf]->pseudoTrack().vz() << 
      " direct vz() " <<  pfcandidates[ipf]->vz() << 
      " direct dz() " << pfcandidates[ipf]->dz(vtx->position()) << 
      " pv " << vtx->z() << 
      std::endl;
    */

    if( dxyTkToVtx > dxyMax ) continue;
    if( dzTkToVtx  > dzMax  ) continue;
    if( dRTkToVtx > coneSize || dRTkToVtx < coneVeto ) continue;

    isovalue += pfcandidates[ipf]->pt();
  }

  return isovalue;
}


vector<float> PhotonIdUtils::pfIsoChgWrtAllVtx( const edm::Ptr<flashgg::Photon>& photon, 
						const edm::PtrVector<reco::Vertex>& vertices,
						const edm::PtrVector<pat::PackedCandidate>& pfcandidates, 
					     	float coneSize, float coneVeto, 
						float dzMax, float dxyMax )
{
  vector<float> isovalues;

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    
    float iso = pfIsoChgWrtVtx( photon, vertices[iv], pfcandidates, coneSize, coneVeto, dzMax, dxyMax );
    isovalues.push_back(iso);
  }

  return isovalues;
}


float PhotonIdUtils::pfIsoGamma( const edm::Ptr<flashgg::Photon>& photon, 
				 const edm::PtrVector<pat::PackedCandidate>& pfcandidates,
				 float dRMax,
				 float dRVetoBarrel,
				 float dRVetoEndcap,
				 float etaStripBarrel,
				 float etaStripEndcap, 
				 float minEnergyBarrel, 
				 float minEnergyEndcap
				)
{

  float isovalue = 0;
  
  float maxdR = 99;
  float maxetaStrip = 99;

  if( photon->isEB() ) {
    maxdR        = dRVetoBarrel;
    maxetaStrip  = etaStripBarrel;
  } else if( photon->isEE() ){
    maxdR        = dRVetoEndcap;
    maxetaStrip  = etaStripEndcap; 
  }   

  for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 

    int nass = 0;
    edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf]; 

    if( pfcand->pdgId() != 22 ) continue;
    if( photon->isEB() ) if( fabs(pfcand->pt()) < minEnergyBarrel )     continue;  
    if( photon->isEE() ) if( fabs(pfcand->energy()) < minEnergyEndcap ) continue;

    edm::RefVector<pat::PackedCandidateCollection> associated =  photon->associatedPackedPFCandidates();
    if( associated.size() > 0 ) {

      for( unsigned int ipc = 0; ipc < associated.size(); ipc++ ) {

	edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated[ipc] );
	if( associatedPtr == pfcand ) nass++;
      }  
    }
    if( nass > 0 ) continue;
   
    math::XYZPoint  pfcandvtx = pfcand->vertex();
    math::XYZVector SCdirectionWrtCandVtx( photon->superCluster()->x() - pfcandvtx.x(),
					   photon->superCluster()->y() - pfcandvtx.y(),
					   photon->superCluster()->z() - pfcandvtx.z() 
					   );

    float dEta = fabs( SCdirectionWrtCandVtx.Eta() - pfcand->momentum().Eta() );
    float dR   = deltaR( SCdirectionWrtCandVtx.Eta(), SCdirectionWrtCandVtx.Phi(), pfcand->momentum().Eta(), pfcand->momentum().Phi() ); 

    if( dEta > maxetaStrip ) continue;
    if( dR > maxdR )         continue;
      
    isovalue += pfcand->pt();
  }

  return isovalue;

}
