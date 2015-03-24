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

                        const std::vector<edm::Ptr<pat::Muon> > muons() const { return Muons_;} 
			const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
			const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
			const std::vector<edm::Ptr<Jet> > bJets() const { return BJets_;}

			void setJets(std::vector<edm::Ptr<Jet> > Jets) { Jets_ = Jets; }
			void setBJets(std::vector<edm::Ptr<Jet> > BJets)  { BJets_ = BJets;}
			void setMuons(std::vector<edm::Ptr<pat::Muon> > Muons) {Muons_ = Muons;}
			void setElectrons(std::vector<edm::Ptr<Electron> > Electrons) {Electrons_ = Electrons;}

			TTHLeptonicTag* clone() const;

                private:
                	std::vector<edm::Ptr<pat::Muon> > Muons_;  
			std::vector<edm::Ptr<Electron> > Electrons_;
			std::vector<edm::Ptr<Jet> > Jets_;
			std::vector<edm::Ptr<Jet> > BJets_;
  	};
} 

#endif 
