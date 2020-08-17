#ifndef flashgg_PerPhotonMVADiPhotonComputer_h
#define flashgg_PerPhotonMVADiPhotonComputer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAOD/interface/PhotonMVAComputer.h"

#include <tuple>
#include <vector>
#include <map>

using namespace std;
using namespace edm;

namespace flashgg {
    class PerPhotonMVADiPhotonProducer : public EDProducer
    {

    public:
        PerPhotonMVADiPhotonProducer( const edm::ParameterSet &config ) :
            srcToken_( consumes<View<flashgg::DiPhotonCandidate> >( config.getParameter<InputTag>( "src" ) ) ),
            computer_( config, this->consumesCollector() )
        {
            produces<vector<flashgg::DiPhotonCandidate> >();
        };

        void produce( Event &evt, const EventSetup & )
        {
            Handle<View<flashgg::DiPhotonCandidate> > input;
            evt.getByToken( srcToken_, input );
            unique_ptr<vector<DiPhotonCandidate> > output( new vector<DiPhotonCandidate> );
            computer_.update( evt );
            for( auto dipho : *input ) {
                dipho.makePhotonsPersistent();
                computer_.fill( dipho.getLeadingPhoton() );
                computer_.fill( dipho.getSubLeadingPhoton() );
                output->push_back( dipho );
            }
            evt.put( std::move( output ) );
        };

    private:
        EDGetTokenT<View<DiPhotonCandidate> > srcToken_;
        PhotonMVAComputer computer_;
    };
}

typedef flashgg::PerPhotonMVADiPhotonProducer FlashggPerPhotonMVADiPhotonProducer;
DEFINE_FWK_MODULE( FlashggPerPhotonMVADiPhotonProducer );

#endif // flashgg_PerPhotonMVADiPhotonComputer_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

