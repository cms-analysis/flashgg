#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace flashgg {
  
  class LegacyVertexSelector : public VertexSelectorBase {
  public:
    LegacyVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf) {}

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&,
                                  const edm::AssociationMap<edm::OneToMany<reco::VertexCollection, pat::PackedCandidateCollection> > &) const override;

  private:
    unsigned int _whichVertex; 
  };

  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
                                                      const edm::AssociationMap<edm::OneToMany<reco::VertexCollection, pat::PackedCandidateCollection> > & vertexAssociationMap
                                                      ) const {
    std::cout<<"Running the LegacyVertexSelector"<<std::endl;

    // Now we have a very technical problem. We want to do something like this...
    //    pat::PackedCandidateCollection candidates = vertexAssociationMap[vtxs[0]];
    // but vertexAssociationMap wants a edm::Ref<std::vector<reco::Vertex> > and we have a edm::PtrVector<reco::Vertex>

    return vtxs[0];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
