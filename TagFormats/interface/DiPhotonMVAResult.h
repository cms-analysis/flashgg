#ifndef FLASHgg_DiPhotonMVAResult_h
#define FLASHgg_DiPhotonMVAResult_h

#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

  // Yes, it's a struct. Why not, right?
  // We'll make it a class if it ever actually needs any member functions or interfaces
  struct DiPhotonMVAResult {

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

    // Output
    float result;
  };

  typedef std::map<edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult> DiPhotonMVAResultMap;

}

#endif
