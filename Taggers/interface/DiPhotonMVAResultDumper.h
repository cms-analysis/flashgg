#ifndef DiPhotonMVAResultDumper_h
#define DiPhotonMVAResultDumper_h

#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg {
  typedef CollectionDumper<std::vector<DiPhotonMVAResult>> DiPhotonMVAResultDumper;
  typedef CollectionDumper<std::vector<DiPhotonMVAResult>, DiPhotonMVAResult, CutBasedClassifier<DiPhotonMVAResult>> CutBasedDiPhotonMVAResultDumper;
}

#endif
