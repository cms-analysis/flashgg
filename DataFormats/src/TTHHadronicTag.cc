#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/Jet.h"

using namespace flashgg;

TTHHadronicTag::TTHHadronicTag() : DiPhotonTagBase::DiPhotonTagBase() {}

TTHHadronicTag::~TTHHadronicTag() {}

TTHHadronicTag::TTHHadronicTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes,
                                std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec ) :
    TTHHadronicTag::TTHHadronicTag( diPho, *mvaRes, theJetVec, theBJetVec ) {}

TTHHadronicTag::TTHHadronicTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares,
                                std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares )
{
    theJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >( theJetVec );
    theBJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >( theBJetVec );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

