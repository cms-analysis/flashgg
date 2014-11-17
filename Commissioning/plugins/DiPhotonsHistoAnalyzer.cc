#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/HistoAnalyzer.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

typedef HistoAnalyzer<edm::View<flashgg::DiPhotonCandidate> > DiPhotonsHistoAnalyzer;

DEFINE_FWK_MODULE( DiPhotonsHistoAnalyzer );
