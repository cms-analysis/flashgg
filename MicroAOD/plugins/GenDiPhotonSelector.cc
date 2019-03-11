/* \class CandViewRefSelector
 *
 * Candidate Selector based on a configurable cut.
 * Reads a edm::View<Candidate> as input
 * and saves a OwnVector of clones.
 * Usage:
 *
 * module selectedCands = CandViewSelector {
 *   InputTag src = myCollection
 *   string cut = "pt > 15.0"
 * };
 *
 * \author: Luca Lista, INFN
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "flashgg/DataFormats/interface/GenDiPhoton.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelectorStream.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelectorPlusEvent.h"

#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

typedef SingleObjectSelector <
edm::View<flashgg::GenDiPhoton>,
    StringCutObjectSelector<flashgg::GenDiPhoton, true>,
    std::vector<flashgg::GenDiPhoton>
    > GenDiPhotonSelector;

typedef ObjectSelector <
SortCollectionSelector <
edm::View<flashgg::GenDiPhoton>,
    std::greater<flashgg::GenDiPhoton>
    >,
    std::vector<flashgg::GenDiPhoton>
    > GenDiPhotonSorter;


class greaterByHt
{ 
public: 
    bool operator()(const flashgg::GenDiPhoton& a, const flashgg::GenDiPhoton& b) const { return a.sumHt() > b.sumHt(); }
};

typedef ObjectSelector <
SortCollectionSelector <
edm::View<flashgg::GenDiPhoton>,
    greaterByHt
    >,
    std::vector<flashgg::GenDiPhoton>
    > GenDiPhotonDiJetSorter;

DEFINE_FWK_MODULE( GenDiPhotonSelector );
DEFINE_FWK_MODULE( GenDiPhotonSorter );
DEFINE_FWK_MODULE( GenDiPhotonDiJetSorter );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

