#include "flashgg/MicroAOD/interface/IsolationAlgoBase.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

/// #include "FWCore/Utilities/interface/RandomNumberGenerator.h"
/// #include "CLHEP/Random/RandomEngine.h"

namespace flashgg {

    class RandomConeIsolationAlgo : public IsolationAlgoBase
    {
    public:
        RandomConeIsolationAlgo( const edm::ParameterSet &conf, edm::ConsumesCollector && iC )  : IsolationAlgoBase( conf, std::forward<edm::ConsumesCollector>(iC) ),
            conesize_( conf.getParameter<double>( "coneSize" ) )
        {
            if( conf.exists( "charged" ) ) {
                chargedVetos_ = conf.getParameter<std::vector<double> >( "charged" );
                assert( chargedVetos_.size()  == 3 );
            }
            if( conf.exists( "neutral" ) ) {
                neutralVetos_ = conf.getParameter<std::vector<double> >( "neutral" );
                assert( neutralVetos_.size()  == 6 );
            }
            if( conf.exists( "photon" ) ) {
                photonVetos_ = conf.getParameter<std::vector<double> >( "photon" );
                assert( photonVetos_.size()  == 6 );
            }

            if( conf.exists( "vetoCollections" ) ) {
                vetoCollections_ = conf.getParameter<std::vector<edm::InputTag> >( "vetoCollections" );
                veto_ = conf.getParameter<double>( "veto" );
                for (unsigned i = 0 ; i < vetoCollections_.size() ; i++) {
                    auto token = iC.consumes<edm::View<reco::Candidate> >(vetoCollections_[i]);
                    tokenVetos_.push_back(token);
                }
            }

            utils_.removeOverlappingCandidates( conf.getParameter<bool>( "doOverlapRemoval" ) );
        }

        virtual void begin( const pat::Photon &, const edm::Event &, const edm::EventSetup & );
        virtual bool hasChargedIsolation() { return ! chargedVetos_.empty(); };
        virtual float chargedIsolation( const edm::Ptr<pat::Photon> &, const edm::Ptr<reco::Vertex>, const flashgg::VertexCandidateMap & );
        virtual bool hasCaloIsolation( reco::PFCandidate::ParticleType typ )
        {
            return ( typ == reco::PFCandidate::gamma && ! photonVetos_.empty() ) ||
                   ( typ == reco::PFCandidate::h0    && ! neutralVetos_.empty() );
        };
        virtual float caloIsolation( const edm::Ptr<pat::Photon> &, const std::vector<edm::Ptr<pat::PackedCandidate> > &, reco::PFCandidate::ParticleType,
                                     const reco::Vertex *vtx = 0 );

        virtual void end( pat::Photon & );

    private:
        double conesize_, deltaPhi_, veto_;
        bool found_;
        PhotonIdUtils utils_;
        std::vector<double> chargedVetos_, photonVetos_, neutralVetos_;
        std::vector<edm::InputTag> vetoCollections_;
        std::vector<edm::EDGetTokenT<edm::View<reco::Candidate> > > tokenVetos_;
    };

    void RandomConeIsolationAlgo::begin( const pat::Photon &pho, const edm::Event &event, const edm::EventSetup & )
    {
        std::vector<edm::Handle<edm::View<reco::Candidate> > > vetos( vetoCollections_.size() );

        for( size_t icoll = 0; icoll < vetoCollections_.size(); ++icoll ) {
            //            event.getByLabel( vetoCollections_[icoll], vetos[icoll] );
            event.getByToken( tokenVetos_[icoll], vetos[icoll] );
        }

        found_ = false;
        std::vector<double> test{0.5 * TMath::Pi(), -0.5 * TMath::Pi()};
        for( auto it : test ) {
            deltaPhi_ = it;
            found_ = true;
            for( auto &coll : vetos ) {
                for( auto &cand : *coll ) {
                    float dEta = pho.eta() - cand.eta();
                    float dPhi = reco::deltaPhi( pho.phi() + deltaPhi_, cand.phi() );
                    float dR = sqrt( dEta * dEta + dPhi * dPhi );
                    if( dR < veto_ ) {
                        found_ = false;
                        break;
                    }
                }
                if( ! found_ ) { break; }
            }
            if( found_ ) { break; }
        }

        utils_.deltaPhiRotation( deltaPhi_ );
    }

    float RandomConeIsolationAlgo::chargedIsolation( const edm::Ptr<pat::Photon> &pho, const edm::Ptr<reco::Vertex> vtx,
            const flashgg::VertexCandidateMap &mp )
    {
        if( ! chargedVetos_.empty() ) {
            return found_ ? utils_.pfIsoChgWrtVtx( pho, vtx, mp, conesize_, chargedVetos_[0], chargedVetos_[1], chargedVetos_[2] ) : 999.;
        }
        return 0.;
    }

    float RandomConeIsolationAlgo::caloIsolation( const edm::Ptr<pat::Photon> &pho, const std::vector<edm::Ptr<pat::PackedCandidate> > &ptrs,
            reco::PFCandidate::ParticleType typ, const reco::Vertex *vtx )
    {
        if( typ == reco::PFCandidate::gamma && ! photonVetos_.empty() ) {
            return found_ ? utils_.pfCaloIso( pho, ptrs, conesize_, photonVetos_[0], photonVetos_[1], photonVetos_[2], photonVetos_[3], photonVetos_[4], photonVetos_[5],
                                              typ, vtx ) : 999.;
        } else if( typ == reco::PFCandidate::h0 && ! neutralVetos_.empty() ) {
            return found_ ? utils_.pfCaloIso( pho, ptrs, conesize_, neutralVetos_[0], neutralVetos_[1], neutralVetos_[2], neutralVetos_[3], neutralVetos_[4],
                                              neutralVetos_[5], typ, vtx ) : 999.;
        }
        return 0.;
    }

    void RandomConeIsolationAlgo::end( pat::Photon &pho )
    {
        pho.addUserFloat( name() + "_rndcone_deltaphi", deltaPhi_ );
    }


}

DEFINE_EDM_PLUGIN( FlashggIsolationAlgoFactory,
                   flashgg::RandomConeIsolationAlgo,
                   "FlashggRandomConeIsolationAlgo" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

