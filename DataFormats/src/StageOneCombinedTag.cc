#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/StageOneCombinedTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

StageOneCombinedTag::StageOneCombinedTag() {}

StageOneCombinedTag::~StageOneCombinedTag() {}

StageOneCombinedTag::StageOneCombinedTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes ) :
    StageOneCombinedTag::StageOneCombinedTag( diPho, *mvaRes ) {}

StageOneCombinedTag::StageOneCombinedTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {
}

StageOneCombinedTag::StageOneCombinedTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFMVAResult> vbf_mvaRes, edm::Ptr<VHhadMVAResult> vhHad_mvaRes ) :
    StageOneCombinedTag::StageOneCombinedTag( diPho, *mvaRes, *vbf_mvaRes, *vhHad_mvaRes ) {}

StageOneCombinedTag::StageOneCombinedTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFMVAResult vbf_mvaRes, VHhadMVAResult vhHad_mvaRes ) :
    StageOneCombinedTag::StageOneCombinedTag( dipho, mvares )
{
    vbfmva_result_ = vbf_mvaRes;
    vhhadmva_result_ = vhHad_mvaRes;
}

const VBFMVAResult StageOneCombinedTag::VBFMVA() const
{
    return vbfmva_result_;

}

const VHhadMVAResult StageOneCombinedTag::VHhadMVA() const
{
    return vhhadmva_result_;
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

