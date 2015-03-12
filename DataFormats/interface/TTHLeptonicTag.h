#ifndef FLASHgg_TTHLeptonicTag_h
#define FLASHgg_TTHLeptonicTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

	class TTHLeptonicTag: public DiPhotonTagBase {
		public:
			TTHLeptonicTag();
                        TTHLeptonicTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
			TTHLeptonicTag(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult);

			~TTHLeptonicTag();

                        const edm::PtrVector<pat::Muon> muons() const { return Muons_;} 
			const edm::PtrVector<flashgg::Electron> electrons() const {return Electrons_;}
			const edm::PtrVector<Jet> jets() const { return Jets_;}
			const edm::PtrVector<Jet> bJets() const { return BJets_;}

			void setJets(edm::PtrVector<Jet> Jets) { Jets_ = Jets; }
			void setBJets(edm::PtrVector<Jet> BJets)  { BJets_ = BJets;}
			void setMuons(edm::PtrVector<pat::Muon> Muons) {Muons_ = Muons;}
			void setElectrons(edm::PtrVector<Electron> Electrons) {Electrons_ = Electrons;}

			TTHLeptonicTag* clone() const;

                private:
                	edm::PtrVector<pat::Muon> Muons_;  
			edm::PtrVector<Electron> Electrons_;
			edm::PtrVector<Jet> Jets_;
			edm::PtrVector<Jet> BJets_;
  	};
} 

#endif 
