#ifndef FLASHgg_TTHhadronicTag_h
#define FLASHgg_TTHhadronicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


namespace flashgg {

	class TTHhadronicTag: public DiPhotonTagBase {
		public:
			TTHhadronicTag();
                        TTHhadronicTag(edm::Ptr<flashgg::Jet>);
			~TTHhadronicTag();

                const edm::Ptr<flashgg::Jet> getBJet() const { return theBJet_;} 

                private:
                edm::Ptr<flashgg::Jet> theBJet_;  
	};
} 

#endif 

