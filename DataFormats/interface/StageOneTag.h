#ifndef FLASHgg_StageOneTag_h
#define FLASHgg_StageOneTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {
    enum stage1recoTag { LOGICERROR = -1, NOTAG = 0, RECO_0J_Tag0, RECO_0J_Tag1, RECO_0J_Tag2, RECO_1J_PTH_0_60_Tag0, RECO_1J_PTH_0_60_Tag1, RECO_1J_PTH_60_120_Tag0, RECO_1J_PTH_60_120_Tag1,
                         RECO_1J_PTH_120_200_Tag0, RECO_1J_PTH_120_200_Tag1, RECO_1J_PTH_GT200, 
                         RECO_GE2J_PTH_0_60_Tag0, RECO_GE2J_PTH_0_60_Tag1, RECO_GE2J_PTH_60_120_Tag0, RECO_GE2J_PTH_60_120_Tag1, RECO_GE2J_PTH_120_200_Tag0, RECO_GE2J_PTH_120_200_Tag1, 
                         RECO_GE2J_PTH_GT200_Tag0, RECO_GE2J_PTH_GT200_Tag1,
                         RECO_VBFTOPO_JET3VETO_Tag0, RECO_VBFTOPO_JET3VETO_Tag1, RECO_VBFTOPO_JET3_Tag0, RECO_VBFTOPO_JET3_Tag1, RECO_VBFTOPO_REST, RECO_VBFTOPO_BSM,
                         RECO_WHLEP, RECO_ZHLEP, RECO_VHLEPLOOSE, RECO_VHMET, RECO_VHHAD, RECO_TTH_LEP, RECO_TTH_HAD };

    class StageOneTag: public DiPhotonTagBase
    {
    public:
        StageOneTag();
        ~StageOneTag();

        StageOneTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        StageOneTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        StageOneTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFDiPhoDiJetMVAResult> );
        StageOneTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFDiPhoDiJetMVAResult );

        StageOneTag *clone() const override { return ( new StageOneTag( *this ) ); }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kStageOne; }

        int stage1recoEnum() const { return stage1recoTag_; }

        void computeStage1Kinematics( const edm::Handle<edm::View<flashgg::Jet> > & jets, float ptV = -1., float lepphi1 = -999., float lepeta1 = -999., float lepphi2 = -999., float lepeta2 = -999. );
        void computeStage1Kinematics( const edm::Ptr<flashgg::Jet> j0, const edm::Ptr<flashgg::Jet> j1, float ptV = -1., float lepphi1 = -999., float lepeta1 = -999., float lepphi2 = -999., float lepeta2 = -999. );
        string stage1KinematicLabel() const;
        void setStage1recoTag( const int tag ) { stage1recoTag_ = tag; }

        const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
        const VBFMVAResult VBFMVA() const ;


    private:
        int stage1recoTag_;
        VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;

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

