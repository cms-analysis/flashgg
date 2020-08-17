#ifndef FLASHgg_VHTagTruth_h
#define FLASHgg_VHTagTruth_h

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace flashgg {

    class VHTagTruth : public TagTruthBase
    {

    public:

        VHTagTruth();
        ~VHTagTruth();
        //        VHTagTruth(const VHTagTruth &b);


        const bool associatedZ() const { return associatedZ_; }
        const bool associatedW() const { return associatedW_; }
        const bool VhasDaughters() const { return VhasDaughters_; }
        const bool VhasNeutrinos() const { return VhasNeutrinos_; }
        const bool VhasLeptons() const { return VhasLeptons_; }
        const bool VhasHadrons() const { return VhasHadrons_; }
        const bool VhasMissingLeptons() const { return VhasMissingLeptons_; }
        const float Vpt() const { return Vpt_; }
        //Setter methods
        void setAssociatedZ( const bool &val ) { associatedZ_ = val; }
        void setAssociatedW( const bool &val ) { associatedW_ = val; }
        void setVhasDaughters( const bool &val ) { VhasDaughters_ = val; }
        void setVhasNeutrinos( const bool &val ) { VhasNeutrinos_ = val; }
        void setVhasLeptons( const bool &val ) { VhasLeptons_ = val; }
        void setVhasHadrons( const bool &val ) { VhasHadrons_ = val; }
        void setVhasMissingLeptons( const bool &val ) { VhasMissingLeptons_ = val; }
        void setVpt( const float &val ) { Vpt_ = val; }
        
        //Counts
        //Clone
        VHTagTruth *clone() const;

    private:
        bool associatedZ_;
        bool associatedW_;
        bool VhasDaughters_;
        bool VhasNeutrinos_;
        bool VhasLeptons_;
        bool VhasHadrons_;
        bool VhasMissingLeptons_;
        float Vpt_;

        //edm::Ptr<reco::GenJet> closestGenJetToLeadingJet_;
        //edm::Ptr<reco::GenJet> subSubLeadingGenJet_;
        //edm::Ptr<flashgg::DiPhotonCandidate> diPhoton_;
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
