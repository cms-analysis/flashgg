#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/PhotonJetDumpers.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

typedef edm::AnalyzerWrapper<flashgg::PhotonJetDumper> PhotonJetDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedPhotonJetDumper> CutBasedPhotonJetDumper;

DEFINE_FWK_MODULE( PhotonJetDumper );
DEFINE_FWK_MODULE( CutBasedPhotonJetDumper );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

