#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace flashgg {
  
  class ZerothVertexSelector : public VertexSelectorBase {
  public:
    ZerothVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf),
      _whichVertex(conf.getUntrackedParameter<unsigned int>("whichVertex",0)) {}

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&) override;

  private:
    unsigned int _whichVertex; // set this variable to something non-zero to make this stupid selector both stupider and poorly-named
  };

  edm::Ptr<reco::Vertex> ZerothVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs) {
    return vtxs[_whichVertex];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::ZerothVertexSelector,
		  "FlashggZerothVertexSelector");
