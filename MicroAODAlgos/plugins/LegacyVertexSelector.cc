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
   ~LegacyVertexSelector();
   edm::Ptr<reco::Vertex> select(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Vertex>&,
				 const VertexCandidateMap& vertexCandidateMap,
				 const edm::PtrVector<reco::Conversion>&,
				 const math::XYZPoint&
				 ) override;

    void writeInfoFromLastSelectionTo(flashgg::DiPhotonCandidate&) override;

    double vtxZFromConvOnly         (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxZFromConvSuperCluster (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxZFromConv             (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&,const math::XYZPoint&) const;
    double vtxdZFromConv            (const edm::Ptr<flashgg::Photon>&,const edm::Ptr<reco::Conversion>&) const;

    double getZFromConvPair(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,
			    const int ,const int,
			    const edm::PtrVector<reco::Conversion>&,
			    const math::XYZPoint &) const;  
   
    double getsZFromConvPair(const edm::Ptr<flashgg::Photon>&,const edm::Ptr<flashgg::Photon>&,
			     const int ,const int,
			     const edm::PtrVector<reco::Conversion>&) const;
    
    int IndexMatchedConversion(const edm::Ptr<flashgg::Photon>&,const edm::PtrVector<reco::Conversion>&) const;

    void Initialize();
    
  private:

   edm::FileInPath vertexIdMVAweightfile_;
   edm::FileInPath vertexProbMVAweightfile_;

   double dRexclude;
   double sigma1Pix;
   double sigma1Tib;
   double sigma1Tob;
   double sigma1PixFwd;
   double sigma1Tid;
   double sigma1Tec;
   double sigma2Pix;
   double sigma2Tib;
   double sigma2Tob;
   double sigma2PixFwd;
   double sigma2Tid;
   double sigma2Tec;
   double singlelegsigma1Pix;
   double singlelegsigma1Tib;
   double singlelegsigma1Tob;
   double singlelegsigma1PixFwd;
   double singlelegsigma1Tid;
   double singlelegsigma1Tec;
   double singlelegsigma2Pix;
   double singlelegsigma2Tib;
   double singlelegsigma2Tob;
   double singlelegsigma2PixFwd;
   double singlelegsigma2Tid;
   double singlelegsigma2Tec;

 protected: 
   TMVA::Reader * VertexIdMva_;
   bool initialized_;
   float logsumpt2_;
   float ptbal_;
   float ptasym_;
   float nConv_;
   float pull_conv_;

   float logsumpt2selected_;
   float ptbalselected_;
   float ptasymselected_;

   TMVA::Reader * VertexProbMva_; 
   float dipho_pt_;
   float nVert_;
   float MVA0_;
   float MVA1_;
   float dZ1_;
   float MVA2_;
   float dZ2_;
   float vtxprobmva_;

  };

  LegacyVertexSelector::LegacyVertexSelector(const edm::ParameterSet& iConfig) :
    VertexSelectorBase(iConfig) 
  {
    vertexIdMVAweightfile_ = iConfig.getParameter<edm::FileInPath>("vertexIdMVAweightfile");
    vertexProbMVAweightfile_ = iConfig.getParameter<edm::FileInPath>("vertexProbMVAweightfile");

    dRexclude             =iConfig.getUntrackedParameter<double>("dRexclude", 0.05);
    sigma1Pix             =iConfig.getUntrackedParameter<double>("sigma1Pix", 0.011);
    sigma1Tib             =iConfig.getUntrackedParameter<double>("sigma1Tib", 0.492);
    sigma1Tob             =iConfig.getUntrackedParameter<double>("sigma1Tob", 4.398);
    sigma1PixFwd          =iConfig.getUntrackedParameter<double>("sigma1PixFwd", 0.054);
    sigma1Tid             =iConfig.getUntrackedParameter<double>("sigma1Tid", 0.320);
    sigma1Tec             =iConfig.getUntrackedParameter<double>("sigma1Tec", 1.092);
    sigma2Pix             =iConfig.getUntrackedParameter<double>("sigma2Pix", 0.022);
    sigma2Tib             =iConfig.getUntrackedParameter<double>("sigma2Tib", 0.297);
    sigma2Tob             =iConfig.getUntrackedParameter<double>("sigma2Tob", 1.728);
    sigma2PixFwd          =iConfig.getUntrackedParameter<double>("sigma2PixFwd", 0.150);
    sigma2Tid             =iConfig.getUntrackedParameter<double>("sigma2Tid", 0.393);
    sigma2Tec             =iConfig.getUntrackedParameter<double>("sigma2Tec", 1.008);
    singlelegsigma1Pix    =iConfig.getUntrackedParameter<double>("singlelegsigma1Pix", 0.009);
    singlelegsigma1Tib    =iConfig.getUntrackedParameter<double>("singlelegsigma1Tib", 1.163);
    singlelegsigma1Tob    =iConfig.getUntrackedParameter<double>("singlelegsigma1Tob", 2.130);
    singlelegsigma1PixFwd =iConfig.getUntrackedParameter<double>("singlelegsigma1PixFwd", 0.071);
    singlelegsigma1Tid    =iConfig.getUntrackedParameter<double>("singlelegsigma1Tid", 0.384);
    singlelegsigma1Tec    =iConfig.getUntrackedParameter<double>("singlelegsigma1Tec", 1.923);
    singlelegsigma2Pix    =iConfig.getUntrackedParameter<double>("singlelegsigma2Pix", 0.054);
    singlelegsigma2Tib    =iConfig.getUntrackedParameter<double>("singlelegsigma2Tib", 0.597);
    singlelegsigma2Tob    =iConfig.getUntrackedParameter<double>("singlelegsigma2Tob", 0.480);
    singlelegsigma2PixFwd =iConfig.getUntrackedParameter<double>("singlelegsigma2PixFwd", 0.276);
    singlelegsigma2Tid    =iConfig.getUntrackedParameter<double>("singlelegsigma2Tid", 0.497);
    singlelegsigma2Tec    =iConfig.getUntrackedParameter<double>("singlelegsigma2Tec", 1.046);


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

    VertexProbMva_ = new TMVA::Reader("!Color:Silent");
    VertexProbMva_->AddVariable("diphoPt0",&dipho_pt_); 
    VertexProbMva_->AddVariable("nVert",&nVert_); 
    VertexProbMva_->AddVariable("MVA0",&MVA0_); 
    VertexProbMva_->AddVariable("MVA1",&MVA1_); 
    VertexProbMva_->AddVariable("dZ1",&dZ1_); 
    VertexProbMva_->AddVariable("MVA2",&MVA2_); 
    VertexProbMva_->AddVariable("dZ2",&dZ2_); 
    VertexProbMva_->AddVariable("nConv",&nConv_); 
    VertexProbMva_->BookMVA("BDT",vertexProbMVAweightfile_.fullPath());

    initialized_ = true;
  }

  LegacyVertexSelector::~LegacyVertexSelector() {
    delete VertexIdMva_;
    delete VertexProbMva_;
  }


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
    double ReturnValue = 0;
    double perp = sqrt(conversion->conversionVertex().x()*conversion->conversionVertex().x()+conversion->conversionVertex().y()*conversion->conversionVertex().y());
    if(fabs(pho->superCluster()->eta())<1.5) { 
      if(perp<=15.0){
	//Pixel Barrel
	ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);
      }else if(perp>15 && perp<=60.0) {
	  //Tracker Inner Barrel
	ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
      }else{
	//Tracker Outer Barrel
	ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
      }
    }else{
      if(fabs(conversion->conversionVertex().z())<=50.0){
	//Pixel Forward
	ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);
      }else if(fabs(conversion->conversionVertex().z())>50.0 && fabs(conversion->conversionVertex().z())<=100.0){
	  //Tracker Inner Disk
	ReturnValue = vtxZFromConvOnly(pho,conversion,beamSpot);
      }else{
	//Track EndCap
	ReturnValue = vtxZFromConvSuperCluster(pho,conversion,beamSpot);
      }
    }
    return ReturnValue;
  }
  
  double LegacyVertexSelector::vtxdZFromConv (const edm::Ptr<flashgg::Photon>& pho, const edm::Ptr<reco::Conversion> & conversion) const{
    double dz=-99999;
    double perp = sqrt(conversion->conversionVertex().x()*conversion->conversionVertex().x()+conversion->conversionVertex().y()*conversion->conversionVertex().y());
    if (conversion->nTracks()==2) {
      if ( fabs(pho->superCluster()->eta())<1.5 ) { // barrel
	if ( perp <=15 ) {
	  dz=sigma1Pix;
	} else if ( perp > 15 && perp <=60 ) {
	  dz=sigma2Tib;
	} else {
	  dz=sigma2Tob;
	}
      } else { // endcap
	if ( fabs(conversion->conversionVertex().z() ) <=50 ) {
	  dz=sigma1PixFwd;
	} else if ( fabs(conversion->conversionVertex().z() ) > 50 && fabs(conversion->conversionVertex().z()) <= 100 ) {
	  dz=sigma1Tid;
	} else {
	  dz=sigma2Tec;
	}
      }
    } else if (conversion->nTracks()==1) {
      if ( fabs(pho->superCluster()->eta())<1.5 ) { // barrel
	if ( perp <=15 ) {
	  dz=singlelegsigma1Pix;
	} else if ( perp > 15 && perp <=60 ) {
	  dz=singlelegsigma2Tib;
	} else {
	  dz=singlelegsigma2Tob;
	}
	
      } else { // endcap
	if ( fabs(conversion->conversionVertex().z() ) <=50 ) {
	  dz=singlelegsigma1PixFwd;
	} else if ( fabs(conversion->conversionVertex().z() ) > 50 && fabs(conversion->conversionVertex().z()) <= 100 ) {
	  dz=singlelegsigma1Tid;
	} else {
	  dz=singlelegsigma2Tec;
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
						const math::XYZPoint & beamSpot) const {
    double zconv=0;
    if(index_conversionLead!=-1  && index_conversionTrail==-1){ 
      zconv=vtxZFromConv(p1,conversionsVector[index_conversionLead],beamSpot);
    }
    if(index_conversionLead==-1 && index_conversionTrail!=-1){
      zconv=vtxZFromConv (p2,conversionsVector[index_conversionTrail],beamSpot);
    }
    
    if (index_conversionLead!=-1 && index_conversionTrail!=-1){
      float z1  = vtxZFromConv (p1,conversionsVector[index_conversionLead],beamSpot);
      float sz1 = vtxdZFromConv(p1,conversionsVector[index_conversionLead]); //,param);
      float z2  = vtxZFromConv (p2,conversionsVector[index_conversionTrail],beamSpot);
      float sz2 = vtxdZFromConv(p2,conversionsVector[index_conversionTrail]); //,param);
      zconv  = (z1/sz1/sz1 + z2/sz2/sz2)/(1./sz1/sz1 + 1./sz2/sz2 );  // weighted average
    }
    return zconv;
  }
  
  double LegacyVertexSelector::getsZFromConvPair(const edm::Ptr<flashgg::Photon>& p1,
						 const edm::Ptr<flashgg::Photon>& p2,
						 int index_conversionLead,
						 int index_conversionTrail,
						 const edm::PtrVector<reco::Conversion>& conversionsVector) const {
    double szconv=0;
    if ( index_conversionLead!=-1  && index_conversionTrail==-1 ){
      szconv = vtxdZFromConv(p1,conversionsVector[index_conversionLead]); 
    }
    if ( index_conversionLead==-1 && index_conversionTrail!=-1 ){
      szconv = vtxdZFromConv(p2,conversionsVector[index_conversionTrail]);
    }
    
    if (index_conversionLead!=-1 && index_conversionTrail!=-1){
      float sz1 = vtxdZFromConv(p1,conversionsVector[index_conversionLead]);
      float sz2 = vtxdZFromConv(p2,conversionsVector[index_conversionTrail]);
      szconv = sqrt( 1./(1./sz1/sz1 + 1./sz2/sz2)) ;
    }
    return szconv;
  }
 

  int LegacyVertexSelector::IndexMatchedConversion(const edm::Ptr<flashgg::Photon>& g,const edm::PtrVector<reco::Conversion>& conversionsVector) const{
    double mindR = 999;
    
    assert(g->hasConversionTracks());   //The photon has to have conversion tracks!
    assert(conversionsVector.size()>0); //The photon has to have conversion tracks!
    int selected_conversion_index = -1; //returned value if no match was found
    
    if(g->hasConversionTracks()){
      for (unsigned int i=0; i<conversionsVector.size();i++){
        edm::Ptr<reco::Conversion> conv = conversionsVector[i];
        if(!conv->isConverted()) continue;
        if(conv->refittedPair4Momentum().pt()<10.) continue;
        if(TMath::Prob(conv->conversionVertex().chi2(),conv->conversionVertex().ndof()) < 1e-6 ) continue;
        
        TVector3 VtxtoSC;
        VtxtoSC.SetXYZ(g->superCluster()->position().x() - conv->conversionVertex().x(), 
                       g->superCluster()->position().y() - conv->conversionVertex().y(), 
                       g->superCluster()->position().z() - conv->conversionVertex().z());
        TVector3 RefPairMo;
        RefPairMo.SetXYZ(conv->refittedPairMomentum().x(),conv->refittedPairMomentum().y(),conv->refittedPairMomentum().z());
        double dR = 0;
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
						      const math::XYZPoint & beamSpot
						      //						      const std::map<std::string,double> & param,
						      //                                                      const float & beamsig 
                                                       ) {

    int IndexMatchedConversionLeadPhoton=-1;
    int IndexMatchedConversionTrailPhoton=-1;

    if(conversionsVector.size()>0){
      if(g1->hasConversionTracks())
        IndexMatchedConversionLeadPhoton = IndexMatchedConversion(g1,conversionsVector);
      if(g2->hasConversionTracks())
        IndexMatchedConversionTrailPhoton = IndexMatchedConversion(g2,conversionsVector);
    }

    unsigned int vertex_index,track_index;

    unsigned int selected_vertex_index=0;
    unsigned int second_selected_vertex_index=0;
    unsigned int third_selected_vertex_index=0;
    float max_mva_value=-999;
    float second_max_mva_value=-999;
    float third_max_mva_value=-999;

    if (!initialized_) {
      Initialize();
    }
  
    for (vertex_index = 0 ; vertex_index < vtxs.size() ; vertex_index++) {
      edm::Ptr<reco::Vertex> vtx = vtxs[vertex_index];
      TVector3 Photon1Dir;
      TVector3 Photon1Dir_uv;
      TVector3 Photon2Dir;
      TVector3 Photon2Dir_uv;
      TLorentzVector p14;
      TLorentzVector p24;
      Photon1Dir.SetXYZ(g1->superCluster()->position().x() - vtx->position().x(),g1->superCluster()->position().y() - vtx->position().y(),g1->superCluster()->position().z() - vtx->position().z()); 
      Photon2Dir.SetXYZ(g2->superCluster()->position().x() - vtx->position().x(),g2->superCluster()->position().y() - vtx->position().y(),g2->superCluster()->position().z() - vtx->position().z()); 
      Photon1Dir_uv = Photon1Dir.Unit()*g1->superCluster()->rawEnergy();
      Photon2Dir_uv = Photon2Dir.Unit()*g2->superCluster()->rawEnergy();
      p14.SetPxPyPzE(Photon1Dir_uv.x(),Photon1Dir_uv.y(),Photon1Dir_uv.z(),g1->superCluster()->rawEnergy()); 
      p24.SetPxPyPzE(Photon2Dir_uv.x(),Photon2Dir_uv.y(),Photon2Dir_uv.z(),g2->superCluster()->rawEnergy()); 
     
      TVector2 sumpt;
      double sumpt2_out = 0; 
      double sumpt2_in = 0; 
      double ptbal = 0; 
      double ptasym = 0;

      sumpt.Set(0.,0.);
      
      if(vertexCandidateMap.count(vtx) == 0) continue;
      for(track_index = 0 ; track_index < vertexCandidateMap.at(vtx).size() ; track_index++) {
        edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMap.at(vtx)[track_index];
        TVector3 tk;
        TVector2 tkXY;
        double dr1 = 0;
        double dr2 = 0;
        tk.SetXYZ(cand->px(),cand->py(),cand->pz()); 
        tkXY = tk.XYvector();
        sumpt += tkXY;
        dr1 = tk.DeltaR(p14.Vect());
        dr2 = tk.DeltaR(p24.Vect());
        if(dr1 < dRexclude || dr2 < dRexclude){
          sumpt2_in+=tkXY.Mod2();
          continue;
        }
        sumpt2_out+=tkXY.Mod2();
        ptbal-=tkXY*(p14+p24).Vect().XYvector().Unit();
      }

      ptasym = (sumpt.Mod() - (p14+p24).Vect().XYvector().Mod())/(sumpt.Mod() + (p14+p24).Vect().XYvector().Mod());
      ptasym_ = ptasym;
      
      float nConv = 0;
      if (IndexMatchedConversionLeadPhoton != -1) ++nConv;
      if (IndexMatchedConversionTrailPhoton != -1) ++nConv;
      float zconv=0;
      float szconv=0;
      float pull_conv = 999;
      
      if (nConv !=0){
	zconv=getZFromConvPair(g1,g2,IndexMatchedConversionLeadPhoton,IndexMatchedConversionTrailPhoton,conversionsVector,beamSpot);
	szconv=getsZFromConvPair(g1,g2,IndexMatchedConversionLeadPhoton,IndexMatchedConversionTrailPhoton,conversionsVector);
	if(szconv != 0){
	  pull_conv = fabs(vtx->position().z()-zconv)/szconv;
	}else{
	  pull_conv = 10.;
	}
      }
      
      if(pull_conv>10.)pull_conv = 10.;  
      
      logsumpt2_=log(sumpt2_in+sumpt2_out);
      ptbal_=ptbal;
      pull_conv_=pull_conv;
      nConv_=nConv;
      float mva_value = VertexIdMva_->EvaluateMVA("BDT"); 

      if(mva_value>max_mva_value){
	max_mva_value=mva_value;
        selected_vertex_index=vertex_index;
	logsumpt2selected_=logsumpt2_;
	ptbalselected_=ptbal_;
	ptasymselected_=ptasym_;
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
  
    dipho_pt_ = (g1->p4()+g2->p4()).pt();
    nVert_    = vtxs.size();
    MVA0_     = max_mva_value;
    MVA1_     = second_max_mva_value;
    dZ1_      = vtxs[selected_vertex_index]->position().z() - vtxs[second_selected_vertex_index]->position().z();
    MVA2_     = third_max_mva_value;
    dZ2_      = vtxs[selected_vertex_index]->position().z() - vtxs[third_selected_vertex_index]->position().z();  

    vtxprobmva_ = VertexProbMva_->EvaluateMVA("BDT");  
    
    return vtxs[selected_vertex_index];
  }

  void LegacyVertexSelector::writeInfoFromLastSelectionTo(flashgg::DiPhotonCandidate& dipho) {

    dipho.setLogSumPt2(logsumpt2selected_);
    dipho.setPtBal(ptbalselected_);
    dipho.setPtAsym(ptasymselected_);

    dipho.setNConv(nConv_);
    dipho.setPullConv(pull_conv_);
    dipho.setNVert(nVert_);
    dipho.setMVA0(MVA0_);
    dipho.setMVA1(MVA1_);
    dipho.setMVA2(MVA2_);
    dipho.setDZ1(dZ1_);
    dipho.setDZ2(dZ2_);

    dipho.setVtxProbMVA(vtxprobmva_);

  } 

} // namespace flashgg


DEFINE_EDM_PLUGIN(FlashggVertexSelectorFactory,
                  flashgg::LegacyVertexSelector,
                  "FlashggLegacyVertexSelector");
