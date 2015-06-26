#ifndef FLASHgg_BaseSystMethod_h
#define FLASHgg_BaseSystMethod_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CLHEP/Random/RandomEngine.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

namespace flashgg {

    template <class flashgg_object, class param_var>
    class BaseSystMethod
    {

    public:

        BaseSystMethod( const edm::ParameterSet &conf ):
            _Name( conf.getParameter<std::string>( "MethodName" ) ),
            _Label( conf.getParameter<std::string>( "Label" ) ),
            _RandomEngine( nullptr )
        { }
        BaseSystMethod() {};
        virtual ~BaseSystMethod() {};

        BaseSystMethod( const BaseSystMethod & ) = delete;
        BaseSystMethod &operator=( const BaseSystMethod & ) = delete;

        virtual void applyCorrection( flashgg_object &, param_var syst_value ) = 0; //main function//

        const std::string &name() const { return _Name; };
        const std::string &label() const { return _Label; };

        virtual std::string shiftLabel( param_var syst_val ) const = 0;

        virtual void setRandomEngine( CLHEP::HepRandomEngine &eng )
        {
            //            std::cout << " BaseSystMethod::setRandomEngine - " << _Name << std::endl;
            _RandomEngine = &eng;
        }

        CLHEP::HepRandomEngine *RandomEngine() const
        {
            if( _RandomEngine == nullptr ) {
                throw cms::Exception( "Configuration" ) << "Tried to access BaseSystMethod::RandomEngine() when it is not set";
            }
            return _RandomEngine;
        }

    private:
        const std::string _Name;
        const std::string _Label;
        CLHEP::HepRandomEngine *_RandomEngine;

    };
}

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
//template <class T, class U> struct A {
//    typedef edmplugin::PluginFactory< flashgg::BaseSystMethod<T,U>* (const edm::ParameterSet & ) > FlashggSystematicMethodsFactory;
//}

//typedef template< class T, class U> edmplugin::PluginFactory< flashgg::BaseSystMethod<T,U>* (const edm::ParameterSet & ) > FlashggSystematicMethodsFactory;
template< class T, class U > using FlashggSystematicMethodsFactory = edmplugin::PluginFactory< flashgg::BaseSystMethod<T, U>* ( const edm::ParameterSet & ) >;
typedef FlashggSystematicMethodsFactory<flashgg::Photon, int> FlashggSystematicPhotonMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::DiPhotonCandidate, int> FlashggSystematicDiPhotonMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::Photon, std::pair<int, int> > FlashggSystematicPhotonMethodsFactory2D;
typedef FlashggSystematicMethodsFactory<flashgg::DiPhotonCandidate, std::pair<int, int> > FlashggSystematicDiPhotonMethodsFactory2D;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethod<flashgg::Photon, int>* ( const edm::ParameterSet & ) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethod<flashgg::Photon,float>* ( const edm::ParameterSet&) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethod<flashgg::DiPhotonCandidate>* ( const edm::ParameterSet&) > FlashggSystematicDiPhotonMethodsFactory;

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

