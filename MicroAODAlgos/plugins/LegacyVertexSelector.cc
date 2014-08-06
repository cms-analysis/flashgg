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

    //  edm::Ptr<reco::Conversion> MatchedConversion(const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Conversion>&) const;
    double vtxZFromConvOnly   (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&) const;
    int IndexMatchedConversion(const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Conversion>&) const;
    
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
  
  double LegacyVertexSelector::vtxZFromConvOnly(const edm::Ptr<flashgg::Photon>& g,const edm::Ptr<reco:: Conversion> & conversion) const{
    TVector3 beamSpot; //To be replaced by g->beamSpot()!!!!
    beamSpot.SetXYZ(0,0,0);
    //std::cout<<(conversion->conversionVertex().XYvector()-beamSpot.XYvector()).dot(conversion->refittedPair4Momentum().XYvector())<<std::endl;
    //    double r=sqrt(conversion->refittedPairMomentum().x()*conversion->refittedPairMomentum().x()+conversion->refittedPairMomentum().x()*conversion->refittedPairMomentum().y()*conversion->refittedPairMomentum().x()*conversion->refittedPairMomentum().y());

    double r=sqrt(conversion->refittedPairMomentum().perp2());
    double dz = (conversion->conversionVertex().z()-beamSpot.z()) 
      - 
      ((conversion->conversionVertex().x()-beamSpot.x())*conversion->refittedPair4Momentum().x()+(conversion->conversionVertex().y()-beamSpot.y())*conversion->refittedPair4Momentum().y())/r * conversion->refittedPair4Momentum().z()/r;
    return dz + beamSpot.z();
  }
  
  //edm::Ptr<reco::Conversion> LegacyVertexSelector::MatchedConversion(const edm::Ptr<flashgg::Photon>& g,const edm::PtrVector<reco::Conversion>& conversionsVector) const {
  int LegacyVertexSelector::IndexMatchedConversion(const edm::Ptr<flashgg::Photon>& g,const edm::PtrVector<reco::Conversion>& conversionsVector) const{
    double mindR = 999;
    
    assert(g->hasConversionTracks()); //The photon has to have conversion tracks!
    assert(conversionsVector.size()>0); //The photon has to have conversion tracks!

    int selected_conversion_index = -1;
    
    if(g->hasConversionTracks()){
      for (unsigned int i=0; i<conversionsVector.size();i++){
	edm::Ptr<reco::Conversion> conv = conversionsVector[i];
	if(!conv->isConverted()) continue;
	if(conv->refittedPair4Momentum().pt()<10.) continue;
	if(TMath::Prob(conv->conversionVertex().chi2(),conv->conversionVertex().ndof()) < 1e-6 ) continue;
	
	VtxtoSC.SetXYZ(g->superCluster()->position().x() - conv->conversionVertex().x(), 
		       g->superCluster()->position().y() - conv->conversionVertex().y(), 
		       g->superCluster()->position().z() - conv->conversionVertex().z());
	RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
	dR = VtxtoSC.DeltaR(RefPairMo); 
	if(dR<mindR){
	  mindR=dR;
	  selected_conversion_index=i;
	}
      }
    }  
    //std::cout<<"selected index "<<selected_conversion_index<<" size of converted vertices vector "<<conversionsVector.size()<<std::endl;
    return selected_conversion_index;
  }
  
  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
						      const VertexCandidateMap& vertexCandidateMap,
						      const edm::PtrVector<reco::Conversion>& conversionsVector) const {
    if(conversionsVector.size()>0){
      if(g1->hasConversionTracks()){
	int IndexMatchedConversionLeadPhoton = IndexMatchedConversion(g1,conversionsVector);
	if(IndexMatchedConversionLeadPhoton!=-1){
	  std::cout<<"dz Lead Photon"<<vtxZFromConvOnly(g1,conversionsVector[IndexMatchedConversionLeadPhoton])<<std::endl;
	}
      }
      if(g2->hasConversionTracks()){
	int IndexMatchedConversionTrailPhoton = IndexMatchedConversion(g2,conversionsVector);
	if(IndexMatchedConversionTrailPhoton!=-1){
	  std::cout<<"dz Lead Photon"<<vtxZFromConvOnly(g2,conversionsVector[IndexMatchedConversionTrailPhoton])<<std::endl;
	}
      }
    }

    //    int indexselected_vertex=-1;
    //for (unsigned int i = 0 ; i < vtxs.size() ; i++) {
    //  edm::Ptr<reco::Vertex> vtx = vtxs[i];
    //  std::cout<<"dZ vertex = "vtxs[i]
    //}


    
    //------------------------------------------

    bool verbose = false;
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
    }

    std::cout<< "Vertices in this event: "<<vtxs.size()<< std::endl;
    for (unsigned int index_vertex = 0 ; index_vertex < vtxs.size() ; index_vertex++) {
      edm::Ptr<reco::Vertex> vtx = vtxs[index_vertex];
      if(vertexCandidateMap.count(vtx)==0)continue;
      std::cout<< "\t On vertex " << index_vertex << " with " << vertexCandidateMap.at(vtx).size() <<" Tracks"<< std::endl;
      for (unsigned int index_track = 0 ; index_track < vertexCandidateMap.at(vtx).size() ; index_track++) {
	edm::Ptr<pat::PackedCandidate> track = vertexCandidateMap.at(vtx)[index_track];
	std::cout <<"\t \t track # "<<index_track<<"has dz (w.r.t that vertex) of="<< track->dz(vtx->position())<< std::endl;
	tk.SetXYZ(track->px(),track->py(),track->pz());  
	tkPlane = tk.XYvector();
	sumpt += tkPlane.Mod(); 
	sumpt2 += tkPlane.Mod2();
	ptbal -= tkPlane * diPhoXY.Unit(); 
      }
      ptasym = (sumpt - diPhoXY.Mod())/(sumpt+diPhoXY.Mod());
    }
    
    return vtxs[0];
  }  
  
}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
