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

#include "flashgg/DataFormats/interface/GenJetExtra.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelectorStream.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelectorPlusEvent.h"

#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

typedef SingleObjectSelector <
    edm::View<flashgg::GenJetExtra>,
    StringCutObjectSelector<flashgg::GenJetExtra, true>,
    std::vector<flashgg::GenJetExtra>
    > GenJetExtraSelector;

namespace flashgg {
    class GreaterByPt
    {
    public:
        bool operator()( const flashgg::GenJetExtra &lh, const flashgg::GenJetExtra &rh ) const
        {
            return lh.pt() > rh.pt();
        };
    };
};

typedef ObjectSelector <
SortCollectionSelector <
edm::View<flashgg::GenJetExtra>,
    flashgg::GreaterByPt
    >,
    std::vector<flashgg::GenJetExtra>
    > GenJetExtraSorter;

DEFINE_FWK_MODULE( GenJetExtraSelector );
DEFINE_FWK_MODULE( GenJetExtraSorter );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

