#ifndef FLASHgg_TTHleptonicTag_h
#define FLASHgg_TTHleptonicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/MicroAODFormats/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

namespace flashgg {

	class TTHleptonicTag: public DiPhotonTagBase {
		public:
			TTHleptonicTag();
                        TTHleptonicTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
			TTHleptonicTag(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult);

			~TTHleptonicTag();

                	const edm::PtrVector<pat::Muon> getMuons() const { return Muons_;} 
			const edm::PtrVector<flashgg::Electron> getElectrons() const {return Electrons_;}
			const edm::PtrVector<Jet> getJets() const { return Jets_;}
			const edm::PtrVector<Jet> getBJets() const { return BJets_;}

			void setJets(edm::PtrVector<Jet> Jets) { Jets_ = Jets; }
			void setBJets(edm::PtrVector<Jet> BJets)  { BJets_ = BJets;}
			void setMuons(edm::PtrVector<pat::Muon> Muons) {Muons_ = Muons;}
			void setElectrons(edm::PtrVector<Electron> Electrons) {Electrons_ = Electrons;}

			TTHleptonicTag* clone() const;

                private:
                	edm::PtrVector<pat::Muon> Muons_;  
			edm::PtrVector<Electron> Electrons_;
			edm::PtrVector<Jet> Jets_;
			edm::PtrVector<Jet> BJets_;
  	};
} 

#endif 

