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
				 const math::XYZPoint&,
				 const std::map<std::string,double>&
				 ) const override;

    double vtxZFromConvOnly         (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxZFromConvSuperCluster (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxZFromConv             (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxdZFromConv            (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const std::map<std::string,double>&) const;

    double getZFromConvPair(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,
			    const int ,const int,
			    const edm::PtrVector<reco::Conversion>&,
			    const math::XYZPoint &,
			    const std::map<std::string,double>&) const;
   
    
    double getsZFromConvPair(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,
			     const int ,const int,
			     const edm::PtrVector<reco::Conversion>&,
			     const std::map<std::string,double>&) const;
    
    int IndexMatchedConversion(const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Conversion>&) const;
    
  private:
    unsigned int _whichVertex; 
  };
  TVector3 diPho;  
  TVector3 tk;
  TVector2 tkXY;
  TVector2 sumpt;
  TVector3 VtxtoSC;
  TVector3 VtxtoSCPho1;
  TVector3 VtxtoSCPho2;
  TVector3 RefPairMo;
  TVector3 Photon1Dir;
  TVector3 Photon1Dir_uv;
  TVector3 Photon2Dir;
  TVector3 Photon2Dir_uv;
  TLorentzVector p14;
  TLorentzVector p24;
  
  double dr1 = 0;
  double dr2 = 0;
  double sumpt2_out = 0; 
  double sumpt2_in = 0; 
  double ptbal = 0; 
  double ptasym = 0;
  double dR = 0;
  double dRPho1 = 0;
  double dRPho2 = 0;
  double zconv=0;
  double szconv=0;

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
  

  double LegacyVertexSelector::vtxZFromConv (const edm::Ptr<flashgg::Photon>& pho,const edm::Ptr<reco::Conversion> & conversion,const math::XYZPoint & beamSpot) const{
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

  double LegacyVertexSelector::vtxdZFromConv (const edm::Ptr<flashgg::Photon>& pho, const edm::Ptr<reco::Conversion> & conversion,const std::map<std::string,double> & param) const{
    // method 0 is combined (default)
    // method 1 is conversion only
    // method 2 is supercluster only
    // attribute the error depending on the tracker region
    double dz=-99999;
    int method=0;//to be assigned dynamically
    double perp = sqrt(conversion->conversionVertex().x()*conversion->conversionVertex().x()+conversion->conversionVertex().y()*conversion->conversionVertex().y());

    if (conversion->nTracks()==2) {
      if ( pho->eta()<1.5 ) { // barrel
	if ( perp <=15 ) {
	  if (method==0) dz=param.at("sigma1Pix");
	  if (method==1) dz=param.at("sigma1Pix");
	  if (method==2) dz=param.at("sigma2Pix");
	} else if ( perp > 15 && perp <=60 ) {
	  if (method==0) dz=param.at("sigma2Tib");
	  if (method==1) dz=param.at("sigma1Tib");
	  if (method==2) dz=param.at("sigma2Tib");
	} else {
	  if (method==0) dz=param.at("sigma2Tob");
	  if (method==1) dz=param.at("sigma1Tob");
	  if (method==2) dz=param.at("sigma2Tob");
	}
	
      } else { // endcap
	
	if ( fabs(conversion->conversionVertex().z() ) <=50 ) {
	  if (method==0) dz=param.at("sigma1PixFwd");
	  if (method==1) dz=param.at("sigma1PixFwd");
	  if (method==2) dz=param.at("sigma2PixFwd");
	} else if ( fabs(conversion->conversionVertex().z() ) > 50 && fabs(conversion->conversionVertex().z()) <= 100 ) {
	  if (method==0) dz=param.at("sigma1Tid");
	  if (method==1) dz=param.at("sigma1Tid");
	  if (method==2) dz=param.at("sigma2Tid");
	} else {
	  if (method==0) dz=param.at("sigma2Tec");
	  if (method==1) dz=param.at("sigma1Tec");
	  if (method==2) dz=param.at("sigma2Tec");
	}
      }
    } else if (conversion->nTracks()==1) {
      if ( pho->eta() <1.5 ) { // barrel
	if ( perp <=15 ) {
	  if (method==0) dz=param.at("singlelegsigma1Pix");
	  if (method==1) dz=param.at("singlelegsigma1Pix");
	  if (method==2) dz=param.at("singlelegsigma2Pix");
	} else if ( perp > 15 && perp <=60 ) {
	  if (method==0) dz=param.at("singlelegsigma2Tib");
	  if (method==1) dz=param.at("singlelegsigma1Tib");
	  if (method==2) dz=param.at("singlelegsigma2Tib");
	} else {
	  if (method==0) dz=param.at("singlelegsigma2Tob");
	  if (method==1) dz=param.at("singlelegsigma1Tob");
	  if (method==2) dz=param.at("singlelegsigma2Tob");
	}
	
      } else { // endcap
	
	if ( fabs(conversion->conversionVertex().z() ) <=50 ) {
	  if (method==0) dz=param.at("singlelegsigma1PixFwd");
	  if (method==1) dz=param.at("singlelegsigma1PixFwd");
	  if (method==2) dz=param.at("singlelegsigma2PixFwd");
	} else if ( fabs(conversion->conversionVertex().z() ) > 50 && fabs(conversion->conversionVertex().z()) <= 100 ) {
	  if (method==0) dz=param.at("singlelegsigma1Tid");
	  if (method==1) dz=param.at("singlelegsigma1Tid");
	  if (method==2) dz=param.at("singlelegsigma2Tid");
	} else {
	  if (method==0) dz=param.at("singlelegsigma2Tec");
	  if (method==1) dz=param.at("singlelegsigma1Tec");
	  if (method==2) dz=param.at("singlelegsigma2Tec");
	}
      }
    }
    return dz;
  }
  
  double LegacyVertexSelector::getZFromConvPair(const edm::Ptr<flashgg::Photon>& p1,
						const edm::Ptr<flashgg::Photon>& p2,
						const int index_conversionLead,
						const int index_conversionTrail,
						const edm::PtrVector<reco::Conversion>& conversionsVector,
						const math::XYZPoint & beamSpot,
						const std::map<std::string,double>& param) const{
    
    double zconv=0;

    if ( index_conversionLead!=-1  && index_conversionTrail==-1 ){ //Warning could be also the method g->hasConversionTracks() OR vtx->isConverted?
      zconv  = vtxZFromConv(p1,conversionsVector[index_conversionLead],beamSpot);
    }
    if ( index_conversionTrail==-1 && index_conversionTrail!=-1 ){
      zconv  = vtxZFromConv (p2,conversionsVector[index_conversionTrail],beamSpot);
    }
    
    if (index_conversionLead!=-1 && index_conversionTrail!=-1){
      float z1  = vtxZFromConv (p1,conversionsVector[index_conversionLead],beamSpot);
      float sz1 = vtxdZFromConv(p1,conversionsVector[index_conversionLead],param);
      
      float z2  = vtxZFromConv (p2,conversionsVector[index_conversionTrail],beamSpot);
      float sz2 = vtxdZFromConv(p2,conversionsVector[index_conversionTrail],param);
      
      zconv  = (z1/sz1/sz1 + z2/sz2/sz2)/(1./sz1/sz1 + 1./sz2/sz2 );  // weighted average
    }
    return zconv;
  }

  
  double LegacyVertexSelector::getsZFromConvPair(const edm::Ptr<flashgg::Photon>& p1,
						 const edm::Ptr<flashgg::Photon>& p2,
						 int index_conversionLead,
						 int index_conversionTrail,
						 const edm::PtrVector<reco::Conversion>& conversionsVector,
						 const std::map<std::string,double> & param) const{
    
    double szconv=0;
    
    if ( index_conversionLead!=-1  && index_conversionTrail==-1 ){ //Warning could be also the method g->hasConversionTracks()?
      szconv = vtxdZFromConv(p1,conversionsVector[index_conversionLead],param);
    }
    if ( index_conversionTrail==-1 && index_conversionTrail!=-1 ){
      szconv = vtxdZFromConv(p2,conversionsVector[index_conversionTrail],param);
    }
    
    if (index_conversionLead!=-1 && index_conversionTrail!=-1){
      float sz1 = vtxdZFromConv(p1,conversionsVector[index_conversionLead],param);
      float sz2 = vtxdZFromConv(p2,conversionsVector[index_conversionTrail],param);
      
      szconv = sqrt( 1./(1./sz1/sz1 + 1./sz2/sz2)) ;
    }
    return szconv;
  }
 

  int LegacyVertexSelector::IndexMatchedConversion(const edm::Ptr<flashgg::Photon>& g,const edm::PtrVector<reco::Conversion>& conversionsVector) const{
    double mindR = 999;
    
    assert(g->hasConversionTracks()); //The photon has to have conversion tracks!
    assert(conversionsVector.size()>0); //The photon has to have conversion tracks!

    int selected_conversion_index = -1; //returned value if no match was found
    
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
    return selected_conversion_index; //-1 if no match was found
  }
  
  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
						      const VertexCandidateMap& vertexCandidateMap,
						      const edm::PtrVector<reco::Conversion>& conversionsVector,
						      const math::XYZPoint & beamSpot,
						      const std::map<std::string,double> & param) const {

    int IndexMatchedConversionLeadPhoton=-1;
    int IndexMatchedConversionTrailPhoton=-1;

    if(conversionsVector.size()>0){
      if(g1->hasConversionTracks()){
	IndexMatchedConversionLeadPhoton = IndexMatchedConversion(g1,conversionsVector);
	if(IndexMatchedConversionLeadPhoton!=-1){
	  /*
	    std::cout<<"dz Lead Photon from vtxZFromConvOnly         "<<vtxZFromConvOnly(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
	    std::cout<<"dz Lead Photon from vtxZFromConvSuperCluster "<<vtxZFromConvSuperCluster(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
	    std::cout<<"dz Lead Photon from vtxZFromConv             "<<vtxZFromConv(g1,conversionsVector[IndexMatchedConversionLeadPhoton],beamSpot)<<std::endl;
	    std::cout<<"szconv Lead Photon from vtxdZFromConv             "<<vtxdZFromConv(g1,conversionsVector[IndexMatchedConversionLeadPhoton],param)<<std::endl;
	  */
	  
	}
      }
      if(g2->hasConversionTracks()){
	IndexMatchedConversionTrailPhoton = IndexMatchedConversion(g2,conversionsVector);
	if(IndexMatchedConversionTrailPhoton!=-1){
	  /*
	    std::cout<<"dz Trail Photon from vtxZFromConvOnly         "<<vtxZFromConvOnly(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
	    std::cout<<"dz Trail Photon from vtxZFromConvSuperCluster "<<vtxZFromConvSuperCluster(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
	    std::cout<<"dz Trail Photon from vtxZFromConv             "<<vtxZFromConv(g2,conversionsVector[IndexMatchedConversionTrailPhoton],beamSpot)<<std::endl;
	    std::cout<<"szconv Trail Photon from vtxdZFromConv             "<<vtxdZFromConv(g2,conversionsVector[IndexMatchedConversionTrailPhoton],param)<<std::endl;
	  */
	}
      }
    }
    
    //------------------------------------------
    
    unsigned int vertex_index,track_index;

    for (vertex_index = 0 ; vertex_index < vtxs.size() ; vertex_index++) {
      edm::Ptr<reco::Vertex> vtx = vtxs[vertex_index];
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
      
      sumpt2_in = 0;
      sumpt2_out = 0;
      ptbal = 0;
      ptasym = 0;
      sumpt.Set(0.,0.);

      if(vertexCandidateMap.count(vtx) == 0) continue;
      for(track_index = 0 ; track_index < vertexCandidateMap.at(vtx).size() ; track_index++) {
        edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMap.at(vtx)[track_index];
        tk.SetXYZ(cand->px(),cand->py(),cand->pz()); 
        tkXY = tk.XYvector();
        sumpt += tkXY;
        dr1 = tk.DeltaR(p14.Vect());
        dr2 = tk.DeltaR(p24.Vect());
	if(dr1 < param.at("dRexclude") || dr2 < param.at("dRexclude")){
          sumpt2_in+=tkXY.Mod2();
          continue;
        }
	sumpt2_out+=tkXY.Mod2();
        ptbal-=tkXY*(p14+p24).Vect().XYvector().Unit();
      }

      ptasym = (sumpt.Mod() - (p14+p24).Vect().XYvector().Mod())/(sumpt.Mod() + (p14+p24).Vect().XYvector().Mod());
      zconv=getZFromConvPair(g1,g2,IndexMatchedConversionLeadPhoton,IndexMatchedConversionTrailPhoton,conversionsVector,beamSpot,param);
      szconv=getsZFromConvPair(g1,g2,IndexMatchedConversionLeadPhoton,IndexMatchedConversionTrailPhoton,conversionsVector,param);
      
      //std::cout<<"zconv+/-szconv: "<<zconv<<"+/-"<<szconv<<std::endl;

      if(zconv==0 && szconv==0.0){
	std::cout<<"vertex:"<<vertex_index<<std::endl;
	std::cout<<"plot_unconverted_case_MVA |sumpt| "<<sumpt.Mod()<<" sumpt2_out "<<sumpt2_out<<" ptbal "<<ptbal<<" ptasym "<<ptasym<<" pull_conv 0 0"<<std::endl;
      }else{
	double pull_conv = fabs(vtx->position().z()-zconv)/szconv;
	std::cout<<"vertex:"<<vertex_index<<std::endl;
	std::cout<<"plot_converted_case_MVA   |sumpt| "<<sumpt.Mod()<<" sumpt2_out "<<sumpt2_out<<" ptbal "<<ptbal<<" ptasym "<<ptasym<<" pull_conv "<<pull_conv<<" 0"<<std::endl;
      }
    } 
    return vtxs[0];
  }  
  
}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
