#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/TagAlgos/interface/PhotonDumpers.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

typedef edm::AnalyzerWrapper<flashgg::PhotonDumper> PhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::DiPhotonDumper> DiPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedDiPhotonDumper> CutBasedDiPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::DiPhotonTagDumper> DiPhotonTagDumper;

DEFINE_FWK_MODULE(PhotonDumper);
DEFINE_FWK_MODULE(DiPhotonDumper);
DEFINE_FWK_MODULE(CutBasedDiPhotonDumper);
DEFINE_FWK_MODULE(DiPhotonTagDumper);

