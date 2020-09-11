
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/DataFormats/interface/GenLeptonExtra.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"
#include "flashgg/MicroAOD/interface/LeptonMCUtils.h"


using namespace std;
using namespace edm;

namespace flashgg {

    class GenLeptonExtraProducer : public EDProducer
    {

    public:
        GenLeptonExtraProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<pat::PackedGenParticle> > genLeptonsToken_;
        EDGetTokenT<View<pat::PackedGenParticle> > genParticlesToken_;
        double isoConeSize_, dressingConeSize_;
        std::vector<int> promptMothers_;
        //        flashgg::GenLeptonExtra::match_type defaultType_;
    };


    GenLeptonExtraProducer::GenLeptonExtraProducer( const ParameterSet &iConfig ) :
        genLeptonsToken_( consumes<View<pat::PackedGenParticle> >( iConfig.getParameter<InputTag>( "genLeptons" ) ) ),
        genParticlesToken_( consumes<View<pat::PackedGenParticle> >( iConfig.getParameter<InputTag>( "genParticles" ) ) ),
        isoConeSize_( iConfig.getParameter<double>( "isoConeSize" ) ),
        dressingConeSize_( iConfig.getParameter<double>( "dressingConeSize" ) )
        //        defaultType_( flashgg::Photon::kUnkown ) //FIXME
    {
        if( iConfig.exists( "promptMothers" ) ) {
            promptMothers_ = iConfig.getParameter<std::vector<int> >( "promptMothers" );
        }
//        if( iConfig.exists( "defaultType" ) ) {
//            defaultType_ = static_cast<flashgg::GenLeptonExtra::match_type>( iConfig.getParameter<int>( "defaultType" ) );
//        }
        produces<vector<flashgg::GenLeptonExtra> >();
    }

    void GenLeptonExtraProducer::produce( Event &evt, const EventSetup &iSetup )
    {

        Handle<View<pat::PackedGenParticle> > genLeptons, genParticles;
        evt.getByToken( genLeptonsToken_, genLeptons );
        evt.getByToken( genParticlesToken_, genParticles );

        unique_ptr<vector<flashgg::GenLeptonExtra> > extraColl( new vector<flashgg::GenLeptonExtra> );

        auto genLeptonPointers = genLeptons->ptrs();
        for( auto &genLep : genLeptonPointers ) {
            flashgg::GenLeptonExtra extra( genLep );


     //       extra.setType( PhotonMCUtils::determineMatchType( *genLep, promptMothers_, defaultType_ ) );
            extra.setGenIso( PhotonMCUtils::isoSum( *genLep, *genParticles, isoConeSize_ ) );
     //       extra.setFrixioneIso( PhotonMCUtils::frixioneIso( *genLep, *genParticles, isoConeSize_, epsilon0_, n0_ ) );

            extra.setP4( LeptonMCUtils::dressedP4( *genLep, *genParticles, dressingConeSize_) );
            //            extra.setP4( genLep->p4() );
            extra.setGenIsoAfterDressing( LeptonMCUtils::isoSumAfterDressing( *genLep, *genParticles, isoConeSize_, dressingConeSize_) );


            extraColl->push_back( extra );
        }

        evt.put( std::move( extraColl ) );

        /// orig_collection = 0;
    }
}



typedef flashgg::GenLeptonExtraProducer FlashggGenLeptonExtraProducer;
DEFINE_FWK_MODULE( FlashggGenLeptonExtraProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

