#ifndef FLASHgg_StageOneCombinedTag_h
#define FLASHgg_StageOneCombinedTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {
    enum stage1recoTag { LOGICERROR = -1, NOTAG = 0, RECO_0J_PTH_0_10_Tag0, RECO_0J_PTH_0_10_Tag1, RECO_0J_PTH_GT10_Tag0, RECO_0J_PTH_GT10_Tag1,
                         RECO_1J_PTH_0_60_Tag0, RECO_1J_PTH_0_60_Tag1, RECO_1J_PTH_60_120_Tag0, RECO_1J_PTH_60_120_Tag1,
                         RECO_1J_PTH_120_200_Tag0, RECO_1J_PTH_120_200_Tag1,  
                         RECO_GE2J_PTH_0_60_Tag0, RECO_GE2J_PTH_0_60_Tag1, RECO_GE2J_PTH_60_120_Tag0, RECO_GE2J_PTH_60_120_Tag1, RECO_GE2J_PTH_120_200_Tag0, RECO_GE2J_PTH_120_200_Tag1, 
                         RECO_PTH_GT200_Tag0, RECO_PTH_GT200_Tag1,
                         RECO_VBFTOPO_VHHAD, RECO_VBFTOPO_JET3VETO_LOWMJJ, RECO_VBFTOPO_JET3VETO_HIGHMJJ, RECO_VBFTOPO_JET3_LOWMJJ, RECO_VBFTOPO_JET3_HIGHMJJ, RECO_VBFTOPO_BSM, RECO_VBFLIKEGGH,
                         RECO_WHLEP, RECO_ZHLEP, RECO_VHLEPLOOSE, RECO_VHMET, RECO_VHHAD, RECO_TTH_LEP, RECO_TTH_HAD };

    class StageOneCombinedTag: public DiPhotonTagBase
    {
    public:
        StageOneCombinedTag();
        ~StageOneCombinedTag();

        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFMVAResult> );
        StageOneCombinedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFMVAResult );

        StageOneCombinedTag *clone() const override { return ( new StageOneCombinedTag( *this ) ); }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kStageOneCombined; }

        const VBFMVAResult VBFMVA() const;
    private:
        int stage1recoTag_;
        VBFMVAResult vbfmva_result_;

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
