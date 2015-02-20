#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/TTHhadronicTag.h"
#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>

using namespace std;
using namespace edm;

namespace flashgg {

	class TTHhadronicTagProducer : public EDProducer {

		public:
			TTHhadronicTagProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<View<Jet> > thejetToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
	};

	TTHhadronicTagProducer::TTHhadronicTagProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		thejetToken_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("TTHJetTag",InputTag("flashggJets")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag>("MVAResultTag",InputTag("flashggDiPhotonMVA"))))



	{
		produces<vector<TTHhadronicTag> >();
	}

	void TTHhadronicTagProducer::produce( Event & evt, const EventSetup & ){

		Handle<View<flashgg::Jet> > theJets;
		evt.getByToken(thejetToken_,theJets);
		const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();

		Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
		evt.getByToken(diPhotonToken_,diPhotons);
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

		Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
		evt.getByToken(mvaResultToken_,mvaResults);
		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();


		std::auto_ptr<vector<TTHhadronicTag> > tthhtags(new vector<TTHhadronicTag>);


		for(unsigned int diphoIndex = 0; diphoIndex < diPhotonPointers.size(); diphoIndex++ ){

			int jetcount = 0; 
			int njets_btagloose = 0;
			int njets_btagmedium = 0;

			std::vector<edm::Ptr<flashgg::Jet> > JetVect;
			std::vector<edm::Ptr<flashgg::Jet> > BJetVect;	

			edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[diphoIndex];
			edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[diphoIndex]; 

			if(dipho->leadingPhoton()->pt() < (60*(dipho->mass()))/120. && dipho->subLeadingPhoton()->pt() < 25. && dipho->subLeadingPhoton()->pt() < 33.) continue;	
			if(mvares->getMVAValue() < .2)continue;

			for (unsigned int jetIndex =0; jetIndex < jetPointers.size() ; jetIndex++){
				edm::Ptr<flashgg::Jet> thejet = jetPointers[jetIndex];
				if(fabs(thejet->eta()) > 2.4) continue;

				float bDiscriminatorValue = 0;

				float dEtaLead = thejet->eta() - dipho->leadingPhoton()->eta();
				float dEtaSublead = thejet->eta() - dipho->subLeadingPhoton()->eta();

				float dPhiLead = deltaPhi(thejet->phi(),dipho->leadingPhoton()->phi());
				float dPhiSublead = deltaPhi(thejet->phi(),dipho->subLeadingPhoton()->phi());

				float dRJetPhoLead = sqrt(dEtaLead*dEtaLead + dPhiLead*dPhiLead);
				float dRJetPhoSubLead = sqrt(dEtaSublead*dEtaSublead + dPhiSublead*dPhiSublead);


				if(dRJetPhoLead < 0.5 || dRJetPhoSubLead < 0.5) continue;    
				if(thejet->pt() < 30.) continue;

				jetcount++;
				JetVect.push_back(thejet);

				bDiscriminatorValue = thejet->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags");

				if(bDiscriminatorValue>0.244)njets_btagloose++; 
				if(bDiscriminatorValue>0.679){ 

					njets_btagmedium++;
					JetVect.pop_back();
					BJetVect.push_back(thejet);
				}
			}

			if(njets_btagmedium > 0 && jetcount >= 5) {

				TTHhadronicTag tthhtags_obj(dipho,mvares,JetVect,BJetVect);
				tthhtags_obj.setNBLoose(njets_btagloose);
				tthhtags_obj.setNBMedium(njets_btagmedium);
				tthhtags_obj.setDiPhotonIndex(diphoIndex);
				tthhtags->push_back(tthhtags_obj);

			}
		}
		evt.put(tthhtags);
	}
}
typedef flashgg::TTHhadronicTagProducer FlashggTTHhadronicTagProducer;
DEFINE_FWK_MODULE(FlashggTTHhadronicTagProducer);





