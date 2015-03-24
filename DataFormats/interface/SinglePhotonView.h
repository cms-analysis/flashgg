#ifndef FLASHgg_SinglePhotonView_h
#define FLASHgg_SinglePhotonView_h

#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {

    class SinglePhotonView
    {

    public:
        typedef edm::Ptr<DiPhotonCandidate> dipho_ptr_type;
        typedef Photon cand_type;

        SinglePhotonView() : pho_( 0 ), dipho_( 0 ) {}
        SinglePhotonView( dipho_ptr_type dipho, int daughter ) : edmdipho_( dipho ), daughter_( daughter ), pho_( 0 ), dipho_( 0 ) {}
        SinglePhotonView( const DiPhotonCandidate *dipho, int daughter ) : daughter_( daughter ), pho_( 0 ), dipho_( dipho ) {}

        const cand_type &photon() const { daughterMaybe(); return *pho_; }

        const cand_type *operator->() const { daughterMaybe(); return pho_; }

        float pfChIso02WrtChosenVtx() const { return photon().pfChgIso02WrtVtx( dipho_->vtx() ); }
        float pfChIso03WrtChosenVtx() const { return photon().pfChgIso03WrtVtx( dipho_->vtx() ); }
        float pfChIso04WrtChosenVtx() const { return photon().pfChgIso04WrtVtx( dipho_->vtx() ); }

        float phoIdMvaWrtChosenVtx() const { return photon().phoIdMvaDWrtVtx( dipho_->vtx() ); }

        float extraChIsoWrtChoosenVtx( const std::string &key ) const { return photon().extraChgIsoWrtVtx( key, dipho_->vtx() ); }

    private:
        void daughterMaybe() const;

        dipho_ptr_type edmdipho_;
        int daughter_;
        mutable const Photon *pho_;
        mutable const DiPhotonCandidate *dipho_;

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

