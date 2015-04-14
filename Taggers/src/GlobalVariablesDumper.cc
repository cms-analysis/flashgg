#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TTree.h"

using namespace edm;
using namespace reco;

namespace flashgg {
    

    GlobalVariablesDumper::GlobalVariablesDumper( const edm::ParameterSet &cfg ) :
        GlobalVariablesComputer( cfg )
    {}


    GlobalVariablesDumper::~GlobalVariablesDumper()
    {}

    void GlobalVariablesDumper::bookTreeVariables( TTree *tree, const std::map<std::string, std::string> &replacements )
    {
        tree->Branch( "rho", &cache_.rho );
        tree->Branch( "nvtx", &cache_.nvtx );
        tree->Branch( "event", &cache_.event, "event/i" );
        tree->Branch( "lumi", &cache_.lumi, "lumi/b" );
        tree->Branch( "run", &cache_.run, "run/i" );
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

