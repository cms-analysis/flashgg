#ifndef FLASHgg_VHMetTag_h
#define FLASHgg_VHMetTag_h

#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class VHMetTag: public DiPhotonTagBase
    {
    public:
        VHMetTag();
        ~VHMetTag();

        VHMetTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        VHMetTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );

        VHMetTag *clone() const override { return ( new VHMetTag( *this ) ); }

        const edm::Ptr<flashgg::Met> met() const {return theMet_;}
        const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
        const edm::Ptr<DiPhotonCandidate> diPhotonCandidate() const { return theDiPhotonCandidate_;}
        const float MinDeltaPhiJetMet() const { return minDeltaPhiJetMet_; }
        const float MaxJetDeepCSV() const { return max_dcsv_val_; }

        const bool associatedZ() const { return associatedZ_; }
        const bool associatedW() const { return associatedW_; }
        const bool VhasNeutrinos() const { return VhasNeutrinos_; }
        const bool VhasLeptons() const { return VhasLeptons_; }
        const bool VhasHadrons() const { return VhasHadrons_; }
        const float Vpt() const { return Vpt_; }

        void setMet( edm::Ptr<flashgg::Met> );
        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setMinDeltaPhiJetMet( float val ) { minDeltaPhiJetMet_ = val; }
        void setMaxJetDeepCSV( float val ) { max_dcsv_val_ = val; }

        void setAssociatedZ( const bool &val ) { associatedZ_ = val; }
        void setAssociatedW( const bool &val ) { associatedW_ = val; }
        void setVhasNeutrinos( const bool &val ) { VhasNeutrinos_ = val; }
        void setVhasLeptons( const bool &val ) { VhasLeptons_ = val; }
        void setVhasHadrons( const bool &val ) { VhasHadrons_ = val; }
        void setVpt( const float &val ) { Vpt_ = val; }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kVHMet; }

    private:
        edm::Ptr<DiPhotonCandidate> theDiPhotonCandidate_;
        edm::Ptr<flashgg::Met> theMet_;
        std::vector<edm::Ptr<Jet> > Jets_;
        float minDeltaPhiJetMet_;
        float max_dcsv_val_;

        bool associatedZ_;
        bool associatedW_;
        bool VhasNeutrinos_;
        bool VhasLeptons_;
        bool VhasHadrons_;
        float Vpt_;

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

