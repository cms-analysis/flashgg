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
  TVector3 diPho;  
  TVector3 tk;
  TVector2 tkPlane;
  TVector2 diPhoXY;
  TVector3 VtxtoSC;
  TVector3 VtxtoSCPho1;
  TVector3 VtxtoSCPho2;
  TVector3 RefPairMo;
  double sumpt = 0;
  double sumpt2 = 0;  
  double ptbal = 0; 
  double ptasym = 0;
  double dR = 0;
  double dRPho1 = 0;
  double dRPho2 = 0;
  
  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
						      const VertexCandidateMap& vertexCandidateMap,
						      const edm::PtrVector<reco::Conversion>& conversionsVector) const {
    bool verbose = true;
    if (verbose) {
      
      diPho.SetXYZ(g1->px()+g2->px(),g1->py()+g2->py(),g1->pz()+g2->pz());
      diPhoXY = diPho.XYvector();
      
      for (unsigned int i=0; i<conversionsVector.size();i++){
	edm::Ptr<reco::Conversion> conv = conversionsVector[i];
	std::cout << "conversion x,y" << "   " << conv->conversionVertex().x() <<" "<< conv->conversionVertex().y() <<std::endl;

	if (g1->hasConversionTracks() && !g2->hasConversionTracks()){      
	  if(conv->isConverted() == 1){
	    VtxtoSC.SetXYZ(g1->superCluster()->position().x() - conv->conversionVertex().x(), 
			   g1->superCluster()->position().y() - conv->conversionVertex().y(), 
			   g1->superCluster()->position().z() - conv->conversionVertex().z());
	    
	    RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
	    dR = VtxtoSC.DeltaR(RefPairMo); 
            
	    std::cout << "g1 conv track? "<< g1->hasConversionTracks() << "  " << "g2 conv track?" << g2->hasConversionTracks() << "g1,g2->pt " <<g1->pt()<<" "<<g2->pt()<<std::endl;                       
	    std::cout << "dR1 " << dR << std::endl;   
	  } 
	}else if(!g1->hasConversionTracks() && g2->hasConversionTracks()){
	  if(conv->isConverted() == 1){
	    VtxtoSC.SetXYZ(g2->superCluster()->position().x() - conv->conversionVertex().x(), 
			   g2->superCluster()->position().y() - conv->conversionVertex().y(), 
			   g2->superCluster()->position().z() - conv->conversionVertex().z());

	    RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
	    dR = VtxtoSC.DeltaR(RefPairMo);               
            
	    
	    std::cout << "g2 conv track?" << "  " << g1->hasConversionTracks() << "  " << "g2 conv track?" << g2->hasConversionTracks() << "g1,g2->pt " <<g1->pt()<<" "<<g2->pt()<<std::endl;                       
	    std::cout << "dR2 " << dR << std::endl;   
	  }
	}else if(g1->hasConversionTracks() && g2->hasConversionTracks()){
	  if(conv->isConverted() == 1){
	    RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),
			     conv->refittedPairMomentum().y(),
			     conv->refittedPairMomentum().z());
	    
	    VtxtoSCPho1.SetXYZ(g1->superCluster()->position().x() - conv->conversionVertex().x(), 
			       g1->superCluster()->position().y() - conv->conversionVertex().y(), 
			       g1->superCluster()->position().z() - conv->conversionVertex().z());
	    dRPho1 = VtxtoSCPho1.DeltaR(RefPairMo);               
	    VtxtoSCPho2.SetXYZ(g2->superCluster()->position().x() - conv->conversionVertex().x(), 
			       g2->superCluster()->position().y() - conv->conversionVertex().y(), 
			       g2->superCluster()->position().z() - conv->conversionVertex().z());
	    
	    dRPho2 = VtxtoSCPho2.DeltaR(RefPairMo);               
	    
	    std::cout << "g1 conv track?" << "  " << g1->hasConversionTracks() << "  " << "g2 conv track?" << g2->hasConversionTracks() << "g1,g2->pt " <<g1->pt()<<" "<<g2->pt()<<std::endl;
	    std::cout << "dRPho1 "<< dRPho1 << std::endl;   
	    std::cout << "dRPho2 "<< dRPho2 << std::endl;   
            
	  }
	}
      }
      /*
	for (unsigned int i = 0 ; i < vtxs.size() ; i++) {
	edm::Ptr<reco::Vertex> vtx = vtxs[i];
	std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
	for (unsigned int j = 0 ; j < vertexCandidateMap.at(vtx).size() ; j++) {
	  edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMap.at(vtx)[j];
	  std::cout << " Candidate " << j << " in vertex " << i << " has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
	  tk.SetXYZ(cand->px(),cand->py(),cand->pz());  
	  tkPlane = tk.XYvector();
	  sumpt += tkPlane.Mod(); 
	  sumpt2 += tkPlane.Mod2();
	  ptbal -= tkPlane * diPhoXY.Unit(); 
	}
	ptasym = (sumpt - diPhoXY.Mod())/(sumpt+diPhoXY.Mod());
	}
      */
    }

    return vtxs[0];
  }

}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
