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

void GenDiPhoton::computeP4AndOrder()
{
    if( leadingPhoton().pt() < subLeadingPhoton().pt() ) {
        std::swap( leadingPhoton_, subLeadingPhoton_ );
    }
    this->setP4( leadingPhoton().p4() + subLeadingPhoton().p4() );
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

