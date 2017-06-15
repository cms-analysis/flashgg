// VectorVectorJetCollector.cc
// S. Zenz, July 2015
// Because sometimes internal rhyme is more important than consistent collection naming

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/Jet.h"

using namespace std;
using namespace edm;

namespace flashgg {
    class VectorVectorJetCollector : public EDProducer
    {

    public:
        VectorVectorJetCollector( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetViewVector;

        std::vector<edm::InputTag> inputTagJets_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
    };

    VectorVectorJetCollector::VectorVectorJetCollector( const ParameterSet &iConfig ) :
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) )
    {
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<vector<Jet> > >();
    }

    void VectorVectorJetCollector::produce( Event &evt, const EventSetup & )
    {
        unique_ptr<vector<vector<Jet> > > result( new vector<vector<Jet> > );

        size_t output_size = 0;
        JetViewVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            //            evt.getByLabel( inputTagJets_[j], Jets[j] );
            evt.getByToken( tokenJets_[j], Jets[j] );
            if( Jets[j]->size() > 0 ) { output_size = j + 1; }
        }

        result->resize( output_size );

        for( size_t j = 0 ; j < result->size() ; ++j ) {
            for( size_t k = 0 ; k < Jets[j]->size() ; ++k ) {
                result->at( j ).push_back( *( Jets[j]->ptrAt( k ) ) );
            }
        }

        evt.put( std::move( result ) );
    }
}
typedef flashgg::VectorVectorJetCollector FlashggVectorVectorJetCollector;
DEFINE_FWK_MODULE( FlashggVectorVectorJetCollector );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
