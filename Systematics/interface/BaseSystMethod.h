#ifndef FLASHgg_BaseSystMethod_h
#define FLASHgg_BaseSystMethod_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "CLHEP/Random/RandomEngine.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

namespace flashgg {

    template <class flashgg_object, class param_var>
    class BaseSystMethod : public WeightedObject
    {

    public:

        BaseSystMethod( const edm::ParameterSet &conf ):
            _Name( conf.getParameter<std::string>( "MethodName" ) ),
            _Label( conf.getParameter<std::string>( "Label" ) ),
            _MakesWeight( false ),
            _RandomEngine( nullptr ),
            _ApplyCentralValue( conf.getParameter<bool>( "ApplyCentralValue" ) )
        { }
        BaseSystMethod() {};
        virtual ~BaseSystMethod() {};

        BaseSystMethod( const BaseSystMethod & ) = delete;
        BaseSystMethod &operator=( const BaseSystMethod & ) = delete;

        virtual void applyCorrection( flashgg_object &, param_var syst_value ) //main function for object-modified case//
        {
            throw cms::Exception( "NotImplemented" ) << " concrete classes need only implement one of applyCorrection or makeWeight - check class setup";
        }

        virtual float makeWeight( const flashgg_object &, param_var syst_value ) //main function for weight-making case//
        {
            throw cms::Exception( "NotImplemented" ) << " concrete classes need only implement one of applyCorrection or makeWeight - check class setup";
        }

        const std::string &name() const { return _Name; };
        const std::string &label() const { return _Label; };
        bool makesWeight() const { return _MakesWeight; }
        bool applyCentralValue() const { return _ApplyCentralValue; }

        virtual std::string shiftLabel( param_var syst_val ) const = 0;
        virtual void setJECUncertainty ( const JetCorrectorParameters & ) {
            throw cms::Exception( "NotImplemented" ) << " should be used for JEC only";
        } 
        virtual void setJEC( const JetCorrector*, const edm::Event &, const edm::EventSetup &  ) {
            throw cms::Exception( "NotImplemented" ) << " should be used for JEC only";
        }


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

    protected:
        void setMakesWeight( bool makes_weight ) { _MakesWeight = makes_weight; }

    private:
        const std::string _Name;
        const std::string _Label;
        bool _MakesWeight;
        CLHEP::HepRandomEngine *_RandomEngine;
        bool _ApplyCentralValue;
    };
}

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
//template <class T, class U> struct A {
//    typedef edmplugin::PluginFactory< flashgg::BaseSystMethod<T,U>* (const edm::ParameterSet & ) > FlashggSystematicMethodsFactory;
//}

//typedef template< class T, class U> edmplugin::PluginFactory< flashgg::BaseSystMethod<T,U>* (const edm::ParameterSet & ) > FlashggSystematicMethodsFactory;
template< class T, class U > using FlashggSystematicMethodsFactory = edmplugin::PluginFactory< flashgg::BaseSystMethod<T, U>* ( const edm::ParameterSet & ) >;
typedef FlashggSystematicMethodsFactory<flashgg::Photon, int> FlashggSystematicPhotonMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::DiPhotonCandidate, int> FlashggSystematicDiPhotonMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::Photon, std::pair<int, int> > FlashggSystematicPhotonMethodsFactory2D;
typedef FlashggSystematicMethodsFactory<flashgg::DiPhotonCandidate, std::pair<int, int> > FlashggSystematicDiPhotonMethodsFactory2D;
typedef FlashggSystematicMethodsFactory<flashgg::Electron, int> FlashggSystematicElectronMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::Muon, int> FlashggSystematicMuonMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::Electron, std::pair<int, int> > FlashggSystematicElectronMethodsFactory2D;
typedef FlashggSystematicMethodsFactory<flashgg::Muon, std::pair<int, int> > FlashggSystematicMuonMethodsFactory2D;
typedef FlashggSystematicMethodsFactory<flashgg::Jet, int > FlashggSystematicJetMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::Jet, std::pair<int,int> > FlashggSystematicJetMethodsFactory2D;
typedef FlashggSystematicMethodsFactory<flashgg::DiPhotonTagBase, int> FlashggSystematicTagMethodsFactory;
typedef FlashggSystematicMethodsFactory<flashgg::DiPhotonTagBase, std::pair<int,int> > FlashggSystematicTagMethodsFactory2D;
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

