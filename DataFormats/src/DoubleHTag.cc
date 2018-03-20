#include "flashgg/DataFormats/interface/DoubleHTag.h"

using namespace flashgg;

DoubleHTag::DoubleHTag() : DiPhotonTagBase::DiPhotonTagBase(), mva_(-2.) 
{  
}

DoubleHTag::~DoubleHTag() {}

DoubleHTag::DoubleHTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<flashgg::Jet> leadJet, edm::Ptr<flashgg::Jet> subleadJet )
    : mva_(-.2), MX_(0.), leadJet_(leadJet), subleadJet_(subleadJet) 
{
    dipho_ = diPho;
    dijet_ = leadJet_->p4() + subleadJet->p4();
    this->setP4( dijet_ + dipho_->p4() );
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

