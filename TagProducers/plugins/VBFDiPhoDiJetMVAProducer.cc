#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/TagFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/TagFormats/interface/VBFMVAResult.h"

#include "TMVA/Reader.h"
#include "TMath.h"

using namespace std;
using namespace edm;

namespace flashgg {

	class VBFDiPhoDiJetMVAProducer : public EDProducer {

		public:
			VBFDiPhoDiJetMVAProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<View<VBFMVAResult> > vbfMvaResultToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

			unique_ptr<TMVA::Reader>vbfDiPhoDiJetMva_;
			FileInPath vbfDiPhoDiJetMVAweightfile_;

			float dijet_mva_;
			float dipho_mva_;
			float dipho_PToM_;


	};

	VBFDiPhoDiJetMVAProducer::VBFDiPhoDiJetMVAProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		vbfMvaResultToken_(consumes<View<flashgg::VBFMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("VBFMVAResultTag", InputTag("flashggVBFMVA")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA"))))
	{

		vbfDiPhoDiJetMVAweightfile_ = iConfig.getParameter<edm::FileInPath>("vbfDiPhoDiJetMVAweightfile");

		dijet_mva_ = 0.; 
		dipho_mva_ = 0.;
		dipho_PToM_ = 0.;


		vbfDiPhoDiJetMva_.reset( new TMVA::Reader("!Color:Silent"));
		vbfDiPhoDiJetMva_->AddVariable("dipho_mva", &dipho_mva_);
		vbfDiPhoDiJetMva_->AddVariable("bdt_dijet_maxdPhi", &dijet_mva_);
		vbfDiPhoDiJetMva_->AddVariable("dipho_pt/mass", &dipho_PToM_);

		vbfDiPhoDiJetMva_->BookMVA("BDT",vbfDiPhoDiJetMVAweightfile_.fullPath());


		produces<vector<VBFDiPhoDiJetMVAResult> >(); 
	} 

	void VBFDiPhoDiJetMVAProducer::produce( Event & evt, const EventSetup & ) {
		Handle<View<flashgg::DiPhotonCandidate> > diPhotons; evt.getByToken(diPhotonToken_,diPhotons); 
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector(); 

		Handle<View<flashgg::VBFMVAResult> > vbfMvaResults;
		evt.getByToken(vbfMvaResultToken_,vbfMvaResults);
		const PtrVector<flashgg::VBFMVAResult>& vbfMvaResultPointers = vbfMvaResults->ptrVector();

		Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
		evt.getByToken(mvaResultToken_,mvaResults);
		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

		std::auto_ptr<vector<VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJet_results(new vector<VBFDiPhoDiJetMVAResult>); // one per diphoton, always in same order, vector is more efficient than map 

		for (unsigned int candIndex =0; candIndex < diPhotonPointers.size() ; candIndex++){

      edm::Ptr<flashgg::DiPhotonMVAResult> dipho_mvares = mvaResultPointers[candIndex];
			dipho_mva_ = dipho_mvares->result;

      edm::Ptr<flashgg::VBFMVAResult> vbf_mvares = vbfMvaResultPointers[candIndex];
			dijet_mva_ = vbf_mvares->vbfMvaResult_value;

		  flashgg::VBFDiPhoDiJetMVAResult mvares;
		

			auto leadPho_p4 = diPhotonPointers[candIndex]->leadingPhoton()->p4();
			auto sublPho_p4 =  diPhotonPointers[candIndex]->subLeadingPhoton()->p4();
			auto diphoton_p4 =leadPho_p4 + sublPho_p4;
			dipho_PToM_ = diphoton_p4.Pt() / diphoton_p4.M();

			mvares.vbfDiPhoDiJetMvaResult = vbfDiPhoDiJetMva_->EvaluateMVA("BDT");

			mvares.dijet_mva =   dijet_mva_ ;
			mvares.dipho_mva =   dipho_mva_ ;
			mvares.dipho_PToM =   dipho_PToM_ ;

			std::cout << "dijet MVA " << dijet_mva_ << std::endl;

			mvares.vbfMvaResult =  (VBFMVAResult)vbf_mvares;

			vbfDiPhoDiJet_results->push_back(mvares);
		}
		evt.put(vbfDiPhoDiJet_results);
	}
}

typedef flashgg::VBFDiPhoDiJetMVAProducer FlashggVBFDiPhoDiJetMVAProducer;
DEFINE_FWK_MODULE(FlashggVBFDiPhoDiJetMVAProducer);
