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

        const float jetPt() const { return theJet_->pt(); }
        const float jetEta() const { return theJet_->eta(); }
        const float jetPhi() const { return theJet_->phi(); }

        const float jet_HFHadronEnergyFraction() const { return theJet_->HFHadronEnergyFraction(); }
        const float jet_HFHadronEnergy() const { return theJet_->HFHadronEnergy(); }
        const float jet_HFHadronMultiplicity() const { return theJet_->HFHadronMultiplicity(); }
        const float jet_HFEMEnergyFraction() const { return theJet_->HFEMEnergyFraction(); }
        const float jet_HFEMEnergy() const { return theJet_->HFEMEnergy(); }
        const float jet_HFEMMultiplicity() const { return theJet_->HFEMMultiplicity(); }
 
        const float jet_rms() const { return theJet_->rms(); }
        const float jet_QGL() const { return theJet_->QGL(); }
        const float jet_rawPt() const { return theJet_->correctedJet("Uncorrected").pt(); }

        edm::Ptr<DiPhotonCandidate> theZ() const { return diPhoton(); }
        const float zMass() const { return diPhoton()->mass(); }
        const float zPt() const { return diPhoton()->pt(); }
        const float zEta() const { return diPhoton()->eta(); }
        const float zPhi() const { return diPhoton()->phi(); }

        const float deltaPhiZJet() const { return deltaPhi( diPhoton()->phi(), theJet_->phi() ) ; } // not the absolute value

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

