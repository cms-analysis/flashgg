#ifndef FLASHgg_Jet_h
#define FLASHgg_Jet_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"

namespace flashgg {

  struct MinimalPileupJetIdentifier {
    float RMS;
    float betaStar;
    int idFlag;
  };

  class Jet : public pat::Jet {

  public:
    Jet();
    Jet(const pat::Jet &);
    ~Jet();
    void setPuJetId(const edm::Ptr<reco::Vertex> vtx, const PileupJetIdentifier &);
    bool hasPuJetId(const edm::Ptr<reco::Vertex> vtx) const;
    bool passesPuJetId(const edm::Ptr<reco::Vertex> vtx, PileupJetIdentifier::Id level = PileupJetIdentifier::kLoose) const;
    float RMS(const edm::Ptr<reco::Vertex> vtx) const;
    float betaStar(const edm::Ptr<reco::Vertex> vtx) const;
    bool passesPuJetId(const edm::Ptr<DiPhotonCandidate> dipho, PileupJetIdentifier::Id level = PileupJetIdentifier::kLoose) const;
    float RMS(const edm::Ptr<DiPhotonCandidate> dipho) const;
    float betaStar(const edm::Ptr<DiPhotonCandidate> dipho) const;
  private:
    std::map<edm::Ptr<reco::Vertex>,MinimalPileupJetIdentifier> puJetId_;
  };
}

#endif
