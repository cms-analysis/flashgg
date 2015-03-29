#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

namespace flashgg {
  
  class ZerothVertexSelector : public VertexSelectorBase {
  public:
    ZerothVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf),
      _whichVertex(conf.getUntrackedParameter<unsigned int>("whichVertex",0)) {}

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,
      const std::vector<edm::Ptr<reco::Vertex> >&,
      const VertexCandidateMap&,
      const std::vector<edm::Ptr<reco::Conversion> >&,
      const math::XYZPoint&
      //  const Parameters_Selector_Type&,
      //  const float&
      ) override;

    void writeInfoFromLastSelectionTo(flashgg::DiPhotonCandidate&) override;

  private:
    unsigned int _whichVertex; // set this variable to something non-zero to make this stupid selector both stupider and poorly-named
                               // Also it would cause crashes if there was only 1 vertex in the collection
  };

  edm::Ptr<reco::Vertex> ZerothVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,
						      const edm::Ptr<flashgg::Photon>& g2,
						      const std::vector<edm::Ptr<reco::Vertex> >& vtxs,
						      const VertexCandidateMap& vertexCandidateMap,
						      const std::vector<edm::Ptr<reco::Conversion> >& convs,
						      const math::XYZPoint& beamSpot
						      //						      const Parameters_Selector_Type& param,
						      //                                                      const float& beamsig 
						      ) {
    return vtxs[_whichVertex];
  }

  void ZerothVertexSelector::writeInfoFromLastSelectionTo(flashgg::DiPhotonCandidate& dipho) {
    // No need to store anything if we're just taking the zeroth vertex
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
                  flashgg::ZerothVertexSelector,
                  "FlashggZerothVertexSelector");
