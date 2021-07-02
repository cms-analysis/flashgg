#ifndef FLASHgg_ZHLeptonicTag_h
#define FLASHgg_ZHLeptonicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

namespace flashgg {

    class ZHLeptonicTag: public DiPhotonTagBase
    {
    public:
        ZHLeptonicTag();
        ZHLeptonicTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        ZHLeptonicTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        ~ZHLeptonicTag();

        ZHLeptonicTag *clone() const override { return ( new ZHLeptonicTag( *this ) ); }
        
        const std::vector<edm::Ptr<Muon> > muons() const { return Muons_;}
        const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
        const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
        const edm::Ptr<flashgg::Met>  met() const { return MET_;}

        const bool associatedZ() const { return associatedZ_; }
        const bool associatedW() const { return associatedW_; }
        const bool VhasNeutrinos() const { return VhasNeutrinos_; }
        const bool VhasLeptons() const { return VhasLeptons_; }
        const bool VhasHadrons() const { return VhasHadrons_; }
        const float Vpt() const { return Vpt_; }

        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setMuons( std::vector<edm::Ptr<Muon> > Muons ) {Muons_ = Muons;}
        void setMET( edm::Ptr<flashgg::Met> MET ) {MET_ = MET;}
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons ) {Electrons_ = Electrons;}

        void setAssociatedZ( const bool &val ) { associatedZ_ = val; }
        void setAssociatedW( const bool &val ) { associatedW_ = val; }
        void setVhasNeutrinos( const bool &val ) { VhasNeutrinos_ = val; }
        void setVhasLeptons( const bool &val ) { VhasLeptons_ = val; }
        void setVhasHadrons( const bool &val ) { VhasHadrons_ = val; }
        void setVpt( const float &val ) { Vpt_ = val; }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kZHLeptonic; }

    private:
        std::vector<edm::Ptr<Muon> > Muons_;
        std::vector<edm::Ptr<Electron> > Electrons_;
        std::vector<edm::Ptr<Jet> > Jets_;
        edm::Ptr<flashgg::Met> MET_;

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

