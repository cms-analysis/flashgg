#ifndef FLASHgg_GenPhotonExtra_h
#define FLASHgg_GenPhotonExtra_h

#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {

    class GenPhotonExtra
    {

    public:
        typedef edm::Ptr<pat::PackedGenParticle> ptr_type;
        typedef ptr_type::value_type cand_type;
        typedef Photon::mcMatch_t match_type;

        GenPhotonExtra() {}
        GenPhotonExtra( ptr_type ptr ) : ptr_( ptr ) {}


        bool operator==( const ptr_type &rhs ) const { return ptr_ == rhs; }
        bool operator!=( const ptr_type &rhs ) const { return ptr_ != rhs; }

        const cand_type &cand() const { return *ptr_; }
        ptr_type ptr() const { return ptr_; }
        void setPtr( ptr_type x ) { ptr_ = x; }

        match_type type() const { return type_; }
        void setType( match_type x ) { type_ = x; }

        float genIso() const { return genIso_; }
        void setGenIso( float x ) { genIso_ = x; }

        bool frixioneIso() const { return frixioneIso_; }
        void setFrixioneIso( bool x ) { frixioneIso_ = x; }

        void copyTo( flashgg::Photon &fg, const std::string &postFix = "" ) const
        {
            fg.setGenMatchType( type_ );
            fg.addUserFloat( "genIso" + postFix, genIso_ );
            fg.addUserInt( "frixioneIso" + postFix, frixioneIso_ );
        }

    private:
        ptr_type ptr_;
        match_type type_;
        float genIso_;
        bool  frixioneIso_;

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

