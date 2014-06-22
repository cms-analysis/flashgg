#include "flashgg/MicroAODAlgos/interface/PhotonPreselectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace flashgg {
  
  class LegacyPhotonPreselector : public PhotonPreselectorBase {
  public:
    LegacyPhotonPreselector(const edm::ParameterSet& conf) :
      PhotonPreselectorBase(conf) {}

    bool ispreselected(const edm::Ptr<pat::Photon>&, const edm::PtrVector<pat::PackedCandidate>&) const override;

  };

  bool LegacyPhotonPreselector::ispreselected(const edm::Ptr<pat::Photon>& g, const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers) const {
    // photon preselection is mickmicking as much as possible
    // what is documented in AN 2013/253 v8 page 41 table 18,
    // but with pat::Photons instead of reco objects
    float ChargedPFIso = 0;
    for(unsigned int i = 0 ; i < pfcandidatePointers.size() ; i++)
    {
      if( (*pfcandidatePointers[i]).charge() == 0 ) continue;
      if( deltaR( (*pfcandidatePointers[i]).p4(), g->p4() ) >= 0.2) continue;
      if( deltaR( (*pfcandidatePointers[i]).p4(), g->p4() ) <= 0.02) continue;
      ChargedPFIso += (*pfcandidatePointers[i]).pt(); 
    }
    
    if( g->r9() <= 0.9 )
    {
        if( g->isEB() && ! (g->hadronicOverEm() < 0.075 && g->sigmaIetaIeta() < 0.014) ) return false;
        if( g->isEE() && ! (g->hadronicOverEm() < 0.075 && g->sigmaIetaIeta() < 0.034) ) return false;
        if( ! (g->hcalTowerSumEtConeDR03() - 0.005 * g->pt() < 4.0
            && g->trkSumPtHollowConeDR03() - 0.002* g->pt()  < 4.0
            && ChargedPFIso       < 4.0
            ) ) return false;
    } else {
        if( g->isEB() && ! (g->hadronicOverEm() < 0.082 && g->sigmaIetaIeta() < 0.014) ) return false;
        if( g->isEE() && ! (g->hadronicOverEm() < 0.075 && g->sigmaIetaIeta() < 0.034) ) return false;
        if( ! (g->hcalTowerSumEtConeDR03() - 0.005 * g->pt() < 50.0
            && g->trkSumPtHollowConeDR03() - 0.002* g->pt()  < 50.0
            && ChargedPFIso       < 4.0
            ) ) return false;
    }
    return true;
  }

}

DEFINE_EDM_PLUGIN(FlashggPhotonPreselectorFactory,
		  flashgg::LegacyPhotonPreselector,
		  "FlashggLegacyPhotonPreselector");
