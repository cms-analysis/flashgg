#ifndef FLASHgg_VBFTagTruth_h
#define FLASHgg_VBFTagTruth_h

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"

namespace flashgg {

    class VBFTagTruth : public TagTruthBase
    {

    public:

        VBFTagTruth();
        ~VBFTagTruth();
        //        VBFTagTruth(const VBFTagTruth &b);

        const edm::Ptr<reco::GenJet> closestGenJetToLeadingJet() const { return closestGenJetToLeadingJet_; }
        const edm::Ptr<reco::GenJet> closestGenJetToSubLeadingJet() const { return closestGenJetToSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToLeadingJet() const { return closestParticleToLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubLeadingJet() const { return closestParticleToSubLeadingJet_; }
        const edm::Ptr<reco::GenParticle> closestParticleToLeadingPhoton() const { return closestParticleToLeadingPhoton_; }
        const edm::Ptr<reco::GenParticle> closestParticleToSubLeadingPhoton() const { return closestParticleToSubLeadingPhoton_; }
        const edm::Ptr<reco::GenParticle> leadingQuark() const { return leadingQuark_; }
        const edm::Ptr<reco::GenParticle> subLeadingQuark() const { return subLeadingQuark_; }

        void setClosestGenJetToLeadingJet( const edm::Ptr<reco::GenJet> &val ) { closestGenJetToLeadingJet_ = val; }
        void setClosestGenJetToSubLeadingJet( const edm::Ptr<reco::GenJet> &val ) { closestGenJetToSubLeadingJet_ = val; }
        void setClosestParticleToLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToLeadingJet_ = val; }
        void setClosestParticleToSubLeadingJet( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubLeadingJet_ = val; }
        void setClosestParticleToLeadingPhoton( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToLeadingPhoton_ = val; }
        void setClosestParticleToSubLeadingPhoton( const edm::Ptr<reco::GenParticle> &val ) { closestParticleToSubLeadingPhoton_ = val; }
        void setLeadingQuark( const edm::Ptr<reco::GenParticle> &val ) { leadingQuark_ = val; }
        void setSubLeadingQuark( const edm::Ptr<reco::GenParticle> &val ) { subLeadingQuark_ = val; }

        VBFTagTruth *clone() const;

    private:
        edm::Ptr<reco::GenJet> closestGenJetToLeadingJet_;
        edm::Ptr<reco::GenJet> closestGenJetToSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingJet_;
        edm::Ptr<reco::GenParticle> closestParticleToLeadingPhoton_;
        edm::Ptr<reco::GenParticle> closestParticleToSubLeadingPhoton_;
        edm::Ptr<reco::GenParticle> leadingQuark_;
        edm::Ptr<reco::GenParticle> subLeadingQuark_;
    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
