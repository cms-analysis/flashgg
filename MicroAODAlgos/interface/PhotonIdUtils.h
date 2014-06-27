#ifndef FLASHgg_PhotonIdUtils_h
#define FLASHgg_PhotonIdUtils_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"


#include "flashgg/MicroAODFormats/interface/Photon.h"

namespace flashgg {

  class PhotonIdUtils {

  public:
    
    PhotonIdUtils();
    // add a non-default constructor? 
    ~PhotonIdUtils();

    void               initialize( );
    float              pfIsoChgWrtVtx( const edm::Ptr<pat::Photon>&, edm::Ptr<reco::Vertex>, const edm::PtrVector<pat::PackedCandidate>&, float, float, float, float );
    std::vector<float> pfIsoChgWrtAllVtx( const edm::Ptr<pat::Photon>&, edm::Handle<reco::Vertex>, const edm::PtrVector<pat::PackedCandidate>&, float, float, float, float );

  private: 
    
    edm::Handle<reco::Vertex>  vtxHandle;
    
    









  };


}


#endif
