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
//#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
//#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
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
//using namespace reco;
class ElectronProducer : public edm::EDProducer {
	public:
		ElectronProducer(const edm::ParameterSet&);
		~ElectronProducer();

	private:
		void produce(edm::Event&, const edm::EventSetup&);

		// ----------member data ---------------------------
		bool verbose_;
		edm::EDGetTokenT<pat::Electron> electronToken_;
		edm::EDGetTokenT<flashgg::DiPhotonCandidate> diPhotonToken_;
		float _Rho;
		edm::EDGetTokenT<float> eventrhoToken_;
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
ElectronProducer::ElectronProducer(const edm::ParameterSet& iConfig) {
	verbose_ = iConfig.getUntrackedParameter<bool>("verbose", false);
	electronToken_ = consumes<pat::Electron>(iConfig.getParameter<edm::InputTag>("electronTag"));
	diPhotonToken_ = consumes<flashgg::DiPhotonCandidate>(iConfig.getParameter<edm::InputTag>("diphoTag"));
	method_ = iConfig.getParameter<string>("method");
	std::vector<string> fpMvaWeightFiles = iConfig.getParameter<std::vector<std::string> >("mvaWeightFile");
	eventrhoToken_ = consumes<float>(iConfig.getParameter<edm::InputTag>("Rho"));

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
void ElectronProducer::produce(edm::Event& evt, const edm::EventSetup& Setup) {
	using namespace edm;

	Handle<pat::Electron>  pelectrons;
	evt.getByToken(electronToken_,pelectrons);
	const pat::Electron electronCandidates = (*pelectrons.product());

	Handle<View<flashgg::DiPhotonCandidate> > diphotons;
	evt.getByToken(diPhotonToken_,diphotons);
	const PtrVector<flashgg::DiPhotonCandidate> diphoPointers = diphotons->ptrVector();

	_Rho=0;
	edm::Handle<float> rhoPtr;
	//const edm::InputTag eventrhoToken_("kt6PFJets", "rho");
	evt.getByToken(eventrhoToken_,rhoPtr);
	_Rho=*rhoPtr;

	auto_ptr<vector<flashgg::Electron> > elecColl(new vector<flashgg::Electron>);

	for(unsigned int diphoIndex =0; diphoIndex < diphoPointers.size(); diphoIndex++ ){	

		for ( pat::Electron::const_iterator egIter = electronCandidates.begin(); egIter != electronCandidates.end(); egIter++ ) {
			flashgg::Electron felec = flashgg::Electron(*egIter);
			double nontrigmva_ = -999999;

			nontrigmva_ = mvaID_->mvaValue( *egIter, diphoPointers[diphoIndex]->getVertex() , _Rho, verbose_);
			elecColl->push_back(felec);

		}
	}
	evt.put(out);

}

//define this as a plug-in
typedef flashgg::ElectronProducer FlashggElectronProducer;
DEFINE_FWK_MODULE(FlashggElectronProducer);
