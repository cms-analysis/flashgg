#ifndef FLASHgg_MuMuGammaCandidate_h
#define FLASHgg_MuMuGammaCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "flashgg/DataFormats/interface/DiMuonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"

//-----------J. Tao from IHEP-Beijing--------------

namespace flashgg {
    class MuMuGammaCandidate : public reco::CompositeCandidate
    {
    public:
        MuMuGammaCandidate();
        MuMuGammaCandidate( edm::Ptr<flashgg::DiMuonCandidate>, edm::Ptr<flashgg::Photon>, edm::Ptr<reco::Vertex> );
        //MuMuGammaCandidate( const flashgg::DiMuonCandidate &, const flashgg::Photon &, edm::Ptr<reco::Vertex>, edm::Ptr<flashgg::DiMuonCandidate>);
        MuMuGammaCandidate( edm::Ptr<flashgg::DiMuonCandidate>, const flashgg::Photon &, edm::Ptr<reco::Vertex> ); //mixed
        ~MuMuGammaCandidate();

        const flashgg::DiMuonCandidate *MMG_DiMu() const;
        const flashgg::Photon *MMG_Photon() const;

        edm::Ptr<flashgg::DiMuonCandidate> DiMuPtr() const { return dimuptr_; }
        void setDiMuPtr( edm::Ptr<flashgg::DiMuonCandidate> val ) { dimuptr_ = val; }

        edm::Ptr<reco::Vertex> Vertex() const { return vertex_; }
        void setVertex( edm::Ptr<reco::Vertex> val ) { vertex_ = val; }

        bool Is2012FSRZMMG() const { return Is2012FSRZMMG_; }
        void setIs2012FSRZMMG( bool val ) { Is2012FSRZMMG_  = val;}

        bool IsHGammaStarGamma() const { return IsHGammaStarGamma_; }
        void setIsHGammaStarGamma( bool val ) { IsHGammaStarGamma_  = val;}

        bool IsHZgamma() const { return IsHZgamma_; }
        void setIsHZgamma( bool val ) { IsHZgamma_  = val;}

        double PhotonTrkIsoHollow03() const { return PhotonTrkIsoHollow03_; }
        void setPhotonTrkIsoHollow03( double val ) { PhotonTrkIsoHollow03_  = val;}

        double PhotonTrkIsoHollow03MuCorr() const { return PhotonTrkIsoHollow03MuCorr_; }
        void setPhotonTrkIsoHollow03MuCorr( double val ) { PhotonTrkIsoHollow03MuCorr_  = val;}


    private:

        edm::Ptr<flashgg::DiMuonCandidate> dimuptr_;
        edm::Ptr<reco::Vertex> vertex_;
        bool Is2012FSRZMMG_;
        bool IsHGammaStarGamma_;
        bool IsHZgamma_;
        double PhotonTrkIsoHollow03_;
        double PhotonTrkIsoHollow03MuCorr_;

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
