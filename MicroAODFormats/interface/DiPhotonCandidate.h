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
    ~DiPhotonCandidate();

    const reco::Vertex & getVertex() const {return *vertex_;};
    const flashgg::Photon & getPhoton(size_type) const;

    // These are an alternative to getPhoton above, but if we put them in we have to enforce ordering
    //    const flashggPhoton& leadingPhoton() const { return static_cast<const flashggPhoton&>(*daughter(0)); }
    //    const flashggPhoton& subLeadingPhoton() const { return static_cast<const flashggPhoton&>(*daughter(1)); }

  private:
    edm::Ptr<reco::Vertex> vertex_;
    
  };


}


#endif
