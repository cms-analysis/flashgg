#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/MuMuDumpers.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

//-----------J. Tao from IHEP-Beijing--------------

typedef edm::AnalyzerWrapper<flashgg::DiMuonDumper> DiMuonDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedDiMuonDumper> CutBasedDiMuonDumper;
typedef edm::AnalyzerWrapper<flashgg::MuMuGammaDumper> MuMuGammaDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedMuMuGammaDumper> CutBasedMuMuGammaDumper;

DEFINE_FWK_MODULE( DiMuonDumper );
DEFINE_FWK_MODULE( CutBasedDiMuonDumper );
DEFINE_FWK_MODULE( MuMuGammaDumper );
DEFINE_FWK_MODULE( CutBasedMuMuGammaDumper );


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

