#ifndef FLASHgg_Photon_h
#define FLASHgg_Photon_h

#include "DataFormats/PatCandidates/interface/Photon.h"

namespace flashgg {

  class Photon : public pat::Photon {

  public:
    Photon();
    Photon(const pat::Photon &);
    ~Photon();
    void setTestVariable(unsigned int i) { testVariable_ = i; }
    unsigned int const getTestVariable() const {return testVariable_;};

  private:
    unsigned int testVariable_;

    };
}

#endif
