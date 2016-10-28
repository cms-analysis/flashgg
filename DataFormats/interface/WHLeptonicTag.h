#ifndef FLASHgg_WHLeptonicTag_h
#define FLASHgg_WHLeptonicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

namespace flashgg {

    class WHLeptonicTag: public DiPhotonTagBase
    {
    public:
        WHLeptonicTag();
        WHLeptonicTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        WHLeptonicTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        ~WHLeptonicTag();

        WHLeptonicTag *clone() const override { return ( new WHLeptonicTag( *this ) ); }

        const std::vector<edm::Ptr<Muon> > muons() const { return Muons_;}
        const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
        const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
        //const std::vector<edm::Ptr<flashgg::Met> > met() const { return MET_;}
        const edm::Ptr<flashgg::Met> met() const { return MET_;}

        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setMuons( std::vector<edm::Ptr<Muon> >Muons ) {Muons_ = Muons;}
        //void setMET( std::vector<edm::Ptr<flashgg::Met> > MET ) {MET_ = MET;}
        void setMET( edm::Ptr<flashgg::Met>  MET ) {MET_ = MET;}
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons ) {Electrons_ = Electrons;}

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kWHLeptonic; }

    private:
        std::vector<edm::Ptr<Muon> > Muons_;
        std::vector<edm::Ptr<Electron> > Electrons_;
        std::vector<edm::Ptr<Jet> > Jets_;
        //std::vector<edm::Ptr<flashgg::Met> > MET_;
        edm::Ptr<flashgg::Met> MET_;
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

