#ifndef FLASHgg_DiPhotonMVAResult_h
#define FLASHgg_DiPhotonMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

  class DiPhotonMVAResult {

  public:
    DiPhotonMVAResult();
    //    DiPhotonMVAResult(const DiPhotonMVAResult&) = default;  // C++11 only? Should happen automagically anyway

    // Input variables
    float leadptom;
    float subleadptom;
    float subleadmva;
    float leadmva;
    float leadeta;
    float subleadeta;
    float sigmarv;
    float sigmawv;
    float CosPhi;
    float vtxprob;
		float getMVAValue() const {return result;}
    // Output
    float result;
  };

  typedef std::map<edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult> DiPhotonMVAResultMap;

}

#endif
