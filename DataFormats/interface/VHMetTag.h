#ifndef FLASHgg_VHMetTag_h
#define FLASHgg_VHMetTag_h

//#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class VHMetTag: public DiPhotonTagBase
    {
    public:
        VHMetTag();
        ~VHMetTag();

        VHMetTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        VHMetTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );

        VHMetTag *clone() const override { return ( new VHMetTag( *this ) ); }

        const edm::Ptr<flashgg::Met> met() const {return theMet_;}
        const edm::Ptr<flashgg::Jet> jet() const {return theJet_;}
        const edm::Ptr<DiPhotonCandidate> diPhotonCandidate() const { return theDiPhotonCandidate_;}
        void setMet( edm::Ptr<flashgg::Met> );
        void setJet( edm::Ptr<flashgg::Jet> );
        
        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kVHMet; }

    private:
        edm::Ptr<DiPhotonCandidate> theDiPhotonCandidate_;
        edm::Ptr<flashgg::Met> theMet_;
        edm::Ptr<flashgg::Jet> theJet_;
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

