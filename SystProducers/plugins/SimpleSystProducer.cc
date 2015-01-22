#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "flashgg/SystAlgos/interface/BaseSystMethods.h"
//#include "flashgg/SystAlgos/interface/ApplySystVariations.h"

using namespace edm;
using namespace std;

namespace flashgg {

	class PhotonSystematicProducer : public edm::EDProducer {
		public:

			PhotonSystematicProducer(const edm::ParameterSet&);

		private:

			void produce(edm::Event&, const edm::EventSetup&) override;

			edm::EDGetTokenT<View<flashgg::Photon> > PhotonToken_; 	

			std::vector<shared_ptr<BaseSystMethods> > Corrections_;
	};

	PhotonSystematicProducer::PhotonSystematicProducer(const ParameterSet & iConfig) :

		PhotonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag>("PhotonTag", InputTag("flashggPhotons"))))
		{
			std::vector<edm::ParameterSet> vpset = iConfig.getParameter<std::vector<edm::ParameterSet> >("SystMethodNames");

			int size = vpset.size();

			std::vector<std::string> Names;

			for(std::vector<edm::ParameterSet>::const_iterator it = vpset.begin(); it < vpset.end() ; it++){

				std::string Name =  it->getParameter<std::string>("MethodName");

				Names.push_back(Name);	

			}


			for(int s = 0; s < size ; s++){

				std::string& MethodName = Names.at(s); 

				Corrections_.push_back(NULL);

				Corrections_.at(s).reset(FlashggSystematicPhotonMethodsFactory::get()->create(MethodName,vpset[s]));

			}


			for(int sys_size = 0; sys_size<3;sys_size++){
				for( int g = 0; g<size; g++){

					if(g == 1 && sys_size == 1)continue;
					std::cout << Form("%i_%i",sys_size,g) << std::endl;
					produces<vector<flashgg::Photon> >(Form("collection-%i-%i",sys_size,g));

				}
			}
		}

	///fucntion takes in the current corection one is looping through and compares with its own internal loop, given that this will be within the corr and sys loop it takes care of the 2n+1 collection number////

	void ApplyCorrection(flashgg::Photon & y, std::vector<shared_ptr<BaseSystMethods> > Corr, shared_ptr<BaseSystMethods> CorrToShift, double syst_shift, int corr_size){


		for( int shift = 0; shift < corr_size; shift++){


			if(CorrToShift == Corr.at(shift)){

				CorrToShift->applyCorrection(y,syst_shift);	

			}else{

				CorrToShift->applyCorrection(y,0.);

			}
		}

	}

	void PhotonSystematicProducer::produce( Event & evt, const EventSetup & ) {

		Handle<View<flashgg::Photon> > photons;
		evt.getByToken(PhotonToken_,photons);
		const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();

		int num_corr = Corrections_.size();

		std::vector<int> syst_shift = {-1,0,1};

		std::vector<flashgg::Photon> ** collections;////////create 2D array where rows are the # of systematci variations and colums are the # of corrections

		collections = new std::vector<flashgg::Photon> * [3];//width

		for(int l = 0; l<3; l++){

			collections[l] = new std::vector<flashgg::Photon>[num_corr];//height
		}

		std::cout << sizeof(collections) << " " << sizeof(collections[0]) << std::endl;

		int photon_size = photonPointers.size();

		

		for(int sys = 0; sys < 3 ; sys++){

			std::cout << "sys iteration " << sys << std::endl;
			std::cout << "Number of photons " << photon_size << std::endl;

			for(int i = 0; i < photon_size; i++){
				
				std::cout  << "photon #  "  << i << std::endl;

				for(int corr = 0; corr < num_corr; corr++){

					if(sys == 1 && corr == 1)continue;	

					if(sys==0){
						flashgg::Photon pho = *photonPointers[i];
						std::cout << "corrections name  " << Corrections_.at(corr)->name() << std::endl;
						Corrections_.at(corr)->applyCorrection(pho,syst_shift.at(sys));
						collections[sys][corr].push_back(pho);
					}else{
						std::cout << "correction index " << corr << std::endl;
						flashgg::Photon pho = *photonPointers[i];
						std::cout << "corrections name  " << Corrections_.at(corr)->name() << std::endl;
						ApplyCorrection(pho,Corrections_,Corrections_.at(corr),syst_shift.at(sys),num_corr);
						collections[sys][corr].push_back(pho);
						
					}
				}	

			}
		}
			
		

		for(int j = 0; j<3;j++){	
			for(int i = 0; i<num_corr; i++){
				if(j==1 && i==1)continue;
				std::cout << Form("%i-%i",j,i) << std::endl;
				auto_ptr<std::vector<flashgg::Photon> > p(new std::vector<flashgg::Photon>);
				std::cout << "photon momentum before reseting " << collections[j][i].at(0).pt() << std::endl;
				std::cout << "photon momentum before reseting " << collections[j][i].at(1).pt() << std::endl;
				std::cout << "photon momentum before reseting " << collections[j][i].at(2).pt() << std::endl;
				std::cout << "photon momentum before reseting " << collections[j][i].at(3).pt() << std::endl;
				p.reset(&collections[j][i]);//transfers the owner ship of the pointer  "p"  //
				//p = collections[i][j];	
				std::cout << "momentum after reseting the pointer Photon 1  " << (*p).at(0).pt() << std::endl;
				std::cout << "momentum after reseting the pointer Photon 2  " << (*p).at(1).pt() << std::endl;
				std::cout << "momentum after reseting the pointer Photon 3  " << (*p).at(2).pt() << std::endl;
				std::cout << "momentum after reseting the pointer Photon 4  " << (*p).at(3).pt() << std::endl;
		
				evt.put(p,Form("collection-%i-%i",j,i));////ran out of ideas on how exactly to dynamically name the collections, this doesn't compile///
				std::cout << "worked put step" << std::endl;
			}
				//std::cout << "worked after pu loop " << std::endl;
		}
		//std::cout << "end of produce function" << std::endl;
	}

}


typedef flashgg::PhotonSystematicProducer FlashggPhotonSystematicProducer;
DEFINE_FWK_MODULE(FlashggPhotonSystematicProducer);
