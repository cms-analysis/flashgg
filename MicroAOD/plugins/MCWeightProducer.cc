// system include files
#include <memory>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/DataFormats/interface/MCWeightObject.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class MCWeightProducer : public edm::EDProducer
    {
    public:
        MCWeightProducer( const edm::ParameterSet & );
    private:
        void produce( edm::Event &, const edm::EventSetup & );
        EDGetTokenT<LHEEventProduct> LHEEventToken_;
        int upper_weight_index_;
        int lower_weight_index_;
    };

    MCWeightProducer::MCWeightProducer( const edm::ParameterSet &iConfig ):
        LHEEventToken_( consumes<LHEEventProduct>( iConfig.getUntrackedParameter<InputTag>( "LHEEventTag", InputTag( "LHEEventProduct" ) ) ) )
    {

        upper_weight_index_ = iConfig.getUntrackedParameter<int>( "upper_weight_index" );
        lower_weight_index_ = iConfig.getUntrackedParameter<int>( "lower_weight_index" );

        produces<vector<flashgg::MCWeightObject> >();

    }


    void MCWeightProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<LHEEventProduct> LHEEventHandle;
        evt.getByToken( LHEEventToken_, LHEEventHandle );

        std::auto_ptr<vector<flashgg::MCWeightObject> > mcWeightColl( new vector<flashgg::MCWeightObject> );

        flashgg::MCWeightObject mcWeight;

        vector<double> weights;

        double weight = 1;

        for( int i = lower_weight_index_; i < upper_weight_index_; i++ ) {

            weight = LHEEventHandle->weights()[i].wgt;

            weights.push_back( weight );

        }

        mcWeight.mc_weight_container.insert( pair<string, vector<double> >( "pdfWeight", weights ) );

        mcWeightColl->push_back( mcWeight );

        evt.put( mcWeightColl );
    }


}
typedef flashgg::MCWeightProducer FlashggMCWeightProducer;
DEFINE_FWK_MODULE( FlashggMCWeightProducer );
