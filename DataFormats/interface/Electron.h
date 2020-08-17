#ifndef FLASHgg_Electron_h
#define FLASHgg_Electron_h

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {

    class Electron : public pat::Electron, public WeightedObject
    {

    public:
        Electron();
        Electron( const pat::Electron & );
        ~Electron();

        float nonTrigMVA() const {return nontrigmva_;}
        void setNonTrigMVA( float val ) { nontrigmva_ = val; }

        float standardHggIso() const { return PfRhoAreaCorrectedIso_; }
        void setStandardHggIso( float val ) { PfRhoAreaCorrectedIso_ = val; }

        bool hasMatchedConversion() const { return hasMatchedConversion_; }
        void setHasMatchedConversion( bool val ) { hasMatchedConversion_ = val;}

        bool passMVATightId( ) const {return passMVATightId_ ;}
        void setPassMVATightId( bool val ) { passMVATightId_ = val;}
        
        bool passMVAMediumId( ) const {return passMVAMediumId_ ;}
        void setPassMVAMediumId( bool val ) { passMVAMediumId_ = val;}

        bool passMVALooseId( ) const {return passMVALooseId_ ;}
        void setPassMVALooseId( bool val ) { passMVALooseId_ = val;}

        bool passMVATightNoIsoId( ) const {return passMVATightNoIsoId_ ;}
        void setPassMVATightNoIsoId( bool val ) { passMVATightNoIsoId_ = val;}
        
        bool passMVAMediumNoIsoId( ) const {return passMVAMediumNoIsoId_ ;}
        void setPassMVAMediumNoIsoId( bool val ) { passMVAMediumNoIsoId_ = val;}

        bool passMVALooseNoIsoId( ) const {return passMVALooseNoIsoId_ ;}
        void setPassMVALooseNoIsoId( bool val ) { passMVALooseNoIsoId_ = val;}

        bool passTightId( ) const {return passTightId_ ;}
        void setPassTightId( bool val ) { passTightId_ = val;}
        
        bool passMediumId( ) const {return passMediumId_ ;}
        void setPassMediumId( bool val ) { passMediumId_ = val;}

        bool passLooseId( ) const {return passLooseId_ ;}
        void setPassLooseId( bool val ) { passLooseId_ = val;}

        bool passVetoId( ) const {return passVetoId_ ;}
        void setPassVetoId( bool val ) { passVetoId_ = val;}

        float fggMiniIsoSumRel() const {return fggMiniIsoSumRel_;}
        void setFggMiniIsoSumRel( float val ) {fggMiniIsoSumRel_ = val;}

        Electron *clone() const { return ( new Electron( *this ) ); }

    private:
        float nontrigmva_;
        float PfRhoAreaCorrectedIso_;
        bool hasMatchedConversion_;
        bool passMVATightId_;
        bool passMVAMediumId_;
        bool passMVALooseId_;
        bool passMVATightNoIsoId_;
        bool passMVAMediumNoIsoId_;
        bool passMVALooseNoIsoId_;
        bool passTightId_;
        bool passMediumId_;
        bool passLooseId_;
        bool passVetoId_;
        float fggMiniIsoSumRel_;
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

