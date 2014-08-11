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
                                  const VertexCandidateMap& vertexCandidateMap,
				  const edm::PtrVector<reco::Conversion>&,
				  const math::XYZPoint&) const override;
    
    double vtxZFromConvOnly         (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxZFromConvSuperCluster (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxZFromConv             (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;

    
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
  
  double LegacyVertexSelector::vtxZFromConvOnly(const edm::Ptr<flashgg::Photon>& pho,const edm::Ptr<reco:: Conversion> & conversion,const math::XYZPoint & beamSpot) const{

    double r=sqrt(conversion->refittedPairMomentum().perp2());
    double dz = (conversion->conversionVertex().z()-beamSpot.z()) 
      - 
      ((conversion->conversionVertex().x()-beamSpot.x())*conversion->refittedPair4Momentum().x()+(conversion->conversionVertex().y()-beamSpot.y())*conversion->refittedPair4Momentum().y())/r * conversion->refittedPair4Momentum().z()/r;
    return dz + beamSpot.z();
  }

  double LegacyVertexSelector::vtxZFromConvSuperCluster (const edm::Ptr<flashgg::Photon>& pho,const edm::Ptr<reco:: Conversion> & conversion,const math::XYZPoint & beamSpot) const{

    // get the z from conversion plus SuperCluster
    double deltaX1 =  pho->caloPosition().x()- conversion->conversionVertex().x();
    double deltaY1 =  pho->caloPosition().y()- conversion->conversionVertex().y();
    double deltaZ1 =  pho->caloPosition().z()- conversion->conversionVertex().z();
    double R1 = sqrt(deltaX1*deltaX1+deltaY1*deltaY1);
    double tantheta = R1/deltaZ1;
  
    double deltaX2 = conversion->conversionVertex().x()-beamSpot.x();
    double deltaY2 = conversion->conversionVertex().y()-beamSpot.y();
    double R2 = sqrt(deltaX2*deltaX2+deltaY2*deltaY2);
    double deltaZ2 = R2/tantheta;
    double higgsZ =  pho->caloPosition().z()-deltaZ1-deltaZ2;
    return higgsZ;
  }
  

  double LegacyVertexSelector::vtxZFromConv (const edm::Ptr<flashgg::Photon>& pho,const edm::Ptr<reco:: Conversion> & conversion,const math::XYZPoint & beamSpot) const{
    // method 0 is combined (default)
    // method 1 is conversion only
    // method 2 is supercluster only

    int method=0;

    double ReturnValue = 0;

    double perp = sqrt(conversion->conversionVertex().x()*conversion->conversionVertex().x()+conversion->conversionVertex().y()*conversion->conversionVertex().y());

    //Mixed Method Conversion Vertex
    if (method==0) {
      if(pho->eta()<1.5) { //FIXME!!!!!
	if (perp<=15.0) {
	  //Pixel Barrel
	  ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);
	} else if  (perp>15 && perp<=60.0) {
	  //Tracker Inner Barrel
	  ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
	} else {
	  //Tracker Outer Barrel
	  ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
	}
      } else {
	if (fabs(conversion->conversionVertex().z())<=50.0) {
	  //Pixel Forward
	  ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);
	}  else if (fabs(conversion->conversionVertex().z())>50.0 && fabs(conversion->conversionVertex().z())<=100.0) {
	  //Tracker Inner Disk
	  ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);
	}  else {
	  //Track EndCap
	  ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
	}
      }
    }
    if (method==1) ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
    if (method==2) ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);

    return ReturnValue;

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

    int IndexMatchedConversionLeadPhoton=-1;
    int IndexMatchedConversionTrailPhoton=-1;

    
    if(conversionsVector.size()>0){
      if(g1->hasConversionTracks()){
	IndexMatchedConversionLeadPhoton = IndexMatchedConversion(g1,conversionsVector);
	if(IndexMatchedConversionLeadPhoton!=-1){
	  std::cout<<"dz Lead Photon from vtxZFromConvOnly         "<<vtxZFromConvOnly(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
	  std::cout<<"dz Lead Photon from vtxZFromConvSuperCluster "<<vtxZFromConvSuperCluster(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
	  std::cout<<"dz Lead Photon from vtxZFromConv             "<<vtxZFromConv(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
	}
      }
      if(g2->hasConversionTracks()){
	IndexMatchedConversionTrailPhoton = IndexMatchedConversion(g2,conversionsVector);
	if(IndexMatchedConversionTrailPhoton!=-1){
	  std::cout<<"dz Trail Photon from vtxZFromConvOnly         "<<vtxZFromConvOnly(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
	  std::cout<<"dz Trail Photon from vtxZFromConvSuperCluster "<<vtxZFromConvSuperCluster(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
	  std::cout<<"dz Trail Photon from vtxZFromConv             "<<vtxZFromConv(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
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
 
    for (unsigned int i = 0 ; i < vtxs.size() ; i++) {
      edm::Ptr<reco::Vertex> vtx = vtxs[i];
      //std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
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
	//skip if ouside cone
	sumpt2_out+=tkPlane.Mod2();
        ptbal -= tkPlane * diPhoXY.Unit();
      }
      //std::cout << "sumpt2_out " << sumpt2_out << std::endl; 
      //std::cout << "sumpt2_in  " << sumpt2_in << std::endl;
      //std::cout << " Candidate " << j << " in vertex " << i << " has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
      ptasym = (sumpt - diPhoXY.Mod())/(sumpt+diPhoXY.Mod());

      double sigmaz=0.01; //FIXME

      double pull_conv_lead   = fabs(vtx->position().z()- vtxZFromConv(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot))/sigmaz;
      double pull_conv_trail  = fabs(vtx->position().z()- vtxZFromConv(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot))/sigmaz;

      std::cout<<"converted_case_MVA_variables sumpt "<<sumpt<<" sumpt2_out "<<sumpt2_out<<" ptbal "<<ptbal
	       <<" pull_conv_lead "<<pull_conv_lead<<" pull_conv_trail "<<pull_conv_trail
	       <<std::endl;
    } 
    
    return vtxs[0];
  }  
  
}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
