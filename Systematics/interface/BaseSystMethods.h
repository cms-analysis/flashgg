#ifndef FLASHgg_BaseSystMethods_h
#define FLASHgg_BaseSystMethods_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

    template <class flashgg_object, class param_var>
    class BaseSystMethods
    {

    public:

        BaseSystMethods( const edm::ParameterSet &conf ):
            _Name( conf.getParameter<std::string>( "MethodName" ) ),
            _Label( conf.getParameter<std::string>( "Label" ) )  { }
        BaseSystMethods() {};
        virtual ~BaseSystMethods() {};

        BaseSystMethods( const BaseSystMethods & ) = delete;
        BaseSystMethods &operator=( const BaseSystMethods & ) = delete;

        virtual void applyCorrection( flashgg_object &, param_var syst_value ) = 0; //main function//

        const std::string &name() const { return _Name; };
        const std::string &label() const { return _Label; };

        virtual std::string shiftLabel( param_var syst_val ) = 0;

    private:
        const std::string _Name;
        const std::string _Label;
        //typename flashgg_object object;


    };
}
#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::Photon, int>* ( const edm::ParameterSet & ) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::Photon,float>* ( const edm::ParameterSet&) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::DiPhotonCandidate>* ( const edm::ParameterSet&) > FlashggSystematicDiPhotonMethodsFactory;

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

