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
    };

    VectorVectorJetCollector::VectorVectorJetCollector( const ParameterSet &iConfig ) :
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) )
    {
        produces<vector<vector<Jet> > >();
    }

    void VectorVectorJetCollector::produce( Event &evt, const EventSetup & )
    {
        auto_ptr<vector<vector<Jet> > > result( new vector<vector<Jet> > );

        size_t output_size = 0;
        JetViewVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            std::cout << " before getByLabel " << j << std::endl;
            evt.getByLabel( inputTagJets_[j], Jets[j] );
            if( Jets[j]->size() > 0 ) { output_size = j + 1; }
            std::cout << " Jets[j]->size()=" << Jets[j]->size() << std::endl;
        }

        std::cout << " output_size=" << output_size << std::endl;

        result->resize( output_size );

        std::cout << " Resized!" << std::endl;

        for( size_t j = 0 ; j < result->size() ; ++j ) {
            std::cout << "  j=" << j << std::endl;
            for( size_t k = 0 ; k < Jets[j]->size() ; ++k ) {
                std::cout << "   j=" << j << " k=" << k << std::endl;
                result->at( j ).push_back( *( Jets[j]->ptrAt( k ) ) );
            }
        }

        std::cout << " Before put" << std::endl;

        evt.put( result );
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
