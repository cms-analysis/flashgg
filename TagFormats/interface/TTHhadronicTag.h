#ifndef FLASHgg_TTHhadronicTag_h
#define FLASHgg_TTHhadronicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


namespace flashgg {

	class TTHhadronicTag: public DiPhotonTagBase {
		public:
			TTHhadronicTag();
			TTHhadronicTag(std::vector<edm::Ptr<flashgg::Jet> >,std::vector<edm::Ptr<flashgg::Jet> >);
			~TTHhadronicTag();

			const std::vector<edm::Ptr<flashgg::Jet> > getJetVector() const {return theJetVec_;}
			const std::vector<edm::Ptr<flashgg::Jet> > getBJetVector() const {return theBJetVec_; }
			float getNBLoose() const {return btagloose;}
			float getNBMedium() const {return btagmedium;}

			float btagloose;
			float btagmedium;

		private:
			std::vector<edm::Ptr<flashgg::Jet> > theJetVec_;
			std::vector<edm::Ptr<flashgg::Jet> > theBJetVec_;
	};
} 

#endif 























