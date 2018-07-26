#ifndef FLASHgg_TTHHadronicTag_h
#define FLASHgg_TTHHadronicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"

namespace flashgg {

    class TTHHadronicTag: public DiPhotonTagBase
    {
    public:
        TTHHadronicTag();
        TTHHadronicTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, std::vector<edm::Ptr<flashgg::Jet> >, std::vector<edm::Ptr<flashgg::Jet> > );
        TTHHadronicTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, std::vector<edm::Ptr<flashgg::Jet> >, std::vector<edm::Ptr<flashgg::Jet> > );
        TTHHadronicTag *clone() const override { return ( new TTHHadronicTag( *this ) ); }
        ~TTHHadronicTag();

        const std::vector<edm::Ptr<flashgg::Jet> > jetVector() const {return theJetVec_;}
        const std::vector<edm::Ptr<flashgg::Jet> > bJetVector() const {return theBJetVec_; }
        const std::vector<float> jetBTagValVec() const {return theJetBTagValVec_; }
        const edm::Ptr<flashgg::Met>  met() const { return MET_;}

        // Getters
        int nJet() const {return Njet_;}
        int nBLoose() const {return Nbtagloose_;}
        int nBMedium() const {return Nbtagmedium_;}
        int nBTight() const {return Nbtagtight_;}
        float leadJetPt() const {return leadJetPt_;}
        float subLeadJetPt() const {return subLeadJetPt_;}
        float sumJetPt() const {return sumJetPt_;}
        float maxBTagVal() const {return maxBTagVal_;}
        float secondMaxBTagVal() const {return secondMaxBTagVal_;}
        float thirdMaxBTagVal() const {return thirdMaxBTagVal_;}
        float fourthMaxBTagVal() const {return fourthMaxBTagVal_;}
        float tthMvaRes() const {return tthMvaRes_;}

        float MetPt() const {return MetPt_;}
        float MetPhi() const {return MetPhi_;}
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


        // Setters
        void setNjet( int nb ) { Njet_ = nb; }
        void setNBLoose( int nb ) { Nbtagloose_ = nb; }
        void setNBMedium( int nb ) { Nbtagmedium_ = nb; }
        void setNBTight( int nb ) { Nbtagtight_ = nb; }
        void setLeadJetPt( float dval ) { leadJetPt_ = dval;}
        void setSubLeadJetPt( float dval ) { subLeadJetPt_ = dval;}
        void setSumJetPt( float dval ) { sumJetPt_ = dval;}
        void setMaxBTagVal( float dval ) { maxBTagVal_ = dval;}
        void setSecondMaxBTagVal( float dval ) { secondMaxBTagVal_ = dval;}
        void setThirdMaxBTagVal( float dval ) { thirdMaxBTagVal_ = dval;}
        void setFourthMaxBTagVal( float dval ) { fourthMaxBTagVal_ = dval;}
        void setJetBTagValVec( std::vector<float> vec ) { theJetBTagValVec_ = vec;}
        void setMVAres(float val) {tthMvaRes_ = val;}
        void setMET( edm::Ptr<flashgg::Met> MET ) {MET_ = MET;}

        void setMetPt(float metPt) {MetPt_ = (float)metPt;}
        void setMetPhi(float metPhi) {MetPhi_ = (float)metPhi;}
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

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kTTHHadronic; }

    private:
        int Njet_;
        int Nbtagloose_;
        int Nbtagmedium_;
        int Nbtagtight_;
        std::vector<edm::Ptr<flashgg::Jet> > theJetVec_;
        std::vector<edm::Ptr<flashgg::Jet> > theBJetVec_;
        std::vector<float> theJetBTagValVec_;
        edm::Ptr<flashgg::Met> MET_;

        float leadJetPt_;
        float subLeadJetPt_;
        float sumJetPt_;
        float maxBTagVal_;
        float secondMaxBTagVal_;
        float thirdMaxBTagVal_;
        float fourthMaxBTagVal_;
        float tthMvaRes_;
 
        float MetPt_;
        float MetPhi_;
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

