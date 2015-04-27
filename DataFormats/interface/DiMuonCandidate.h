#ifndef FLASHgg_DiMuonCandidate_h
#define FLASHgg_DiMuonCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

//-----------J. Tao from IHEP-Beijing--------------

namespace flashgg {
    class DiMuonCandidate : public reco::CompositeCandidate
    {
    public:
        DiMuonCandidate();
        DiMuonCandidate( edm::Ptr<pat::Muon>, edm::Ptr<pat::Muon> );
        DiMuonCandidate( const pat::Muon &, const pat::Muon & );
        ~DiMuonCandidate();

        const pat::Muon *leadingMuon() const;
        const pat::Muon *subleadingMuon() const;

        bool IsOSDiMuPair() const { return IsOSDiMuPair_; }
        void setIsOSDiMuPair( bool val ) { IsOSDiMuPair_ = val;}

        bool IfBothTightMu() const { return IfBothTightMu_; }
        void setIfBothTightMu( bool val ) { IfBothTightMu_  = val;}

        bool IfBothGlobalAndPF() const { return IfBothGlobalAndPF_; }
        void setIfBothGlobalAndPF( bool val ) { IfBothGlobalAndPF_  = val;}

    private:

        bool IsOSDiMuPair_;
        bool IfBothTightMu_;
        bool IfBothGlobalAndPF_;

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
