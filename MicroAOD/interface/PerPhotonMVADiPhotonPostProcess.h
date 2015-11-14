#ifndef flashgg_PerPhotonMVADiPhotonComputer_h
#define flashgg_PerPhotonMVADiPhotonComputer_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Common/interface/EventBase.h"

#include "flashgg/MicroAOD/interface/PhotonMVAComputer.h"

#include <tuple>
#include <vector>
#include <map>

namespace flashgg {
    template<class OutputCollection> class PerPhotonMVADiPhotonPostProcess
    {

    public:
        PerPhotonMVADiPhotonPostProcess( const edm::ParameterSet &config, edm::ConsumesCollector &&cc ) : computer_( config ) {};

        template<class EdmFilter> void init( EdmFilter & ) { }

        void process( edm::OrphanHandle<OutputCollection> output, edm::EventBase &event )
        {
            computer_.update( event );
            for( auto &dipho : *output ) {
                dipho.makePhotonsPersistent();
                computer_.fill( dipho.leadingPhoton() );
                computer_.fill( dipho.subLeadingPhoton() );
            }
        };
    private:
        PhotonMVAComputer computer_;
    };
}

#endif // flashgg_PerPhotonMVADiPhotonComputer_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

