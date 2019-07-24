// system include files
#include <memory>
// user include file
#include "flashgg/DataFormats/interface/Met.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

namespace flashgg {

    class MetProducer : public edm::EDProducer
    {
    public:
        MetProducer( const edm::ParameterSet & );

    private:
        void produce( edm::Event &, const edm::EventSetup & );

        edm::EDGetTokenT<edm::View<pat::MET> > metToken_;
        edm::EDGetTokenT<bool> ecalBadCalibFilterUpdate_token;
    };

    MetProducer::MetProducer( const edm::ParameterSet &iConfig ):
        metToken_( consumes<edm::View<pat::MET> >( iConfig.getParameter<edm::InputTag>( "metTag" ) ) )
    {
        ecalBadCalibFilterUpdate_token = consumes<bool> (edm::InputTag("ecalBadCalibReducedMINIAODFilter"));
        produces<vector<flashgg::Met> >();
    }

    void MetProducer::produce( edm::Event &evt, const edm::EventSetup & )
    {
        edm::Handle<edm::View<pat::MET> >  pmets;
        evt.getByToken( metToken_, pmets );
        std::unique_ptr<vector<flashgg::Met> > metColl( new vector<flashgg::Met> );

        // Check if event passes ecalBadCalibFilter
        edm::Handle<bool> passecalBadCalibFilterUpdate;
        evt.getByToken(ecalBadCalibFilterUpdate_token, passecalBadCalibFilterUpdate);
        bool _passecalBadCalibFilterUpdate = (*passecalBadCalibFilterUpdate);

        for( unsigned int metIndex = 0; metIndex < pmets->size(); metIndex++ ) 
            {
                edm::Ptr<pat::MET> pmet = pmets->ptrAt( metIndex );
                flashgg::Met fmet = flashgg::Met( *pmet );
                fmet.setPassEcalBadCalibFilter(_passecalBadCalibFilterUpdate);
                metColl->push_back(fmet);
            }
        evt.put( std::move(metColl ) );
    }
}

typedef flashgg::MetProducer FlashggMetProducer;
DEFINE_FWK_MODULE( FlashggMetProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

