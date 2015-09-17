#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

using namespace edm;
using namespace reco;

namespace flashgg {


    GlobalVariablesComputer::GlobalVariablesComputer( const edm::ParameterSet &cfg ) :
        rhoTag_( cfg.getParameter<edm::InputTag>( "rho" ) ),
        vtxTag_( cfg.getParameter<edm::InputTag>( "vertexes" ) )
    {
    }

    float *GlobalVariablesComputer::addressOf( const std::string &varName )
    {
        if( varName == "rho" ) { return &cache_.rho; }
        return 0;
    }

    int GlobalVariablesComputer::indexOf( const std::string &varName )
    {
        if( varName == "rho" ) { return 0; }
        else if( varName == "nvtx" ) { return 1; }
        else if( varName == "run" ) { return 2; }
        else if( varName == "event" ) { return 3; }
        else if( varName == "lumi" ) { return 4; }
        return -1;
    }

    float GlobalVariablesComputer::valueOf( const std::string &varName )
    {
        return valueOf( indexOf( varName ) );
    }

    float GlobalVariablesComputer::valueOf( int varIndex )
    {
        if( varIndex == 0 ) { return cache_.rho; }
        else if( varIndex == 1 ) { return ( float )cache_.nvtx; }
        else if( varIndex == 2 ) { return ( float )cache_.run; }
        else if( varIndex == 3 ) { return ( float )cache_.event; }
        else if( varIndex == 4 ) { return ( float )cache_.lumi; }
        return -1e+6;

    }

    void GlobalVariablesComputer::update( const EventBase &evt )
    {
        Handle<double> rhoHandle;
        evt.getByLabel( rhoTag_, rhoHandle );

        Handle<VertexCollection> vertices;
        evt.getByLabel( vtxTag_, vertices );

        cache_.event = evt.id().event();
        cache_.lumi = evt.id().luminosityBlock();
        cache_.run  = evt.id().run();
        cache_.rho = *rhoHandle;
        cache_.nvtx = vertices->size();
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

