#ifndef FLASHgg_RandomizedPerPhotonDiPhotonProducer_h
#define FLASHgg_RandomizedPerPhotonDiPhotonProducer_h

/* This is a *temporary* producer supposed to be replaced by the
 * RandomizedPhotonProducer run at MicroAOD production time. This producer is
 * meant to be run on-the-fly on MicroAOD produced already with no random
 * information attached to the Photon's. It is close to a mere copy-n-paste of
 * RandomizedObjectProducer.h
 */

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

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

    class RandomizedPerPhotonDiPhotonProducer : public edm::EDProducer
    {
    public:
        RandomizedPerPhotonDiPhotonProducer( const edm::ParameterSet & );
        void produce( edm::Event &, const edm::EventSetup & ) override;

    private:
        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > token_;
        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > input;
        std::vector<std::string> labels_;
        //std::string pdf_; // only gaussians with mean 0 and width 1 for the time being
    };

    RandomizedPerPhotonDiPhotonProducer::RandomizedPerPhotonDiPhotonProducer( const edm::ParameterSet &ps ) :
        token_(consumes<edm::View<flashgg::DiPhotonCandidate> >(ps.getParameter<edm::InputTag>("src"))),
        labels_(ps.getParameter<std::vector<std::string> >("labels"))
        //prefix_(ps.getParameter<std::string>("pdf")
    {
        produces<std::vector<flashgg::DiPhotonCandidate> >();
    }

    void RandomizedPerPhotonDiPhotonProducer::produce( edm::Event &evt, const edm::EventSetup & )
    {
        edm::Service<edm::RandomNumberGenerator> rng;
        if( ! rng.isAvailable() ) {
            throw cms::Exception( "Configuration" ) << "ObjectSystematicProducer requires the RandomNumberGeneratorService  - please add to configuration";
        }

        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > objects;
        evt.getByToken( token_, objects );

        CLHEP::HepRandomEngine & engine = rng->getEngine( evt.streamID() );
        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > out_obj( new std::vector<flashgg::DiPhotonCandidate>() );
        CLHEP::RandGauss::shoot(&engine, 0., 1.);

        for (const auto & obj : *objects) {
            auto o = obj;
            for (auto l : labels_) {
                    o.makePhotonsPersistent();
                    o.getLeadingPhoton().addUserFloat(l, CLHEP::RandGauss::shoot(&engine, 0., 1.));
                    o.getSubLeadingPhoton().addUserFloat(l, CLHEP::RandGauss::shoot(&engine, 0., 1.));
                    out_obj->push_back(o);
            }
        }
        evt.put( std::move(out_obj) );
    }
}

typedef flashgg::RandomizedPerPhotonDiPhotonProducer FlashggRandomizedPerPhotonDiPhotonProducer;
DEFINE_FWK_MODULE( FlashggRandomizedPerPhotonDiPhotonProducer );

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
