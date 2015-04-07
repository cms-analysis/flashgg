#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"

#include <iostream>
using namespace std;

namespace flashgg {

    map<string, AnalyzerFactory *> &AnalyzerFactory::getRegister()
    {
        static map<string, AnalyzerFactory *> theRegister;
        return theRegister;
    }


    void AnalyzerFactory::registerClass( const string &name, AnalyzerFactory *factory )
    {
        getRegister()[name] = factory;
    }


    const AnalyzerFactory *AnalyzerFactory::allocator( const string &name )
    {
        return getRegister()[name];
    }

    PluggableAnalyzer::PluggableAnalyzer( const edm::ParameterSet &cfg, TFileDirectory &fileService )
    {
        string className = cfg.getUntrackedParameter<string>( "className" );
        const AnalyzerFactory *factory = AnalyzerFactory::allocator( className );
        analyzer_ = factory->allocate( cfg, fileService );
    }

    void PluggableAnalyzer::beginJob()
    {
        static_cast<edm::BasicAnalyzer *>( analyzer_ )->beginJob();
    }


    void PluggableAnalyzer::endJob()
    {
        static_cast<edm::BasicAnalyzer *>( analyzer_ )->endJob();
    }

    void PluggableAnalyzer::analyze( const edm::EventBase &event )
    {
        static_cast<edm::BasicAnalyzer *>( analyzer_ )->analyze( event );
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

