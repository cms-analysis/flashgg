#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

ZPlusJetTag::ZPlusJetTag() {}

ZPlusJetTag::~ZPlusJetTag() {}

ZPlusJetTag::ZPlusJetTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<Jet> theJet, unsigned njets) :
    DiPhotonTagBase::DiPhotonTagBase( diPho, mvaRes ) 
{
    if (njets>0) theJet_ = theJet;
    njets_ = njets;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

