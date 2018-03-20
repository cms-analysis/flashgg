#ifndef flashgg_DoubleHTag
#define flashgg_DoubleHTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

namespace flashgg {

    class DoubleHTag: public DiPhotonTagBase, reco::LeafCandidate
    {
    public:
        DoubleHTag();
        ~DoubleHTag();

        DoubleHTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> );
        virtual DoubleHTag *clone() const override;
        /// DiPhotonTagBase::tag_t tagEnum() const override { return DiPhotonTagBase::kDoubleH; }

        void setMVA(double x) { mva_ = x; }
        double MVA() const { return mva_; }
        void setMX(double x) { MX_ = x; }
        double MX() const { return MX_; }

        const flashgg::Jet & leadJet() const { return *leadJet_; } 
        const flashgg::Jet & subleadJet() const { return *subleadJet_; } 
        
        const LorentzVector & dijet() const { return dijet_; }
        
    private:
        double mva_, MX_;
        edm::Ptr<flashgg::Jet> leadJet_, subleadJet_;
        LorentzVector dijet_;
        

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

