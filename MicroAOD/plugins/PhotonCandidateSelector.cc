#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/MicroAOD/interface/CutBasedPhotonObjectSelector.h"
#include "flashgg/MicroAOD/interface/CutBasedPhotonViewSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelectorStream.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelectorPlusEvent.h"

#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

typedef SingleObjectSelector <
edm::View<flashgg::Photon>,
    StringCutObjectSelector<flashgg::Photon, true>,
    std::vector<flashgg::Photon>
    > FlashggPhotonSelector;

typedef ObjectSelectorStream <
SingleElementCollectionSelectorPlusEvent <
edm::View<flashgg::Photon>,
    flashgg::CutBasedPhotonObjectSelector,
    std::vector<flashgg::Photon>
    >,
    std::vector<flashgg::Photon> > GenericPhotonSelector;


typedef SingleObjectSelector <
edm::View<flashgg::SinglePhotonView>,
    StringCutObjectSelector<flashgg::SinglePhotonView, true>,
    std::vector<flashgg::SinglePhotonView>
    > PhotonViewSelector;

typedef ObjectSelectorStream <
SingleElementCollectionSelectorPlusEvent <
edm::View<flashgg::SinglePhotonView>,
    flashgg::CutBasedPhotonViewSelector,
    std::vector<flashgg::SinglePhotonView>
    >,
    std::vector<flashgg::SinglePhotonView> > GenericPhotonViewSelector;

typedef SingleObjectSelector<
    std::vector<flashgg::Photon>,
    StringCutObjectSelector<flashgg::Photon>,
    edm::RefVector<std::vector<flashgg::Photon> >
    > FlashggPhotonRefSelector;

DEFINE_FWK_MODULE( FlashggPhotonSelector );
DEFINE_FWK_MODULE( GenericPhotonSelector );
DEFINE_FWK_MODULE( FlashggPhotonRefSelector );

DEFINE_FWK_MODULE( PhotonViewSelector );
DEFINE_FWK_MODULE( GenericPhotonViewSelector );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

