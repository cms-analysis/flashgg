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
#include "TMVA/Reader.h"

namespace flashgg {
  
 class LegacyVertexSelector : public VertexSelectorBase {
  
    
 public:
   LegacyVertexSelector(const edm::ParameterSet& );
   edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&,
				 const VertexCandidateMap& vertexCandidateMap,
				 const edm::PtrVector<reco::Conversion>&,
				 const math::XYZPoint&,
				 const std::map<std::string,double>&,
                                 const float &
				 ) override;

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

   void Initialize();
    
  private:

    edm::FileInPath vertexIdMVAweightfile_;

 protected: 
   TMVA::Reader * VertexIdMva_;
   bool initialized_;
   float logsumpt2_;
   float ptbal_;
   float ptasym_;
   float nConv_;
   float pull_conv_;

   TMVA::Reader * VertexProbMva_; 
   float dipho_pt_;
   float nVert_;
   float MVA0_;
   float MVA1_;
   float dZ1_;
   float MVA2_;
   float dZ2_;

   TMVA::Reader * DiphotonMva_;
   float sigmarv_;
   float sigmawv_;
   float vtxprob_; 
   float leadptom_;
   float subleadptom_;
   float leadeta_;
   float subleadeta_;
   float CosPhi_;
   float leadmva_;
   float subleadmva_;

  };

  LegacyVertexSelector::LegacyVertexSelector(const edm::ParameterSet& iConfig) :
    VertexSelectorBase(iConfig) 
  {
    vertexIdMVAweightfile_ = iConfig.getParameter<edm::FileInPath>("vertexIdMVAweightfile");
    initialized_ = false;
  }

  void LegacyVertexSelector::Initialize() 
  {
    VertexIdMva_ = new TMVA::Reader("!Color:Silent");
    VertexIdMva_->AddVariable("ptbal",&ptbal_);
    VertexIdMva_->AddVariable("ptasym",&ptasym_);
    VertexIdMva_->AddVariable("logsumpt2",&logsumpt2_);
    VertexIdMva_->AddVariable("limPullToConv",&pull_conv_);
    VertexIdMva_->AddVariable("nConv",&nConv_);
    VertexIdMva_->BookMVA("BDT", vertexIdMVAweightfile_.fullPath());

    std::cout << " Booked MVA for LegacyVertexSelector from " << vertexIdMVAweightfile_.fullPath() << std::endl;

    VertexProbMva_ = new TMVA::Reader("!Color:Silent");
    VertexProbMva_->AddVariable("diphoPt0",&dipho_pt_); 
    VertexProbMva_->AddVariable("nVert",&nVert_); 
    VertexProbMva_->AddVariable("MVA0",&MVA0_); 
    VertexProbMva_->AddVariable("MVA1",&MVA1_); 
    VertexProbMva_->AddVariable("dZ1",&dZ1_); 
    VertexProbMva_->AddVariable("MVA2",&MVA2_); 
    VertexProbMva_->AddVariable("dZ2",&dZ2_); 
    VertexProbMva_->AddVariable("nConv",&nConv_); 
    VertexProbMva_->BookMVA("BDT","/afs/cern.ch/work/m/molmedon/public/Forflash/TMVAClassification_BDTvtxprob2012.weights.xml");

    DiphotonMva_ = new TMVA::Reader("!Color:Silent");
    DiphotonMva_->AddVariable("masserrsmeared/mass",&sigmarv_);
    DiphotonMva_->AddVariable("masserrsmearedwrongvtx/mass",&sigmawv_);
    DiphotonMva_->AddVariable("vtxprob",&vtxprob_);
    DiphotonMva_->AddVariable("ph1.pt/mass",&leadptom_);
    DiphotonMva_->AddVariable("ph2.pt/mass",&subleadptom_);
    DiphotonMva_->AddVariable("ph1.eta",&leadeta_);
    DiphotonMva_->AddVariable("ph2.eta",&subleadeta_);
    DiphotonMva_->AddVariable("TMath::Cos(ph1.phi-ph2.phi)",&CosPhi_);
    DiphotonMva_->AddVariable("ph1.idmva",&leadmva_);
    DiphotonMva_->AddVariable("ph2.idmva",&subleadmva_);
    DiphotonMva_->BookMVA("BDT","/afs/cern.ch/work/m/molmedon/public/Forflash/HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml");

    initialized_ = true;
  }

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
  TMVA::Reader * VertexProbMva_;
  TMVA::Reader * DiphotonMva_; 
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

  TLorentzVector diphoton_objects;
  std::vector<TLorentzVector> diphoton_objects_vector;

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
    int method=0;//to be assigned dynamically? FIXME!
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
    if(index_conversionLead!=-1  && index_conversionTrail==-1){ //Warning could be also the method g->hasConversionTracks() OR vtx->isConverted?
      zconv=vtxZFromConv(p1,conversionsVector[index_conversionLead],beamSpot);
    }
    if(index_conversionTrail==-1 && index_conversionTrail!=-1){
      zconv=vtxZFromConv (p2,conversionsVector[index_conversionTrail],beamSpot);
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
    return selected_conversion_index; //-1 if no match was found
  }
  
  edm::Ptr<reco::Vertex> LegacyVertexSelector::select(const edm::Ptr<flashgg::Photon>& g1,const edm::Ptr<flashgg::Photon>& g2,const edm::PtrVector<reco::Vertex>& vtxs,
						      const VertexCandidateMap& vertexCandidateMap,
						      const edm::PtrVector<reco::Conversion>& conversionsVector,
						      const math::XYZPoint & beamSpot,
						      const std::map<std::string,double> & param,
                                                      const float & beamsig 
                                                       ) {

    int IndexMatchedConversionLeadPhoton=-1;
    int IndexMatchedConversionTrailPhoton=-1;

    unsigned int vertex_index,track_index;

    unsigned int selected_vertex_index=0;
    unsigned int second_selected_vertex_index=0;
    unsigned int third_selected_vertex_index=0;
    float max_mva_value=-999;
    float second_max_mva_value=-999;
    float third_max_mva_value=-999;

    std::vector<TLorentzVector> diphoton_objects_vector;
    std::vector<float> mass_reswrongvtx_vector;
 
    if (!initialized_) {
      Initialize();
    }
	  
    for (vertex_index = 0 ; vertex_index < vtxs.size() ; vertex_index++) {
      edm::Ptr<reco::Vertex> vtx = vtxs[vertex_index];
      Photon1Dir.SetXYZ(g1->superCluster()->position().x() - vtx->position().x(),g1->superCluster()->position().y() - vtx->position().y(),g1->superCluster()->position().z() - vtx->position().z()); 
      Photon2Dir.SetXYZ(g2->superCluster()->position().x() - vtx->position().x(),g2->superCluster()->position().y() - vtx->position().y(),g2->superCluster()->position().z() - vtx->position().z()); 
      Photon1Dir_uv = Photon1Dir.Unit()*g1->superCluster()->rawEnergy();
      Photon2Dir_uv = Photon2Dir.Unit()*g2->superCluster()->rawEnergy();
      p14.SetPxPyPzE(Photon1Dir_uv.x(),Photon1Dir_uv.y(),Photon1Dir_uv.z(),g1->superCluster()->rawEnergy()); 
      p24.SetPxPyPzE(Photon2Dir_uv.x(),Photon2Dir_uv.y(),Photon2Dir_uv.z(),g2->superCluster()->rawEnergy()); 
     
      diphoton_objects = p14+p24;
 
      diphoton_objects_vector.push_back(diphoton_objects);  

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

     //resolution variables for diphoton mva//
     //

      float r1 = TMath::Sqrt(Photon1Dir.X()*Photon1Dir.X()+Photon1Dir.Y()*Photon1Dir.Y()+Photon1Dir.Z()*Photon1Dir.Z()); 
   
      float r2 = TMath::Sqrt(Photon2Dir.X()*Photon2Dir.X()+Photon2Dir.Y()*Photon2Dir.Y()+Photon2Dir.Z()*Photon2Dir.Z());

      float cos_term = TMath::Cos(g1->phi()-g2->phi()); 
    
      float sech1 = 1.0/TMath::CosH(g1->eta());  

      float sech2 = 1.0/TMath::CosH(g2->eta());  

      float tanh1 = 1.0/TMath::TanH(g1->phi());
    
      float tanh2 = 1.0/TMath::TanH(g2->phi());

      float numerator1 = sech1*(sech1*tanh2-tanh1*sech2*cos_term);

      float numerator2 = sech2*(sech2*tanh1-tanh2*sech1*cos_term);

      float denominator = 1. - tanh1*tanh2 - sech1*sech2*cos_term;

      float angleResolution = ((-1.*beamsig*TMath::Sqrt(.2))/denominator)*(numerator1/r1 + numerator2/r2);

      float alpha_sig = 0.5*angleResolution; 

      float SigmaM = TMath::Sqrt(g1->getSigEOverE()*g1->getSigEOverE() + g2->getSigEOverE()*g2->getSigEOverE());   

      float MassResolutionWrongVtx = TMath::Sqrt((SigmaM*SigmaM)+(alpha_sig*alpha_sig));

      mass_reswrongvtx_vector.push_back(MassResolutionWrongVtx);
   

      ptasym = (sumpt.Mod() - (p14+p24).Vect().XYvector().Mod())/(sumpt.Mod() + (p14+p24).Vect().XYvector().Mod());
      zconv=getZFromConvPair(g1,g2,IndexMatchedConversionLeadPhoton,IndexMatchedConversionTrailPhoton,conversionsVector,beamSpot,param);
      szconv=getsZFromConvPair(g1,g2,IndexMatchedConversionLeadPhoton,IndexMatchedConversionTrailPhoton,conversionsVector,param);
      float nConv = conversionsVector.size();      
      float pull_conv = 0;
      if(zconv==0 && szconv==0.0){
	//FIXME WHAT TO DO WHEN WE DON'T HAVE PULL_CONV CASE? 
      }else{
	pull_conv = fabs(vtx->position().z()-zconv)/szconv;
      }

      logsumpt2_=log(sumpt2_in+sumpt2_out);
      ptbal_=ptbal;
      pull_conv_=pull_conv;
      nConv_=nConv;
      float mva_value = VertexIdMva_->EvaluateMVA("BDT"); 
      if(mva_value>max_mva_value){
	max_mva_value=mva_value;
	selected_vertex_index=vertex_index;
      }
      
      if(mva_value<max_mva_value && mva_value>second_max_mva_value){
        second_max_mva_value=mva_value;
        second_selected_vertex_index=vertex_index;

      }
      if(mva_value<second_max_mva_value && mva_value>third_max_mva_value){
        third_max_mva_value=mva_value;
	third_selected_vertex_index=vertex_index;
      }   
    }
  
    dipho_pt_ = g1->pt()+g2->pt();
    nVert_    = vtxs.size();
    MVA0_     = max_mva_value;
    MVA1_     = second_max_mva_value;
    dZ1_      = vtxs[selected_vertex_index]->position().z() - vtxs[second_selected_vertex_index]->position().z();
    MVA2_     = third_max_mva_value;
    dZ2_      = vtxs[selected_vertex_index]->position().z() - vtxs[third_selected_vertex_index]->position().z();  

    float vtxprobmva = VertexProbMva_->EvaluateMVA("BDT");  
    //std::cout<<"\t selected vertex_index:"<<selected_vertex_index<<" with max_mva_value:"<<max_mva_value<<std::endl;
    
    leadptom_       = g1->pt()/(diphoton_objects_vector.at(selected_vertex_index).M());
    subleadptom_    = g2->pt()/(diphoton_objects_vector.at(selected_vertex_index).M());
    subleadmva_     = g2->getPhoIdMvaDWrtVtx(vtxs[selected_vertex_index]);
    leadmva_        = g1->getPhoIdMvaDWrtVtx(vtxs[selected_vertex_index]);
    leadeta_        = g2->eta();
    subleadeta_     = g1->eta();
    sigmarv_        = .5*sqrt((g1->getSigEOverE())*(g1->getSigEOverE()) + (g2->getSigEOverE())*(g2->getSigEOverE()));  
    sigmawv_        = mass_reswrongvtx_vector.at(selected_vertex_index); 
    CosPhi_         = TMath::Cos(g1->phi()-g2->phi());
    vtxprob_        =  1.-.49*(1+vtxprobmva); 
   
    float diphotonmva = DiphotonMva_->EvaluateMVA("BDT");
    std::cout << diphotonmva << std::endl;

    
    return vtxs[selected_vertex_index];
  }  
  
}

DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
		  flashgg::LegacyVertexSelector,
		  "FlashggLegacyVertexSelector");
