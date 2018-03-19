#ifndef flashgg_DoubleHTag
#define flashgg_DoubleHTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class DoubleHTag: public DiPhotonTagBase
    {
    public:
        DoubleHTag();
        ~DoubleHTag();

        DoubleHTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<reco::Candidate>, edm::Ptr<reco::Candidate> );
        virtual DoubleHTag *clone() const override;
        /// DiPhotonTagBase::tag_t tagEnum() const override { return DiPhotonTagBase::kDoubleH; }

        void setMVA(double x) { mva_ = x; }
        double MVA() const { return mva_; }
        double MX() const { return 350.; } // FIXME

    private:
        double mva_;
        edm::Ptr<reco::Candidate> leadJet_, subleadJet_;
        

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

