#ifndef FLASHgg_RandomizedObjectProducer_h
#define FLASHgg_RandomizedObjectProducer_h

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandGauss.h"

namespace flashgg {

    template <typename pat_object>
    class RandomizedObjectProducer : public edm::EDProducer
    {
    public:
        RandomizedObjectProducer( const edm::ParameterSet & );
        void produce( edm::Event &, const edm::EventSetup & ) override;

    private:
        edm::EDGetTokenT<edm::View<pat_object> > token_;
        std::vector<std::string> labels_;
        //std::string pdf_; // only gaussians with mean 0 and width 1 for the time being

    };

    template <typename pat_object>
    RandomizedObjectProducer<pat_object>::RandomizedObjectProducer( const edm::ParameterSet &ps ) :
        token_(consumes<edm::View<pat_object> >(ps.getParameter<edm::InputTag>("src"))),
        labels_(ps.getParameter<std::vector<std::string> >("labels"))
        //prefix_(ps.getParameter<std::string>("pdf")
    {
        produces<std::vector<pat_object> >();
    }

    template <typename pat_object>
    void RandomizedObjectProducer<pat_object>::produce( edm::Event &evt, const edm::EventSetup & )
    {
        edm::Service<edm::RandomNumberGenerator> rng;
        if( ! rng.isAvailable() ) {
            throw cms::Exception( "Configuration" ) << "ObjectSystematicProducer requires the RandomNumberGeneratorService  - please add to configuration";
        }

        edm::Handle<edm::View<pat_object> > objects;
        evt.getByToken( token_, objects );

        CLHEP::HepRandomEngine & engine = rng->getEngine( evt.streamID() );
        unique_ptr<std::vector<pat_object> > out_obj( new std::vector<pat_object>() );
        CLHEP::RandGauss::shoot(&engine, 0., 1.);

        for (const auto & obj : *objects) {
            auto o = obj;
            for (auto l : labels_) {
                    o.addUserFloat(l, CLHEP::RandGauss::shoot(&engine, 0., 1.));
                    out_obj->push_back(o);
            }
        }
        evt.put(std::move(out_obj));
    }
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
