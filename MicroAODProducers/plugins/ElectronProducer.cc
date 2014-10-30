// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
//
// class declaration
//

using namespace std;
using namespace edm;
namespace flashgg {

	class ElectronProducer : public edm::EDProducer {
		public:
			ElectronProducer(const edm::ParameterSet&);
			~ElectronProducer();

		private:
			void produce(edm::Event&, const edm::EventSetup&);

			// ----------member data ---------------------------
			bool verbose_;
			edm::EDGetTokenT<View<pat::Electron> > electronToken_;
			edm::EDGetTokenT<View<reco::Vertex> > vertexToken_;
			float _Rho;
			edm::InputTag rhoFixedGrid_;

			string method_;
			vector<string> mvaWeightFiles_;


			EGammaMvaEleEstimator* mvaID_;

	};

	//
	// constants, enums and typedefs
	//

	//
	// static data member definitions
	//

	//
	// constructors and destructor
	//
	ElectronProducer::ElectronProducer(const ParameterSet& iConfig): 
		electronToken_(consumes<View<pat::Electron> >(iConfig.getUntrackedParameter<InputTag>("electronTag",InputTag("slimmedElectrons")))),
		vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag>("vertexTag",InputTag("offlineSlimmedPrimaryVertices"))))

	{		
		verbose_ = iConfig.getUntrackedParameter<bool>("verbose", false);
		method_ = iConfig.getParameter<string>("method");
		std::vector<string> fpMvaWeightFiles = iConfig.getParameter<std::vector<std::string> >("mvaWeightFile");
		//	eventrhoToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("Rho"));
		rhoFixedGrid_  = iConfig.getParameter<edm::InputTag>("rhoFixedGridCollection");


		produces<edm::ValueMap<float> >();

		mvaID_ = new EGammaMvaEleEstimator();

		EGammaMvaEleEstimator::MVAType type_;


		type_ = EGammaMvaEleEstimator::kNonTrig;


		bool manualCat_ = true;

		string path_mvaWeightFileEleID;
		for(unsigned ifile=0 ; ifile < fpMvaWeightFiles.size() ; ++ifile) {
			path_mvaWeightFileEleID = edm::FileInPath ( fpMvaWeightFiles[ifile].c_str() ).fullPath();
			mvaWeightFiles_.push_back(path_mvaWeightFileEleID);
		}

		mvaID_->initialize(method_, type_, manualCat_, mvaWeightFiles_);

	}


	ElectronProducer::~ElectronProducer()
	{

		// do anything here that needs to be done at desctruction time
		// (e.g. close files, deallocate resources etc.)
		produces<vector<flashgg::Electron> >();	
	}


	//
	// member functions
	//

	// ------------ method called on each new Event  ------------
	void ElectronProducer::produce(Event& evt, const EventSetup& ) {
		using namespace edm;

		Handle<View<pat::Electron> >  pelectrons;
		evt.getByToken(electronToken_,pelectrons);
		const PtrVector<pat::Electron> pelectronPointers = pelectrons->ptrVector();

		Handle<View<reco::Vertex> >  vtxs;
		evt.getByToken(vertexToken_,vtxs);
		const PtrVector<reco::Vertex> vertexPointers = vtxs->ptrVector();


		_Rho=0;
		Handle<double> rhoHandle;        // the old way for now...move to getbytoken?
		evt.getByLabel(rhoFixedGrid_, rhoHandle );

		//		edm::Handle<float> rhoPtr;
		//const edm::InputTag eventrhoToken_("kt6PFJets", "rho");
		//		evt.getByToken(eventrhoToken_,rhoPtr);
		//		_Rho=*rhoPtr;

		auto_ptr<vector<flashgg::Electron> > elecColl(new vector<flashgg::Electron>);


		for ( unsigned int elecIndex =0; elecIndex < pelectronPointers.size(); elecIndex++ ) {
			flashgg::Electron felec = flashgg::Electron(*pelectronPointers[elecIndex]);
			double nontrigmva_ = -999999;

			nontrigmva_ = mvaID_->mvaValue( *pelectronPointers[elecIndex], *vertexPointers[0], _Rho, verbose_);

			felec.nontrigmva = nontrigmva_;			

			elecColl->push_back(felec);

		}
		evt.put(elecColl);
	}
}
//define this as a plug-in
typedef flashgg::ElectronProducer FlashggElectronProducer;
DEFINE_FWK_MODULE(FlashggElectronProducer);
