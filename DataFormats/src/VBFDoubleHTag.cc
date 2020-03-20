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
float VBFDoubleHTag::getVBFJet_Delta_eta() const
{
   float VBFJet_Delta_eta = abs(VBFleadJet().eta() - VBFsubleadJet().eta());
   return VBFJet_Delta_eta;
}
float VBFDoubleHTag::getVBFleadJet_pt() const
{
   float VBFleadJet_pt = VBFleadJet().pt();
   return VBFleadJet_pt;
}
float VBFDoubleHTag::getVBFsubleadJet_pt() const
{
   float VBFsubleadJet_pt = VBFsubleadJet().pt();
   return VBFsubleadJet_pt;
}
float VBFDoubleHTag::getVBFleadJet_eta() const
{
   float VBFleadJet_eta = VBFleadJet().eta();
   return VBFleadJet_eta;
}
float VBFDoubleHTag::getVBFsubleadJet_eta() const
{
   float VBFsubleadJet_eta = VBFsubleadJet().eta();
   return VBFsubleadJet_eta;
}
float VBFDoubleHTag::getVBFleadJet_phi() const
{
   float VBFleadJet_phi = VBFleadJet().phi();
   return VBFleadJet_phi;
}
float VBFDoubleHTag::getVBFsubleadJet_phi() const
{
   float VBFsubleadJet_phi = VBFsubleadJet().phi();
   return VBFsubleadJet_phi;
}
float VBFDoubleHTag::getVBFleadJet_pz() const
{
   float VBFleadJet_pz = VBFleadJet().pz();
   return VBFleadJet_pz;
}
float VBFDoubleHTag::getVBFsubleadJet_pz() const
{
   float VBFsubleadJet_pz = VBFsubleadJet().pz();
   return VBFsubleadJet_pz;
}
float VBFDoubleHTag::getVBFleadJet_QGL() const
{
   float VBFleadJet_QGL = VBFleadJet().QGL();
   return VBFleadJet_QGL;
}
float VBFDoubleHTag::getVBFsubleadJet_QGL() const
{
   float VBFsubleadJet_QGL = VBFsubleadJet().QGL();
   return VBFsubleadJet_QGL;
}
float VBFDoubleHTag::getVBFleadJet_PUID() const
{
   float VBFleadJet_PUID = VBFleadJet().puJetIdMVA();
   return VBFleadJet_PUID;
}
float VBFDoubleHTag::getVBFsubleadJet_PUID() const
{
   float VBFsubleadJet_PUID = VBFsubleadJet().puJetIdMVA();
   return VBFsubleadJet_PUID;
}

float VBFDoubleHTag::getVBF_angleHH() const
{
      TVector3 lead_Jet, sublead_Jet, leadPho, subleadPho, JJ, PhoPho;
      leadPho.SetPtEtaPhi(diPhoton()->leadingPhoton()->pt(), diPhoton()->leadingPhoton()->eta(), diPhoton()->leadingPhoton()->phi());
      subleadPho.SetPtEtaPhi(diPhoton()->subLeadingPhoton()->pt(), diPhoton()->subLeadingPhoton()->eta(), diPhoton()->subLeadingPhoton()->phi());
      lead_Jet.SetPtEtaPhi(leadJet().pt(), leadJet().eta(), leadJet().phi());
      sublead_Jet.SetPtEtaPhi(subleadJet().pt(), subleadJet().eta(), subleadJet().phi());
      JJ=lead_Jet+sublead_Jet;
      PhoPho=leadPho+subleadPho;
 
  float VBF_angleHH = JJ.Angle(PhoPho);
   return VBF_angleHH;
}
float VBFDoubleHTag::getVBF_dRHH() const
{
      TVector3 lead_Jet, sublead_Jet, leadPho, subleadPho, JJ, PhoPho;
      leadPho.SetPtEtaPhi(diPhoton()->leadingPhoton()->pt(), diPhoton()->leadingPhoton()->eta(), diPhoton()->leadingPhoton()->phi());
      subleadPho.SetPtEtaPhi(diPhoton()->subLeadingPhoton()->pt(), diPhoton()->subLeadingPhoton()->eta(), diPhoton()->subLeadingPhoton()->phi());
      lead_Jet.SetPtEtaPhi(leadJet().pt(), leadJet().eta(), leadJet().phi());
      sublead_Jet.SetPtEtaPhi(subleadJet().pt(), subleadJet().eta(), subleadJet().phi());
      JJ=lead_Jet+sublead_Jet;
      PhoPho=leadPho+subleadPho;

//   float VBF_dRHH = deltaR((diPhoton()->leadingPhoton()->p4()+diPhoton()->subLeadingPhoton()->p4()),(leadJet().p4()+subleadJet().p4()));
   float VBF_dRHH = JJ.DeltaR(PhoPho);
   return VBF_dRHH;
}
float VBFDoubleHTag::getVBF_etaHH() const
{
   float VBF_etaHH = (diPhoton()->p4() + dijet()).eta();
   return VBF_etaHH;
}
float VBFDoubleHTag::getdiVBFjet_pt() const
{

   float diVBFjet_pt =  (VBFleadJet().p4()+VBFsubleadJet().p4()).pt();
   return diVBFjet_pt;
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4





