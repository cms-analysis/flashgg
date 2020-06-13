#ifndef FLASHgg_GenJetExtra_h
#define FLASHgg_GenJetExtra_h

//#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {

    class GenJetExtra : public reco::GenJet
    {

    public:
        GenJetExtra(){}
        GenJetExtra(const reco::GenJet &j) : reco::GenJet(j) {}
        
        bool hasBottom() const {return hasBottom_;}
        void setHasBottom( bool x ){hasBottom_ = x;}

        bool hasBquark() const {return hasBquark_;}
        void setHasBquark( bool x ){hasBquark_ = x;}

        bool deltaRBquarkGenJet() const {return deltaRBquarkGenjet_;}
        void setDeltaRBquarkGenjet( double x ){deltaRBquarkGenjet_ = x;}

        bool jetPtOverBquarkPt() const {return jetPtOverBquarkPt_;}
        void setJetPtOverBquarkPt( double x ){jetPtOverBquarkPt_ = x;}


    private:
        bool hasBottom_;
        bool hasBquark_;
        double deltaRBquarkGenjet_;
        double jetPtOverBquarkPt_;
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

