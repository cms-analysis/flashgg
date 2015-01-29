#include "flashgg/SystAlgos/interface/BaseSystMethods.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"

namespace flashgg {

	class SimplePhotonSmear: public BaseSystMethods {


		public:
			SimplePhotonSmear(const edm::ParameterSet& conf):
				BaseSystMethods(conf){}
		//	SimplePhotonSmear();
			
		
			void applySystematic( flashgg::Photon & y, float syst_shift) override;
	};

	void SimplePhotonSmear::applySystematic( flashgg::Photon & y, float syst_shift=0.5)
	{
		y.updateEnergy("smearedEnergy",y.energy()*syst_shift);
	}

}

DEFINE_EDM_PLUGIN(FlashggSystematicMethodsFactory,
		flashgg::SimplePhotonSmear,
		"FlashggSimplePhotonSmear");	
