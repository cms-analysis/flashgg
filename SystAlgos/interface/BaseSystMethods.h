#ifndef FLASHgg_BaseSystMethods_h
#define FLASHgg_BaseSystMethods_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/Common/interface/PtrVector.h"


namespace flashgg {

	class BaseSystMethods {

		public:

			BaseSystMethods( const edm::ParameterSet& conf):
			_SystematicName( conf.getParameter<std::string>("SystMethodName") ){ }
			//BaseSystMethods();

			BaseSystMethods( const BaseSystMethods& ) = delete;
			BaseSystMethods& operator=( const BaseSystMethods&) = delete;

			typedef std::map<std::string,double> Parameters_Selector_Type;
			virtual void applySystematic( flashgg::Photon &, float syst_value)  = 0;

			const std::string& name() const { return _SystematicName; };

		private:
			const std::string _SystematicName;


	};
}
#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::BaseSystMethods* ( const edm::ParameterSet&) > FlashggSystematicMethodsFactory;

#endif
