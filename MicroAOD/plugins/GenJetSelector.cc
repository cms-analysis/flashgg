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

#include "DataFormats/JetReco/interface/GenJet.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelectorStream.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelectorPlusEvent.h"

#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

typedef SingleObjectSelector <
    edm::View<reco::GenJet>,
    StringCutObjectSelector<reco::GenJet, true>,
    std::vector<reco::GenJet>
    > GenJetSelector;

namespace flashgg {
    class GreaterByPt
    {
    public:
        bool operator()( const reco::GenJet &lh, const reco::GenJet &rh ) const
        {
            return lh.pt() > rh.pt();
        };
    };
};

typedef ObjectSelector <
SortCollectionSelector <
edm::View<reco::GenJet>,
    flashgg::GreaterByPt
    >,
    std::vector<reco::GenJet>
    > GenJetSorter;

DEFINE_FWK_MODULE( GenJetSelector );
DEFINE_FWK_MODULE( GenJetSorter );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

