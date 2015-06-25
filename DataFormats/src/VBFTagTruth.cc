#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include <iostream>

using namespace flashgg;

VBFTagTruth::VBFTagTruth() {}

VBFTagTruth::~VBFTagTruth() {}

/*
VBFTagTruth::VBFTagTruth(const VBFTagTruth &b) : TagTruthBase::TagTruthBase(b)
{
    std::cout << " Derived copy constructor!" << std::endl;
    setClosestGenJetToLeadingJet(b.closestGenJetToLeadingJet());
    setClosestGenJetToSubLeadingJet(b.closestGenJetToSubLeadingJet());
    setClosestParticleToLeadingJet(b.closestParticleToLeadingJet());
    setClosestParticleToSubLeadingJet(b.closestParticleToSubLeadingJet());
    setClosestParticleToLeadingPhoton(b.closestParticleToLeadingPhoton());
    setClosestParticleToSubLeadingPhoton(b.closestParticleToSubLeadingPhoton());
    setLeadingQuark(b.leadingQuark());
    setSubLeadingQuark(b.subLeadingQuark());
}
*/

VBFTagTruth *VBFTagTruth::clone() const
{
    std::cout << " Derived clone!" << std::endl;
    //    return (new VBFTagTruth(*this));
    VBFTagTruth *result = new VBFTagTruth;
    result->setClosestGenJetToLeadingJet( closestGenJetToLeadingJet() );
    result->setClosestGenJetToSubLeadingJet( closestGenJetToSubLeadingJet() );
    result->setClosestParticleToLeadingJet( closestParticleToLeadingJet() );
    result->setClosestParticleToSubLeadingJet( closestParticleToSubLeadingJet() );
    result->setClosestParticleToLeadingPhoton( closestParticleToLeadingPhoton() );
    result->setClosestParticleToSubLeadingPhoton( closestParticleToSubLeadingPhoton() );
    result->setLeadingQuark( leadingQuark() );
    result->setSubLeadingQuark( subLeadingQuark() );
    result->setGenPV( genPV() );
    return result;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
