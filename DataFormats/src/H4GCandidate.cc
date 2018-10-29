#include "flashgg/DataFormats/interface/H4GCandidate.h"

using namespace flashgg;

H4GCandidate::H4GCandidate() {}

H4GCandidate::~H4GCandidate() {}

//-----------constructors--------------------------------------------------------
//---4 resolved photons constructor
H4GCandidate::H4GCandidate( edm::Ptr<flashgg::DiPhotonCandidate> dipho1, edm::Ptr<flashgg::DiPhotonCandidate> dipho2 ):
  n_photons(4), dipho1_(dipho1), dipho2_(dipho2)
{
  p4_ = dipho1_->p4() + dipho2_->p4();
}

//---1 resolved 1 merged pair constructor
H4GCandidate::H4GCandidate( edm::Ptr<flashgg::DiPhotonCandidate> dipho1, edm::Ptr<flashgg::Photon> photon ):
  n_photons(4), dipho1_(dipho1), singlePhoton_(photon)
{
  p4_ = dipho1_->p4() + singlePhoton_->p4();
}

//---2 merged pair constructor
H4GCandidate::H4GCandidate( edm::Ptr<flashgg::DiPhotonCandidate> dipho1):
  n_photons(4), dipho1_(dipho1)
{
  p4_ = dipho1_->p4();
}
