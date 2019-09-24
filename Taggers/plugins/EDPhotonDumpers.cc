#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/PhotonDumpers.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

typedef edm::AnalyzerWrapper<flashgg::GenDiPhotonDumper> GenDiPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedGenDiPhotonDumper> CutBasedGenDiPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::PhotonDumper> PhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedPhotonDumper> CutBasedPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::DiPhotonDumper> DiPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedDiPhotonDumper> CutBasedDiPhotonDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedSinglePhotonViewDumper> CutBasedSinglePhotonViewDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedSingleVertexViewDumper> CutBasedSingleVertexViewDumper;
typedef edm::AnalyzerWrapper<flashgg::DiPhotonTagDumper> DiPhotonTagDumper;
typedef edm::AnalyzerWrapper<flashgg::StageOneDiPhotonTagDumper> StageOneDiPhotonTagDumper;

DEFINE_FWK_MODULE( GenDiPhotonDumper );
DEFINE_FWK_MODULE( CutBasedGenDiPhotonDumper );
DEFINE_FWK_MODULE( PhotonDumper );
DEFINE_FWK_MODULE( CutBasedPhotonDumper );
DEFINE_FWK_MODULE( DiPhotonDumper );
DEFINE_FWK_MODULE( CutBasedDiPhotonDumper );
DEFINE_FWK_MODULE( CutBasedSinglePhotonViewDumper );
DEFINE_FWK_MODULE( CutBasedSingleVertexViewDumper );
DEFINE_FWK_MODULE( DiPhotonTagDumper );
DEFINE_FWK_MODULE( StageOneDiPhotonTagDumper );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

