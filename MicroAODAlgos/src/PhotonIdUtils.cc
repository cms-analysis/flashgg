#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace std;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils() {};
PhotonIdUtils::~PhotonIdUtils() {
  // These delete commands caused an unpredictable segfault,
  // because phoIdMva was pointing to the same place as either phoIdMva_2012_EB_ or phoIdMva_2012_EE_.
  // So something got deleted twice, which is not allowed and can cause memory issues.
  // Replaced with shared_ptr that do not have this problem: 
  // they keep count internally and delete their memory automagically when they go out of scope.
  //    delete phoIdMva;
  //    delete phoIdMva_2012_EB_;
  //    delete phoIdMva_2012_EE_;
  //    phoIdMva = 0;
  //    phoIdMva_2012_EB_ = 0;
  //    phoIdMva_2012_EE_ = 0;
};

// *****************************************************************************************************************
//                    PHOTON isolation 
// *****************************************************************************************************************

float PhotonIdUtils::pfIsoChgWrtVtx( edm::Ptr<pat::Photon>& photon, 
				     const edm::Ptr<reco::Vertex> vtx,  
				     const flashgg::VertexCandidateMap vtxcandmap,
				     float coneSize, float coneVetoBarrel, float coneVetoEndcap, 
				     float ptMin 
				     ) 
{
  float isovalue = 0;

  float coneVeto = 0;
  if( photon->isEB() )      coneVeto = coneVetoBarrel;
  else if( photon->isEE() ) coneVeto = coneVetoEndcap;
    

  math::XYZVector SCdirection( photon->superCluster()->x() - vtx->x(),
			       photon->superCluster()->y() - vtx->y(),
			       photon->superCluster()->z() - vtx->z() 
			       );

  if( vtxcandmap.count(vtx) ) {  

    edm::PtrVector<pat::PackedCandidate> pfcandidates = vtxcandmap.at(vtx);
        
    for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 
      
      edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf];
      
      //if( fabs(pfcand->pdgId()) != 211 ) continue;    
      if( pfcand->pt() < ptMin )         continue;    
      float dRTkToVtx  = deltaR( pfcand->momentum().Eta(), pfcand->momentum().Phi(),
				 SCdirection.Eta(), SCdirection.Phi() );
      if( dRTkToVtx > coneSize || dRTkToVtx < coneVeto ) continue;
      
      isovalue += pfcand->pt();
    }
    return isovalue;  
  }
  else return -1;   // return -1 if the vertex is not found in the map

}


std::map<edm::Ptr<reco::Vertex>,float> PhotonIdUtils::pfIsoChgWrtAllVtx( edm::Ptr<pat::Photon>& photon, 
									 const edm::PtrVector<reco::Vertex>& vertices,
									 const flashgg::VertexCandidateMap vtxcandmap,
									 float coneSize, float coneVetoBarrel, float coneVetoEndcap, 
									 float ptMin )
{
  std::map<edm::Ptr<reco::Vertex>,float> isomap;
  isomap.clear();

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    
    float iso = pfIsoChgWrtVtx( photon, vertices[iv], vtxcandmap, coneSize, coneVetoBarrel, coneVetoEndcap, ptMin );
    isomap.insert( std::make_pair(vertices[iv],iso) );
  }

  return isomap;
}



float PhotonIdUtils::pfIsoChgWrtWorstVtx( std::map<edm::Ptr<reco::Vertex>,float>& vtxIsoMap )
{
  float MaxValueMap = -1000;  
  float itValue = 0;
  
  for (std::map<edm::Ptr<reco::Vertex>,float>::iterator it=vtxIsoMap.begin(); it!=vtxIsoMap.end(); ++it) {
    itValue = it->second;
    if (itValue > MaxValueMap) MaxValueMap = itValue;
  }

  return MaxValueMap;
}



float PhotonIdUtils::pfIsoGamma( edm::Ptr<pat::Photon>& photon, 
				 const edm::PtrVector<pat::PackedCandidate>& pfcandidates,
				 float dRMax,
				 float dRVetoBarrel,
				 float dRVetoEndcap,
				 float etaStripBarrel,
				 float etaStripEndcap, 
				 float minEnergyBarrel, 
				 float minEnergyEndcap
				)
{

  float isovalue = 0;
  
  float dRVeto = 99;
  float maxetaStrip = 99;

  if( photon->isEB() ) {
    dRVeto        = dRVetoBarrel;
    maxetaStrip  = etaStripBarrel;
  } else if( photon->isEE() ){
    dRVeto        = dRVetoEndcap;
    maxetaStrip  = etaStripEndcap; 
  }   

  for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 

    edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf]; 
    
    if( pfcand->pdgId() != 22 ) continue;
    if( photon->isEB() ) if( fabs(pfcand->pt()) < minEnergyBarrel )     continue;  
    if( photon->isEE() ) if( fabs(pfcand->energy()) < minEnergyEndcap ) continue;
    
    edm::RefVector<pat::PackedCandidateCollection> associated =  photon->associatedPackedPFCandidates();
    
    int nass = 0;
    for( unsigned int ipc = 0; ipc < associated.size(); ipc++ ) {
      edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated[ipc] );
      if( associatedPtr == pfcand )  nass++;
    }
    
    if( nass > 0 ) continue;

    math::XYZPoint  pfcandvtx = pfcand->vertex();
    math::XYZVector SCdirectionWrtCandVtx( photon->superCluster()->x() - pfcandvtx.x(),
					   photon->superCluster()->y() - pfcandvtx.y(),
					   photon->superCluster()->z() - pfcandvtx.z() 
					   );

    float dEta = fabs( SCdirectionWrtCandVtx.Eta() - pfcand->momentum().Eta() );
    float dR   = deltaR( SCdirectionWrtCandVtx.Eta(), SCdirectionWrtCandVtx.Phi(), pfcand->momentum().Eta(), pfcand->momentum().Phi() ); 
    
    if( dEta < maxetaStrip )        continue;
    if( dR < dRVeto || dR > dRMax ) continue;
      
    isovalue += pfcand->pt();
  }

  return isovalue;
}


// *****************************************************************************************************************
//                    PHOTON MVA CALCULATION
// *****************************************************************************************************************

/* to do:
- configurable input file
- decide what to do with EE and EB
*/

void PhotonIdUtils::setupMVA( const std::string& xmlfilenameEB, const std::string& xmlfilenameEE )
{

  cout << " photonId MVA weights for EB set from file: " << xmlfilenameEB << endl;
  cout << " photonId MVA weights for EE set from file: " << xmlfilenameEE << endl;
  // **** bdt 2012 EB ****
 
  string mvamethod = "BDT";
  
  phoIdMva_2012_EB_ = std::make_shared<TMVA::Reader>("!Color:Silent");
 
  phoIdMva_2012_EB_->AddVariable( "ph.scrawe", &phoIdMva_SCRawE_);
  phoIdMva_2012_EB_->AddVariable( "ph.r9",                &phoIdMva_R9_ );
  phoIdMva_2012_EB_->AddVariable( "ph.sigietaieta",       &phoIdMva_covIEtaIEta_ );
  phoIdMva_2012_EB_->AddVariable( "ph.scetawidth",        &phoIdMva_EtaWidth_);
  phoIdMva_2012_EB_->AddVariable( "ph.scphiwidth",        &phoIdMva_PhiWidth_);
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_CoviEtaiPhi", &phoIdMva_covIEtaIPhi_);
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_s4ratio",     &phoIdMva_S4_);
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_GammaIso",    &phoIdMva_pfPhoIso03_ );
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_ChargedIso_selvtx",   &phoIdMva_pfChgIso03_ );
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_ChargedIso_worstvtx", &phoIdMva_pfChgIso03worst_ );
  phoIdMva_2012_EB_->AddVariable( "ph.sceta",             &phoIdMva_ScEta_ );
  phoIdMva_2012_EB_->AddVariable( "rho",                  &phoIdMva_rho_);
  phoIdMva_2012_EB_->BookMVA( mvamethod.c_str(), xmlfilenameEB );

  // **** bdt 2012 EE ****

  phoIdMva_2012_EE_ = std::make_shared<TMVA::Reader>("!Color:Silent");
  
  phoIdMva_2012_EE_->AddVariable( "ph.scrawe", &phoIdMva_SCRawE_);
  phoIdMva_2012_EE_->AddVariable( "ph.r9",                &phoIdMva_R9_ );
  phoIdMva_2012_EE_->AddVariable( "ph.sigietaieta",       &phoIdMva_covIEtaIEta_ );
  phoIdMva_2012_EE_->AddVariable( "ph.scetawidth",        &phoIdMva_EtaWidth_);
  phoIdMva_2012_EE_->AddVariable( "ph.scphiwidth",        &phoIdMva_PhiWidth_);
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_CoviEtaiPhi", &phoIdMva_covIEtaIPhi_);
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_s4ratio",     &phoIdMva_S4_); 
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_GammaIso",    &phoIdMva_pfPhoIso03_ );
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_ChargedIso_selvtx",   &phoIdMva_pfChgIso03_ );
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_ChargedIso_worstvtx", &phoIdMva_pfChgIso03worst_ );
  phoIdMva_2012_EE_->AddVariable( "ph.sceta",             &phoIdMva_ScEta_ );                          
  phoIdMva_2012_EE_->AddVariable( "rho",                  &phoIdMva_rho_);
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_PsEffWidthSigmaRR",   &phoIdMva_ESEffSigmaRR_ );
  phoIdMva_2012_EE_->BookMVA( mvamethod.c_str(), xmlfilenameEE );
}


float PhotonIdUtils::computeMVAWrtVtx( /*edm::Ptr<flashgg::Photon>& photon,*/
				      flashgg::Photon& photon,
				      const edm::Ptr<reco::Vertex>& vtx,
				      const double rho )
{

  phoIdMva_SCRawE_          = photon.superCluster()->rawEnergy();
  phoIdMva_R9_              = photon.r9();
  phoIdMva_S4_              = photon.getS4();
  phoIdMva_covIEtaIEta_     = photon.sigmaIetaIeta();
  phoIdMva_EtaWidth_        = photon.superCluster()->etaWidth();
  phoIdMva_PhiWidth_        = photon.superCluster()->phiWidth();
  phoIdMva_covIEtaIPhi_     = photon.getSieip();                        
  phoIdMva_pfPhoIso03_      = photon.getpfPhoIso03();
  phoIdMva_pfChgIso03_      = photon.getpfChgIso03WrtVtx(vtx);
  phoIdMva_pfChgIso03worst_ = photon.getpfChgIsoWrtWorstVtx03();
  phoIdMva_ScEta_           = photon.superCluster()->eta();
  phoIdMva_rho_             = rho; // we don't want to add the event-based rho as flashgg::photon member
  phoIdMva_ESEffSigmaRR_    = photon.getESEffSigmaRR();  

 
  if( photon.isEB() )      phoIdMva = phoIdMva_2012_EB_;
  else if( photon.isEE() ) phoIdMva = phoIdMva_2012_EE_;
  
  float mvavalue = phoIdMva->EvaluateMVA("BDT");
  return mvavalue;
}

std::map<edm::Ptr<reco::Vertex>,float> PhotonIdUtils::computeMVAWrtAllVtx( /*edm::Ptr<flashgg::Photon>& photon,*/
									  flashgg::Photon& photon,
									  const edm::PtrVector<reco::Vertex>& vertices,
									  const double rho )
  
{  
  std::map<edm::Ptr<reco::Vertex>,float> mvamap;
  mvamap.clear();

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    edm::Ptr<reco::Vertex> vertex = vertices[iv];
    float mvapervtx = computeMVAWrtVtx( photon, vertex, rho);
    mvamap.insert( std::make_pair(vertex, mvapervtx) );
  }

  return mvamap;
}



flashgg::Photon PhotonIdUtils::pho4MomCorrection(edm::Ptr<flashgg::Photon>& photon, 
				       edm::Ptr<reco::Vertex> vtx) {

  /// vtx should be the chosen vtx, not primary vtx ///
  float vtx_X = vtx->x();
  float vtx_Y = vtx->y();
  float vtx_Z = vtx->z();

  float sc_X = photon->superCluster()->x();
  float sc_Y = photon->superCluster()->y();
  float sc_Z = photon->superCluster()->z();

  math::XYZVector vtx_Pos(vtx_X,vtx_Y,vtx_Z);
  math::XYZVector sc_Pos(sc_X,sc_Y,sc_Z);
  
  math::XYZVector direction = sc_Pos - vtx_Pos;
  math::XYZVector p = (direction.Unit()) * (photon->energy());
  math::XYZTLorentzVector corrected_p4(p.x(),p.y(),p.z(),photon->energy());
  flashgg::Photon *p4CorrPho = photon->clone();
  p4CorrPho->setP4(corrected_p4);
  return *p4CorrPho;
}
