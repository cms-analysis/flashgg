#include "flashgg/DataFormats/interface/SigmaMpTTag.h"

using namespace flashgg;

SigmaMpTTag::SigmaMpTTag() : DiPhotonTagBase::DiPhotonTagBase() {}

SigmaMpTTag::~SigmaMpTTag() {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : SigmaMpTTag::SigmaMpTTag( diPho, *mvaRes ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, std::map<std::string, edm::Ptr<reco::CompositeCandidate> > compObjMap )
    : SigmaMpTTag::SigmaMpTTag( diPho, *mvaRes, compObjMap ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, std::map<std::string, edm::Ptr<reco::CompositeCandidate> > compObjMap ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {
    
    compObjMap_ = compObjMap;

}

const edm::Ptr<reco::CompositeCandidate > SigmaMpTTag::getCompCand(const std::string &name) const
{
    auto itr = compObjMap_.find(name);
    if(itr==compObjMap_.end()){
        assert(0);
    }
    return itr->second;
}

SigmaMpTTag *SigmaMpTTag::clone() const
{
    SigmaMpTTag *result = new SigmaMpTTag( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

