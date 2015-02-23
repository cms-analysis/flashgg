#ifndef FLASHgg_BaseSystMethods_h
#define FLASHgg_BaseSystMethods_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

		template <class flashgg_object, class param_var>  
		class BaseSystMethods {

		public:

			BaseSystMethods( const edm::ParameterSet& conf):
			_SystematicName( conf.getParameter<std::string>("MethodName") ){ }
			//BaseSystMethods();
			//virtual ~BaseSystMethods();

			BaseSystMethods( const BaseSystMethods& ) = delete;
			BaseSystMethods& operator=( const BaseSystMethods&) = delete;

			virtual void applyCorrection( flashgg_object&, param_var syst_value) = 0;//main function//

			const std::string& name() const { return _SystematicName; };

		private:
			const std::string _SystematicName;
			//typename flashgg_object object;


	};
}
#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::Photon,int>* ( const edm::ParameterSet&) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::Photon,float>* ( const edm::ParameterSet&) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::DiPhotonCandidate>* ( const edm::ParameterSet&) > FlashggSystematicDiPhotonMethodsFactory;

#endif
