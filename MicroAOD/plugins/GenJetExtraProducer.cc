
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/DataFormats/interface/GenJetExtra.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"
#include "PhysicsTools/JetMCUtils/interface/CandMCTag.h"
#include "PhysicsTools/CandUtils/interface/pdgIdUtils.h"
//#include "flashgg/MicroAOD/interface/JetMCUtils.h"


using namespace std;
using namespace edm;

namespace flashgg {

    class GenJetExtraProducer : public EDProducer
    {

    public:
        GenJetExtraProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        //        EDGetTokenT<View<pat::PackedGenParticle> > genJetsToken_;
        //        EDGetTokenT<View<pat::PackedGenParticle> > genParticlesToken_;
        
        //        edm::EDGetTokenT<reco::GenParticleCollection> genJetsToken_;
        edm::EDGetTokenT<View<reco::GenJet> > genJetsToken_;
        double isoConeSize_, dressingConeSize_;
        std::vector<int> promptMothers_;
        //        flashgg::GenJetExtra::match_type defaultType_;
    };


    GenJetExtraProducer::GenJetExtraProducer( const ParameterSet &iConfig ) :
        //        genJetsToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag>( "genJets" ) ) )
        genJetsToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<edm::InputTag>( "genJets" ) ) )
    {
        produces<vector<flashgg::GenJetExtra> >();
    }

    void GenJetExtraProducer::produce( Event &evt, const EventSetup &iSetup )
    {

        Handle<View<reco::GenJet> > genJets;
        evt.getByToken( genJetsToken_, genJets );
        unique_ptr<vector<flashgg::GenJetExtra> > extraColl( new vector<flashgg::GenJetExtra> );
        
        for( auto &genJet : *genJets ){
            flashgg::GenJetExtra extra( genJet );
            bool hasBottom = false;
            for(size_t d = 0; d < genJet.numberOfDaughters(); d++){
                std::cout<<"looking at daughter number "<<d<<std::endl;
                std::cout<<"with pdgid "<<genJet.daughter( d )->pdgId()<<std::endl;
                for (size_t m = 0; m < genJet.daughter( d )->numberOfMothers(); m++){
                    if(CandMCTagUtils::hasBottom( *( genJet.daughter( d )->mother(m) ) ) ){//  ||  ( std::abs( genJet.daughter( d )->mother(m)->pdgId() ) == 5 )   ){
                        //                if( std::abs( genJet.daughter( d )->pdgId() ) == 5){
                        hasBottom=true;
                        std::cout<<"found mother of daughter with bottom, setting hasBottom to true and break"<<std::endl;
                        break;
                    } 
                }
                if(hasBottom == true) break;
            }
            std::cout<<"set jet hasbBottom to "<<hasBottom<<std::endl;
            extra.setHasBottom( hasBottom );
            

            extraColl->push_back( extra );
        }

        evt.put( std::move( extraColl ) );

        /// orig_collection = 0;
    }
}



typedef flashgg::GenJetExtraProducer FlashggGenJetExtraProducer;
DEFINE_FWK_MODULE( FlashggGenJetExtraProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

