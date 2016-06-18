#ifndef PluggableFWLiteAnalyzer_h
#define PluggableFWLiteAnalyzer_h

#include <map>
#include <string>

#include "PhysicsTools/UtilAlgos/interface/FWLiteAnalyzerWrapper.h"

#include <iostream>

#define PLUGGABLE_ANALYZER(NAME)  char NAME ## name[] = #NAME; \
	typedef AnalyzerFactoryImpl<NAME,NAME ## name> NAME ## Factory; \
	NAME ## Factory __ ## NAME ## FactoryInstance;

namespace flashgg {
    class AnalyzerFactory
    {
    public:
        virtual void *allocate( const edm::ParameterSet &cfg, TFileDirectory &fileService ) const = 0;

        static const AnalyzerFactory *allocator( const std::string &name );

    protected:
        static void registerClass( const std::string &name, AnalyzerFactory *factory );

    private:
        static std::map<std::string, AnalyzerFactory *> &getRegister();
    };

    template<class T, const char *name>
    class AnalyzerFactoryImpl : public AnalyzerFactory
    {
    public:
        AnalyzerFactoryImpl();

        void *allocate( const edm::ParameterSet &cfg, TFileDirectory &fileService ) const
        {
            return new T( cfg, fileService );
        }
    };

    template<class T, const char *name>
    AnalyzerFactoryImpl<T, name>::AnalyzerFactoryImpl()
    {
        AnalyzerFactory::registerClass( name, this );
    }

    class PluggableAnalyzer
    {
    public:
        PluggableAnalyzer( const edm::ParameterSet &cfg, TFileDirectory &fileService );

        /// everything that needs to be done before the event loop
        void beginJob();
        /// everything that needs to be done after the event loop
        void endJob();
        /// everything that needs to be done during the event loop
        void analyze( const edm::EventBase &event );

    private:
        void *analyzer_;
    };

    namespace fwlite {
        typedef ::fwlite::AnalyzerWrapper<PluggableAnalyzer> PluggableAnalyzer;
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

