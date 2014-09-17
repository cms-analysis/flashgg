#ifndef FLASHgg_DiPhotonCandidate_h
#define FLASHgg_DiPhotonCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"


namespace flashgg {
  class DiPhotonCandidate : public reco::CompositeCandidate {
  public:
    DiPhotonCandidate();
    DiPhotonCandidate(edm::Ptr<flashgg::Photon>,edm::Ptr<flashgg::Photon>,edm::Ptr<reco::Vertex>);
    DiPhotonCandidate(flashgg::Photon,flashgg::Photon,edm::Ptr<reco::Vertex>);
    ~DiPhotonCandidate();

    const edm::Ptr<reco::Vertex> getVertex() const { return vertex_; }
    const flashgg::Photon* leadingPhoton() const;
    const flashgg::Photon* subLeadingPhoton() const;

  private:
    edm::Ptr<reco::Vertex> vertex_;
  };


}


#endif
