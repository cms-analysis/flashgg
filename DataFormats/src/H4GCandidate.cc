#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "flashgg/DataFormats/interface/H4GCandidate.h"

using namespace flashgg;

H4GCandidate::H4GCandidate():
pho1_ (),
pho2_ (),
pho3_ (),
pho4_(),
pho12_ (),
pho13_ (),
pho14_ (),
pho23_ (),
pho24_ (),
pho34_ (),
pho1_r9_ (-9999.),
pho2_r9_ (-9999.),
pho3_r9_ (-9999.),
pho4_r9_ (-9999.),
pho1_full5x5_r9_ (-9999.),
pho2_full5x5_r9_ (-9999.),
pho3_full5x5_r9_ (-9999.),
pho4_full5x5_r9_ (-9999.),
pho1_EGMVA_ (-9999.),
pho2_EGMVA_ (-9999.),
pho3_EGMVA_ (-9999.),
pho4_EGMVA_ (-9999.),
pho1_MVA_ (-9999.),
pho2_MVA_ (-9999.),
pho3_MVA_ (-9999.),
pho4_MVA_ (-9999.),
dp1_ (),
dp2_ (),
dp1_pho1_ (),
dp1_pho2_ (),
dp2_pho1_ (),
dp2_pho2_ (),
dp1_ipho1_ (),
dp1_ipho2_ (),
dp2_ipho1_ (),
dp2_ipho2_ (),
tp_ ()


{}

H4GCandidate::~H4GCandidate() {}

//-----------constructors--------------------------------------------------------
//---4  photons constructor
H4GCandidate::H4GCandidate( flashgg::Photon pho1, flashgg::Photon pho2, flashgg::Photon pho3, flashgg::Photon pho4, edm::Ptr<reco::Vertex> vertex):
pho1_(pho1), pho2_(pho2), pho3_(pho3), pho4_(pho4), vertex_(vertex)
{
  n_photons = 4;
  phoVec_.push_back(pho1_);phoVec_.push_back(pho2_); phoVec_.push_back(pho3_); phoVec_.push_back(pho4_);
  pho12_ = pho1_.p4() + pho2_.p4();
  pho13_ = pho1_.p4() + pho3_.p4();
  pho14_ = pho1_.p4() + pho4_.p4();
  pho23_ = pho2_.p4() + pho3_.p4();
  pho24_ = pho2_.p4() + pho4_.p4();
  pho34_ = pho3_.p4() + pho4_.p4();
  pho1_r9_ = pho1_.old_r9();
  pho2_r9_ = pho2_.old_r9();
  pho3_r9_ = pho3_.old_r9();
  pho4_r9_ = pho4_.old_r9();
  pho1_full5x5_r9_ = pho1_.full5x5_r9();
  pho2_full5x5_r9_ = pho2_.full5x5_r9();
  pho3_full5x5_r9_ = pho3_.full5x5_r9();
  pho4_full5x5_r9_ = pho4_.full5x5_r9();
  pho1_EGMVA_ = pho1_.userFloat("EGMPhotonMVA");
  pho2_EGMVA_ = pho2_.userFloat("EGMPhotonMVA");
  pho3_EGMVA_ = pho3_.userFloat("EGMPhotonMVA");
  pho4_EGMVA_ = pho4_.userFloat("EGMPhotonMVA");
  pho1_MVA_ = pho1_.phoIdMvaDWrtVtx(vertex_);
  pho2_MVA_ = pho2_.phoIdMvaDWrtVtx(vertex_);
  pho3_MVA_ = pho3_.phoIdMvaDWrtVtx(vertex_);
  pho4_MVA_ = pho4_.phoIdMvaDWrtVtx(vertex_);
  float minDM = 1000000;
  for (int i1=0; i1 < (int) phoVec_.size(); i1++)
  {
    flashgg::Photon pho1 = phoVec_[i1];
    for (int i2=0; i2 < (int) phoVec_.size(); i2++)
    {
      if (i2 <= i1 ){continue;}
      flashgg::Photon pho2 = phoVec_[i2];
      for (int i3=0; i3 < (int) phoVec_.size(); i3++)
      {
        if (i3 == i2 || i3 == i1){continue;}
        flashgg::Photon pho3 = phoVec_[i3];
        for (int i4=0; i4 < (int) phoVec_.size(); i4++)
        {
          if (i4 <= i3){continue;}
          if (i4 == i1 || i4 == i2){continue;}
          flashgg::Photon pho4 = phoVec_[i4];
          auto dipho1 = pho1.p4() + pho2.p4();
          auto dipho2 = pho3.p4() + pho4.p4();
          float deltaM = fabs( dipho1.mass() - dipho2.mass());
          if (deltaM < minDM){
            minDM = deltaM;
            dp1_pho1_ = pho1.p4();
            dp1_ipho1_ = i1;
            dp1_pho2_ = pho2.p4();
            dp1_ipho2_ = i2;
            dp2_pho1_ = pho3.p4();
            dp2_ipho1_ = i3;
            dp2_pho2_ = pho4.p4();
            dp2_ipho2_ = i4;
            if ((pho1.pt() + pho2.pt()) > (pho3.pt() + pho4.pt()) )
            {
              dp1_ = dipho1;
              dp2_ = dipho2;
            }
            else if ((pho1.pt() + pho2.pt()) < (pho3.pt() + pho4.pt()) )
            {
              dp1_ = dipho2;
              dp2_ = dipho1;
            }
          }
        }
      }
    }
  }
  tp_ = pho1_.p4() + pho2_.p4() + pho3_.p4() + pho4_.p4();
}

float H4GCandidate::getCosThetaStar_CS(float ebeam) const {
    TLorentzVector p1, p2;
    p1.SetPxPyPzE(0, 0,  ebeam, ebeam);
    p2.SetPxPyPzE(0, 0, -ebeam, ebeam);

    reco::Candidate::LorentzVector h_lor = dp1_ + dp2_;
    TLorentzVector h;
    h.SetPxPyPzE(h_lor.Px(),h_lor.Py(),h_lor.Pz(),h_lor.E()) ;

    TVector3 boost = - h.BoostVector();
    p1.Boost(boost);
    p2.Boost(boost);
    reco::Candidate::LorentzVector a1_lor = dp1_;
    TLorentzVector a_1;
    a_1.SetPxPyPzE(a1_lor.Px(),a1_lor.Py(),a1_lor.Pz(),a1_lor.E()) ;
    a_1.Boost(boost);

    TVector3 CSaxis = p1.Vect().Unit() - p2.Vect().Unit();
    CSaxis.Unit();


    return cos(   CSaxis.Angle( a_1.Vect().Unit() )    );
}
std::vector<float> H4GCandidate::CosThetaAngles() const {
    //helicityThetas[0] = cosTheta_gg
    //helicityThetas[1] = cosTheta_bb

    std::vector<float> helicityThetas;

    TLorentzVector Boosted_a1(0,0,0,0);
    Boosted_a1.SetPxPyPzE(dp1_.px(),dp1_.py(),dp1_.pz(),dp1_.energy()) ;
    TLorentzVector BoostedLeadingPhoton_a1(0,0,0,0);
    BoostedLeadingPhoton_a1.SetPxPyPzE(dp1_pho1_.px(),dp1_pho1_.py(),dp1_pho1_.pz(),dp1_pho1_.energy()) ;

    helicityThetas.push_back( HelicityCosTheta(Boosted_a1, BoostedLeadingPhoton_a1));

    TLorentzVector Boosted_a2(0,0,0,0);
    Boosted_a2.SetPxPyPzE(dp2_.px(),dp2_.py(),dp2_.pz(),dp2_.energy()) ;
    TLorentzVector BoostedLeadingPhoton_a2(0,0,0,0);
    BoostedLeadingPhoton_a2.SetPxPyPzE(dp2_pho1_.px(),dp2_pho1_.py(),dp2_pho1_.pz(),dp2_pho1_.energy()) ;

    helicityThetas.push_back( HelicityCosTheta(Boosted_a2, BoostedLeadingPhoton_a2));

    return helicityThetas;

}

float H4GCandidate::HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const
{
    TVector3 BoostVector = Booster.BoostVector();
    Boosted.Boost( -BoostVector.x(), -BoostVector.y(), -BoostVector.z() );
    return Boosted.CosTheta();
}

//---3  photons constructor
H4GCandidate::H4GCandidate( flashgg::Photon pho1, flashgg::Photon pho2, flashgg::Photon pho3, edm::Ptr<reco::Vertex> vertex):
pho1_(pho1), pho2_(pho2), pho3_(pho3), vertex_(vertex)
{
  n_photons = 3;
  pho12_ = pho1_.p4() + pho2_.p4();
  pho13_ = pho1_.p4() + pho3_.p4();
  pho23_ = pho2_.p4() + pho3_.p4();
  pho1_r9_ = pho1.old_r9();
  pho2_r9_ = pho2.old_r9();
  pho3_r9_ = pho3.old_r9();
  pho1_full5x5_r9_ = pho1_.full5x5_r9();
  pho2_full5x5_r9_ = pho2_.full5x5_r9();
  pho3_full5x5_r9_ = pho3_.full5x5_r9();
  pho1_EGMVA_ = pho1_.userFloat("EGMPhotonMVA");
  pho2_EGMVA_ = pho2_.userFloat("EGMPhotonMVA");
  pho3_EGMVA_ = pho3_.userFloat("EGMPhotonMVA");
  pho1_MVA_ = pho1_.phoIdMvaDWrtVtx(vertex_);
  pho2_MVA_ = pho2_.phoIdMvaDWrtVtx(vertex_);
  pho3_MVA_ = pho3_.phoIdMvaDWrtVtx(vertex_);
  tp_ = pho1_.p4() + pho2_.p4() + pho3_.p4();
}
//---2  photons constructor
H4GCandidate::H4GCandidate( flashgg::Photon pho1, flashgg::Photon pho2, edm::Ptr<reco::Vertex> vertex):
pho1_(pho1), pho2_(pho2), vertex_(vertex)
{
  n_photons = 2;
  pho12_ = pho1_.p4() + pho2_.p4();
  pho1_r9_ = pho1.old_r9();
  pho2_r9_ = pho2.old_r9();
  pho1_full5x5_r9_ = pho1_.full5x5_r9();
  pho2_full5x5_r9_ = pho2_.full5x5_r9();
  pho1_EGMVA_ = pho1_.userFloat("EGMPhotonMVA");
  pho2_EGMVA_ = pho2_.userFloat("EGMPhotonMVA");
  pho1_MVA_ = pho1_.phoIdMvaDWrtVtx(vertex_);
  pho2_MVA_ = pho2_.phoIdMvaDWrtVtx(vertex_);
  tp_ = pho1_.p4() + pho2_.p4();
}
