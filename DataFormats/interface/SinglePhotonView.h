#ifndef FLASHgg_SinglePhotonView_h
#define FLASHgg_SinglePhotonView_h

#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>
#include <vector>

namespace flashgg {

    class SinglePhotonView
    {

    public:
        typedef edm::Ptr<flashgg::Photon> pho_ptr_type;
        typedef edm::Ptr<reco::Vertex> vtx_ptr_type;
        typedef Photon cand_type;
        
        virtual ~SinglePhotonView() {}
        SinglePhotonView() : hasPhoton_( 0 ) {}
        SinglePhotonView( edm::Ptr<flashgg::Photon> pho, edm::Ptr<reco::Vertex> vtx ) : phoPtr_( pho ), vtxRef_( vtx ), hasPhoton_( 0 ), hasVtx_( 1 ) {}
        SinglePhotonView( edm::Ptr<flashgg::Photon> pho ) : phoPtr_( pho ), hasPhoton_( 0 ), hasVtx_( 0 ) {}

        const cand_type *photon() const;
        cand_type &getPhoton(); // You can only have a non-const pointer if you call makePersistent() first
        edm::Ptr<flashgg::Photon> originalPhoton() const { return phoPtr_; }

        float pfChIso02WrtChosenVtx() const { MakePhoton(); return ( photon()->pfChgIso02WrtVtx( vtxRef_ ) ); }
        float pfChIso03WrtChosenVtx() const { MakePhoton(); return ( photon()->pfChgIso03WrtVtx( vtxRef_ ) ); }
        float pfChIso04WrtChosenVtx() const { MakePhoton(); return ( photon()->pfChgIso04WrtVtx( vtxRef_ ) ); }

        float phoIdMvaWrtChosenVtx() const { MakePhoton(); return ( photon()->phoIdMvaDWrtVtx( vtxRef_ ) ); }

        float extraChIsoWrtChoosenVtx( const std::string &key ) const { MakePhoton(); return ( photon()->extraChgIsoWrtVtx( key, vtxRef_ ) ); }

        void MakePersistent();

        // DO NOT USE THIS FUNCTION UNLESS YOU HAVE A GOOD REASON AND KNOW IT WON'T CAUSE INTERNAL INCONSISTENCIES
        void replacePtr( edm::Ptr<flashgg::Photon> replacement ) { phoPtr_ = replacement; }

    private:
        mutable flashgg::Photon pho_;
        edm::Ptr<flashgg::Photon> phoPtr_;
        edm::Ptr<reco::Vertex> vtxRef_;
        mutable bool hasPhoton_;
        bool hasVtx_;
        bool MakePhoton() const;
        std::vector<flashgg::Photon> persistVec_;
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

