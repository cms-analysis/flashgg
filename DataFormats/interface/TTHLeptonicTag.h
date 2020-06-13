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

        int leadPrompt() const { return leadPrompt_; }
        int subleadPrompt() const { return subleadPrompt_; }        
        int leadMad() const { return leadMad_; }
        int subleadMad() const { return subleadMad_; }
        int leadPythia() const { return leadPythia_; }
        int subleadPythia() const { return subleadPythia_; }
        int leadSimpleMomID() const { return lead_simpleMotherID_; }
        int leadSimpleMomStatus() const { return lead_simpleMotherStatus_; }
        int leadMomID() const { return lead_motherID_; }
        int leadMomMomID() const { return lead_motherMotherID_; }
        int leadPassFrix() const { return lead_passFrix_; }
        double leadSmallestDr() const { return lead_smallestDr_; }
        int subleadSimpleMomID() const { return sublead_simpleMotherID_; }
        int subleadSimpleMomStatus() const { return sublead_simpleMotherStatus_; }
        int subleadMomID() const { return sublead_motherID_; }
        int subleadMomMomID() const { return sublead_motherMotherID_; }
        int subleadPassFrix() const { return sublead_passFrix_; }
        double subleadSmallestDr() const { return sublead_smallestDr_; }
        
        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setBJets( std::vector<edm::Ptr<Jet> > BJets )  { BJets_ = BJets;}
        void setMuons( std::vector<edm::Ptr<Muon> > Muons ) {Muons_ = Muons;}
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons ) {Electrons_ = Electrons;}
        void setLepPt( std::vector<double> lepPt ) { lepPt_ = lepPt; }
        void setLepE( std::vector<double> lepE ) { lepE_ = lepE; }
        void setLepEta( std::vector<double> lepEta ) { lepEta_ = lepEta; }
        void setLepPhi( std::vector<double> lepPhi ) { lepPhi_ = lepPhi; }
        void setLepType( std::vector<int> lepType ) { lepType_ = lepType; }

        void setLeadPrompt(int leadPrompt) { leadPrompt_ = leadPrompt; }
        void setSubleadPrompt(int subleadPrompt) { subleadPrompt_ = subleadPrompt; }
        void setLeadMad(int leadMad) { leadMad_ = leadMad; }
        void setSubleadMad(int subleadMad) { subleadMad_ = subleadMad; }
        void setLeadPythia(int leadPythia) { leadPythia_ = leadPythia; }
        void setSubleadPythia(int subleadPythia) { subleadPythia_ = subleadPythia; }
        void setLeadSimpleMomID(int lead_simpleMotherID) { lead_simpleMotherID_ = lead_simpleMotherID; }
        void setLeadSimpleMomStatus(int lead_simpleMotherStatus) { lead_simpleMotherStatus_ = lead_simpleMotherStatus; }
        void setLeadMomID(int lead_motherID) { lead_motherID_ = lead_motherID; }
        void setLeadMomMomID(int lead_motherMotherID) { lead_motherMotherID_ = lead_motherMotherID; }
        void setLeadPassFrix(int lead_passFrix) { lead_passFrix_ = lead_passFrix; }
        void setLeadSmallestDr(double lead_smallestDr) { lead_smallestDr_ = lead_smallestDr; }
        void setSubleadSimpleMomID(int sublead_simpleMotherID) { sublead_simpleMotherID_ = sublead_simpleMotherID; }
        void setSubleadSimpleMomStatus(int sublead_simpleMotherStatus) { sublead_simpleMotherStatus_ = sublead_simpleMotherStatus; }
        void setSubleadMomID(int sublead_motherID) { sublead_motherID_ = sublead_motherID; }
        void setSubleadMomMomID(int sublead_motherMotherID) { sublead_motherMotherID_ = sublead_motherMotherID; }
        void setSubleadPassFrix(int sublead_passFrix) { sublead_passFrix_ = sublead_passFrix; }
        void setSubleadSmallestDr(double sublead_smallestDr) { sublead_smallestDr_ = sublead_smallestDr; }

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

        int leadPrompt_;
        int subleadPrompt_;
        int leadMad_;
        int subleadMad_;
        int leadPythia_;
        int subleadPythia_;
        int lead_simpleMotherID_;
        int lead_simpleMotherStatus_;
        int lead_motherID_;
        int lead_motherMotherID_;
        int lead_passFrix_;
        double lead_smallestDr_;
        int sublead_simpleMotherID_;
        int sublead_simpleMotherStatus_;
        int sublead_motherID_;
        int sublead_motherMotherID_;
        int sublead_passFrix_;
        double sublead_smallestDr_;
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

