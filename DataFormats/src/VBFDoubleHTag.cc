#include "flashgg/DataFormats/interface/VBFDoubleHTag.h"

using namespace flashgg;

VBFDoubleHTag::VBFDoubleHTag() : DoubleHTag::DoubleHTag() 
{  
}

VBFDoubleHTag::~VBFDoubleHTag() {}

VBFDoubleHTag::VBFDoubleHTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<flashgg::Jet> leadJet, edm::Ptr<flashgg::Jet> subleadJet, edm::Ptr<flashgg::Jet> VBFleadJet, edm::Ptr<flashgg::Jet> VBFsubleadJet )
    : DoubleHTag::DoubleHTag( diPho, leadJet, subleadJet),  VBFleadJet_(VBFleadJet), VBFsubleadJet_(VBFsubleadJet) 
 
{
    diVBFjet_ = VBFleadJet_->p4() + VBFsubleadJet_->p4();
}

VBFDoubleHTag *VBFDoubleHTag::clone() const
{
    VBFDoubleHTag *result = new VBFDoubleHTag( *this );
    return result;
}


float VBFDoubleHTag::MinDeltaR_VBF_gamma() 
{
  float MinDeltaR_VBF_gamma = min( min( deltaR( diPhoton()->leadingPhoton()->p4(), VBFleadJet().p4() ), deltaR( diPhoton()->leadingPhoton()->p4(), VBFsubleadJet().p4() ) ), min( deltaR( diPhoton()->subLeadingPhoton()->p4(), VBFleadJet().p4() ), deltaR( diPhoton()->subLeadingPhoton()->p4(), VBFsubleadJet().p4() ) ) );
 
  return MinDeltaR_VBF_gamma;
}

float VBFDoubleHTag::MinDeltaR_VBF_b() 
{
  float MinDeltaR_VBF_b = min( min( deltaR( leadJet().p4(), VBFleadJet().p4() ), deltaR( subleadJet().p4() , VBFsubleadJet().p4() ) ), min( deltaR( leadJet().p4() , VBFleadJet().p4() ), deltaR( VBFsubleadJet().p4() , VBFsubleadJet().p4() ) ) );

  return MinDeltaR_VBF_b;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4





