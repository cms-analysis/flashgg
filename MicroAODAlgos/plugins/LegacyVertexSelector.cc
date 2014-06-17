#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace flashgg {
  
  class LegacyVertexSelector : public VertexSelectorBase {
  public:
    LegacyVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf),
      _whichVertex(conf.getUntrackedParameter<unsigned int>("whichVertex",0)) {}

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&) const override;

  private:
    unsigned int _whichVertex; 
  };

  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs) const {
    std::cout<<"Running the LegacyVertexSelector"<<std::endl;
    return vtxs[_whichVertex];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
