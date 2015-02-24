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
			void ApplyCorrections(flashgg::Photon & y, shared_ptr<BaseSystMethods<flashgg::Photon, int> > CorrToShift, int syst_shift);
	  
			edm::EDGetTokenT<View<flashgg::Photon> > PhotonToken_; 	

			std::vector<shared_ptr<BaseSystMethods<flashgg::Photon, int> > > Corrections_;
			std::vector<std::vector<int> > sigmas_;

			std::vector<std::string> collectionLabelsNonCentral_;  
	};

	PhotonSystematicProducer::PhotonSystematicProducer(const ParameterSet & iConfig) :
		PhotonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag>("PhotonTag", InputTag("flashggPhotons"))))
	{
		produces<std::vector<flashgg::Photon> >(); // Central value
		std::vector<edm::ParameterSet> vpset = iConfig.getParameter<std::vector<edm::ParameterSet> >("SystMethods");

		Corrections_.resize(vpset.size());

		unsigned int ipset = 0;
		for( const auto& pset : vpset ) {
			std::string methodName = pset.getParameter<string>("MethodName");

			// If nsigmas ends up being empty for a given corrector,
                        // the central operation will still be applied to all collections
			std::vector<int> nsigmas = pset.getParameter<vector<int> >("NSigmas");

			// Remove 0 values, because we always have a central collection
			nsigmas.erase(std::remove(nsigmas.begin(), nsigmas.end(), 0), nsigmas.end()); 

			sigmas_.push_back(nsigmas);
			Corrections_.at(ipset).reset(FlashggSystematicPhotonMethodsFactory::get()->create(methodName,pset));
			for (const auto &sig : sigmas_.at(ipset)) {
				std::string collection_label = Corrections_.at(ipset)->shiftLabel(sig);  
				produces<vector<flashgg::Photon> >(collection_label);
				collectionLabelsNonCentral_.push_back(collection_label); // 2N elements, current code gets labels right only if loops are consistent
			}
			ipset++;
		}
	}
  
	///fucntion takes in the current corection one is looping through and compares with its own internal loop, given that this will be within the corr and sys loop it takes care of the 2n+1 collection number////
	void PhotonSystematicProducer::ApplyCorrections(flashgg::Photon & y, shared_ptr<BaseSystMethods<flashgg::Photon, int> > CorrToShift, int syst_shift)
	{
		for( unsigned int shift = 0; shift < Corrections_.size(); shift++){
			if(CorrToShift == Corrections_.at(shift)){
				Corrections_.at(shift)->applyCorrection(y,syst_shift);	
			}else{
				Corrections_.at(shift)->applyCorrection(y,0);
			}
		}
	}

	void PhotonSystematicProducer::produce( Event & evt, const EventSetup & ) {

		Handle<View<flashgg::Photon> > photons;
		evt.getByToken(PhotonToken_,photons);
		const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();
		
		// Build central collection
		auto_ptr<vector<flashgg::Photon> > centralPhotonColl(new vector<flashgg::Photon>);
		for(unsigned int i = 0; i < photonPointers.size(); i++) {
			flashgg::Photon pho = flashgg::Photon(*photonPointers[i]);
			ApplyCorrections(pho, nullptr, 0);
			centralPhotonColl->push_back(pho);
		}
		evt.put(centralPhotonColl); // put central collection in event
		      
		// build 2N shifted collections
		// A dynamically allocated array of auto_ptrs may be a bit "unsafe" to maintain,
		// although I think I have done it correctly - the delete[] statement below is vital
		// Problem: vector<auto_ptr> is not allowed
		// Possible alternate solutions: map, multimap, vector<unique_ptr> + std::move ??
		std::auto_ptr<std::vector<flashgg::Photon> >* all_shifted_collections;
		all_shifted_collections= new std::auto_ptr<std::vector<flashgg::Photon> >[collectionLabelsNonCentral_.size()];
		for (unsigned int ncoll = 0 ; ncoll < collectionLabelsNonCentral_.size() ; ncoll++) {
		  all_shifted_collections[ncoll].reset(new std::vector<flashgg::Photon>);
		}
		for(unsigned int i = 0; i < photonPointers.size(); i++) {
			unsigned int ncoll = 0;  
			for (unsigned int ncorr = 0 ; ncorr < Corrections_.size() ; ncorr++) {
				for (  const auto & sig : sigmas_.at(ncorr)) {
					flashgg::Photon pho = flashgg::Photon(*photonPointers[i]);
					ApplyCorrections(pho, Corrections_.at(ncorr), sig);
					all_shifted_collections[ncoll]->push_back(pho);
					ncoll++;
				}
			}
		}
		
		// Put shifted collections in event
		for (unsigned int ncoll = 0 ; ncoll < collectionLabelsNonCentral_.size() ; ncoll++) {
			evt.put(all_shifted_collections[ncoll],collectionLabelsNonCentral_[ncoll]);
		}

		// See note above about array of auto_ptr 
		delete[] all_shifted_collections;
		
	} // end of event
}


typedef flashgg::PhotonSystematicProducer FlashggPhotonSystematicProducer;
DEFINE_FWK_MODULE(FlashggPhotonSystematicProducer);
