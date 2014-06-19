#include "flashgg/MicroAODAlgos/interface/PhotonPreselectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace flashgg {
  
  class Run1LegacyPhotonPreselector : public PhotonPreselectorBase {
  public:
    Run1LegacyPhotonPreselector(const edm::ParameterSet& conf) :
      PhotonPreselectorBase(conf) {}

    bool ispreselected(const edm::Ptr<pat::Photon>&) const override;

  };

  bool Run1LegacyPhotonPreselector::ispreselected(const edm::Ptr<pat::Photon>& g) const {
    // photon preselection is mickmicking as much as possible
    // what is documented in AN 2013/253 v8 page 41 table 18,
    // but with pat::Photons instead of reco objects
    
    // FIXME: return The sum of PT of the charged particle flow candidates within 0.02 < DeltaR < 0.2.
//    for( unsigned int i = 0 ; i < g->numberOfSourceCandidatePtrs() ; i++)
//    {
//        std::cout << "(*g->sourceCandidatePtr(i)).pt()= " << (*g->sourceCandidatePtr(i)).pt() << std::endl;
//    }
//    for( edm::RefVector<pat::PackedCandidateCollection>::iterator pf_it = g->associatedPackedPFCandidates().begin(); pf_it != g->associatedPackedPFCandidates().end(); pf_it++ ) {
//      std::cout << "(*pf_it)->pt()= " << (*pf_it)->pt() << std::endl;;
//    }
    
    if( g->r9() <= 0.9 )
    {
        if( g->isEB() && ! (g->hadronicOverEm() < 0.075 && g->sigmaIetaIeta() < 0.014) ) return false;
        if( g->isEE() && ! (g->hadronicOverEm() < 0.075 && g->sigmaIetaIeta() < 0.034) ) return false;
        if( ! (g->hcalTowerSumEtConeDR03() - 0.005 * g->pt() < 4.0
            && g->trkSumPtHollowConeDR03() - 0.002* g->pt()  < 4.0
//            && ChargedPFIso       < 4.0
            ) ) return false;
    } else {
        if( g->isEB() && ! (g->hadronicOverEm() < 0.082 && g->sigmaIetaIeta() < 0.014) ) return false;
        if( g->isEE() && ! (g->hadronicOverEm() < 0.075 && g->sigmaIetaIeta() < 0.034) ) return false;
        if( ! (g->hcalTowerSumEtConeDR03() - 0.005 * g->pt() < 50.0
            && g->trkSumPtHollowConeDR03() - 0.002* g->pt()  < 50.0
//            && ChargedPFIso       < 4.0
            ) ) return false;
    }
    return true;
  }

}

DEFINE_EDM_PLUGIN(FlashggPhotonPreselectorFactory,
		  flashgg::Run1LegacyPhotonPreselector,
		  "FlashggRun1LegacyPhotonPreselector");
