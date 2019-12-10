#include "flashgg/DataFormats/interface/DoubleHTag.h"

using namespace flashgg;

DoubleHTag::DoubleHTag() : DiPhotonTagBase::DiPhotonTagBase(), mva_(-2.) 
{  
}

DoubleHTag::~DoubleHTag() {}

DoubleHTag::DoubleHTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<flashgg::Jet> leadJet, edm::Ptr<flashgg::Jet> subleadJet )
    : mva_(-.2), MX_(0.),genMhh_(0.),genCosThetaStar_CS_(0.),leadJet_(leadJet), subleadJet_(subleadJet) 
 
{
    dipho_ = diPho;
    dijet_ = leadJet_->p4() + subleadJet_->p4();
    this->setP4( dijet_ + dipho_->p4() );
}

DoubleHTag *DoubleHTag::clone() const
{
    DoubleHTag *result = new DoubleHTag( *this );
    return result;
}

float DoubleHTag::getCosThetaStar_CS() const {

    LorentzVector hh_lor = diPhoton()->p4() + dijet();
    TLorentzVector hh;
    hh.SetPxPyPzE(hh_lor.Px(),hh_lor.Py(),hh_lor.Pz(),hh_lor.E()) ;

    LorentzVector h1_lor = diPhoton()->p4();
    TLorentzVector h_1;
    h_1.SetPxPyPzE(h1_lor.Px(),h1_lor.Py(),h1_lor.Pz(),h1_lor.E()) ; 

    h_1.Boost(-hh.BoostVector());   

    return h_1.CosTheta();
}

float DoubleHTag::getCosThetaStar_CS_old(float ebeam) const {
    // cos theta star angle in the Collins Soper frame
    // Copied directly from here: https://github.com/ResonantHbbHgg/Selection/blob/master/selection.h#L3367-L3385
    TLorentzVector p1, p2;
    p1.SetPxPyPzE(0, 0,  ebeam, ebeam);
    p2.SetPxPyPzE(0, 0, -ebeam, ebeam);

    LorentzVector hh_lor = diPhoton()->p4() + dijet();
    TLorentzVector hh;
    hh.SetPxPyPzE(hh_lor.Px(),hh_lor.Py(),hh_lor.Pz(),hh_lor.E()) ;

    TVector3 boost = - hh.BoostVector();
    p1.Boost(boost);
    p2.Boost(boost);
    LorentzVector h1_lor = diPhoton()->p4();
    TLorentzVector h_1;
    h_1.SetPxPyPzE(h1_lor.Px(),h1_lor.Py(),h1_lor.Pz(),h1_lor.E()) ; 
    h_1.Boost(boost);

    TVector3 CSaxis = p1.Vect().Unit() - p2.Vect().Unit();
    CSaxis.Unit();
    

    return cos(   CSaxis.Angle( h_1.Vect().Unit() )    );
}


std::vector<float> DoubleHTag::CosThetaAngles() const {
    //helicityThetas[0] = cosTheta_gg
    //helicityThetas[1] = cosTheta_bb

    std::vector<float> helicityThetas;

    TLorentzVector BoostedHgg(0,0,0,0);
    BoostedHgg.SetPxPyPzE(diPhoton()->px(),diPhoton()->py(),diPhoton()->pz(),diPhoton()->energy()) ;
    TLorentzVector BoostedLeadingPhoton(0,0,0,0);
    BoostedLeadingPhoton.SetPxPyPzE(diPhoton()->leadingPhoton()->px(),diPhoton()->leadingPhoton()->py(),diPhoton()->leadingPhoton()->pz(),diPhoton()->leadingPhoton()->energy()) ;
    
    helicityThetas.push_back( HelicityCosTheta(BoostedHgg, BoostedLeadingPhoton));
    
    TLorentzVector BoostedHbb(0,0,0,0);
    BoostedHbb.SetPxPyPzE(dijet().px(),dijet().py(),dijet().pz(),dijet().energy()) ;
    TLorentzVector BoostedLeadingJet(0,0,0,0);
    BoostedLeadingJet.SetPxPyPzE(leadJet().px(),leadJet().py(),leadJet().pz(),leadJet().energy()) ;

    helicityThetas.push_back( HelicityCosTheta(BoostedHbb, BoostedLeadingJet));

    return helicityThetas;

}


float DoubleHTag::HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const
{
    TVector3 BoostVector = Booster.BoostVector();
    Boosted.Boost( -BoostVector.x(), -BoostVector.y(), -BoostVector.z() );
    return Boosted.CosTheta();
}

float DoubleHTag::getPhoJetMinDr() const
{
    float PhoJetMinDr = min( min( deltaR( diPhoton()->leadingPhoton()->p4(), leadJet().p4() ), deltaR( diPhoton()->leadingPhoton()->p4(), subleadJet().p4() ) ), min( deltaR( diPhoton()->subLeadingPhoton()->p4(), leadJet().p4() ), deltaR( diPhoton()->subLeadingPhoton()->p4(), subleadJet().p4() ) ) );
    
    return PhoJetMinDr;
}

float DoubleHTag::getPhoJetOtherDr() const
{
    float dR11 = deltaR( diPhoton()->leadingPhoton()->p4(), leadJet().p4() ); 
    float dR12 = deltaR( diPhoton()->leadingPhoton()->p4(), subleadJet().p4() );
    float dR21 = deltaR( diPhoton()->subLeadingPhoton()->p4(), leadJet().p4() ); 
    float dR22 = deltaR( diPhoton()->subLeadingPhoton()->p4(), subleadJet().p4() );

    float MinDr = min( min( dR11, dR12 ), min( dR21, dR22 ) );
    float PhoJetOtherDr = 0.0;     
    if( dR11 == MinDr ){ PhoJetOtherDr = dR22; }
    if( dR12 == MinDr ){ PhoJetOtherDr = dR21; }
    if( dR21 == MinDr ){ PhoJetOtherDr = dR12; }
    if( dR22 == MinDr ){ PhoJetOtherDr = dR11; }

    return PhoJetOtherDr;
}


float DoubleHTag::getSigmaMDecorr() const
{
    double mass_sigma[2]={0.,0.};
    double dummy[1]={0.};
    mass_sigma[0]=diPhoton()->mass();
    mass_sigma[1] = 0.5*sqrt((diPhoton()->leadingPhoton()->sigEOverE()*diPhoton()->leadingPhoton()->sigEOverE() + diPhoton()->subLeadingPhoton()->sigEOverE()*diPhoton()->subLeadingPhoton()->sigEOverE()));
    float sigmaMOverMDecorr=-99;
    //Splitting EBEB and !EBEB, using cuts as in preselection
    if(abs(diPhoton()->leadingPhoton()->superCluster()->eta())<1.4442 && abs(diPhoton()->subLeadingPhoton()->superCluster()->eta())<1.4442){
        sigmaMOverMDecorr = (*transfEBEB_)(mass_sigma,dummy);
    }
    else{
        sigmaMOverMDecorr = (*transfNotEBEB_)(mass_sigma,dummy);
    }
    return sigmaMOverMDecorr;
}

float DoubleHTag::getSigmaMOverMJets() const
{
    float dijetSigmaMOverM = 1./pow(dijet().M(),2)*sqrt(
                                                        pow(leadJet().userFloat("bRegNNResolution"),2)*pow(pow(leadJet().p4().M(),2) + leadJet().p4().Dot(subleadJet().p4()) ,2)  + 
                                                        pow(subleadJet().userFloat("bRegNNResolution"),2)*pow( pow(subleadJet().p4().M(),2) + subleadJet().p4().Dot(leadJet().p4()),2) );                                     

    return dijetSigmaMOverM;

}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

