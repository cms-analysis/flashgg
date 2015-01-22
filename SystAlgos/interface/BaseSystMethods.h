#ifndef FLASHgg_BaseSystMethods_h
#define FLASHgg_BaseSystMethods_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

//	template <typename flashgg_object, typename... Args> 

		class BaseSystMethods {

		public:

			BaseSystMethods( const edm::ParameterSet& conf):
			_SystematicName( conf.getParameter<std::string>("MethodName") ){ }
			//BaseSystMethods();
			//virtual ~BaseSystMethods();

			BaseSystMethods( const BaseSystMethods& ) = delete;
			BaseSystMethods& operator=( const BaseSystMethods&) = delete;

			//typedef std::map<std::string,double> Parameters_Selector_Type;
		
			virtual void applyCorrection( flashgg::Photon&, float syst_value) = 0;//main function//
			//virtual void applyCorrection( flashgg_object& , Args ...) = 0;//main function//
//			virtual void applyCorrection( flashgg_object& object, float syst_value);//main function//

			const std::string& name() const { return _SystematicName; };

//		private:
			const std::string _SystematicName;
			//typename flashgg_object object;


	};
}
#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::BaseSystMethods* ( const edm::ParameterSet&) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::Photon,float>* ( const edm::ParameterSet&) > FlashggSystematicPhotonMethodsFactory;
//typedef edmplugin::PluginFactory< flashgg::BaseSystMethods<flashgg::DiPhotonCandidate>* ( const edm::ParameterSet&) > FlashggSystematicDiPhotonMethodsFactory;

#endif
