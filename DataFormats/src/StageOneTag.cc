#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/StageOneTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

StageOneTag::StageOneTag() {}

StageOneTag::~StageOneTag() {}

StageOneTag::StageOneTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes ) :
    StageOneTag::StageOneTag( diPho, *mvaRes ) {}

StageOneTag::StageOneTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {
}

StageOneTag::StageOneTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFMVAResult> vbf_mvaRes ) :
    StageOneTag::StageOneTag( diPho, *mvaRes, *vbf_mvaRes ) {}

StageOneTag::StageOneTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFMVAResult vbf_mvaRes ) :
    StageOneTag::StageOneTag( dipho, mvares )
{
    vbfmva_result_ = vbf_mvaRes;
}

const VBFMVAResult StageOneTag::VBFMVA() const
{
    return vbfmva_result_;
}

string StageOneTag::stage1KinematicLabel() const { 
    switch(stage1recoTag_) {
    case stage1recoTag::LOGICERROR:
        return string("LOGICERROR");
    case stage1recoTag::NOTAG:
        return string("NOTAG");
    case stage1recoTag::RECO_0J_PTH_0_10_Tag0:
        return string("RECO_0J_PTH_0_10_Tag0");
    case stage1recoTag::RECO_0J_PTH_0_10_Tag1:
        return string("RECO_0J_PTH_0_10_Tag1");
    case stage1recoTag::RECO_0J_PTH_GT10_Tag0:
        return string("RECO_0J_PTH_GT10_Tag0");
    case stage1recoTag::RECO_0J_PTH_GT10_Tag1:
        return string("RECO_0J_PTH_GT10_Tag1");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag0:
        return string("RECO_1J_PTH_0_60_Tag0");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag1:
        return string("RECO_1J_PTH_0_60_Tag1");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag0:
        return string("RECO_1J_PTH_60_120_Tag0");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag1:
        return string("RECO_1J_PTH_60_120_Tag1");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag0:
        return string("RECO_1J_PTH_120_200_Tag0");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag1:
        return string("RECO_1J_PTH_120_200_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag0:
        return string("RECO_GE2J_PTH_0_60_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag1:
        return string("RECO_GE2J_PTH_0_60_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag0:
        return string("RECO_GE2J_PTH_60_120_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag1:
        return string("RECO_GE2J_PTH_60_120_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag0:
        return string("RECO_GE2J_PTH_120_200_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag1:
        return string("RECO_GE2J_PTH_120_200_Tag1");
    case stage1recoTag::RECO_PTH_GT200_Tag0:
        return string("RECO_PTH_GT200_Tag0");
    case stage1recoTag::RECO_PTH_GT200_Tag1:
        return string("RECO_PTH_GT200_Tag1");
    case stage1recoTag::RECO_VBFTOPO_VHHAD:
        return string("RECO_VBFTOPO_VHHAD");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_LOWMJJ:
        return string("RECO_VBFTOPO_JET3VETO_LOWMJJ");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_HIGHMJJ:
        return string("RECO_VBFTOPO_JET3VETO_HIGHMJJ");
    case stage1recoTag::RECO_VBFTOPO_JET3_LOWMJJ:
        return string("RECO_VBFTOPO_JET3_LOWMJJ");
    case stage1recoTag::RECO_VBFTOPO_JET3_HIGHMJJ:
        return string("RECO_VBFTOPO_JET3_HIGHMJJ");
    case stage1recoTag::RECO_VBFTOPO_BSM:
        return string("RECO_VBFTOPO_BSM");
    case stage1recoTag::RECO_VBFLIKEGGH:
        return string("RECO_VBFLIKEGGH");
    case stage1recoTag::RECO_WHLEP:
        return string("RECO_WHLEP");
    case stage1recoTag::RECO_ZHLEP:
        return string("RECO_ZHLEP");
    case stage1recoTag::RECO_VHLEPLOOSE:
        return string("RECO_VHLEPLOOSE");
    case stage1recoTag::RECO_VHMET:
        return string("RECO_VHMET");
    case stage1recoTag::RECO_VHHAD:
        return string("RECO_VHHAD");
    case stage1recoTag::RECO_TTH_LEP:
        return string("RECO_TTH_LEP");
    case stage1recoTag::RECO_TTH_HAD:
        return string("RECO_TTH_HAD");
    default:
        break;
    }
    return string("TAG NOT ON LIST");
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

