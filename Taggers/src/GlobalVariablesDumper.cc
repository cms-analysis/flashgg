#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TTree.h"

using namespace edm;
using namespace reco;

namespace flashgg {


    GlobalVariablesComputer::GlobalVariablesComputer( const edm::ParameterSet &cfg ) :
        rhoTag_( cfg.getParameter<edm::InputTag>( "rho" ) ),
        vtxTag_( cfg.getParameter<edm::InputTag>( "vertexes" ) )
    {
    }

    // GlobalVariablesComputer::~GlobalVariablesComputer()
    // {}

    float *GlobalVariablesComputer::addressOf( const std::string &varName )
    {
        if( varName == "rho" ) { return &cache_.rho; }
        return 0;
    }

    void GlobalVariablesComputer::update( const EventBase &evt )
    {
        Handle<double> rhoHandle;
        evt.getByLabel( rhoTag_, rhoHandle );

        Handle<VertexCollection> vertices;
        evt.getByLabel( vtxTag_, vertices );


        cache_.rho = *rhoHandle;
        cache_.nvtx = vertices->size();
    }

    GlobalVariablesDumper::GlobalVariablesDumper( const edm::ParameterSet &cfg ) :
        GlobalVariablesComputer( cfg )
    {}


    GlobalVariablesDumper::~GlobalVariablesDumper()
    {}

    void GlobalVariablesDumper::bookTreeVariables( TTree *tree, const std::map<std::string, std::string> &replacements )
    {
        tree->Branch( "rho", &cache_.rho );
        tree->Branch( "nvtx", &cache_.nvtx );
    }

    void GlobalVariablesDumper::fill( const EventBase &evt )
    {
        update( evt );
    }


}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

