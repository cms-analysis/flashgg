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

//typedef SingleObjectSelector <
//edm::View<flashgg::Jet>,
//    StringCutObjectSelector<flashgg::Jet, true>,
//    std::vector<flashgg::Jet>
//    > FlashggJetCandidateSelector;

typedef ObjectSelectorStream <
SingleElementCollectionSelectorPlusEvent <
edm::View<flashgg::Jet>,
    flashgg::CutBasedJetObjectSelector,
    std::vector<flashgg::Jet>
    >,
    std::vector<flashgg::Jet> > FlashggJetCandidateSelector;

//GenericJetSelector;

//typedef ObjectSelectorStream <
//SingleElementCollectionSelectorPlusEvent <
//edm::View<flashgg::Jet>,
//    flashgg::CutBasedJetObjectSelector,
//    std::vector<flashgg::Jet>
//    >,
//    std::vector<flashgg::Jet> > GenericJetSelector;


DEFINE_FWK_MODULE( FlashggJetCandidateSelector );
//DEFINE_FWK_MODULE( GenericJetSelector );


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

