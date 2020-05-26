#ifndef FLASHgg_GenLeptonExtra_h
#define FLASHgg_GenLeptonExtra_h

//#include "flashgg/DataFormats/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {

    class GenLeptonExtra : public reco::ShallowCloneCandidate
    {

    public:
        typedef edm::Ptr<pat::PackedGenParticle> ptr_type;
        typedef ptr_type::value_type cand_type;
        //        typedef Lepton::mcMatch_t match_type; //FIXME

        GenLeptonExtra() {}
        GenLeptonExtra( ptr_type ptr ) : ptr_( ptr ) {}


        bool operator==( const ptr_type &rhs ) const { return ptr_ == rhs; }
        bool operator!=( const ptr_type &rhs ) const { return ptr_ != rhs; }

        const cand_type &cand() const { return *ptr_; }
        ptr_type ptr() const { return ptr_; }
        void setPtr( ptr_type x ) { ptr_ = x; }

        float genIso() const { return genIso_; }
        void setGenIso( float x ) { genIso_ = x; }

        float genIsoAfterDressing() const { return genIsoAfterDressing_; }
        void setGenIsoAfterDressing( float x ) { genIsoAfterDressing_ = x; }

        //        reco::Particle::LorentzVector dressedP4() const { return dressedP4_; }
        //        void setDressedP4( reco::Particle::LorentzVector x ) { dressedP4_ = x; }
        //        void setP4( reco::Particle::LorentzVector x ) { p4_ = x; }

//        void copyTo( flashgg::Lepton &fg, const std::string &postFix = "" ) const
//        {
//            fg.addUserFloat( "genIso" + postFix, genIso_ );
//        }

    private:
        ptr_type ptr_;
        float genIso_;
        float genIsoAfterDressing_;
        //        reco::Particle::LorentzVector dressedP4_;
        //        reco::Particle::LorentzVector p4_;
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

