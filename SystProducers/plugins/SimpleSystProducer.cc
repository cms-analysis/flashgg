#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "flashgg/SystAlgos/interface/BaseSystMethods.h"

using namespace edm;
using namespace std;

namespace flashgg {

	class PhotonSystematicProducer : public edm::EDProducer {
		public:

			PhotonSystematicProducer(const edm::ParameterSet&);

		private:

			void produce(edm::Event&, const edm::EventSetup&) override;

			edm::EDGetTokenT<View<flashgg::Photon> > PhotonToken_; 	

			unique_ptr<BaseSystMethods> systematicMethod_;
	};

	PhotonSystematicProducer::PhotonSystematicProducer(const ParameterSet & iConfig) :

		PhotonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag>("PhotonTag", InputTag("flashggPhotons"))))
		{
			const std::string& SystematicMethod = iConfig.getParameter<std::string>("SystMethodName");
			systematicMethod_.reset(FlashggSystematicMethodsFactory::get()->create(SystematicMethod,iConfig));
			produces<vector<flashgg::Photon> >("A");
			produces<vector<flashgg::Photon> >("B");

		}	

	void PhotonSystematicProducer::produce( Event & evt, const EventSetup & ) {

		Handle<View<flashgg::Photon> > photons;
		evt.getByToken(PhotonToken_,photons);
		const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();	
	
		auto_ptr<vector<flashgg::Photon> > correctionA(new vector<flashgg::Photon>);
		auto_ptr<vector<flashgg::Photon> > correctionB(new vector<flashgg::Photon>);

		for(unsigned int i = 0; i < photonPointers.size(); i++){

			flashgg::Photon pho = *photonPointers[i]; 		

			systematicMethod_->applySystematic(pho,0.5);				
			correctionA->push_back(pho);
			systematicMethod_->applySystematic(pho,2.0);				
			correctionB->push_back(pho);	
		}

		evt.put(correctionA,"A");
		evt.put(correctionB,"B");
	}
}


typedef flashgg::PhotonSystematicProducer FlashggPhotonSystematicProducer;
DEFINE_FWK_MODULE(FlashggPhotonSystematicProducer);
