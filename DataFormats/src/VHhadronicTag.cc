#include "flashgg/DataFormats/interface/VHHadronicTag.h"

using namespace flashgg;

VHHadronicTag::VHHadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHHadronicTag::~VHHadronicTag()
{}


VHHadronicTag::VHHadronicTag( edm::Ptr<DiPhotonCandidate> diPho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( diPho, mvares ) {}

VHHadronicTag::VHHadronicTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : VHHadronicTag::VHHadronicTag( diPho, *mvares ) {}


void VHHadronicTag::setJets( edm::Ptr<flashgg::Jet> ljet, edm::Ptr<flashgg::Jet> sjet )
{
    leadingJet_    = ljet;
    subleadingJet_ = sjet;
}


VHHadronicTag *VHHadronicTag::clone() const
{
    VHHadronicTag *result = new VHHadronicTag( *this );
    return result;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

