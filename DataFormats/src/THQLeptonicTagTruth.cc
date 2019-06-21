#include "flashgg/DataFormats/interface/THQLeptonicTagTruth.h"
#include <iostream>

using namespace flashgg;

THQLeptonicTagTruth::THQLeptonicTagTruth() {}

THQLeptonicTagTruth::~THQLeptonicTagTruth() {}

/*
THQLeptonicTagTruth::THQLeptonicTagTruth(const THQLeptonicTagTruth &b) : TagTruthBase::TagTruthBase(b)
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

THQLeptonicTagTruth *THQLeptonicTagTruth::clone() const
{
    //    return (new THQLeptonicTagTruth(*this));
    THQLeptonicTagTruth *result = new THQLeptonicTagTruth;
    result->setClosestGenJetToLeadingJet( closestGenJetToLeadingJet() );
    result->setClosestGenJetToSubLeadingJet( closestGenJetToSubLeadingJet() );
    result->setClosestGenJetToSubSubLeadingJet( closestGenJetToSubSubLeadingJet() );
    result->setClosestParticleToLeadingJet( closestParticleToLeadingJet() );
    result->setClosestParticleToSubLeadingJet( closestParticleToSubLeadingJet() );
    result->setClosestParticleToSubSubLeadingJet( closestParticleToSubSubLeadingJet() );
    result->setClosestParticleToLeadingPhoton( closestParticleToLeadingPhoton() );
    result->setClosestParticleToSubLeadingPhoton( closestParticleToSubLeadingPhoton() );
    result->setLeadingParton( leadingParton() );
    result->setSubLeadingParton( subLeadingParton() );
    result->setSubLeadingParton( subSubLeadingParton() );
    result->setPtOrderedPartons( ptOrderedPartons() );
    result->setPtOrderedGenJets( ptOrderedGenJets() );
    result->setPtOrderedFggJets( ptOrderedFggJets() );
    result->setDiPhoton( diPhoton() );
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
