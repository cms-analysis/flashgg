#ifndef flashgg_MuMuDumpers_h
#define flashgg_MuMuDumpers_h

#include "flashgg/DataFormats/interface/DiMuonCandidate.h"
#include "flashgg/DataFormats/interface/MuMuGammaCandidate.h"

#include "flashgg/Taggers/interface/CollectionDumper.h"

//-----------J. Tao from IHEP-Beijing--------------

namespace flashgg {

    typedef CollectionDumper<std::vector<DiMuonCandidate> > DiMuonDumper;
    typedef CollectionDumper<std::vector<DiMuonCandidate>,
            DiMuonCandidate,
            CutBasedClassifier<DiMuonCandidate> > CutBasedDiMuonDumper;

    typedef CollectionDumper<std::vector<MuMuGammaCandidate> > MuMuGammaDumper;
    typedef CollectionDumper<std::vector<MuMuGammaCandidate>,
            MuMuGammaCandidate,
            CutBasedClassifier<MuMuGammaCandidate> > CutBasedMuMuGammaDumper;

    //// namespace fwlite {
    //// 	typedef ::fwlite::AnalyzerWrapper<DiPhotonDumper> DiPhotonDumper;
    //// 	typedef ::fwlite::AnalyzerWrapper<PhotonDumper> PhotonDumper;
    //// 	typedef ::fwlite::AnalyzerWrapper<DiPhotonTagDumper> DiPhotonTagDumper;
    //// }

}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
