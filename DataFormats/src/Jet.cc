#include "flashgg/DataFormats/interface/Jet.h"

using namespace flashgg;

Jet::Jet() : pat::Jet()
{
    simpleRMS_ = -1.;
    qglikelihood_ = -999.;
    simpleMVA_ = -999.;
    puJetId_.clear();
}

Jet::Jet( const pat::Jet &aJet ) : pat::Jet( aJet )
{
}

Jet::~Jet() {}

void Jet::setPuJetId( const edm::Ptr<reco::Vertex> vtx, const PileupJetIdentifier &id )
{
    MinimalPileupJetIdentifier min_id;
    min_id.RMS = id.RMS();
    min_id.betaStar = id.betaStar();
    min_id.idFlag = id.idFlag();
    puJetId_.insert( std::make_pair( vtx, min_id ) );
}

bool Jet::hasPuJetId( const edm::Ptr<reco::Vertex> vtx ) const
{
    //    return ( puJetId_.count( vtx ) > 0 );
    return true;
}

bool Jet::passesPuJetId( const edm::Ptr<reco::Vertex> vtx, PileupJetIdentifier::Id level ) const
{
    assert( hasPuJetId( vtx ) );
    //    return PileupJetIdentifier::passJetId( puJetId_.at( vtx ).idFlag, level );
    return true;
}

float Jet::rms( const edm::Ptr<reco::Vertex> vtx ) const
{
    assert( hasPuJetId( vtx ) );
    //    return puJetId_.at( vtx ).RMS;
    return simpleRMS_;
}

float Jet::betaStar( const edm::Ptr<reco::Vertex> vtx ) const
{
    assert( hasPuJetId( vtx ) );
    //    return puJetId_.at( vtx ).betaStar;
    return -1.;
}

bool Jet::passesPuJetId( const edm::Ptr<DiPhotonCandidate> dipho, PileupJetIdentifier::Id level ) const
{
    return passesPuJetId( dipho->vtx(), level );
}

float Jet::rms( const edm::Ptr<DiPhotonCandidate> dipho ) const
{
    return rms( dipho->vtx() );
}

float Jet::betaStar( const edm::Ptr<DiPhotonCandidate> dipho ) const
{
    return betaStar( dipho->vtx() );
}

bool Jet::passesJetID( JetIDLevel level) const
{
    float eta      = this->eta();
    float NHF      = this->neutralHadronEnergyFraction();
    float NEMF     = this->neutralEmEnergyFraction();
    float CHF      = this->chargedHadronEnergyFraction();
    //float MUF      = this->muonEnergyFraction();
    float CEMF     = this->chargedEmEnergyFraction();
    int   NumConst = this->chargedMultiplicity()+this->neutralMultiplicity();
    int   CHM      = this->chargedMultiplicity();
    int   NumNeutralParticles = this->neutralMultiplicity();
    
    //std::cout  << "DEBUG:: eta= " << eta << " NHF=" << NHF << std::endl;
    
    bool jetID_barel_loose  =  (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((abs(eta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || abs(eta)>2.4) && abs(eta)<=3.0;
    bool jetID_barel_tight  =  (NHF<0.90 && NEMF<0.90 && NumConst>1) && ((abs(eta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || abs(eta)>2.4) && abs(eta)<=3.0;
    bool jetID_foaward      =  (NEMF<0.90 && NumNeutralParticles >10 && abs(eta)>3.0 );
    
    switch(level){
    case Loose:
        {
            if(abs(eta)<=3.0 ) return jetID_barel_loose;
            if(abs(eta)> 3.0 ) return jetID_foaward;
            
        }break;
    case Tight:
        {
            if(abs(eta)<=3.0 ) return jetID_barel_tight;
            if(abs(eta)> 3.0 ) return jetID_foaward;
        }break;
    default:
        {
            std::cout << "error:: wrong level !!" << std::endl;
        }
        break;
    }
    return 0;
    
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

