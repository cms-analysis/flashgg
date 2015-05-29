#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

VHEtTag::VHEtTag() {}

VHEtTag::~VHEtTag() {}

VHEtTag::VHEtTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes ) :
    VHEtTag::VHEtTag( diPho, *mvaRes ) {}

VHEtTag::VHEtTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}

void VHEtTag::setMet( edm::Ptr<pat::MET> met )
{
    theMet_ = met;
}

VHEtTag *VHEtTag::clone() const
{
    VHEtTag *result = new VHEtTag( diPhoton(), diPhotonMVA() );
    result->setDiPhotonIndex( diPhotonIndex() );
    result->setMet( theMet_ );
    return result;
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

