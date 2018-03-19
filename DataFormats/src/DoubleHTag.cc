#include "flashgg/DataFormats/interface/DoubleHTag.h"

using namespace flashgg;

DoubleHTag::DoubleHTag() : DiPhotonTagBase::DiPhotonTagBase(), mva_(-2.) 
{  
}

DoubleHTag::~DoubleHTag() {}

DoubleHTag::DoubleHTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<reco::Candidate> leadJet, edm::Ptr<reco::Candidate> subleadJet )
    : mva_(-.2), leadJet_(leadJet), subleadJet_(subleadJet) 
{
    dipho_ = diPho;
}

DoubleHTag *DoubleHTag::clone() const
{
    DoubleHTag *result = new DoubleHTag( *this );
    return result;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

