#ifndef FLASHgg_TTHhadronicTag_h
#define FLASHgg_TTHhadronicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


namespace flashgg {

	class TTHhadronicTag: public DiPhotonTagBase {
		public:
			TTHhadronicTag();
			TTHhadronicTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>,std::vector<edm::Ptr<flashgg::Jet> >,std::vector<edm::Ptr<flashgg::Jet> >);
			TTHhadronicTag(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult,std::vector<edm::Ptr<flashgg::Jet> >,std::vector<edm::Ptr<flashgg::Jet> >);
			TTHhadronicTag* clone() const;
			~TTHhadronicTag();

			const std::vector<edm::Ptr<flashgg::Jet> > getJetVector() const {return theJetVec_;}
			const std::vector<edm::Ptr<flashgg::Jet> > getBJetVector() const {return theBJetVec_; }
			int getNBLoose() const {return Nbtagloose_;}
			int getNBMedium() const {return Nbtagmedium_;}
			void setNBLoose(int nb) { Nbtagloose_ = nb; }
			void setNBMedium(int nb) { Nbtagmedium_ = nb; }

		private:
			int Nbtagloose_;
			int Nbtagmedium_;
			std::vector<edm::Ptr<flashgg::Jet> > theJetVec_;
			std::vector<edm::Ptr<flashgg::Jet> > theBJetVec_;
	};
} 

#endif 























