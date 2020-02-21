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

float VBFDoubleHTag::getVBFDeltaR_jg() const 
{
  float VBFDeltaR_jg = min( min( deltaR( diPhoton()->leadingPhoton()->p4(), VBFleadJet().p4() ), deltaR( diPhoton()->leadingPhoton()->p4(), VBFsubleadJet().p4() ) ), min( deltaR( diPhoton()->subLeadingPhoton()->p4(), VBFleadJet().p4() ), deltaR( diPhoton()->subLeadingPhoton()->p4(), VBFsubleadJet().p4() ) ) );
 
  return VBFDeltaR_jg;
}
float VBFDoubleHTag::getVBFDeltaR_jb() const
{
  float VBFDeltaR_jb = min( min( deltaR( leadJet().p4(), VBFleadJet().p4() ), deltaR( subleadJet().p4() , VBFsubleadJet().p4() ) ), min( deltaR( leadJet().p4() , VBFsubleadJet().p4() ), deltaR( subleadJet().p4() , VBFleadJet().p4() ) ) );

  return VBFDeltaR_jb;
}
float VBFDoubleHTag::getVBFJet_mjj() const
{
   float VBFJet_mjj = (VBFleadJet().p4()+VBFsubleadJet().p4()).mass();

   return VBFJet_mjj;
} 
float VBFDoubleHTag::getVBFDelta_phi() const
{
   float VBFDelta_phi = TVector2::Phi_mpi_pi(VBFleadJet().phi() - VBFsubleadJet().phi());
   return VBFDelta_phi;
}
float VBFDoubleHTag::getVBFProd_eta() const
{
   float VBFProd_eta =VBFleadJet().eta()*VBFsubleadJet().eta();
   return VBFProd_eta;
}
float VBFDoubleHTag::getVBFCentrality_jg() const
{
 float VBFCentrality_jg = exp(-4/((VBFleadJet().eta() - VBFsubleadJet().eta())*(VBFleadJet().eta() - VBFsubleadJet().eta())) * (dipho_->p4().eta() - (VBFleadJet().eta() + VBFsubleadJet().eta())/2)* ( dipho_->p4().eta() - (VBFleadJet().eta() + VBFsubleadJet().eta())/2));
 return VBFCentrality_jg;  

}
float VBFDoubleHTag::getVBFCentrality_jb() const
{
  float VBFCentrality_jb = exp(-4/((VBFleadJet().eta() - VBFsubleadJet().eta())*(VBFleadJet().eta() - VBFsubleadJet().eta())) * ( dijet().eta() - (VBFleadJet().eta() + VBFsubleadJet().eta())/2)* ( dijet().eta() - (VBFleadJet().eta() + VBFsubleadJet().eta())/2));
  return VBFCentrality_jb;
}





// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4





