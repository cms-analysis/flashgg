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

#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/MicroAOD/interface/CutBasedMetObjectSelector.h"

//typedef SingleObjectSelector <
//edm::View<flashgg::Met>,
//    StringCutObjectSelector<flashgg::Met, true>,
//    std::vector<flashgg::Met>
//    > FlashggMetCandidateSelector;

typedef ObjectSelectorStream <
SingleElementCollectionSelectorPlusEvent <
edm::View<flashgg::Met>,
    flashgg::CutBasedMetObjectSelector,
    std::vector<flashgg::Met>
    >,
    std::vector<flashgg::Met> > FlashggMetCandidateSelector;

//GenericMetSelector;

//typedef ObjectSelectorStream <
//SingleElementCollectionSelectorPlusEvent <
//edm::View<flashgg::Met>,
//    flashgg::CutBasedMetObjectSelector,
//    std::vector<flashgg::Met>
//    >,
//    std::vector<flashgg::Met> > GenericMetSelector;


DEFINE_FWK_MODULE( FlashggMetCandidateSelector );
//DEFINE_FWK_MODULE( GenericMetSelector );


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

