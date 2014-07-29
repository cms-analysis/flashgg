#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TMath.h"

namespace flashgg {
  
  class LegacyVertexSelector : public VertexSelectorBase {
  public:
    LegacyVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf) {}

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&,
                                  const VertexCandidateMap& vertexCandidateMap,
				  const edm::PtrVector<reco::Conversion>&) const override;

  private:
    unsigned int _whichVertex; 
  };

  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
						      const VertexCandidateMap& vertexCandidateMap,
						      const edm::PtrVector<reco::Conversion>& conversionsVector) const {

    bool verbose = false;
    if (verbose) {
      for (unsigned int i=0; i<conversionsVector.size();i++){
	edm::Ptr<reco::Conversion> conv = conversionsVector[i];
	std::cout << "conversion x,y" << "   " << conv->conversionVertex().x() <<" "<< conv->conversionVertex().y() <<std::endl;
      }
      
      for (unsigned int i = 0 ; i < vtxs.size() ; i++) {
	edm::Ptr<reco::Vertex> vtx = vtxs[i];
	std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
	for (unsigned int j = 0 ; j < vertexCandidateMap.at(vtx).size() ; j++) {
	  edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMap.at(vtx)[j];
	  std::cout << " Candidate " << j << " in vertex " << i << " has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
	}
      }
    }

    return vtxs[0];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
