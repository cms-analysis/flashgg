#ifndef FLASHgg_TTHleptonicTag_h
#define FLASHgg_TTHleptonicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

namespace flashgg {

	class TTHleptonicTag: public DiPhotonTagBase {
		public:
			TTHleptonicTag();
                        TTHleptonicTag(edm::Ptr<DiPhotonCandidate>);
			~TTHleptonicTag();

                const edm::PtrVector<pat::Muon> getMuons() const { return Muons_;} 
		const edm::PtrVector<Jet> getJets() const { return Jets_;}
		const edm::PtrVector<Jet> getBJets() const { return BJets_;}
		const edm::Ptr<DiPhotonCandidate> getDiPhotonCandidate() const { return theDiPhotonCandidate_;} 

		void setJets(edm::PtrVector<Jet> Jets) { Jets_ = Jets; }
		void setBJets(edm::PtrVector<Jet> BJets)  { BJets_ = BJets;}
		void setMuons(edm::PtrVector<pat::Muon> Muons) {Muons_ = Muons;}


                private:
		edm::Ptr<DiPhotonCandidate> theDiPhotonCandidate_;
                edm::PtrVector<pat::Muon> Muons_;  
		edm::PtrVector<Jet> Jets_;
		edm::PtrVector<Jet> BJets_;
	};
} 

#endif 

