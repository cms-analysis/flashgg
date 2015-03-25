#ifndef FLASHgg_VHLooseTag_h
#define FLASHgg_VHLooseTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/DataFormats/interface/Electron.h"

namespace flashgg {

	class VHLooseTag: public DiPhotonTagBase {
		public:
			VHLooseTag();
                        VHLooseTag(edm::Ptr<DiPhotonCandidate>,edm::Ptr<DiPhotonMVAResult>);
			VHLooseTag(edm::Ptr<DiPhotonCandidate>,DiPhotonMVAResult);
			~VHLooseTag();

                const std::vector<edm::Ptr<pat::Muon> > muons() const { return Muons_;}
		const std::vector<edm::Ptr<flashgg::Electron> > electrons() const {return Electrons_;}
		const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
		const std::vector<edm::Ptr<pat::MET> > met() const { return MET_;} 

		void setJets(std::vector<edm::Ptr<Jet> > Jets) { Jets_ = Jets; }
		void setMuons(std::vector<edm::Ptr<pat::Muon> >Muons) {Muons_ = Muons;}
		void setMET(std::vector<edm::Ptr<pat::MET> > MET) {MET_ = MET;}
		void setElectrons(std::vector<edm::Ptr<Electron> > Electrons) {Electrons_ = Electrons;}

		VHLooseTag* clone() const;

                private:
                std::vector<edm::Ptr<pat::Muon> > Muons_;  
		std::vector<edm::Ptr<Electron> > Electrons_;
		std::vector<edm::Ptr<Jet> > Jets_;
		std::vector<edm::Ptr<pat::MET> > MET_;
	};
} 

#endif 

