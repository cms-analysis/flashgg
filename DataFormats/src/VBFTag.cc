#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

VBFTag::VBFTag() {}

VBFTag::~VBFTag() {}

VBFTag::VBFTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes ) :
    VBFTag::VBFTag( diPho, *mvaRes, *vbfDiPhoDiJet_mvaRes ) {}

VBFTag::VBFTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares )
{
    vbfDiPhoDiJet_mva_result_ = vbfDiPhoDiJet_mvaRes;
}


const VBFDiPhoDiJetMVAResult VBFTag::VBFDiPhoDiJetMVA() const
{
    return vbfDiPhoDiJet_mva_result_;
}
const VBFMVAResult VBFTag::VBFMVA() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult;
}

const Jet VBFTag::leadingJet() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet;
}

const Jet VBFTag::subLeadingJet() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

