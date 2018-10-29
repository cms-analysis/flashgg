#ifndef FLASHgg_TTHLeptonicTag_h
#define FLASHgg_TTHLeptonicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

    class TTHLeptonicTag: public DiPhotonTagBase
    {
    public:
        TTHLeptonicTag();
        TTHLeptonicTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        TTHLeptonicTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );

        ~TTHLeptonicTag();

        TTHLeptonicTag *clone() const override { return ( new TTHLeptonicTag( *this ) ); }

        const std::vector<edm::Ptr<Muon> > muons() const { return Muons_;}
        const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
        const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
        const std::vector<edm::Ptr<Jet> > bJets() const { return BJets_;}

        const std::vector<double>  leptonsPt() const { return lepPt_;}
        const std::vector<double>  leptonsE() const { return lepE_;}
        const std::vector<double>  leptonsPhi() const { return lepPhi_;}
        const std::vector<double>  leptonsEta() const { return lepEta_;}
        const std::vector<int>  leptonsType() const { return lepType_;}

        const float& metPt() const { return metPt_; }
        
        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setBJets( std::vector<edm::Ptr<Jet> > BJets )  { BJets_ = BJets;}
        void setMuons( std::vector<edm::Ptr<Muon> > Muons ) {Muons_ = Muons;}
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons ) {Electrons_ = Electrons;}
        void setLepPt( std::vector<double> lepPt ) { lepPt_ = lepPt; }
        void setLepE( std::vector<double> lepE ) { lepE_ = lepE; }
        void setLepEta( std::vector<double> lepEta ) { lepEta_ = lepEta; }
        void setLepPhi( std::vector<double> lepPhi ) { lepPhi_ = lepPhi; }
        void setLepType( std::vector<int> lepType ) { lepType_ = lepType; }
        void setMetPt( float& metPt ) { metPt_ = metPt; }

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kTTHLeptonic; }

        void setMvaRes(float mvaRes) {mvaRes_ = mvaRes;}
        float mvaRes() const {return mvaRes_;}

        private:
        std::vector<edm::Ptr<Muon> > Muons_;
        std::vector<edm::Ptr<Electron> > Electrons_;
        std::vector<edm::Ptr<Jet> > Jets_;
        std::vector<edm::Ptr<Jet> > BJets_;

        std::vector<double> lepPt_;
        std::vector<double> lepE_;
        std::vector<double> lepEta_;
        std::vector<double> lepPhi_;
        std::vector<int>    lepType_;

        float metPt_;
        
        float mvaRes_;
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

