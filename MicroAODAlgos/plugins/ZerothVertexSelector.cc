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

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&) const override;

  private:
    unsigned int _whichVertex; // set this variable to something non-zero to make this stupid selector both stupider and poorly-named
                               // Also it would cause crashes if there was only 1 vertex in the collection
  };

  edm::Ptr<reco::Vertex> ZerothVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs) const {
	std::cout<<v_id<<std::endl;
      float sumpt2=0;
      float ptbal=0;
      float sum_norm_pt=0;
      float ptasym=0;

      DiPhotonCandidate diphoton=DiPhotonCandidate(g1,g2,vtxs[v_id]);
      
      for(reco::Vertex::trackRef_iterator track = vtxs[v_id].tracks_begin();track != vtxs[v_id].tracks_end(); ++track) { //summing over the tracks associated to the vertex
	std::cout<<(*track)->px()<<std::endl;
	sumpt2=sumpt2
	  +(*track)->pt()->Mag2();
	ptbal=ptbal
	  -(*track)->pt()->Dot(diphoton->pt()/(diphoton->pt().Mag()));
	sum_norm_pt=sum_norm_pt
	  +(*track)->pt()->Mag();
      }
      ptasym=sum_norm_pt-diphoton->pt()->Mag()/(sum_norm_pt+diphoton->pt()->Mag());

      std::cout<<"The three quantities for vtx "<<v_id<<" are "<<" sumpt2:"<<sumpt2<<" ptbal:"<<ptbal<<" ptasym:"<<ptasym<<std::endl;

    return vtxs[_whichVertex];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::ZerothVertexSelector,
		  "FlashggZerothVertexSelector");
