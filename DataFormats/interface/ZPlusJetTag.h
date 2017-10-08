#ifndef FLASHgg_ZPlusJetTag_h
#define FLASHgg_ZPlusJetTag_h


#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

namespace flashgg {

    class ZPlusJetTag: public DiPhotonTagBase
    {
    public:
        ZPlusJetTag();
        ~ZPlusJetTag();

        ZPlusJetTag *clone() const { return ( new ZPlusJetTag( *this ) ); }

        ZPlusJetTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<Jet>, unsigned );

        edm::Ptr<Jet> jet() const { return theJet_; }
        unsigned nJets() const { return njets_; }

        const float jetPt() const { if (njets_>0) {return theJet_->pt();} else return -999;}
        const float jetEta() const { if (njets_>0) {return theJet_->eta();} else return -999; }
        const float jetPhi() const { if (njets_>0) {return theJet_->phi();} else return -999; }

        const float jet_HFHadronEnergyFraction() const { if (njets_>0) {return theJet_->HFHadronEnergyFraction();} else return -999; }
        const float jet_HFHadronEnergy() const { if (njets_>0) {return theJet_->HFHadronEnergy();} else return -999; }
        const float jet_HFHadronMultiplicity() const { if (njets_>0) {return theJet_->HFHadronMultiplicity();} else return -999; }
        const float jet_HFEMEnergyFraction() const { if (njets_>0) {return theJet_->HFEMEnergyFraction();} else return -999; }
        const float jet_HFEMEnergy() const { if (njets_>0) {return theJet_->HFEMEnergy();} else return -999; }
        const float jet_HFEMMultiplicity() const { if (njets_>0) {return theJet_->HFEMMultiplicity();} else return -999; }
 
        const float jet_rms() const { if (njets_>0) {return theJet_->rms();} else return -999; }
        const float jet_QGL() const { if (njets_>0) {return theJet_->QGL();} else return -999; }
        const float jet_rawPt() const { if (njets_>0) {return theJet_->correctedJet("Uncorrected").pt();} else return -999; }

        const bool jet_match() const { if (njets_>0) { return (theJet_->genJet() != 0);} else return -999; }

        edm::Ptr<DiPhotonCandidate> theZ() const { return diPhoton(); }
        const float zMass() const { return diPhoton()->mass(); }
        const float zPt() const { return diPhoton()->pt(); }
        const float zEta() const { return diPhoton()->eta(); }
        const float zPhi() const { return diPhoton()->phi(); }

        const float deltaPhiZJet() const { if (njets_>0) { return deltaPhi( diPhoton()->phi(), theJet_->phi() ) ;} else return -999; } // not the absolute value

        const int smartIndex() const { return diPhoton()->jetCollectionIndex(); }

        const float vtxZCoord() const { return diPhoton()->vtx()->z(); }

    private:
        edm::Ptr<Jet> theJet_;
        unsigned njets_;

    };

}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

