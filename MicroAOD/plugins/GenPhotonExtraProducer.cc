
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"


using namespace std;
using namespace edm;

namespace flashgg {

    class GenPhotonExtraProducer : public EDProducer
    {

    public:
        GenPhotonExtraProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<pat::PackedGenParticle> > genPhotonsToken_;
        EDGetTokenT<View<pat::PackedGenParticle> > genParticlesToken_;
        double isoConeSize_, epsilon0_, n0_;
        std::vector<int> promptMothers_;
        flashgg::GenPhotonExtra::match_type defaultType_;
    };


    GenPhotonExtraProducer::GenPhotonExtraProducer( const ParameterSet &iConfig ) :
        genPhotonsToken_( consumes<View<pat::PackedGenParticle> >( iConfig.getParameter<InputTag>( "genPhotons" ) ) ),
        genParticlesToken_( consumes<View<pat::PackedGenParticle> >( iConfig.getParameter<InputTag>( "genParticles" ) ) ),
        isoConeSize_( iConfig.getParameter<double>( "isoConeSize" ) ),
        epsilon0_( iConfig.getParameter<double>( "epsilon0" ) ),
        n0_( iConfig.getParameter<double>( "n0" ) ),
        defaultType_( flashgg::Photon::kUnkown )
    {
        if( iConfig.exists( "promptMothers" ) ) {
            promptMothers_ = iConfig.getParameter<std::vector<int> >( "promptMothers" );
        }
        if( iConfig.exists( "defaultType" ) ) {
            defaultType_ = static_cast<flashgg::GenPhotonExtra::match_type>( iConfig.getParameter<int>( "defaultType" ) );
        }
        produces<vector<flashgg::GenPhotonExtra> >();
    }

    void GenPhotonExtraProducer::produce( Event &evt, const EventSetup &iSetup )
    {

        Handle<View<pat::PackedGenParticle> > genPhotons, genParticles;
        evt.getByToken( genPhotonsToken_, genPhotons );
        evt.getByToken( genParticlesToken_, genParticles );

        unique_ptr<vector<flashgg::GenPhotonExtra> > extraColl( new vector<flashgg::GenPhotonExtra> );

        auto genPhotonPointers = genPhotons->ptrs();
        for( auto &genPho : genPhotonPointers ) {
            flashgg::GenPhotonExtra extra( genPho );
            extra.setType( PhotonMCUtils::determineMatchType( *genPho, promptMothers_, defaultType_ ) );
            extra.setGenIso( PhotonMCUtils::isoSum( *genPho, *genParticles, isoConeSize_ ) );
            extra.setFrixioneIso( PhotonMCUtils::frixioneIso( *genPho, *genParticles, isoConeSize_, epsilon0_, n0_ ) );
            extraColl->push_back( extra );
        }

        evt.put( std::move( extraColl ) );

        /// orig_collection = 0;
    }
}



typedef flashgg::GenPhotonExtraProducer FlashggGenPhotonExtraProducer;
DEFINE_FWK_MODULE( FlashggGenPhotonExtraProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

