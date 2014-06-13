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
    //    void setShowerShapeVariable(std::string showerShapeVariableName, float showerShapeValue);
    //float getShowerShapeVariable(std::string showerShapeVariableName);
  private:
    unsigned int testVariable_;
    float e2nd_;
    float e2x5right_;
    float e2x5left_;
    float e2x5top_;
    float e2x5bottom_;
    float e2x5max_;
    float eright_;
    float eleft_;
    float etop_;
    float ebottom_;
  };
}

#endif
