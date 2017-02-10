#ifndef FLASHgg_Muon_h
#define FLASHgg_Muon_h

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {

    class Muon : public pat::Muon, public WeightedObject
    {

    public:
        Muon();
        Muon( const pat::Muon & );
        ~Muon();

        Muon *clone() const { return ( new Muon( *this ) ); }

        float fggPFIsoSumRelR04() const {return fggPFIsoSumRelR04_;}
        void setFggPFIsoSumRelR04( float val ) {fggPFIsoSumRelR04_ = val;};

        float fggTrkIsoSumRelR03() const {return fggTrkIsoSumRelR03_;}
        void setFggTrkIsoSumRelR03( float val ) {fggTrkIsoSumRelR03_ = val;};

        float fggMiniIsoSumRel() const {return fggMiniIsoSumRel_;}
        void setFggMiniIsoSumRel( float val ) {fggMiniIsoSumRel_ = val;};

        float fggMiniIsoCharged() const {return fggMiniIsoCharged_;}
        void setFggMiniIsoCharged( float val ) {fggMiniIsoCharged_ = val;};

        float fggMiniIsoNeutrals() const {return fggMiniIsoNeutrals_;}
        void setFggMiniIsoNeutrals( float val ) {fggMiniIsoNeutrals_ = val;};

        float fggMiniIsoPhotons() const {return fggMiniIsoPhotons_;}
        void setFggMiniIsoPhotons( float val ) {fggMiniIsoPhotons_ = val;};

        float fggMiniIsoPhotons2() const {return fggMiniIsoPhotons2_;}
        void setFggMiniIsoPhotons2( float val ) {fggMiniIsoPhotons2_ = val;};

        float fggMiniIsoPUCharged() const {return fggMiniIsoPUCharged_;}
        void setFggMiniIsoPUCharged( float val ) {fggMiniIsoPUCharged_ = val;};

    private:
        float fggPFIsoSumRelR04_;
        float fggTrkIsoSumRelR03_;
        float fggMiniIsoSumRel_;
        float fggMiniIsoCharged_;
        float fggMiniIsoNeutrals_;
        float fggMiniIsoPhotons_;
        float fggMiniIsoPhotons2_;
        float fggMiniIsoPUCharged_;
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

