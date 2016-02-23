#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/Merger.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"

typedef Merger<edm::OwnVector<flashgg::DiPhotonTagBase> > TagMerger;
typedef Merger<std::vector<flashgg::VBFTag> > VBFTagMerger;
typedef Merger<std::vector<flashgg::ZPlusJetTag> > ZPlusJetTagMerger;


DEFINE_FWK_MODULE( TagMerger );
DEFINE_FWK_MODULE( VBFTagMerger );
DEFINE_FWK_MODULE( ZPlusJetTagMerger );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
