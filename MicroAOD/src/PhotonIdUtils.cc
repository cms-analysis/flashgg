#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

/// #include <tuple>

using namespace std;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils() : removeOverlappingCandidates_(true) {};
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

bool PhotonIdUtils::vetoPackedCand(const pat::Photon& photon, const edm::Ptr<pat::PackedCandidate> & pfcand)
{
    edm::RefVector<pat::PackedCandidateCollection> associated =  photon.associatedPackedPFCandidates();
    
    int nass = 0;
    for( unsigned int ipc = 0; ipc < associated.size(); ipc++ ) {
      edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated[ipc] );
      if( associatedPtr == pfcand )  { nass++; break; }
    }
    
    return (nass > 0);
}

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

      if( abs(pfcand->pdgId()) == 11 || abs(pfcand->pdgId()) == 13 ) continue; //J. Tao not e/mu       
      if( removeOverlappingCandidates_ && vetoPackedCand(*photon,pfcand) ) { continue; }
      
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


map<edm::Ptr<reco::Vertex>,float> PhotonIdUtils::pfIsoChgWrtAllVtx( edm::Ptr<pat::Photon>& photon, 
									 const edm::PtrVector<reco::Vertex>& vertices,
									 const flashgg::VertexCandidateMap vtxcandmap,
									 float coneSize, float coneVetoBarrel, float coneVetoEndcap, 
									 float ptMin )
{
  map<edm::Ptr<reco::Vertex>,float> isomap;
  isomap.clear();

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    
    float iso = pfIsoChgWrtVtx( photon, vertices[iv], vtxcandmap, coneSize, coneVetoBarrel, coneVetoEndcap, ptMin );
    isomap.insert( make_pair(vertices[iv],iso) );
  }

  return isomap;
}



float PhotonIdUtils::pfIsoChgWrtWorstVtx( map<edm::Ptr<reco::Vertex>,float>& vtxIsoMap )
{
  float MaxValueMap = -1000;  
  float itValue = 0;
  
  for (map<edm::Ptr<reco::Vertex>,float>::iterator it=vtxIsoMap.begin(); it!=vtxIsoMap.end(); ++it) {
    itValue = it->second;
    if (itValue > MaxValueMap) MaxValueMap = itValue;
  }

  return MaxValueMap;
}



float PhotonIdUtils::pfCaloIso( edm::Ptr<pat::Photon>& photon, 
				const edm::PtrVector<pat::PackedCandidate>& pfcandidates,
				float dRMax,
				float dRVetoBarrel,
				float dRVetoEndcap,
				float etaStripBarrel,
				float etaStripEndcap, 
				float minEnergyBarrel, 
				float minEnergyEndcap,
				reco::PFCandidate::ParticleType type,
				const reco::Vertex * vtx
	)
{
  // just used to translate particle types to pdgId. Sure: it's very hugly...
  static reco::PFCandidate helper;
  int pdgId = helper.translateTypeToPdgId(type);
  
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

  //// map<float,tuple<edm::Ptr<pat::PackedCandidate>,float,float> > candidates;
  for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) { 

    edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf]; 
    
    if( pfcand->pdgId() != pdgId ) continue;
    if( photon->isEB() ) if( fabs(pfcand->pt()) < minEnergyBarrel )     continue;  
    if( photon->isEE() ) if( fabs(pfcand->energy()) < minEnergyEndcap ) continue;
    
    if( removeOverlappingCandidates_ && vetoPackedCand(*photon,pfcand) ) { continue; }
    
    double vx, vy, vz;
    if( vtx ) {
	    vx=vtx->x(), vy=vtx->y(), vz=vtx->z();
    } else {
	    math::XYZPoint  pfcandvtx = pfcand->vertex();
	    vx=pfcandvtx.x(), vy=pfcandvtx.y(), vz=pfcandvtx.z();
    }
    math::XYZVector SCdirectionWrtCandVtx( photon->superCluster()->x() - vx,
					   photon->superCluster()->y() - vy,
					   photon->superCluster()->z() - vz 
					   );

    float dEta = fabs( SCdirectionWrtCandVtx.Eta() - pfcand->momentum().Eta() );
    float dR   = deltaR( SCdirectionWrtCandVtx.Eta(), SCdirectionWrtCandVtx.Phi(), pfcand->momentum().Eta(), pfcand->momentum().Phi() ); 
    //// float dPhi = deltaPhi( SCdirectionWrtCandVtx.Phi(),pfcand->momentum().Phi() );

    if( dEta < maxetaStrip )        continue;
    if( dR < dRVeto || dR > dRMax ) continue;
      
    isovalue += pfcand->pt();
    //// candidates[dR] = make_tuple(pfcand,dEta,dPhi);
  }

  //// if( isovalue >= photon->pt() ) {
  //// 	  cout << "isolation value larger than photon pt " <<  photon->pt() << ". Dumping candidates. " <<endl;
  //// 	  int nmax = 2;
  //// 	  for(auto & ci : candidates) {
  //// 		  auto & cand = get<0>(ci.second);
  //// 		  auto & dEta = get<1>(ci.second);
  //// 		  auto & dPhi = get<2>(ci.second);
  //// 		  cout << "dR " << ci.first << " dEta " << dEta << " " << dPhi << " pt " <<  cand->pt() << " dz " << cand->dz() << " dxy " << cand->dxy() << endl;
  //// 		  if( --nmax == 0 ) { break; }
  //// 	  }
  //// }

  return isovalue;
}

// *****************************************************************************************************************
//                    PHOTON MVA CALCULATION
// *****************************************************************************************************************

/* to do:
- configurable input file
- decide what to do with EE and EB
*/

void PhotonIdUtils::setupMVA( const string& xmlfilenameEB, const string& xmlfilenameEE )
{

  //  cout << " photonId MVA weights for EB set from file: " << xmlfilenameEB << endl;
  //  cout << " photonId MVA weights for EE set from file: " << xmlfilenameEE << endl;
  // **** bdt 2012 EB ****
 
  string mvamethod = "BDT";
  
  phoIdMva_2012_EB_ = make_shared<TMVA::Reader>("!Color:Silent");
 
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

  phoIdMva_2012_EE_ = make_shared<TMVA::Reader>("!Color:Silent");
  
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

map<edm::Ptr<reco::Vertex>,float> PhotonIdUtils::computeMVAWrtAllVtx( /*edm::Ptr<flashgg::Photon>& photon,*/
									  flashgg::Photon& photon,
									  const edm::PtrVector<reco::Vertex>& vertices,
									  const double rho )
  
{  
  map<edm::Ptr<reco::Vertex>,float> mvamap;
  mvamap.clear();

  for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
    edm::Ptr<reco::Vertex> vertex = vertices[iv];
    float mvapervtx = computeMVAWrtVtx( photon, vertex, rho);
    mvamap.insert( make_pair(vertex, mvapervtx) );
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
  flashgg::Photon p4CorrPho = *photon;
  p4CorrPho.setP4(corrected_p4);
  return p4CorrPho;
}

void PhotonIdUtils::recomputeNonZsClusterShapes(reco::Photon & pho, noZS::EcalClusterLazyTools & tools )
{
	reco::SuperClusterRef scRef=pho.superCluster();
	
	float maxXtal =   tools.eMax( *(scRef->seed()) );
	//AA
	//Change these to consider severity level of hits
	float e1x5    =   tools.e1x5(  *(scRef->seed()) );
	float e2x5    =   tools.e2x5Max(  *(scRef->seed()) );
	float e3x3    =   tools.e3x3(  *(scRef->seed()) );
	float e5x5    =   tools.e5x5( *(scRef->seed()) );
	std::vector<float> cov =  tools.covariances( *(scRef->seed()) );
	std::vector<float> locCov =  tools.localCovariances( *(scRef->seed()) );
	
	reco::Photon::ShowerShape  showerShape;
	showerShape.e1x5= e1x5;
	showerShape.e2x5= e2x5;
	showerShape.e3x3= e3x3;
	showerShape.e5x5= e5x5;
	showerShape.maxEnergyXtal =  maxXtal;
	showerShape.sigmaIetaIeta =  sqrt(locCov[0]);
	showerShape.sigmaEtaEta =  sqrt(cov[0]);
		
	pho.full5x5_setShowerShapeVariables(showerShape);
	

}


void PhotonIdUtils::recomputeNonZsClusterShapes(reco::Photon & pho, const EcalRecHitCollection* ebRecHists, const EcalRecHitCollection * eeRecHist, const CaloTopology * topology)
{
	noZS::EcalClusterTools tools;
		
	reco::SuperClusterRef scRef=pho.superCluster();

	int subdetId = scRef->seed()->hitsAndFractions()[0].first.subdetId();
	const EcalRecHitCollection * hits = (subdetId == EcalBarrel ? ebRecHists : eeRecHist);

	float maxXtal =   noZS::EcalClusterTools::eMax( *(scRef->seed()), &(*hits) );
	//AA
	//Change these to consider severity level of hits
	float e1x5    =   noZS::EcalClusterTools::e1x5(  *(scRef->seed()), &(*hits), &(*topology));
	float e2x5    =   noZS::EcalClusterTools::e2x5Max(  *(scRef->seed()), &(*hits), &(*topology));    
	float e3x3    =   noZS::EcalClusterTools::e3x3(  *(scRef->seed()), &(*hits), &(*topology));
	float e5x5    =   noZS::EcalClusterTools::e5x5( *(scRef->seed()), &(*hits), &(*topology));   
	std::vector<float> locCov =  noZS::EcalClusterTools::localCovariances( *(scRef->seed()), &(*hits), &(*topology));
	
	reco::Photon::ShowerShape  showerShape;
	showerShape.e1x5= e1x5;
	showerShape.e2x5= e2x5;
	showerShape.e3x3= e3x3;
	showerShape.e5x5= e5x5;
	showerShape.maxEnergyXtal =  maxXtal;
	showerShape.sigmaIetaIeta =  sqrt(locCov[0]);
		
	pho.full5x5_setShowerShapeVariables(showerShape);
}

