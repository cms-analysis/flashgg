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

        int nBLoose() const {return Nbtagloose_;}
        int nBMedium() const {return Nbtagmedium_;}
        int nBTight() const {return Nbtagtight_;}

        float MetPt() const {return MetPt_;}
        float MetPhi() const {return MetPhi_;}
        float MT() const {return mT_;}
        int nGoodEls() const {return nGoodEls_;}
        int nGoodElsFromTau() const {return nGoodElsFromTau_;}
        int nGoodMus() const {return nGoodMus_;}
        int nGoodMusFromTau() const {return nGoodMusFromTau_;}
        int nGoodTaus() const {return nGoodTaus_;}

        float diphoMVARes() const { return diphoMVARes_; }
        int leadPhotonType() const { return leadPhotonType_; }
        int subleadPhotonType() const { return subleadPhotonType_; }

        double leadPhotonClosestDeltaR() const { return leadPhotonClosestDeltaR_; }
        double subleadPhotonClosestDeltaR() const { return subleadPhotonClosestDeltaR_; }
        double leadPhotonClosestPt() const { return leadPhotonClosestPt_; }
        double subleadPhotonClosestPt() const { return subleadPhotonClosestPt_; }
        double rand() const { return rand_; }

        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setBJets( std::vector<edm::Ptr<Jet> > BJets )  { BJets_ = BJets;}
        void setMuons( std::vector<edm::Ptr<Muon> > Muons ) {Muons_ = Muons;}
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons ) {Electrons_ = Electrons;}

        void setNBLoose( int nb ) { Nbtagloose_ = nb; }
        void setNBMedium( int nb ) { Nbtagmedium_ = nb; }
        void setNBTight( int nb ) { Nbtagtight_ = nb; }

        void setMetPt(float metPt) {MetPt_ = (float)metPt;}
        void setMetPhi(float metPhi) {MetPhi_ = (float)metPhi;}
        void setMT(float mT) {mT_ = (float)mT;}

    
        void setnGoodEls(int nGoodEls) {nGoodEls_ = nGoodEls;}
        void setnGoodElsFromTau(int nGoodElsFromTau) {nGoodElsFromTau_ = nGoodElsFromTau;}
        void setnGoodMus(int nGoodMus) {nGoodMus_ = nGoodMus;}
        void setnGoodMusFromTau(int nGoodMusFromTau) {nGoodMusFromTau_ = nGoodMusFromTau;}
        void setnGoodTaus(int nGoodTaus) {nGoodTaus_ = nGoodTaus;}
        void setDiphoMVARes(float diphoMVARes) {diphoMVARes_ = diphoMVARes;}
        
        void setLeadPhotonType(int leadPhotonType) {leadPhotonType_ = leadPhotonType; }
        void setSubleadPhotonType(int subleadPhotonType) {subleadPhotonType_ = subleadPhotonType; }

        void setLeadPhotonClosestDeltaR(double leadPhotonClosestDeltaR) { leadPhotonClosestDeltaR_ = leadPhotonClosestDeltaR;}
        void setSubleadPhotonClosestDeltaR(double subleadPhotonClosestDeltaR) { subleadPhotonClosestDeltaR_ = subleadPhotonClosestDeltaR;}
        void setLeadPhotonClosestPt(double leadPhotonClosestPt) { leadPhotonClosestPt_ = leadPhotonClosestPt;}
        void setSubleadPhotonClosestPt(double subleadPhotonClosestPt) { subleadPhotonClosestPt_ = subleadPhotonClosestPt;}
        void setRand(double rand) { rand_ = rand; }


        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kTTHLeptonic; }

        void setMvaRes(float mvaRes) {mvaRes_ = mvaRes;}
        float mvaRes() const {return mvaRes_;}

    private:
        std::vector<edm::Ptr<Muon> > Muons_;
        std::vector<edm::Ptr<Electron> > Electrons_;
        std::vector<edm::Ptr<Jet> > Jets_;
        std::vector<edm::Ptr<Jet> > BJets_;
        float mvaRes_;

        int Nbtagloose_;
        int Nbtagmedium_;
        int Nbtagtight_;

        float MetPt_;
        float MetPhi_;
        float mT_;
        int nGoodEls_;
        int nGoodElsFromTau_;
        int nGoodMus_;
        int nGoodMusFromTau_;
        int nGoodTaus_;
        float diphoMVARes_;

        int leadPhotonType_;
        int subleadPhotonType_;

        double leadPhotonClosestDeltaR_;
        double subleadPhotonClosestDeltaR_;
        double leadPhotonClosestPt_;
        double subleadPhotonClosestPt_;

        double rand_;
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

