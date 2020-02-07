#ifndef FLASHgg_StageOneCombinedTag_h
#define FLASHgg_StageOneCombinedTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VHhadMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

    class StageOneCombinedTag: public DiPhotonTagBase
    {
    public:
        StageOneCombinedTag();
        ~StageOneCombinedTag();

        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFMVAResult>, edm::Ptr<VHhadMVAResult> );
        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFMVAResult, VHhadMVAResult );

        StageOneCombinedTag *clone() const override { return ( new StageOneCombinedTag( *this ) ); }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kStageOneCombined; }

        const VBFMVAResult VBFMVA() const;
        const VHhadMVAResult VHhadMVA() const;
    private:
        VBFMVAResult vbfmva_result_;
        VHhadMVAResult vhhadmva_result_;

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
