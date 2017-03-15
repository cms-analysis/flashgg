#ifndef FLASHgg_Photon_h
#define FLASHgg_Photon_h

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include <map>
#include <string>
#include <set>

namespace flashgg {

    class Photon : public pat::Photon, public WeightedObject
    {

    public:
        Photon();
        Photon( const pat::Photon & );
        /// Photon( const flashgg::Photon & );
        ~Photon();
        virtual Photon *clone() const;

        void ZeroVariables();
        void removeVerticesExcept( const std::set<edm::Ptr<reco::Vertex> > & );

        enum mcMatch_t { kUnkown = 0, kPrompt, kFake  };
        enum rechitSummaryFlags_t {  kSaturated=0, kLeRecovered, kNeighRecovered, kHasSwitchToGain1, kHasSwitchToGain6, kWeird };

        // someone had the insane idea of shadowing these methods in the pat::Photon....
        float egChargedHadronIso() const {return  reco::Photon::chargedHadronIso();}
        float egNeutralHadronIso() const {return  reco::Photon::neutralHadronIso();}
        float egPhotonIso() const {return  reco::Photon::photonIso();}

        float r9() const { throw cms::Exception("Ambiguous R9") << " Please call flashgg::Photon::old_r9 or flashgg::Photon::full5x5_r9"; return -1.; };
        float old_r9() const { return reco::Photon::r9(); }

        void setSipip( float val ) {sipip_ = val;};
        void setSieip( float val ) {sieip_ = val;};
        void setE2nd( float val ) {e2nd_ = val;};
        void setE2x5right( float val ) {e2x5right_ = val;};
        void setE2x5left( float val ) {e2x5left_ = val;};
        void setE2x5top( float val ) {e2x5top_ = val;};
        void setE2x5bottom( float val ) {e2x5bottom_ = val;};
        void setE2x5max( float val ) {e2x5max_ = val;};
        void setEright( float val ) {eright_ = val;};
        void setEleft( float val ) {eleft_ = val;};
        void setEtop( float val ) {etop_ = val;};
        void setEbottom( float val ) {ebottom_ = val;};
        void setE1x3( float val ) {e1x3_ = val;};
        void setS4( float val ) {S4_ = val;};
        void setpfPhoIso04( float val ) {pfPhoIso04_ = val;};
        void setpfPhoIso03( float val ) {pfPhoIso03_ = val;};
        void setpfPhoIso03Corr( float val ) {pfPhoIso03Cor_ = val;};
        void setpfNeutIso04( float val ) {pfNeutIso04_ = val;};
        void setpfNeutIso03( float val ) {pfNeutIso03_ = val;};
        void setpfChgIso04( std::map<edm::Ptr<reco::Vertex>, float> valmap ) {  pfChgIso04_ = valmap; }; // concept: pass the pre-computed map when calling this in the producer
        void setpfChgIso03( std::map<edm::Ptr<reco::Vertex>, float> valmap ) {  pfChgIso03_ = valmap; }; // concept: pass the pre-computed map when calling this in the producer
        void setpfChgIso02( std::map<edm::Ptr<reco::Vertex>, float> valmap ) {  pfChgIso02_ = valmap; }; // concept: pass the pre-computed map when calling this in the producer
        void setpfChgIsoWrtWorstVtx04( float val ) {pfChgIsoWrtWorstVtx04_ = val;};
        void setpfChgIsoWrtWorstVtx03( float val ) {pfChgIsoWrtWorstVtx03_ = val;};
        void setpfChgIsoWrtChosenVtx02( float val ) {pfChgIsoWrtChosenVtx02_ = val;};
        void setpfChgIsoWrtChosenVtx03( float val ) {pfChgIsoWrtChosenVtx03_ = val;};
        void setESEffSigmaRR( float val ) {ESEffSigmaRR_ = val;};
        void setPhoIdMvaD( std::map<edm::Ptr<reco::Vertex>, float> valmap ) {  phoIdMvaD_ = valmap; };  // concept: pass the pre-computed map when calling this in the producer
        void setPhoIdMvaWrtVtx( edm::Ptr<reco::Vertex> key, float val ) { phoIdMvaD_[key] = val; } // For later updates, e.g. recomputation when vertex is already selected
        void updateEnergy( std::string key, float val );
        void shiftAllMvaValuesBy( float val );
        void shiftMvaValueBy( float val, edm::Ptr<reco::Vertex> vtx );
        void shiftSigmaEOverEValueBy( float val, float cutoff=0. );
        void smearSigmaEOverEValueBy( float val );
        //    void setSigEOverE(float val) { sigEOverE_ = val; };

        // define which regression from reco we use - only this one is valid as of 74X
        static const reco::Photon::P4type regression_type = reco::Photon::regression1;

        float const sipip() const {return sipip_;};
        float const sieip() const {return sieip_;};
        float const e2nd() const {return e2nd_;};
        float const e2x5right() const {return e2x5right_;};
        float const e2x5left() const {return e2x5left_;};
        float const e2x5top() const {return e2x5top_;};
        float const e2x5bottom() const {return e2x5bottom_;};
        float const e2x5max() const {return e2x5max_;};
        float const eright() const {return eright_;};
        float const eleft() const {return eleft_;};
        float const etop() const {return etop_;};
        float const ebottom() const {return ebottom_;};
        float const e1x3() const {return e1x3_;};
        float const s4() const {return S4_;};
        float const pfPhoIso04() const {return pfPhoIso04_;};
        float const pfPhoIso03() const {return pfPhoIso03_;};
        float const pfPhoIso03Corr() const {return pfPhoIso03Cor_;};
        float const pfNeutIso04() const {return pfNeutIso04_;};
        float const pfNeutIso03() const {return pfNeutIso03_;};
        std::map<edm::Ptr<reco::Vertex>, float> const pfChgIso04() const {return pfChgIso04_;};
        std::map<edm::Ptr<reco::Vertex>, float> const pfChgIso03() const {return pfChgIso03_;};
        std::map<edm::Ptr<reco::Vertex>, float> const pfChgIso02() const {return pfChgIso02_;};
        float const pfChgIso04WrtVtx( const edm::Ptr<reco::Vertex> &vtx, bool lazy = false ) const { return findVertexFloat( vtx, pfChgIso04_, lazy ); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)
        float const pfChgIso03WrtVtx( const edm::Ptr<reco::Vertex> &vtx, bool lazy = false ) const { return findVertexFloat( vtx, pfChgIso03_, lazy ); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)
        float const pfChgIso02WrtVtx( const edm::Ptr<reco::Vertex> &vtx, bool lazy = false ) const { return findVertexFloat( vtx, pfChgIso02_, lazy ); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)

        float const pfChgIso04WrtVtx0() const { return findVertex0Float( pfChgIso04_ ); }; // WARNING: no guarantee that vertex 0 is the correct one
        float const pfChgIso03WrtVtx0() const { return findVertex0Float( pfChgIso03_ ); }; // WARNING: no guarantee that vertex 0 is the correct one
        float const pfChgIso02WrtVtx0() const { return findVertex0Float( pfChgIso02_ ); }; // WARNING: no guarantee that vertex 0 is the correct one

        float const pfChgIsoWrtWorstVtx04() const {return pfChgIsoWrtWorstVtx04_;};
        float const pfChgIsoWrtWorstVtx03() const {return pfChgIsoWrtWorstVtx03_;};
        float const pfChgIsoWrtChosenVtx02() const {return pfChgIsoWrtChosenVtx02_;};
        float const pfChgIsoWrtChosenVtx03() const {return pfChgIsoWrtChosenVtx03_;};
        float const esEffSigmaRR() const {return ESEffSigmaRR_;};

        void setExtraNeutIso( const std::string &key, float val ) { extraNeutralIsolations_[key] = val; };
        float const extraNeutIso( const std::string &key ) const
        {
            std::map<std::string, float>::const_iterator it = extraNeutralIsolations_.find( key );
            return it != extraNeutralIsolations_.end() ? it->second : 0.;
        };

        void setExtraPhoIso( const std::string &key, float val ) { extraPhotonIsolations_[key] = val; };
        float const extraPhoIso( const std::string &key ) const
        {
            std::map<std::string, float>::const_iterator it = extraPhotonIsolations_.find( key );
            return it != extraPhotonIsolations_.end() ? it->second : 0.;
        };

        void setExtraChIso( const std::string &key, std::map<edm::Ptr<reco::Vertex>, float> &val ) { extraChargedIsolations_[key] = val; };
        std::map<edm::Ptr<reco::Vertex>, float> extraChIso( const std::string &key ) const
        {
            std::map<std::string, std::map<edm::Ptr<reco::Vertex>, float> >::const_iterator it = extraChargedIsolations_.find( key );
            return it != extraChargedIsolations_.end() ? it->second : std::map<edm::Ptr<reco::Vertex>, float>();
        };

        float const extraChgIsoWrtVtx0( const std::string &key ) const  { return findVertex0Float( extraChIso( key ) ); };
        float const extraChgIsoWrtVtx( const std::string &key, const edm::Ptr<reco::Vertex> &vtx, bool lazy = false ) const { return findVertexFloat( vtx, extraChIso( key ), lazy ); };
        float const extraChgIsoWrtWorstVtx( const std::string &key ) const { return findWorstIso( extraChIso( key ) );  };

        bool hasEnergyAtStep( std::string key ) const;
        float const energyAtStep( std::string key, std::string fallback="" ) const;
        float const sigEOverE() const;

        std::map<edm::Ptr<reco::Vertex>, float> const phoIdMvaD() const {return phoIdMvaD_;};
        float const phoIdMvaDWrtVtx( const edm::Ptr<reco::Vertex> &vtx, bool lazy = false ) const { return findVertexFloat( vtx, phoIdMvaD_, lazy ); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)

        void setMatchedGenPhoton( const edm::Ptr<pat::PackedGenParticle> pgp ) { addUserCand( "matchedGenPhoton", pgp ); };
        const pat::PackedGenParticle * matchedGenPhoton() const { return dynamic_cast<const pat::PackedGenParticle *>( userCand( "matchedGenPhoton" ).get() ); };
        bool hasMatchedGenPhoton() const { return hasUserCand( "matchedGenPhoton" ); };

        void setGenMatchType( mcMatch_t typ ) { addUserInt( "genMatchType", ( int )typ ); };
        mcMatch_t  genMatchType() const { return ( hasUserInt( "genMatchType" ) ? ( mcMatch_t )userInt( "genMatchType" ) : kUnkown ); };
        bool  hasGenMatchType() const { return hasUserInt( "genMatchType" ); };

        void setPassElectronVeto( bool val ) { passElecVeto_ = val; };
        bool passElectronVeto() const { return passElecVeto_ ; };

        static int32_t encodeStatusFlags( bool isSaturated, bool isLeRecovered, bool isNeighRecovered, bool isGain1, bool isGain6, bool isWeired) {
            int32_t flags = 0;
            if( isSaturated ) { flags |= 0x1<<kSaturated; };
            if( isLeRecovered ) { flags |= 0x1<<kLeRecovered; };
            if( isNeighRecovered ) { flags |= 0x1<<kNeighRecovered; };
            if( isGain1 ) { flags |= 0x1<<kHasSwitchToGain1; };
            if( isGain6 ) { flags |= 0x1<<kHasSwitchToGain6; };
            if( isWeired ) { flags |= 0x1<<kWeird; };
            return flags;
        };
        void setStatusFlags( int32_t st) { return addUserInt("rechitStatus",st); };
        int32_t statusFlags() { return  (hasUserInt("rechitStatus")?userInt("rechitStatus"):-1); };
        bool checkStatusFlag( rechitSummaryFlags_t ibit) const { return (hasUserInt("rechitStatus")?userInt("rechitStatus")&(0x1<<ibit):0); };
        inline bool hasSwitchToGain1(void)const{ return checkStatusFlag(kHasSwitchToGain1);};
        inline bool hasSwitchToGain6(void)const{ return (checkStatusFlag(kHasSwitchToGain1)==false && checkStatusFlag(kHasSwitchToGain6));};
        reco::SuperCluster* getSuperCluster() { return &superCluster_[0];};

    private:
        void setEnergyAtStep( std::string key, float val ); // updateEnergy should be used from outside the class to access this
        float const findVertexFloat( const edm::Ptr<reco::Vertex> &vtx, const std::map<edm::Ptr<reco::Vertex>, float> &mp, bool lazy ) const;
        float const findVertex0Float( const std::map<edm::Ptr<reco::Vertex>, float> &mp ) const;
        float const findWorstIso( const std::map<edm::Ptr<reco::Vertex>, float> &mp ) const;

        float sipip_;
        float sieip_;
        //float zernike20_;
        //float zernike42_;
        float e2nd_;
        float e2x5right_;
        float e2x5left_;
        float e2x5top_;
        float e2x5bottom_;
        float e2x5max_;
        float eright_;
        float eleft_;
        float etop_;
        float ebottom_;
        float e1x3_;
        float S4_;
        float pfPhoIso04_;
        float pfPhoIso03_;
        float pfPhoIso03Cor_;
        float pfNeutIso04_;
        float pfNeutIso03_;
        float pfChgIsoWrtWorstVtx04_;
        float pfChgIsoWrtWorstVtx03_;
        float pfChgIsoWrtChosenVtx02_;
        float pfChgIsoWrtChosenVtx03_;
        float ESEffSigmaRR_;
        float sigEOverE_;
        std::map<edm::Ptr<reco::Vertex>, float> pfChgIso04_;
        std::map<edm::Ptr<reco::Vertex>, float> pfChgIso03_;
        std::map<edm::Ptr<reco::Vertex>, float> pfChgIso02_;
        std::map<edm::Ptr<reco::Vertex>, float> phoIdMvaD_;
        bool passElecVeto_;
        std::map<std::string, std::map<edm::Ptr<reco::Vertex>, float> > extraChargedIsolations_;
        std::map<std::string, float> extraPhotonIsolations_, extraNeutralIsolations_;
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

