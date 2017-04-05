#ifndef FLASHgg_TagAndProbeCandidate_h
#define FLASHgg_TagAndProbeCandidate_h

#include "DataFormats/PatCandidates/interface/PATObject.h"

#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"

namespace flashgg {

    class TagAndProbeCandidate: public pat::PATObject<reco::LeafCandidate> , public WeightedObject
    {
    public:
        //---ctors---
        TagAndProbeCandidate() {};
        TagAndProbeCandidate( edm::Ptr<flashgg::DiPhotonCandidate> dipho, bool leadIsTag)
            {
                diphoPtr_ = dipho;
                leadIsTag_ = leadIsTag;
            };

        //---dtor---
        ~TagAndProbeCandidate() {};

        //---getters---
        const edm::Ptr<DiPhotonCandidate> diPhoton() const { return diphoPtr_; }
        const Photon*                     getTag() const {return getTagView()->photon();};
        const Photon*                     getProbe() const {return getProbeView()->photon();};
        const SinglePhotonView*           getTagView() const
            {return leadIsTag_ ? diphoPtr_->leadingView() : diphoPtr_->subLeadingView();};
        const SinglePhotonView*           getProbeView() const
            {return !leadIsTag_ ? diphoPtr_->leadingView() : diphoPtr_->subLeadingView();};
        
    private:
        edm::Ptr<DiPhotonCandidate> diphoPtr_;
        bool                        leadIsTag_;
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

