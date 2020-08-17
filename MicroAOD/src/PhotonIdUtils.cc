#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
/// #include <tuple>

using namespace std;
using namespace reco;
using namespace flashgg;

PhotonIdUtils::PhotonIdUtils( OverlapRemovalAlgo *algo) :
    overlapAlgo_( algo ), removeOverlappingCandidates_( true ), deltaPhiRotation_( 0. )
{

}

PhotonIdUtils::~PhotonIdUtils()
{
};

// *****************************************************************************************************************
//                    PHOTON isolation
// *****************************************************************************************************************

bool PhotonIdUtils::vetoPackedCand( const pat::Photon &photon, const edm::Ptr<pat::PackedCandidate> &pfcand )
{
    edm::RefVector<pat::PackedCandidateCollection> associated =  photon.associatedPackedPFCandidates();

    int nass = 0;
    for( unsigned int ipc = 0; ipc < associated.size(); ipc++ ) {
        edm::Ptr<pat::PackedCandidate> associatedPtr = edm::refToPtr( associated[ipc] );
        if( associatedPtr == pfcand )  { nass++; break; }
    }

    return ( nass > 0 );
}

float PhotonIdUtils::pfIsoChgWrtVtx( const edm::Ptr<pat::Photon> &photon,
                                     const edm::Ptr<reco::Vertex> vtx,
                                     const flashgg::VertexCandidateMap vtxcandmap,
                                     float coneSize, float coneVetoBarrel, float coneVetoEndcap,
                                     float ptMin
                                   )
{
    float isovalue = 0;

    float coneVeto = 0;
    if( photon->isEB() )      { coneVeto = coneVetoBarrel; }
    else if( photon->isEE() ) { coneVeto = coneVetoEndcap; }


    math::XYZVector SCdirection( photon->superCluster()->x() - vtx->x(),
                                 photon->superCluster()->y() - vtx->y(),
                                 photon->superCluster()->z() - vtx->z()
                               );

    auto mapRange = std::equal_range( vtxcandmap.begin(), vtxcandmap.end(), vtx, flashgg::compare_with_vtx() );
    if( mapRange.first == mapRange.second ) { return -1.; } // no entries for this vertex
    for( auto pair_iter = mapRange.first ; pair_iter != mapRange.second ; pair_iter++ ) {
        edm::Ptr<pat::PackedCandidate> pfcand = pair_iter->second;

        if( abs( pfcand->pdgId() ) == 11 || abs( pfcand->pdgId() ) == 13 ) { continue; } //J. Tao not e/mu
        if( removeOverlappingCandidates_ &&
                ( ( overlapAlgo_ == 0 &&  vetoPackedCand( *photon, pfcand ) ) ||
                  ( overlapAlgo_ != 0 && ( *overlapAlgo_ )( *photon, pfcand ) ) ) ) { continue; }


        if( pfcand->pt() < ptMin )         { continue; }
        float dRTkToVtx  = deltaR( pfcand->momentum().Eta(), pfcand->momentum().Phi(),
                                   SCdirection.Eta(), SCdirection.Phi() + deltaPhiRotation_ ); // rotate SC in phi if requested (random cone isolation)
        if( dRTkToVtx > coneSize || dRTkToVtx < coneVeto ) { continue; }

        isovalue += pfcand->pt();
    }
    return isovalue;

}


map<edm::Ptr<reco::Vertex>, float> PhotonIdUtils::pfIsoChgWrtAllVtx( const edm::Ptr<pat::Photon> &photon,
        const std::vector<edm::Ptr<reco::Vertex> > &vertices,
        const flashgg::VertexCandidateMap vtxcandmap,
        float coneSize, float coneVetoBarrel, float coneVetoEndcap,
        float ptMin )
{
    map<edm::Ptr<reco::Vertex>, float> isomap;
    isomap.clear();

    for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {

        float iso = pfIsoChgWrtVtx( photon, vertices[iv], vtxcandmap, coneSize, coneVetoBarrel, coneVetoEndcap, ptMin );
        isomap.insert( make_pair( vertices[iv], iso ) );
    }

    return isomap;
}



float PhotonIdUtils::pfIsoChgWrtWorstVtx( map<edm::Ptr<reco::Vertex>, float> &vtxIsoMap )
{
    float MaxValueMap = -1000;
    float itValue = 0;

    for( map<edm::Ptr<reco::Vertex>, float>::iterator it = vtxIsoMap.begin(); it != vtxIsoMap.end(); ++it ) {
        itValue = it->second;
        if( itValue > MaxValueMap ) { MaxValueMap = itValue; }
    }

    return MaxValueMap;
}



float PhotonIdUtils::pfCaloIso( const edm::Ptr<pat::Photon> &photon,
                                const std::vector<edm::Ptr<pat::PackedCandidate> > &pfcandidates,
                                float dRMax,
                                float dRVetoBarrel,
                                float dRVetoEndcap,
                                float etaStripBarrel,
                                float etaStripEndcap,
                                float minEnergyBarrel,
                                float minEnergyEndcap,
                                reco::PFCandidate::ParticleType type,
                                const reco::Vertex *vtx
                              )
{
    // just used to translate particle types to pdgId. Sure: it's very hugly...
    static reco::PFCandidate helper;
    int pdgId = helper.translateTypeToPdgId( type );

    float isovalue = 0;

    float dRVeto = 99;
    float maxetaStrip = 99;

    if( photon->isEB() ) {
        dRVeto        = dRVetoBarrel;
        maxetaStrip  = etaStripBarrel;
    } else if( photon->isEE() ) {
        dRVeto        = dRVetoEndcap;
        maxetaStrip  = etaStripEndcap;
    }

    //// map<float,tuple<edm::Ptr<pat::PackedCandidate>,float,float> > candidates;
    for( size_t ipf = 0; ipf < pfcandidates.size(); ipf++ ) {

        edm::Ptr<pat::PackedCandidate> pfcand = pfcandidates[ipf];

        if( pfcand->pdgId() != pdgId ) { continue; }
        if( photon->isEB() ) if( fabs( pfcand->pt() ) < minEnergyBarrel )     { continue; }
        if( photon->isEE() ) if( fabs( pfcand->energy() ) < minEnergyEndcap ) { continue; }

        if( removeOverlappingCandidates_ && vetoPackedCand( *photon, pfcand ) ) { continue; }

        double vx, vy, vz;
        if( vtx ) {
            vx = vtx->x(), vy = vtx->y(), vz = vtx->z();
        } else {
            math::XYZPoint  pfcandvtx = pfcand->vertex();
            vx = pfcandvtx.x(), vy = pfcandvtx.y(), vz = pfcandvtx.z();
        }
        math::XYZVector SCdirectionWrtCandVtx( photon->superCluster()->x() - vx,
                                               photon->superCluster()->y() - vy,
                                               photon->superCluster()->z() - vz
                                             );

        float dEta = fabs( SCdirectionWrtCandVtx.Eta() - pfcand->momentum().Eta() );
        float dR   = deltaR( SCdirectionWrtCandVtx.Eta(), SCdirectionWrtCandVtx.Phi(), pfcand->momentum().Eta(), pfcand->momentum().Phi() );
        //// float dPhi = deltaPhi( SCdirectionWrtCandVtx.Phi(),pfcand->momentum().Phi() );

        if( dEta < maxetaStrip )        { continue; }
        if( dR < dRVeto || dR > dRMax ) { continue; }

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

void PhotonIdUtils::setupMVA( const string &xmlfilenameEB, const string &xmlfilenameEE, bool useNewPhoId, bool is2017 )
{

    // **** BDT 2017 EB ****

    string mvamethod = "BDT";

    phoIdMva_EB_ = make_shared<TMVA::Reader>( "!Color:Silent" );

    phoIdMva_EB_->AddVariable( "SCRawE", &phoIdMva_SCRawE_ );
    phoIdMva_EB_->AddVariable( "r9",                &phoIdMva_R9_ );
    phoIdMva_EB_->AddVariable( "sigmaIetaIeta",       &phoIdMva_covIEtaIEta_ );
    phoIdMva_EB_->AddVariable( "etaWidth",        &phoIdMva_EtaWidth_ );
    phoIdMva_EB_->AddVariable( "phiWidth",        &phoIdMva_PhiWidth_ );
    phoIdMva_EB_->AddVariable( "covIEtaIPhi", &phoIdMva_covIEtaIPhi_ );
    phoIdMva_EB_->AddVariable( "s4",     &phoIdMva_S4_ );
    phoIdMva_EB_->AddVariable( "phoIso03",    &phoIdMva_pfPhoIso03_ );
    phoIdMva_EB_->AddVariable( "chgIsoWrtChosenVtx",   &phoIdMva_pfChgIso03_ );
    phoIdMva_EB_->AddVariable( "chgIsoWrtWorstVtx", &phoIdMva_pfChgIso03worst_ );
    phoIdMva_EB_->AddVariable( "scEta",             &phoIdMva_ScEta_ );
    phoIdMva_EB_->AddVariable( "rho",                  &phoIdMva_rho_ );
    phoIdMva_EB_->BookMVA( mvamethod.c_str(), xmlfilenameEB );

    // **** BDT 2017 EE ****

    phoIdMva_EE_ = make_shared<TMVA::Reader>( "!Color:Silent" );

    phoIdMva_EE_->AddVariable( "SCRawE", &phoIdMva_SCRawE_ );
    phoIdMva_EE_->AddVariable( "r9",                &phoIdMva_R9_ );
    phoIdMva_EE_->AddVariable( "sigmaIetaIeta",       &phoIdMva_covIEtaIEta_ );
    phoIdMva_EE_->AddVariable( "etaWidth",        &phoIdMva_EtaWidth_ );
    phoIdMva_EE_->AddVariable( "phiWidth",        &phoIdMva_PhiWidth_ );
    phoIdMva_EE_->AddVariable( "covIEtaIPhi", &phoIdMva_covIEtaIPhi_ );
    phoIdMva_EE_->AddVariable( "s4",     &phoIdMva_S4_ );
    //-----Variable names in MVA weight file different for 2016, make sure same value gets picked up
    if (is2017)
        phoIdMva_EE_->AddVariable( "phoIso03",    &phoIdMva_pfPhoIso03_ );
    else
        phoIdMva_EE_->AddVariable( "isoPhoCorrMax2p5",    &phoIdMva_pfPhoIso03_ );
    phoIdMva_EE_->AddVariable( "chgIsoWrtChosenVtx",   &phoIdMva_pfChgIso03_ );
    phoIdMva_EE_->AddVariable( "chgIsoWrtWorstVtx", &phoIdMva_pfChgIso03worst_ );
    phoIdMva_EE_->AddVariable( "scEta",             &phoIdMva_ScEta_ );
    phoIdMva_EE_->AddVariable( "rho",                  &phoIdMva_rho_ );
    phoIdMva_EE_->AddVariable( "esEffSigmaRR",   &phoIdMva_ESEffSigmaRR_ );
    if(is2017) 
        phoIdMva_EE_->AddVariable( "esEnergyOverRawE",   &phoIdMva_esEnovSCRawEn_ );
    else
        phoIdMva_EE_->AddVariable( "esEnergy/SCRawE",   &phoIdMva_esEnovSCRawEn_ );
    phoIdMva_EE_->BookMVA( mvamethod.c_str(), xmlfilenameEE );

}


float PhotonIdUtils::computeMVAWrtVtx( /*edm::Ptr<flashgg::Photon>& photon,*/
    flashgg::Photon &photon,
    const edm::Ptr<reco::Vertex> &vtx,
    const double rho, const double eA, const std::vector<double> _phoIsoPtScalingCoeff, const double _phoIsoCutoff )
{

    phoIdMva_SCRawE_          = photon.superCluster()->rawEnergy();
    phoIdMva_R9_              = photon.full5x5_r9();
    phoIdMva_S4_              = photon.s4();
    phoIdMva_covIEtaIEta_     = photon.full5x5_sigmaIetaIeta();
    phoIdMva_EtaWidth_        = photon.hasUserFloat("etaWidth") ? photon.userFloat("etaWidth") : photon.superCluster()->etaWidth();
    phoIdMva_PhiWidth_        = photon.hasUserFloat("phiWidth") ? photon.userFloat("phiWidth") : photon.superCluster()->phiWidth();
    phoIdMva_covIEtaIPhi_     = photon.sieip();
    phoIdMva_pfPhoIso03_      = photon.pfPhoIso03();

    //pho iso corr in 2016 for endcap
    phoIdMva_pfPhoIso03Corr_ = photon.pfPhoIso03();

    //    double eA = _effectiveAreas.getEffectiveArea( abs(photon.superCluster()->eta()) );
    double phoIsoPtScalingCoeffVal = 0;
    if( photon.isEB() ) 
        phoIsoPtScalingCoeffVal = _phoIsoPtScalingCoeff.at(0); // barrel case
    else
        phoIsoPtScalingCoeffVal =  _phoIsoPtScalingCoeff.at(1); //endcap case
    
    double phoIsoCorr = photon.pfPhoIso03() - eA*(rho) - phoIsoPtScalingCoeffVal*photon.pt();
    
    phoIdMva_pfPhoIso03Corr_ = TMath::Max(phoIsoCorr, _phoIsoCutoff);
    
    phoIdMva_pfChgIso03_      = photon.pfChgIso03WrtVtx( vtx );
    phoIdMva_pfChgIso03worst_ = photon.pfChgIsoWrtWorstVtx03();
    phoIdMva_ScEta_           = photon.superCluster()->eta();
    phoIdMva_rho_             = rho; // we don't want to add the event-based rho as flashgg::photon member
    phoIdMva_ESEffSigmaRR_    = photon.esEffSigmaRR();
    phoIdMva_esEnovSCRawEn_   = photon.superCluster()->preshowerEnergy()/photon.superCluster()->rawEnergy();
        
    if( photon.isEB() )      { phoIdMva = phoIdMva_EB_; }
    else if( photon.isEE() ) { phoIdMva = phoIdMva_EE_; }

    float mvavalue = phoIdMva->EvaluateMVA( "BDT" );
    return mvavalue;
}

float PhotonIdUtils::computeCorrectPhoIso(    
                                          flashgg::Photon &photon,
                                          const double rho, const double eA, const std::vector<double> _phoIsoPtScalingCoeff, const double _phoIsoCutoff )
{
    //pho iso corr in 2016 for endcap
    pfPhoIso03Corr_ = photon.pfPhoIso03();
     
    //    double eA = _effectiveAreas.getEffectiveArea( abs(photon.superCluster()->eta()) );
    double phoIsoPtScalingCoeffVal = 0;
    if( photon.isEB() ) 
        phoIsoPtScalingCoeffVal = _phoIsoPtScalingCoeff.at(0); // barrel case
    else
        phoIsoPtScalingCoeffVal =  _phoIsoPtScalingCoeff.at(1); //endcap case
    
    double phoIsoCorr = photon.pfPhoIso03() - eA*(rho) - phoIsoPtScalingCoeffVal*photon.pt();
    
    pfPhoIso03Corr_ = TMath::Max(phoIsoCorr, _phoIsoCutoff);
    
    float pfPhoIso03Corr = pfPhoIso03Corr_;
    return pfPhoIso03Corr;
}

map<edm::Ptr<reco::Vertex>, float> PhotonIdUtils::computeMVAWrtAllVtx( /*edm::Ptr<flashgg::Photon>& photon,*/
    flashgg::Photon &photon,
    const std::vector<edm::Ptr<reco::Vertex> > &vertices,
    const double rho, const double eA, const std::vector<double> _phoIsoPtScalingCoeff, const double _phoIsoCutoff )

{
    map<edm::Ptr<reco::Vertex>, float> mvamap;
    mvamap.clear();

    for( unsigned int iv = 0; iv < vertices.size(); iv++ ) {
        edm::Ptr<reco::Vertex> vertex = vertices[iv];
        float mvapervtx = computeMVAWrtVtx( photon, vertex, rho, eA, _phoIsoPtScalingCoeff, _phoIsoCutoff );
        mvamap.insert( make_pair( vertex, mvapervtx ) );
    }

    return mvamap;
}



flashgg::Photon PhotonIdUtils::pho4MomCorrection( edm::Ptr<flashgg::Photon> &photon,
        edm::Ptr<reco::Vertex> vtx )
{

    /// vtx should be the chosen vtx, not primary vtx ///
    float vtx_X = vtx->x();
    float vtx_Y = vtx->y();
    float vtx_Z = vtx->z();

    float sc_X = photon->superCluster()->x();
    float sc_Y = photon->superCluster()->y();
    float sc_Z = photon->superCluster()->z();

    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );

    math::XYZVector direction = sc_Pos - vtx_Pos;
    math::XYZVector p = ( direction.Unit() ) * ( photon->energy() );
    math::XYZTLorentzVector corrected_p4( p.x(), p.y(), p.z(), photon->energy() );
    flashgg::Photon p4CorrPho = *photon;
    p4CorrPho.setP4( corrected_p4 );
    return p4CorrPho;
}

void PhotonIdUtils::recomputeNonZsClusterShapes( reco::Photon &pho, noZS::EcalClusterLazyTools &tools )
{
    reco::SuperClusterRef scRef = pho.superCluster();

    float maxXtal =   tools.eMax( *( scRef->seed() ) );
    //AA
    //Change these to consider severity level of hits
    float e1x5    =   tools.e1x5( *( scRef->seed() ) );
    float e2x5    =   tools.e2x5Max( *( scRef->seed() ) );
    float e3x3    =   tools.e3x3( *( scRef->seed() ) );
    float e5x5    =   tools.e5x5( *( scRef->seed() ) );
    std::vector<float> cov =  tools.covariances( *( scRef->seed() ) );
    std::vector<float> locCov =  tools.localCovariances( *( scRef->seed() ) );

    reco::Photon::ShowerShape  showerShape;
    showerShape.e1x5 = e1x5;
    showerShape.e2x5 = e2x5;
    showerShape.e3x3 = e3x3;
    showerShape.e5x5 = e5x5;
    showerShape.maxEnergyXtal =  maxXtal;
    showerShape.sigmaIetaIeta =  sqrt( locCov[0] );
    showerShape.sigmaEtaEta =  sqrt( cov[0] );
    showerShape.sigmaIetaIphi =  sqrt( locCov[1] );
    showerShape.sigmaIphiIphi =  sqrt( locCov[2] );

    pho.full5x5_setShowerShapeVariables( showerShape );


}


void PhotonIdUtils::recomputeNonZsClusterShapes( reco::Photon &pho, const EcalRecHitCollection *ebRecHists, const EcalRecHitCollection *eeRecHist,
        const CaloTopology *topology )
{
    noZS::EcalClusterTools tools;

    reco::SuperClusterRef scRef = pho.superCluster();

    int subdetId = scRef->seed()->hitsAndFractions()[0].first.subdetId();
    const EcalRecHitCollection *hits = ( subdetId == EcalBarrel ? ebRecHists : eeRecHist );

    float maxXtal =   noZS::EcalClusterTools::eMax( *( scRef->seed() ), &( *hits ) );
    //AA
    //Change these to consider severity level of hits
    float e1x5    =   noZS::EcalClusterTools::e1x5( *( scRef->seed() ), &( *hits ), &( *topology ) );
    float e2x5    =   noZS::EcalClusterTools::e2x5Max( *( scRef->seed() ), &( *hits ), &( *topology ) );
    float e3x3    =   noZS::EcalClusterTools::e3x3( *( scRef->seed() ), &( *hits ), &( *topology ) );
    float e5x5    =   noZS::EcalClusterTools::e5x5( *( scRef->seed() ), &( *hits ), &( *topology ) );
    std::vector<float> locCov =  noZS::EcalClusterTools::localCovariances( *( scRef->seed() ), &( *hits ), &( *topology ) );

    reco::Photon::ShowerShape  showerShape;
    showerShape.e1x5 = e1x5;
    showerShape.e2x5 = e2x5;
    showerShape.e3x3 = e3x3;
    showerShape.e5x5 = e5x5;
    showerShape.maxEnergyXtal =  maxXtal;
    showerShape.sigmaIetaIeta =  sqrt( locCov[0] );
    showerShape.sigmaIetaIphi =  sqrt( locCov[1] );
    showerShape.sigmaIphiIphi =  sqrt( locCov[2] );
    pho.full5x5_setShowerShapeVariables( showerShape );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

