#ifndef FLASHgg_TTHHadronicTag_h
#define FLASHgg_TTHHadronicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


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
        int nJet() const {return Njet_;}
        int nBLoose() const {return Nbtagloose_;}
        int nBMedium() const {return Nbtagmedium_;}
        int nBTight() const {return Nbtagtight_;}
        float leadJetPt() const {return leadJetPt_;}
        float subLeadJetPt() const {return subLeadJetPt_;}
        float sumJetPt() const {return sumJetPt_;}
        float maxBTagVal() const {return maxBTagVal_;}
        float secondMaxBTagVal() const {return secondMaxBTagVal_;}
        float tthMvaRes() const {return tthMvaRes_;}
        void setNjet( int nb ) { Njet_ = nb; }
        void setNBLoose( int nb ) { Nbtagloose_ = nb; }
        void setNBMedium( int nb ) { Nbtagmedium_ = nb; }
        void setNBTight( int nb ) { Nbtagtight_ = nb; }
        void setLeadJetPt( float dval ) { leadJetPt_ = dval;}
        void setSubLeadJetPt( float dval ) { subLeadJetPt_ = dval;}
        void setSumJetPt( float dval ) { sumJetPt_ = dval;}
        void setMaxBTagVal( float dval ) { maxBTagVal_ = dval;}
        void setSecondMaxBTagVal( float dval ) { secondMaxBTagVal_ = dval;}
        void setJetBTagValVec( std::vector<float> vec ) { theJetBTagValVec_ = vec;}
        void setMVAres(float val) {tthMvaRes_ = val;}

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kTTHHadronic; }

    private:
        int Njet_;
        int Nbtagloose_;
        int Nbtagmedium_;
        int Nbtagtight_;
        std::vector<edm::Ptr<flashgg::Jet> > theJetVec_;
        std::vector<edm::Ptr<flashgg::Jet> > theBJetVec_;
        std::vector<float> theJetBTagValVec_;
        float leadJetPt_;
        float subLeadJetPt_;
        float sumJetPt_;
        float maxBTagVal_;
        float secondMaxBTagVal_;
        float tthMvaRes_;

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

