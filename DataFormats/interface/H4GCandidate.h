#ifndef flashgg_H4GCandidate
#define flashgg_H4GCandidate

#include "TLorentzVector.h"

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/Taggers/interface/FunctionHelpers.h"

namespace flashgg {

    class H4GCandidate
    {
    public:
        //---ctors---
        H4GCandidate() ;
        H4GCandidate( edm::Ptr<DiPhotonCandidate> dipho1, edm::Ptr<DiPhotonCandidate> dipho2 );
        H4GCandidate( edm::Ptr<DiPhotonCandidate> dipho1, edm::Ptr<Photon> photon );
        H4GCandidate( edm::Ptr<DiPhotonCandidate> dipho1 );

        //---dtor---
        ~H4GCandidate();
        // virtual ~H4GCandidate() {}

        //---utils---
        bool is4Photons() { return n_photons==4; };
        bool is3Photons() { return n_photons==3; };
        bool is2Photons() { return n_photons==2; };

private:
        // double diphotonPtOverM() const {return diPhoton()->pt()/mass(); }
        int n_photons;
        reco::Candidate::LorentzVector p4_;
        edm::Ptr<flashgg::DiPhotonCandidate> dipho1_;
        edm::Ptr<flashgg::DiPhotonCandidate> dipho2_;
        edm::Ptr<flashgg::Photon>            singlePhoton_;
      };
      typedef std::vector<H4GCandidate> H4GCandidateCollection;


}

#endif
