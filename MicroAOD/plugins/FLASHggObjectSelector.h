#ifndef flashgg_MicroAOD__FLASHggObjectSelector_h
#define flashgg_MicroAOD__FLASHggObjectSelector_h

#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"

#include <vector>


namespace flashgg {

    typedef SingleObjectSelector <
    std::vector<Electron>,
        StringCutObjectSelector<Electron>
        > FLASHggElectronSelector;

    typedef SingleObjectSelector <
    std::vector<Muon>,
        StringCutObjectSelector<Muon>
        > FLASHggMuonSelector;

    typedef SingleObjectSelector <
    std::vector<Photon>,
        StringCutObjectSelector<Photon>
        > FLASHggPhotonSelector;

    typedef SingleObjectSelector <
    std::vector<Jet>,
        StringCutObjectSelector<Jet>
        > FLASHggJetSelector;

    typedef SingleObjectSelector <
        std::vector<DiPhotonCandidate>,
        StringCutObjectSelector<DiPhotonCandidate>
        > FLASHggDiPhotonSelector;
    

}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
