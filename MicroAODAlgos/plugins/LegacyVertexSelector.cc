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
#include "TLorentzVector.h"
namespace flashgg {
  
  class LegacyVertexSelector : public VertexSelectorBase {
  

 public:
    LegacyVertexSelector(const edm::ParameterSet& conf) :
      VertexSelectorBase(conf) {}
    edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&,
<<<<<<< HEAD
                                  const VertexCandidateMap& vertexCandidateMap,
				  const edm::PtrVector<reco::Conversion>&,
				  const math::XYZPoint&) const override;
    
    double vtxZFromConvOnly   (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
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
  TVector3 Photon1Dir;
  TVector3 Photon1Dir_uv;
  TLorentzVector p14;
  TVector3 Photon2Dir;
  TVector3 Photon2Dir_uv;
  TLorentzVector p24;
  
  double dr1 = 0;
  double dr2 = 0;
  double sumpt = 0;
  double sumpt2_out = 0; 
  double sumpt2_in = 0; 
  double ptbal = 0; 
  double ptasym = 0;
  double dR = 0;
  double dRPho1 = 0;
  double dRPho2 = 0;
  double dRexclude = 0.05;
  
  double LegacyVertexSelector::vtxZFromConvOnly(const edm::Ptr<flashgg::Photon>& g,const edm::Ptr<reco:: Conversion> & conversion,const math::XYZPoint & beamSpot) const{

    double r=sqrt(conversion->refittedPairMomentum().perp2());
    double dz = (conversion->conversionVertex().z()-beamSpot.z()) 
      - 
      ((conversion->conversionVertex().x()-beamSpot.x())*conversion->refittedPair4Momentum().x()+(conversion->conversionVertex().y()-beamSpot.y())*conversion->refittedPair4Momentum().y())/r * conversion->refittedPair4Momentum().z()/r;
    return dz + beamSpot.z();
  }
  
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
						      const edm::PtrVector<reco::Conversion>& conversionsVector,
						      const math::XYZPoint & beamSpot) const {

    std::cout<<"beamSpot.z() ="<<beamSpot.z()<<std::endl;
    
    if(conversionsVector.size()>0){
      if(g1->hasConversionTracks()){
	int IndexMatchedConversionLeadPhoton = IndexMatchedConversion(g1,conversionsVector);
	if(IndexMatchedConversionLeadPhoton!=-1){
<<<<<<< HEAD
	  //std::cout<<"dz Lead Photon"<<vtxZFromConvOnly(g1,conversionsVector[IndexMatchedConversionLeadPhoton])<<std::endl;
=======
	  std::cout<<"dz Lead Photon"<<vtxZFromConvOnly(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
>>>>>>> 943f927e416417a6af9458f629bc0e7954edf68d
	}
      }
      if(g2->hasConversionTracks()){
	int IndexMatchedConversionTrailPhoton = IndexMatchedConversion(g2,conversionsVector);
	if(IndexMatchedConversionTrailPhoton!=-1){
<<<<<<< HEAD
	  //std::cout<<"dz Lead Photon"<<vtxZFromConvOnly(g2,conversionsVector[IndexMatchedConversionTrailPhoton])<<std::endl;
=======
	  std::cout<<"dz Lead Photon"<<vtxZFromConvOnly(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
>>>>>>> 943f927e416417a6af9458f629bc0e7954edf68d
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
=======
                                  const VertexCandidateMap& vertexCandidateMap,const edm::PtrVector<reco::Conversion>&) const override;
//    edm::Ptr<reco::Vertex> EstZconv(const)


 
  private:
    unsigned int _whichVertex;
     };
   

    TVector3 diPho;  
    TVector3 tk;
    TVector2 tkPlane;
    TVector2 diPhoPlane;
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
						      const VertexCandidateMap& vertexCandidateMap,const edm::PtrVector<reco::Conversion>& convs) const {
    std::cout<<"Running the LegacyVertexSelector"<<std::endl;

    // Just an example for how to read the vertex-candidate association -- doesn't do anything (yet)
    //bool verbose = false;
    //if (verbose) {
       diPho.SetXYZ(g1->px()+g2->px(),g1->py()+g2->py(),g1->pz()+g2->pz());
       diPhoPlane = diPho.XYvector();
      
      //Geometrical matching of conversion with corresponding photon, dR < ?
      for (unsigned int i=0; i<convs.size();i++){
        edm::Ptr<reco::Conversion> conv = convs[i]; 
       
         if (g1->hasConversionTracks() && !g2->hasConversionTracks()){      
               if(conv->isConverted() == 1){
                   VtxtoSC.SetXYZ(g1->superCluster()->position().x() - conv->conversionVertex().x(), g1->superCluster()->position().y() - conv->conversionVertex().y(), g1->superCluster()->position().z() - conv->conversionVertex().z());
                     RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
                     dR = VtxtoSC.DeltaR(RefPairMo); 
                  
                      std::cout << "g1 conv track?" << "  " << g1->hasConversionTracks() << "  " << "g2 conv track?" << g2->hasConversionTracks() << std::endl;                       
                      std::cout << "dR" << "  " << "=" << "  " << dR << std::endl;   
                               } 
                           }
                 else if(!g1->hasConversionTracks() && g2->hasConversionTracks()){
                         if(conv->isConverted() == 1){
                   VtxtoSC.SetXYZ(g2->superCluster()->position().x() - conv->conversionVertex().x(), g2->superCluster()->position().y() - conv->conversionVertex().y(), g2->superCluster()->position().z() - conv->conversionVertex().z());
                     RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
                     dR = VtxtoSC.DeltaR(RefPairMo);               
                           

                      std::cout << "g1 conv track?" << "  " << g1->hasConversionTracks() << "  " << "g2 conv track?" << g2->hasConversionTracks() << std::endl;                       
                      std::cout << "dR2" << "  " << "=" << "  " << dR << std::endl;   
                               }
                           }
                       
                 else if(g1->hasConversionTracks() && g2->hasConversionTracks()){
                         if(conv->isConverted() == 1){
                      RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
      		       
                   VtxtoSCPho1.SetXYZ(g1->superCluster()->position().x() - conv->conversionVertex().x(), g1->superCluster()->position().y() - conv->conversionVertex().y(), g1->superCluster()->position().z() - conv->conversionVertex().z());
                     dRPho1 = VtxtoSCPho1.DeltaR(RefPairMo);               
                   VtxtoSCPho2.SetXYZ(g2->superCluster()->position().x() - conv->conversionVertex().x(), g2->superCluster()->position().y() - conv->conversionVertex().y(), g2->superCluster()->position().z() - conv->conversionVertex().z());
                     dRPho2 = VtxtoSCPho2.DeltaR(RefPairMo);               
      

                      std::cout << "g1 conv track?" << "  " << g1->hasConversionTracks() << "  " << "g2 conv track?" << g2->hasConversionTracks() << std::endl;                       
                      std::cout << "dRPho1" << "  " << "=" << "  " << dRPho1 << std::endl;   

                      std::cout << "dRPho2" << "  " << "=" << "  " << dRPho2 << std::endl;   
                               
                                          }
                                     }

                              }




     
    for (unsigned int i = 0 ; i < vtxs.size() ; i++) {
      edm::Ptr<reco::Vertex> vtx = vtxs[i];
//	std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
      //Photon1Dir is the direction between the vertex and the supercluster
      Photon1Dir.SetXYZ(g1->superCluster()->position().x() - vtx->position().x(),g1->superCluster()->position().y() - vtx->position().y(),g1->superCluster()->position().z() - vtx->position().z()); 
	Photon2Dir.SetXYZ(g2->superCluster()->position().x() - vtx->position().x(),g2->superCluster()->position().y() - vtx->position().y(),g2->superCluster()->position().z() - vtx->position().z()); 
      //The unit vector is then multipled by the enrgy to have the correct relation p4.p4 = 0 for photons	
      Photon1Dir_uv = Photon1Dir.Unit()*g1->superCluster()->rawEnergy();
      Photon2Dir_uv = Photon2Dir.Unit()*g2->superCluster()->rawEnergy();
      //the photon 4 momentum wrt a given vertex is built
      p14.SetPxPyPzE(Photon1Dir_uv.x(),Photon1Dir_uv.y(),Photon1Dir_uv.z(),g1->superCluster()->rawEnergy()); 
      p24.SetPxPyPzE(Photon2Dir_uv.x(),Photon2Dir_uv.y(),Photon2Dir_uv.z(),g2->superCluster()->rawEnergy()); 
      if(vertexCandidateMap.count(vtx) == 0) continue;
      for (unsigned int j = 0 ; j < vertexCandidateMap.at(vtx).size() ; j++) {
        edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMap.at(vtx)[j];
        tk.SetXYZ(cand->px(),cand->py(),cand->pz());  
        tkPlane = tk.XYvector();
        sumpt += tkPlane.Mod();
        double dr1 = tk.DeltaR(p14.Vect());
        double dr2 = tk.DeltaR(p24.Vect());
        //std::cout << "dr1  " << dr1 << std::endl;
        if(dr1 < dRexclude || dr2 < dRexclude){
          sumpt2_in += tkPlane.Mod2();
          continue;
        }
	//variables que no se suman si continue
	sumpt2_out+=tkPlane.Mod2();
        ptbal -= tkPlane * diPhoXY.Unit();
       }
      std::cout << "sumpt2_out " << sumpt2_out << std::endl; 
      std::cout << "sumpt2_in  " << sumpt2_in << std::endl;
      //std::cout << " Candidate " << j << " in vertex " << i << " has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
      ptasym = (sumpt - diPhoXY.Mod())/(sumpt+diPhoXY.Mod());
    }
    
    return vtxs[0];
  }  
  
}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
