#include "../interface/PluggableAnalyzer.h"

using namespace flashgg::fwlite;

int main( int argc, char *argv[] )
{
    // load framework libraries
    gSystem->Load( "libFWCoreFWLite" );
    AutoLibraryLoader::enable();

    // only allow one argument for this simple example which should be the
    // the python cfg file
    if( argc < 2 ) {
        std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
        return 0;
    }

    // get the python configuration
    PythonProcessDesc builder( argv[1], argc, argv );
    PluggableAnalyzer ana( *( builder.processDesc()->getProcessPSet() ), std::string( "analyzer" ) );
    ana.beginJob();
    ana.analyze();
    ana.endJob();
    return 0;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

