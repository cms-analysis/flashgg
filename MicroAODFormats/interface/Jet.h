#ifndef FLASHgg_Jet_h
#define FLASHgg_Jet_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include <unordered_map>

namespace flashgg {

  class Jet : public pat::Jet {

  public:
    Jet();
    Jet(const pat::Jet &);
    ~Jet();
    float getPuJetId(const edm::Ptr<reco::Vertex>) const;
    float getPuJetId(const edm::Ptr<DiPhotonCandidate>) const;
    void setPuJetId(const edm::Ptr<reco::Vertex>, float);
  private:
    std::map<edm::Ptr<reco::Vertex>, float> PuJetId_; // change to unordered_map later.  getting c++11 errors in 7_0_4
  };
}

#endif
