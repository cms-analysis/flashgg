#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils() {};
PhotonIdUtils::~PhotonIdUtils() {
  delete phoIdMva_2012_EB_;
  delete phoIdMva_2012_EE_;
};

float PhotonIdUtils::pfIsoChgWrtVtx( const edm::Ptr<flashgg::Photon>& photon, 
				     edm::Ptr<reco::Vertex> vtx,  
				     const edm::PtrVector<pat::PackedCandidate>& pfcandidates,
				     float coneSize, float coneVeto, 
				     float dzMax, float dxyMax ) 
{

  float isovalue = 0;

  math::XYZVector SCdirection( photon->superCluster()->x() - vtx->x(),
			       photon->superCluster()->y() - vtx->y(),
			       photon->superCluster()->z() - vtx->z() 
			       );

  
  for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 
      
    edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf];

    if( fabs(pfcand->pdgId()) != 211 ) continue;     // equivalent to particleType::h
    float dxyTkToVtx = pfcand->dxy(vtx->position());
    float dzTkToVtx  = pfcand->dz(vtx->position());
    float dRTkToVtx  = deltaR( pfcand->momentum().Eta(), pfcand->momentum().Phi(),
			       SCdirection.Eta(), SCdirection.Phi() );

    /*
    std::cout << "through pseudoTrack->vz() " <<  pfcandidates[ipf]->pseudoTrack().vz() << 
      " direct vz() " <<  pfcandidates[ipf]->vz() << 
      " direct dz() " << pfcandidates[ipf]->dz(vtx->position()) << 
      " pv " << vtx->z() << 
      std::endl;
    */

    if( dxyTkToVtx > dxyMax ) continue;
    if( dzTkToVtx  > dzMax  ) continue;
    if( dRTkToVtx > coneSize || dRTkToVtx < coneVeto ) continue;

    isovalue += pfcand->pt();
  }

  return isovalue;
}


vector<float> PhotonIdUtils::pfIsoChgWrtAllVtx( const edm::Ptr<flashgg::Photon>& photon, 
						const edm::PtrVector<reco::Vertex>& vertices,
						const edm::PtrVector<pat::PackedCandidate>& pfcandidates, 
					     	float coneSize, float coneVeto, 
						float dzMax, float dxyMax )
{
  vector<float> isovalues;

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    
    float iso = pfIsoChgWrtVtx( photon, vertices[iv], pfcandidates, coneSize, coneVeto, dzMax, dxyMax );
    isovalues.push_back(iso);
  }

  return isovalues;
}


float PhotonIdUtils::pfIsoGamma( const edm::Ptr<flashgg::Photon>& photon, 
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
- decide how to deal with the vertex-dependent MVA: the best approach would be to update the flashgg::photon directly with the proper vertex...
*/

void PhotonIdUtils::setupMVA(  )
{

  string mvaDiscriDir = "inputs/mvaDiscriminants/";

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
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_ChargedIso_selvtx",   &phoIdMva_pfChIso03_ );
  phoIdMva_2012_EB_->AddVariable( "ph.idmva_ChargedIso_worstvtx", &phoIdMva_pfChIso03worst_ );
  phoIdMva_2012_EB_->AddVariable( "ph.sceta",             &phoIdMva_ScEta_ );
  phoIdMva_2012_EB_->AddVariable( "rho",                  &phoIdMva_rho_);
  phoIdMva_2012_EB_->BookMVA(mvamethod.c_str(), mvaDiscriDir + "2013FinalPaper_PhotonID_Barrel_BDT_TrainRangePT15_8TeV.weights.xml");


  // **** bdt 2012 EE ****

  phoIdMva_2012_EE_ = new TMVA::Reader("!Color:Silent");
  
  phoIdMva_2012_EE_->AddVariable( "ph.r9",                &phoIdMva_R9_ );
  phoIdMva_2012_EE_->AddVariable( "ph.sigietaieta",       &phoIdMva_covIEtaIEta_ );
  phoIdMva_2012_EE_->AddVariable( "ph.scetawidth",        &phoIdMva_EtaWidth_);
  phoIdMva_2012_EE_->AddVariable( "ph.scphiwidth",        &phoIdMva_PhiWidth_);
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_CoviEtaiPhi", &phoIdMva_covIEtaIPhi_);
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_s4ratio",     &phoIdMva_S4_); 
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_GammaIso",    &phoIdMva_pfPhoIso03_ );
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_ChargedIso_selvtx",   &phoIdMva_pfChIso03_ );
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_ChargedIso_worstvtx", &phoIdMva_pfChIso03worst_ );
  phoIdMva_2012_EE_->AddVariable( "ph.sceta",             &phoIdMva_ScEta_ );                          
  phoIdMva_2012_EE_->AddVariable( "rho",                  &phoIdMva_rho_);
  phoIdMva_2012_EE_->AddVariable( "ph.idmva_PsEffWidthSigmaRR",   &phoIdMva_ESEffSigmaRR_ );
  phoIdMva_2012_EE_->BookMVA(mvamethod.c_str(), "2013FinalPaper_PhotonID_Endcap_BDT_TrainRangePT15_8TeV.weights.xml");
  
}


float PhotonIdUtils::computeMVAWrtVtx( const edm::Ptr<flashgg::Photon>& photon,
				       const edm::Ptr<reco::Vertex>& vtx )
{

  phoIdMva_SCRawE_         = photon->superCluster()->rawEnergy();
  phoIdMva_R9_             = photon->r9();
  phoIdMva_covIEtaIEta_    = photon->sigmaIetaIeta();
  phoIdMva_EtaWidth_       = photon->superCluster()->etaWidth();
  phoIdMva_PhiWidth_       = photon->superCluster()->phiWidth();
  phoIdMva_covIEtaIPhi_    = photon->getSieip();                        
  phoIdMva_pfPhoIso03_     = photon->getpfPhoIso03();
  phoIdMva_pfChIso03_      = photon->getpfChIso03();
  phoIdMva_pfChIso03worst_ = photon->getpfChIso03worst();
  phoIdMva_ScEta_          = photon->superCluster()->eta();
  phoIdMva_rho_            = photon->r9(); // THIS SHOULD BECOME RHO AT SOME POINT
  phoIdMva_ESEffSigmaRR_   = photon->r9(); // THIS SHOULD BECOME ESEFFSIGMARR AT SOME POINT

 
  if( photon->isEB() )      phoIdMva = phoIdMva_2012_EB_;
  else if( photon->isEE() ) phoIdMva = phoIdMva_2012_EE_;
  
  float mvavalue = phoIdMva->EvaluateMVA("BDT");
  return mvavalue;
}

std::vector<float> PhotonIdUtils::computeMVAWrtAllVtx(  const edm::Ptr<flashgg::Photon>& photon, 
							const edm::PtrVector<reco::Vertex>& vertices
							)
{
  
  std::vector<float> mvavalues;

  /*for( int i=0) {

    float mva = 0;
    mvavalues.push_back(mva);
    }*/

  return mvavalues;

}
