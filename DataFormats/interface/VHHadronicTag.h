#ifndef FLASHgg_VHHadronicTag_h
#define FLASHgg_VHHadronicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"


namespace flashgg {

    class VHHadronicTag: public DiPhotonTagBase
    {

    public:

        VHHadronicTag();
        VHHadronicTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        VHHadronicTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        VHHadronicTag *clone() const override;
        ~VHHadronicTag();

        const edm::Ptr<flashgg::Jet>      leadingJet()  const  { return leadingJet_; }
        const edm::Ptr<flashgg::Jet>      subLeadingJet() const { return subleadingJet_; }
        const edm::Ptr<DiPhotonCandidate> diPhotonCandidate() const { return theDiPhotonCandidate_;}
        //const edm::Ptr<DiPhotonMVAResult> diPhotonMVAResult() const { return theDiPhotonMVAResult_;} // doesn't work for some reason

        void setJets( edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> );

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kVHHadronic; }

    private:
        edm::Ptr<DiPhotonCandidate> theDiPhotonCandidate_;
        //edm::Ptr<DiPhotonMVAResult> theDiPhotonMVAResult_;
        //std::pair<edm::Ptr<flashgg::Jet>,edm::Ptr<flashgg::Jet> > JetPair_;
        edm::Ptr<flashgg::Jet> leadingJet_;
        edm::Ptr<flashgg::Jet> subleadingJet_;

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

