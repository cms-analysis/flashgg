#ifndef FLASHgg_LHEInfoObject_h
#define FLASHgg_LHEInfoObject_h

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <vector>
#include <string>

using namespace std;

namespace flashgg {

    class LHEInfoObject
    {

    public:
        LHEInfoObject();
        virtual ~LHEInfoObject();

        // Functions for dumping
        float pt_iP1()  const { return ( incomingPartons_.size() > 0 ? incomingPartons_[0]->pt()  : -9999. ); }
        float eta_iP1() const { return ( incomingPartons_.size() > 0 ? incomingPartons_[0]->eta() : -9999. ); }
        float phi_iP1() const { return ( incomingPartons_.size() > 0 ? incomingPartons_[0]->phi() : -9999. ); }

        float pt_iP2()  const { return ( incomingPartons_.size() > 1 ? incomingPartons_[1]->pt()  : -9999. ); }
        float eta_iP2() const { return ( incomingPartons_.size() > 1 ? incomingPartons_[1]->eta() : -9999. ); }
        float phi_iP2() const { return ( incomingPartons_.size() > 1 ? incomingPartons_[1]->phi() : -9999. ); }

        float pt_higgs()  const { return higgsBoson_->pt(); }
        float eta_higgs() const { return higgsBoson_->eta(); }
        float phi_higgs() const { return higgsBoson_->phi(); }

        float pt_oP1()  const { return ( outgoingPartons_.size() > 0 ? outgoingPartons_[0]->pt()  : -9999. ); }
        float eta_oP1() const { return ( outgoingPartons_.size() > 0 ? outgoingPartons_[0]->eta() : -9999. ); }
        float phi_oP1() const { return ( outgoingPartons_.size() > 0 ? outgoingPartons_[0]->phi() : -9999. ); }

        float pt_oP2()  const { return ( outgoingPartons_.size() > 1 ? outgoingPartons_[1]->pt()  : -9999. ); }
        float eta_oP2() const { return ( outgoingPartons_.size() > 1 ? outgoingPartons_[1]->eta() : -9999. ); }
        float phi_oP2() const { return ( outgoingPartons_.size() > 1 ? outgoingPartons_[1]->phi() : -9999. ); }

        // Scales
        int nJets() const { return Njets_; }
        double alphaS() const { return alphaS_; }

        //Getter methods
        const std::vector<edm::Ptr<reco::GenParticle>> incomingPartons() const {return incomingPartons_;}
        const std::vector<edm::Ptr<reco::GenParticle>> outgoingPartons() const {return outgoingPartons_;}
        const edm::Ptr<reco::GenParticle> higgsBoson() const {return higgsBoson_;}

        //Setter methods
        void setIncomingPartons( const std::vector<edm::Ptr<reco::GenParticle>> &val ) { incomingPartons_ = val; }
        void setOutgoingPartons( const std::vector<edm::Ptr<reco::GenParticle>> &val ) { outgoingPartons_ = val; }
        void setHiggsBoson( const edm::Ptr<reco::GenParticle> &val ) { higgsBoson_ = val; }

        void setNjets( const int njets ) { Njets_ = njets; }
        void setAlphaS( const double alphas ) { alphaS_ = alphas; }

        //Clone
        LHEInfoObject *clone() const;
        
    private:
        std::vector<edm::Ptr<reco::GenParticle>> incomingPartons_;
        edm::Ptr<reco::GenParticle> higgsBoson_;
        std::vector<edm::Ptr<reco::GenParticle>> outgoingPartons_;

        int Njets_;
        double alphaS_;

    };
}

#endif
