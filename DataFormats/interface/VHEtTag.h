#ifndef FLASHgg_VHEtTag_h
#define FLASHgg_VHEtTag_h

#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class VHEtTag: public DiPhotonTagBase
    {
    public:
        VHEtTag();
        ~VHEtTag();

        VHEtTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        VHEtTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );

        VHEtTag *clone() const { return ( new VHEtTag( *this ) ); }

        const edm::Ptr<pat::MET> met() const {return theMet_;}
        const edm::Ptr<DiPhotonCandidate> diPhotonCandidate() const { return theDiPhotonCandidate_;}
        void setMet( edm::Ptr<pat::MET> );

    private:
        edm::Ptr<DiPhotonCandidate> theDiPhotonCandidate_;
        edm::Ptr<pat::MET> theMet_;
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

