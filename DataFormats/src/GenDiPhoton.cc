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
    if( leadingPhoton().pt() < subLeadingPhoton().pt() ) {
        std::swap( leadingPhoton_, subLeadingPhoton_ );
    }
    this->setP4( leadingPhoton().p4() + subLeadingPhoton().p4() );
}

GenDiPhoton::LorentzVector GenDiPhoton::dijet() const
{
    return leadingJet().p4() + subLeadingJet().p4();
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

