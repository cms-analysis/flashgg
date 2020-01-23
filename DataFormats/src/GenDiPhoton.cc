#include "flashgg/DataFormats/interface/GenDiPhoton.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"


using namespace flashgg;

GenDiPhoton::GenDiPhoton() {}

GenDiPhoton::~GenDiPhoton() {}

GenDiPhoton::GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra> photon1, edm::Ptr<flashgg::GenPhotonExtra> photon2)
{
    leadingPhoton_ = *photon1;
    subLeadingPhoton_ = *photon2;
    computeP4AndOrder();
}

GenDiPhoton::GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra> photon1, edm::Ptr<flashgg::GenPhotonExtra> photon2, edm::Ptr<reco::GenJet> jet1, edm::Ptr<reco::GenJet> jet2)
    : GenDiPhoton(photon1,photon2)
{
    leadingJet_ = jet1;
    subLeadingJet_ = jet2;
    if( leadingJet_->pt() < subLeadingJet_->pt() ) { std::swap(leadingJet_,subLeadingJet_); }
}

void GenDiPhoton::computeP4AndOrder()
{
    if( leadingPhoton()->pt() < subLeadingPhoton()->pt() ) {
        std::swap( leadingPhoton_, subLeadingPhoton_ );
    }
    this->setP4( leadingPhoton()->p4() + subLeadingPhoton()->p4() );
}

GenDiPhoton::LorentzVector GenDiPhoton::dijet() const
{
    return leadingJet().p4() + subLeadingJet().p4();
}

float GenDiPhoton::HHbbggCosThetaStar_CS() const
{
    TLorentzVector h_1;
    h_1.SetPxPyPzE(leadingPhoton()->p4().px(),leadingPhoton()->p4().py(),leadingPhoton()->p4().pz(),leadingPhoton()->p4().energy()) ; 
    TLorentzVector h_2;
    h_2.SetPxPyPzE(subLeadingPhoton()->p4().px(),subLeadingPhoton()->p4().py(),subLeadingPhoton()->p4().pz(),subLeadingPhoton()->p4().energy()) ; 

    TLorentzVector h_jet_1;
    h_jet_1.SetPxPyPzE(leadingJet().p4().px(),leadingJet().p4().py(),leadingJet().p4().pz(),leadingJet().p4().energy()) ; 
    TLorentzVector h_jet_2;
    h_jet_2.SetPxPyPzE(subLeadingJet().p4().px(),subLeadingJet().p4().py(),subLeadingJet().p4().pz(),subLeadingJet().p4().energy()) ;
    TLorentzVector hh = h_1 + h_2 + h_jet_1 + h_jet_2;
 
    h_1.Boost(-hh.BoostVector());  
    return h_1.CosTheta();
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

