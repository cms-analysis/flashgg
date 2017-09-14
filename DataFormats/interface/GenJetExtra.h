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

    private:
        bool hasBottom_;
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

