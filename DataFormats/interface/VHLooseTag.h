#ifndef FLASHgg_VHLooseTag_h
#define FLASHgg_VHLooseTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

namespace flashgg {

    class VHLooseTag: public DiPhotonTagBase
    {
    public:
        VHLooseTag();
        VHLooseTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        VHLooseTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        ~VHLooseTag();

        VHLooseTag *clone() const override { return ( new VHLooseTag( *this ) ); }

        const std::vector<edm::Ptr<Muon> > muons() const { return Muons_;}
        const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
        const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
        const std::vector<edm::Ptr<pat::MET> > met() const { return MET_;}

        void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
        void setMuons( std::vector<edm::Ptr<Muon> >Muons ) {Muons_ = Muons;}
        void setMET( std::vector<edm::Ptr<pat::MET> > MET ) {MET_ = MET;}
        void setElectrons( std::vector<edm::Ptr<Electron> > Electrons ) {Electrons_ = Electrons;}

        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kVHLoose; }

    private:
        std::vector<edm::Ptr<Muon> > Muons_;
        std::vector<edm::Ptr<Electron> > Electrons_;
        std::vector<edm::Ptr<Jet> > Jets_;
        std::vector<edm::Ptr<pat::MET> > MET_;
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

