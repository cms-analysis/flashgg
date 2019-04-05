#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/H4GCandidate.h"

using namespace flashgg;
H4GCandidate::H4GCandidate():
phoVector_ (),
Vertices_(),
// vertex_ (),
vertex_diphoton_(),
phoP4Corrected_ (),
logSumPt2_(),
vtx_diphoton_z_(),
pho1_MVA_ (),
pho2_MVA_ (),
pho3_MVA_ (),
pho4_MVA_ (),
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
pho12_ (),
pho13_ (),
pho14_ (),
pho23_ (),
pho24_ (),
pho34_ (),
tp_ ()
{}

  H4GCandidate::~H4GCandidate() {}
  // H4GCandidate::H4GCandidate( std::vector<flashgg::Photon> phoVector, edm::Ptr<reco::Vertex> vertex, edm::Ptr<reco::Vertex> vertex_diphoton, reco::GenParticle::Point genVertex):
  // phoVector_(phoVector), vertex_(vertex), vertex_diphoton_(vertex_diphoton), genVertex_(genVertex)
  H4GCandidate::H4GCandidate( std::vector<flashgg::Photon> phoVector, std::vector<edm::Ptr<reco::Vertex>> Vertices, edm::Ptr<reco::Vertex> vertex_diphoton, reco::GenParticle::Point genVertex, math::XYZPoint BSPoint, std::vector <edm::Ptr<flashgg::DiPhotonCandidate>> diPhoPtrs):
  phoVector_(phoVector), Vertices_(Vertices), vertex_diphoton_(vertex_diphoton), genVertex_(genVertex), BSPoint_(BSPoint), diPhoPtrs_(diPhoPtrs)
  {
    //variables for vertex study
    int n_diphoPtr = diPhoPtrs_.size();
    std::vector<float> logSumPt2_;
    for (int d = 0; d < n_diphoPtr; d++)
    {
      edm::Ptr<flashgg::DiPhotonCandidate> tmp_diphoPtr = diPhoPtrs_[d];
      // cout << tmp_diphoPtr->logSumPt2() << endl;
      logSumPt2_.push_back(tmp_diphoPtr->logSumPt2());
    }

    float vtx_diphoton_z_ = vertex_diphoton_->z();
    float vtx_X = Vertices_[0]->x();
    float vtx_Y = Vertices_[0]->y();
    float vtx_Z = Vertices_[0]->z();
    // float vtx_X = vertex_->x();
    // float vtx_Y = vertex_->y();
    // float vtx_Z = vertex_->z();
    math::XYZVector vtx_Pos( vtx_X, vtx_Y, vtx_Z );
    if (phoVector_.size() > 0)
    {
    for( int p = 0; p < (int) phoVector_.size(); p++ )
    {
      float sc_X = phoVector_[p].superCluster()->x();
      float sc_Y = phoVector_[p].superCluster()->y();
      float sc_Z = phoVector_[p].superCluster()->z();
      math::XYZVector sc_Pos( sc_X, sc_Y, sc_Z );
      math::XYZVector direction = sc_Pos - vtx_Pos;
      math::XYZVector pho = ( direction.Unit() ) * ( phoVector_[p].energy() );
      math::XYZTLorentzVector corrected_p4( pho.x(), pho.y(), pho.z(), phoVector_[p].energy() );
      phoVector_[p].setP4(corrected_p4);
      phoP4Corrected_.push_back(phoVector_[p]);
    }
  }
  pho1_MVA_ = phoP4Corrected_.size() > 0 ? phoP4Corrected_[0].phoIdMvaDWrtVtx(Vertices_[0]) : -999;
  pho2_MVA_ = phoP4Corrected_.size() > 0 ? phoP4Corrected_[1].phoIdMvaDWrtVtx(Vertices_[0]) : -999;
  pho3_MVA_ = phoP4Corrected_.size() > 2 ? phoP4Corrected_[2].phoIdMvaDWrtVtx(Vertices_[0]) : -999;
  pho4_MVA_ = phoP4Corrected_.size() > 3 ? phoP4Corrected_[3].phoIdMvaDWrtVtx(Vertices_[0]) : -999;

    float minDM = 1000000;
    if (phoP4Corrected_.size() > 3)
    {
      for (int i1=0; i1 < (int) phoP4Corrected_.size(); i1++)
      {
        flashgg::Photon pho1 = phoP4Corrected_[i1];
        for (int i2=0; i2 < (int) phoP4Corrected_.size(); i2++)
        {
          if (i2 <= i1 ){continue;}
          flashgg::Photon pho2 = phoP4Corrected_[i2];
          for (int i3=0; i3 < (int) phoP4Corrected_.size(); i3++)
          {
            if (i3 == i2 || i3 == i1){continue;}
            flashgg::Photon pho3 = phoP4Corrected_[i3];
            for (int i4=0; i4 < (int) phoP4Corrected_.size(); i4++)
            {
              if (i4 <= i3){continue;}
              if (i4 == i1 || i4 == i2){continue;}
              flashgg::Photon pho4 = phoP4Corrected_[i4];
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
    }
    if (phoP4Corrected_.size() == 2)
    {
      tp_ = phoP4Corrected_[0].p4() + phoP4Corrected_[1].p4();
      pho12_ = phoP4Corrected_[0].p4() + phoP4Corrected_[1].p4();
    }
    else if (phoP4Corrected_.size() == 3)
    {
      tp_ = phoP4Corrected_[0].p4() + phoP4Corrected_[1].p4() + phoP4Corrected_[2].p4();
      pho12_ = phoP4Corrected_[0].p4() + phoP4Corrected_[1].p4();
      pho13_ = phoP4Corrected_[0].p4() + phoP4Corrected_[2].p4();
      pho23_ = phoP4Corrected_[1].p4() + phoP4Corrected_[2].p4();
    }
    else if (phoP4Corrected_.size() > 3 )
    {
      tp_ = phoP4Corrected_[0].p4() + phoP4Corrected_[1].p4() + phoP4Corrected_[2].p4() + phoP4Corrected_[3].p4();
      pho12_ = phoP4Corrected_[0].p4() + phoP4Corrected_[1].p4();
      pho13_ = phoP4Corrected_[0].p4() + phoP4Corrected_[2].p4();
      pho14_ = phoP4Corrected_[0].p4() + phoP4Corrected_[3].p4();
      pho23_ = phoP4Corrected_[1].p4() + phoP4Corrected_[2].p4();
      pho24_ = phoP4Corrected_[1].p4() + phoP4Corrected_[3].p4();
      pho34_ = phoP4Corrected_[2].p4() + phoP4Corrected_[3].p4();
    }
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
