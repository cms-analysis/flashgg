#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/MicroAOD/interface/CutBasedJetObjectSelector.h"
#include "flashgg/MicroAOD/interface/CutBasedPhotonViewSelector.h"
#include "flashgg/MicroAOD/interface/JetUtils.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelectorStream.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelectorPlusEvent.h"

#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/MicroAOD/interface/CutBasedMuonObjectSelector.h"

//typedef SingleObjectSelector <
//edm::View<flashgg::Muon>,
//    StringCutObjectSelector<flashgg::Muon, true>,
//    std::vector<flashgg::Muon>
//    > FlashggMuonCandidateSelector;

typedef ObjectSelectorStream <
SingleElementCollectionSelectorPlusEvent <
edm::View<flashgg::Muon>,
    flashgg::CutBasedMuonObjectSelector,
    std::vector<flashgg::Muon>
    >,
    std::vector<flashgg::Muon> > FlashggMuonCandidateSelector;

//GenericMuonSelector;

//typedef ObjectSelectorStream <
//SingleElementCollectionSelectorPlusEvent <
//edm::View<flashgg::Muon>,
//    flashgg::CutBasedMuonObjectSelector,
//    std::vector<flashgg::Muon>
//    >,
//    std::vector<flashgg::Muon> > GenericMuonSelector;


DEFINE_FWK_MODULE( FlashggMuonCandidateSelector );
//DEFINE_FWK_MODULE( GenericMuonSelector );


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

