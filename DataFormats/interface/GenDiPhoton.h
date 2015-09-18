#ifndef FLASHgg_GenDiPhoton_h
#define FLASHgg_GenDiPhoton_h

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"


namespace flashgg {

    class GenDiPhoton : public reco::LeafCandidate
    {
    public:
        GenDiPhoton();
        GenDiPhoton( edm::Ptr<flashgg::GenPhotonExtra>, edm::Ptr<flashgg::GenPhotonExtra>);
        ~GenDiPhoton();

        const flashgg::GenPhotonExtra::cand_type& leadingPhoton() const { return leadingPhoton_.cand(); };
        const flashgg::GenPhotonExtra::cand_type& subLeadingPhoton() const { return subLeadingPhoton_.cand(); }

        const flashgg::GenPhotonExtra& leadingExtra() const { return leadingPhoton_; };
        const flashgg::GenPhotonExtra& subLeadingExtra() const { return subLeadingPhoton_; }
        
        float sumPt() const { return  leadingPhoton_.cand().pt() + subLeadingPhoton_.cand().pt(); }
        bool operator <( const GenDiPhoton &b ) const { return ( sumPt() < b.sumPt() ); }
        bool operator >( const GenDiPhoton &b ) const { return ( sumPt() > b.sumPt() ); }

        
    private:
        void computeP4AndOrder();

        flashgg::GenPhotonExtra leadingPhoton_;
        flashgg::GenPhotonExtra subLeadingPhoton_;
        
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

