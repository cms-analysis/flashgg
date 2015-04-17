#ifndef FLASHgg_SinglePhotonView_h
#define FLASHgg_SinglePhotonView_h

#include "flashgg/DataFormats/interface/Photon.h"
//#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>
#include <vector>

namespace flashgg {

//    class DiPhotonCandidate;

    class SinglePhotonView
    {

    public:
//        typedef edm::Ptr<flashgg::DiPhotonCandidate> dipho_ptr_type;
        typedef edm::Ptr<flashgg::Photon> pho_ptr_type;
        typedef edm::Ptr<reco::Vertex> vtx_ptr_type;
        typedef Photon cand_type;

//		SinglePhotonView( const DiPhotonCandidate *dipho, int daughter );
//		SinglePhotonView( dipho_ptr_type dipho, int daughter );
        SinglePhotonView() : hasPhoton_( 0 ) {}
        SinglePhotonView( edm::Ptr<flashgg::Photon> pho, edm::Ptr<reco::Vertex> vtx ) : phoRef_( pho ), vtxRef_( vtx ), hasPhoton_( 0 ), hasVtx_( 1 ) {}
        SinglePhotonView( edm::Ptr<flashgg::Photon> pho ) : phoRef_( pho ), hasPhoton_( 0 ), hasVtx_( 0 ) {}


//        const cand_type &photon() const { MakePhoton(); return ( (persistVec_.size() > 0) ? persistVec_[0] : pho_  );}
        const cand_type &photon() const { if( persistVec_.size() ) { return persistVec_[0]; } else { MakePhoton(); return pho_; } }

//		const cand_type * photonPtr() const  { MakePhoton(); return ( (persistVec_.size() > 0) ? &persistVec_[0] : &pho_  ); }
        const cand_type *photonPtr() const  { if( persistVec_.size() ) { return &persistVec_[0]; } else { MakePhoton(); return &pho_; } }

//        const cand_type *operator->() const { MakePhoton(); return ( (persistVec_.size() > 0) ? &persistVec_[0] : &pho_  ); }
        const cand_type *operator->() const { if( persistVec_.size() ) { return &persistVec_[0]; } else { MakePhoton(); return &pho_; } }

        const pho_ptr_type photonRef() const { return phoRef_; }

        float pfChIso02WrtChosenVtx() const { MakePhoton(); return ( pho_.pfChgIso02WrtVtx( vtxRef_ ) ); }
        float pfChIso03WrtChosenVtx() const { MakePhoton(); return ( pho_.pfChgIso03WrtVtx( vtxRef_ ) ); }
        float pfChIso04WrtChosenVtx() const { MakePhoton(); return ( pho_.pfChgIso04WrtVtx( vtxRef_ ) ); }

        float phoIdMvaWrtChosenVtx() const { MakePhoton(); return ( pho_.phoIdMvaDWrtVtx( vtxRef_ ) ); }

        float extraChIsoWrtChoosenVtx( const std::string &key ) const { MakePhoton(); return ( pho_.extraChgIsoWrtVtx( key, vtxRef_ ) ); }

        bool hasPhoton() const { return hasPhoton_; }

        void MakePersistent();
//        void MakePersistent()  { MakePhoton(); flashgg::Photon tempPho = pho_;  persistVec_.push_back(tempPho); std::cout << "size of persistent vector: " << persistVec_.size() << std::endl;}
//        void AddPersistent( flashgg::Photon PersPho_) { persistVec_.push_back(PersPho_); }
//        flashgg::Photon getPersistent() { flashgg::Photon Pho; if (persistVec_.size() > 0 ) Pho = persistVec_[0]; return Pho ;}

//		flashgg::Photon pho4MomCorrection( edm::Ptr<flashgg::Photon> photon, edm::Ptr<reco::Vertex> vtx ) const;



    private:
        mutable flashgg::Photon pho_;
        edm::Ptr<flashgg::Photon> phoRef_;
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

