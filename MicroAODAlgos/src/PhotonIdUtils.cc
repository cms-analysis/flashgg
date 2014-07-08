#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils() {};
PhotonIdUtils::~PhotonIdUtils() {
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

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    
    float iso = pfIsoChgWrtVtx( photon, vertices[iv], vtxcandmap, coneSize, coneVetoBarrel, coneVetoEndcap, ptMin );
    isomap.insert( std::make_pair(vertices[iv],iso) );
  }

  return isomap;
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

    int nass = 0;
    edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf]; 

    if( pfcand->pdgId() != 22 ) continue;
    if( photon->isEB() ) if( fabs(pfcand->pt()) < minEnergyBarrel )     continue;  
    if( photon->isEE() ) if( fabs(pfcand->energy()) < minEnergyEndcap ) continue;

    edm::RefVector<pat::PackedCandidateCollection> associated =  photon->associatedPackedPFCandidates();
    if( associated.size() > 0 ) {

      for( unsigned int ipc = 0; ipc < associated.size(); ipc++ ) {

	edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated[ipc] );
	if( associatedPtr == pfcand ) nass++;
      }  
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

void PhotonIdUtils::setupMVA( string& xmlfilenameEB, string& xmlfilenameEE )
{

  //string mvaDiscriDir = "/afs/cern.ch/work/f/favaro/private/miniAODforHgg/CMSSW_7_0_4/src/flashGGAnalyzers/flashggCommissioning/python/inputs/mvaDiscriminants/";

  cout << " photonId MVA weights for EB set from file: " << xmlfilenameEB << endl;
  cout << " photonId MVA weights for EE set from file: " << xmlfilenameEE << endl;
  // **** bdt 2012 EB ****
 
  string mvamethod = "BDT";
  
  phoIdMva_2012_EB_ = new TMVA::Reader("!Color:Silent");
 
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
  //phoIdMva_2012_EB_->BookMVA(mvamethod.c_str(), mvaDiscriDir + "2013FinalPaper_PhotonID_Barrel_BDT_TrainRangePT15_8TeV.weights.xml");
  phoIdMva_2012_EB_->BookMVA( mvamethod.c_str(), xmlfilenameEB );

  // **** bdt 2012 EE ****

  phoIdMva_2012_EE_ = new TMVA::Reader("!Color:Silent");
  
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
  //phoIdMva_2012_EE_->BookMVA(mvamethod.c_str(), mvaDiscriDir + "2013FinalPaper_PhotonID_Endcap_BDT_TrainRangePT15_8TeV.weights.xml");
  phoIdMva_2012_EE_->BookMVA( mvamethod.c_str(), xmlfilenameEE );
}


float PhotonIdUtils::computeMVAWrtVtx( /*edm::Ptr<flashgg::Photon>& photon,*/
				      flashgg::Photon& photon,
				      const edm::Ptr<reco::Vertex>& vtx,
				      const double rho )
{

  phoIdMva_SCRawE_         = photon.superCluster()->rawEnergy();
  phoIdMva_R9_             = photon.r9();
  phoIdMva_S4_             = photon.getS4();
  phoIdMva_covIEtaIEta_    = photon.sigmaIetaIeta();
  phoIdMva_EtaWidth_       = photon.superCluster()->etaWidth();
  phoIdMva_PhiWidth_       = photon.superCluster()->phiWidth();
  phoIdMva_covIEtaIPhi_    = photon.getSieip();                        
  phoIdMva_pfPhoIso03_     = photon.getpfPhoIso03();
  phoIdMva_pfChgIso03_     = photon.getpfChgIso03WrtVtx(vtx);
  phoIdMva_pfChgIso03worst_ = 0;
  phoIdMva_ScEta_          = photon.superCluster()->eta();
  phoIdMva_rho_            = rho; // we don't want to add the event-based rho as flashgg::photon member
  phoIdMva_ESEffSigmaRR_   = photon.r9(); // THIS SHOULD BECOME ESEFFSIGMARR AT SOME POINT

 
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

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    edm::Ptr<reco::Vertex> vertex = vertices[iv];
    float mvapervtx = computeMVAWrtVtx( photon, vertex, rho);
    mvamap.insert( std::make_pair(vertex, mvapervtx) );
  }

  return mvamap;
}
