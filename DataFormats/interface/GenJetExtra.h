#ifndef FLASHgg_GenJetExtra_h
#define FLASHgg_GenJetExtra_h

//#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {

    class GenJetExtra : public reco::GenJet
    {

    public:
        typedef edm::Ptr<reco::GenJet> ptr_type;
        typedef ptr_type::value_type cand_type;
        //        typedef Jet::mcMatch_t match_type; //FIXME

        GenJetExtra() {}
        GenJetExtra( ptr_type ptr ) : ptr_( ptr ) {}


        bool operator==( const ptr_type &rhs ) const { return ptr_ == rhs; }
        bool operator!=( const ptr_type &rhs ) const { return ptr_ != rhs; }

        const cand_type &cand() const { return *ptr_; }
        ptr_type ptr() const { return ptr_; }
        void setPtr( ptr_type x ) { ptr_ = x; }

        bool hasBottom() const {return hasBottom_;}
        void setHasBottom( bool x ){hasBottom_ = x;}

//        void copyTo( flashgg::Jet &fg, const std::string &postFix = "" ) const
//        {
//            fg.addUserFloat( "genIso" + postFix, genIso_ );
//        }

    private:
        ptr_type ptr_;
        bool hasBottom_;
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

