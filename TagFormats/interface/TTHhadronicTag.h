#ifndef FLASHgg_TTHhadronicTag_h
#define FLASHgg_TTHhadronicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


namespace flashgg {

	class TTHhadronicTag: public DiPhotonTagBase {
		public:
			TTHhadronicTag();
			//TTHhadronicTag(edm::Ptr<flashgg::Jet>);
			TTHhadronicTag(edm::Ptr<flashgg::Jet>);
			//const flashgg::Jet* BJet1() const;
			//const flashgg::Jet* BJet2() const;		 
			~TTHhadronicTag();

			const edm::Ptr<flashgg::Jet> getBjet() const {return theBjet_; }
			float getNBLoose() const {return btagloose;}
			float getNBMedium() const {return btagmedium;}

			float btagloose;
			float btagmedium;

		private:
		
                       	edm::Ptr<flashgg::Jet> theBjet_;
	};
} 

#endif 























