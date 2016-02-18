#ifndef FLASHgg_GenDiPhoton_h
#define FLASHgg_GenDiPhoton_h

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/JetReco/interface/GenJet.h"


namespace flashgg {

    class GenDiPhoton : public reco::LeafCandidate
    {
    public:
        GenDiPhoton();
        GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<flashgg::GenPhotonExtra>);
        GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<reco::GenJet>, edm::Ptr<reco::GenJet>);
        ~GenDiPhoton();

        const flashgg::GenPhotonExtra::cand_type& leadingPhoton() const { return leadingPhoton_.cand(); };
        const flashgg::GenPhotonExtra::cand_type& subLeadingPhoton() const { return subLeadingPhoton_.cand(); }

        const reco::GenJet & leadingJet() const { return *leadingJet_; };
        const reco::GenJet & subLeadingJet() const { return *subLeadingJet_; }

        const flashgg::GenPhotonExtra& leadingExtra() const { return leadingPhoton_; };
        const flashgg::GenPhotonExtra& subLeadingExtra() const { return subLeadingPhoton_; }
        
        float sumPt() const { return  leadingPhoton_.cand().pt() + subLeadingPhoton_.cand().pt(); }
        bool operator <( const GenDiPhoton &b ) const { return ( sumPt() < b.sumPt() ); }
        bool operator >( const GenDiPhoton &b ) const { return ( sumPt() > b.sumPt() ); }

        LorentzVector dijet() const; 

    private:
        void computeP4AndOrder();

        flashgg::GenPhotonExtra leadingPhoton_;
        flashgg::GenPhotonExtra subLeadingPhoton_;
        
        edm::Ptr<reco::GenJet> leadingJet_;
        edm::Ptr<reco::GenJet> subLeadingJet_;
        
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

