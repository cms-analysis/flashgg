#include "flashgg/DataFormats/interface/VHTagTruth.h"
#include <iostream>

using namespace flashgg;

VHTagTruth::VHTagTruth() {}

VHTagTruth::~VHTagTruth() {}

/*
VHTagTruth::VHTagTruth(const VHTagTruth &b) : TagTruthBase::TagTruthBase(b)
{
    std::cout << " Derived copy constructor!" << std::endl;
    setClosestGenJetToLeadingJet(b.closestGenJetToLeadingJet());
    setClosestGenJetToSubLeadingJet(b.closestGenJetToSubLeadingJet());
    setClosestParticleToLeadingJet(b.closestParticleToLeadingJet());
    setClosestParticleToSubLeadingJet(b.closestParticleToSubLeadingJet());
    setClosestParticleToLeadingPhoton(b.closestParticleToLeadingPhoton());
    setClosestParticleToSubLeadingPhoton(b.closestParticleToSubLeadingPhoton());
    setLeadingQuark(b.leadingParton());
    setSubLeadingQuark(b.subLeadingParton());
}
*/

VHTagTruth *VHTagTruth::clone() const
{
    //    return (new VHTagTruth(*this));
    VHTagTruth *result = new VHTagTruth;
    result->setAssociatedZ( associatedZ() );
    result->setAssociatedW( associatedW() );
    result->setVhasDaughters( VhasDaughters() );
    result->setVhasNeutrinos( VhasNeutrinos() );
    result->setVhasLeptons( VhasLeptons() );
    result->setVhasHadrons( VhasHadrons() );
    result->setVhasMissingLeptons( VhasMissingLeptons() );
    result->setVpt( Vpt() );
    result->copyBaseInfo( *this );
    return result;

}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
