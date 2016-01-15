#include "flashgg/MicroAOD/interface/IsolationAlgoBase.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

namespace flashgg {

    class StdIsolationAlgo : public IsolationAlgoBase
    {
    public:
        StdIsolationAlgo( const edm::ParameterSet &conf, edm::ConsumesCollector && iC )  : IsolationAlgoBase( conf, std::forward<edm::ConsumesCollector>(iC) ),
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
        double conesize_;
        PhotonIdUtils utils_;
        std::vector<double> chargedVetos_, photonVetos_, neutralVetos_;
    };

    void StdIsolationAlgo::begin( const pat::Photon &, const edm::Event &, const edm::EventSetup & )
    {
    }

    float StdIsolationAlgo::chargedIsolation( const edm::Ptr<pat::Photon> &pho, const edm::Ptr<reco::Vertex> vtx,
            const flashgg::VertexCandidateMap &mp )
    {
        if( ! chargedVetos_.empty() ) {
            return utils_.pfIsoChgWrtVtx( pho, vtx, mp, conesize_, chargedVetos_[0], chargedVetos_[1], chargedVetos_[2] );
        }
        return 0.;
        /// return utils_.pfIsoChgWrtVtx(pho,vtx,mp,conesize_,0.02,0.02,0.1);
    }

    float StdIsolationAlgo::caloIsolation( const edm::Ptr<pat::Photon> &pho, const std::vector<edm::Ptr<pat::PackedCandidate> > &ptrs,
                                           reco::PFCandidate::ParticleType typ, const reco::Vertex *vtx )
    {
        if( typ == reco::PFCandidate::gamma && ! photonVetos_.empty() ) {
            return utils_.pfCaloIso( pho, ptrs, conesize_, photonVetos_[0], photonVetos_[1], photonVetos_[2], photonVetos_[3], photonVetos_[4], photonVetos_[5], typ, vtx );
        } else if( typ == reco::PFCandidate::h0 && ! neutralVetos_.empty() ) {
            return utils_.pfCaloIso( pho, ptrs, conesize_, neutralVetos_[0], neutralVetos_[1], neutralVetos_[2], neutralVetos_[3], neutralVetos_[4], neutralVetos_[5], typ,
                                     vtx );
        }
        return 0.;
        /// return utils_.pfCaloIso(pho, ptrs, conesize_, 0.0, 0.070, 0.015, 0.0, 0.0, 0.0, typ, vtx);
    }

    void StdIsolationAlgo::end( pat::Photon & )
    {

    }


}

DEFINE_EDM_PLUGIN( FlashggIsolationAlgoFactory,
                   flashgg::StdIsolationAlgo,
                   "FlashggStdIsolationAlgo" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

