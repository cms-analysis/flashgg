#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "TVector3.h"
#include "TVector2.h"

namespace flashgg {
  
  class LegacyVertexSelector : public VertexSelectorBase {
  

 public:
    LegacyVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf) {}

    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&,
                                  const VertexCandidateMap& vertexCandidateMap) const override;
//    edm::Ptr<reco::Vertex> EstZconv(const)


 
  private:
    unsigned int _whichVertex;
     };
   

    TVector3 diPho;  
    TVector3 tk;
    TVector2 tkPlane;
    TVector2 diPhoPlane;
    double sumpt = 0;
    double sumpt2 = 0;  
    double ptbal = 0; 
    double ptasym = 0;

  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
						      const VertexCandidateMap& vertexCandidateMap) const {
    std::cout<<"Running the LegacyVertexSelector"<<std::endl;

    // Just an example for how to read the vertex-candidate association -- doesn't do anything (yet)
    //bool verbose = false;
    //if (verbose) {
       diPho.SetXYZ(g1->px()+g2->px(),g1->py()+g2->py(),g1->pz()+g2->pz());
       diPhoPlane = diPho.XYvector();
       
       for (unsigned int i=0; i< g1->conversions().size();i++){

       std::cout << "blah" << "   " << g1->conversions().at(i)->refittedPairMomentum() << std::endl;        

        }

        for (unsigned int i = 0 ; i < vtxs.size() ; i++) {

	edm::Ptr<reco::Vertex> vtx = vtxs[i];
	std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
	for (unsigned int j = 0 ; j < vertexCandidateMap.at(vtx).size() ; j++) {
	    edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMap.at(vtx)[j];
            tk.SetXYZ(cand->px(),cand->py(),cand->pz());  
            tkPlane = tk.XYvector();
            sumpt += tkPlane.Mod(); 
            sumpt2 += tkPlane.Mod2();
            ptbal -= tkPlane * diPhoPlane.Unit(); 	
            
	 //std::cout << " Candidate " << j << " in vertex " << i << " has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
	}
         ptasym = (sumpt - diPhoPlane.Mod())/(sumpt+diPhoPlane.Mod());

         //std::cout << "sumpt2" << "  " << sumpt2  << std::endl;
         //std::cout << "ptbal" << "  " << ptbal   << std::endl;
         //std::cout << "ptasym" << "  " << ptasym  << std::endl;
         //Zconv =  

      }
    //}

    return vtxs[0];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
