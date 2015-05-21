#include "flashgg/DataFormats/interface/VHTightTag.h"
#include <algorithm>

using namespace flashgg;

VHTightTag::VHTightTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHTightTag::~VHTightTag()
{}


VHTightTag::VHTightTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvares ) {}
VHTightTag::VHTightTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}

VHTightTag::VHTightTag( const VHTightTag &b ) : DiPhotonTagBase::DiPhotonTagBase( b )
{
    setJets( b.jets() );
    setMuons( b.muons() );
    setElectrons( b.electrons() );
    setMET( b.met() );
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

