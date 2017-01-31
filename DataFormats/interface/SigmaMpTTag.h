#ifndef flashgg_SigmaMpTTag
#define flashgg_SigmaMpTTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

namespace flashgg {

    class SigmaMpTTag: public DiPhotonTagBase
    {
    public:
        SigmaMpTTag();
        ~SigmaMpTTag();

        SigmaMpTTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        SigmaMpTTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );

        SigmaMpTTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, std::map<std::string, edm::Ptr<reco::CompositeCandidate> > );
        SigmaMpTTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, std::map<std::string, edm::Ptr<reco::CompositeCandidate> > );
        const edm::Ptr<reco::CompositeCandidate > getCompCand(const std::string &name) const;
        
        virtual SigmaMpTTag *clone() const override;
        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kUntagged; }

    private:
        std::map<std::string, edm::Ptr<reco::CompositeCandidate > > compObjMap_;


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

