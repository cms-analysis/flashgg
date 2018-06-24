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
    
    //    std::cout  << "DEBUG:: eta= " << eta << " NHF=" << NHF << " NEMF=" << NEMF << " CHF=" << CHF << " CEMF=" << CEMF 
    //               << " NumConst=" << NumConst << " CHM=" << CHM << " NumNeutralParticles=" << NumNeutralParticles << std::endl;
    
    bool jetID_barrel_loose  =  (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((fabs(eta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || fabs(eta)>2.4) && fabs(eta)<=2.7;
    bool jetID_barrel_tight  =  (NHF<0.90 && NEMF<0.90 && NumConst>1) && ((fabs(eta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || fabs(eta)>2.4) && fabs(eta)<=2.7;
    bool jetID_transition      =  (NEMF>0.01 && NHF<0.98 && NumNeutralParticles>2 && fabs(eta)>2.7 && fabs(eta)<3.0);
    bool jetID_forward      =  (NEMF<0.90 && NumNeutralParticles >10 && fabs(eta)>3.0 );
    
    bool jetID_2017_27 = (NHF < 0.9 && NEMF < 0.9 && NumConst > 1);
    bool jetID_2017_24 = jetID_2017_27 && (CHF > 0. && CHM > 0 );
    bool jetID_2017_30 = (NEMF > 0.02 && NEMF < 0.99 && NumNeutralParticles > 2);
    bool jetID_2017_forward = (NEMF < 0.9 && NHF > 0.02 && NumNeutralParticles > 10);
    
    switch(level){
    case Loose:
        {
            if(fabs(eta)<=2.7 ) return jetID_barrel_loose;
            if(fabs(eta)<=3.0 ) return jetID_transition;
            if(fabs(eta)> 3.0 ) return jetID_forward;
            
        }break;
    case Tight:
        {
            if(fabs(eta)<=2.7 ) return jetID_barrel_tight;
            if(fabs(eta)<=3.0 ) return jetID_transition;
            if(fabs(eta)> 3.0 ) return jetID_forward;
        }break;
    case Tight2017:
        {
            if(fabs(eta)<2.4 ) return jetID_2017_24;
            if(fabs(eta)<2.7 ) return jetID_2017_27;
            if(fabs(eta)<3.0 ) return jetID_2017_30;
            return jetID_2017_forward;
        } break;
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

