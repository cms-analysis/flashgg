// By L D CORPE
// Adapted from the flashggCommissioning tree maker code  by C. Favaro et al.

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/TagFormats/interface/VBFTag.h"
#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/TagFormats/interface/TTHhadronicTag.h"
#include "flashgg/TagFormats/interface/TTHleptonicTag.h"


#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class FlashggPFCollAnalyzer : public edm::EDAnalyzer {
	public:
		explicit FlashggPFCollAnalyzer(const edm::ParameterSet&);
		~FlashggPFCollAnalyzer();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:

		edm::Service<TFileService> fs_;

		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;

		//void initEventStructure();

		EDGetTokenT<View<reco::Candidate> > pfCollPF_;
		EDGetTokenT<View<reco::Candidate> > pfCollPFCHS0_;
		EDGetTokenT<View<reco::Candidate> > pfCollPFCHSLeg_;
		EDGetTokenT<View<reco::Candidate> > pfCollPUPPI0_;
		EDGetTokenT<View<reco::Candidate> > pfCollPUPPILeg_;
TH1F *pfPt ;      
TH1F *pfEta ;     
TH1F *pfPhi  ;    
TH1F *pfchs0Pt;   
TH1F *pfchs0Eta;  
TH1F *pfchs0Phi ; 
TH1F *pfchsLegPt ;
TH1F *pfchsLegEta;
TH1F *pfchsLegPhi;
TH1F *puppi0Pt   ;
TH1F *puppi0Eta  ;
TH1F *puppi0Phi  ;
TH1F *puppiLegPt ;
TH1F *puppiLegEta;
TH1F *puppiLegPhi;




		//TTree *flashggTreeWithTagSorter;

		// Variables to fill
		
};

// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FlashggPFCollAnalyzer::FlashggPFCollAnalyzer(const edm::ParameterSet& iConfig):
	pfCollPF_(consumes<View<reco::Candidate> >(iConfig.getParameter<InputTag>("CollTagPF"))),
	pfCollPFCHS0_(consumes<View<reco::Candidate> >(iConfig.getParameter<InputTag>("CollTagPFPFCHS0"))),
	pfCollPFCHSLeg_(consumes<View<reco::Candidate> >(iConfig.getParameter<InputTag>("CollTagPFPFCHSLeg"))),
	pfCollPUPPI0_(consumes<View<reco::Candidate> >(iConfig.getParameter<InputTag>("CollTagPFPUPPI0"))),
	pfCollPUPPILeg_(consumes<View<reco::Candidate> >(iConfig.getParameter<InputTag>("CollTagPFPUPPILeg")))
{
}

FlashggPFCollAnalyzer::~FlashggPFCollAnalyzer()
{

}

void
FlashggPFCollAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

	// ********************************************************************************
	// access edm objects


	Handle<View<reco::Candidate> > pfCollPF;
	iEvent.getByToken(pfCollPF_,pfCollPF);
	const PtrVector<reco::Candidate>& pfPtrs = pfCollPF->ptrVector();

	Handle<View<reco::Candidate> > pfCollPFCHS0;
	iEvent.getByToken(pfCollPFCHS0_,pfCollPFCHS0);
	const PtrVector<reco::Candidate>& pfchs0Ptrs = pfCollPFCHS0->ptrVector();

	Handle<View<reco::Candidate> > pfCollPFCHSLeg;
	iEvent.getByToken(pfCollPFCHSLeg_,pfCollPFCHSLeg);
	const PtrVector<reco::Candidate>& pfchsLegPtrs = pfCollPFCHSLeg->ptrVector();

	Handle<View<reco::Candidate> > pfCollPUPPI0;
	iEvent.getByToken(pfCollPUPPI0_,pfCollPUPPI0);
	const PtrVector<reco::Candidate>& puppi0Ptrs = pfCollPUPPI0->ptrVector();


	Handle<View<reco::Candidate> > pfCollPUPPILeg;
	iEvent.getByToken(pfCollPUPPILeg_,pfCollPUPPILeg);
	const PtrVector<reco::Candidate>& puppiLegPtrs = pfCollPUPPILeg->ptrVector();


	for (UInt_t Loop =0; Loop < pfPtrs.size() ; Loop++){

		Float_t pt = pfPtrs[Loop]->pt();
		Float_t eta = pfPtrs[Loop]->eta();
		Float_t phi = pfPtrs[Loop]->phi();
		pfPt->Fill(pt);
		pfEta->Fill(eta);
		pfPhi->Fill(phi);
		}
	for (UInt_t Loop =0; Loop < pfchs0Ptrs.size() ; Loop++){

		Float_t pt = pfchs0Ptrs[Loop]->pt();
		Float_t eta = pfchs0Ptrs[Loop]->eta();
		Float_t phi = pfchs0Ptrs[Loop]->phi();
		pfchs0Pt->Fill(pt);
		pfchs0Eta->Fill(eta);
		pfchs0Phi->Fill(phi);
		}
	for (UInt_t Loop =0; Loop < pfchsLegPtrs.size() ; Loop++){

		Float_t pt = pfchsLegPtrs[Loop]->pt();
		Float_t eta = pfchsLegPtrs[Loop]->eta();
		Float_t phi = pfchsLegPtrs[Loop]->phi();
		pfchsLegPt->Fill(pt);
		pfchsLegEta->Fill(eta);
		pfchsLegPhi->Fill(phi);
		}
	for (UInt_t Loop =0; Loop < puppi0Ptrs.size() ; Loop++){

		Float_t pt = puppi0Ptrs[Loop]->pt();
		Float_t eta = puppi0Ptrs[Loop]->eta();
		Float_t phi = puppi0Ptrs[Loop]->phi();
		puppi0Pt->Fill(pt);
		puppi0Eta->Fill(eta);
		puppi0Phi->Fill(phi);
		}
	
	for (UInt_t Loop =0; Loop < puppiLegPtrs.size() ; Loop++){

		Float_t pt = puppiLegPtrs[Loop]->pt();
		Float_t eta = puppiLegPtrs[Loop]->eta();
		Float_t phi = puppiLegPtrs[Loop]->phi();
		puppiLegPt->Fill(pt);
		puppiLegEta->Fill(eta);
		puppiLegPhi->Fill(phi);
		}
	
	}



	void 
FlashggPFCollAnalyzer::beginJob()
{


	pfPt         = fs_->make<TH1F>("pfPt","pfPt",500,0,500);
	pfEta        = fs_->make<TH1F>("pfEta","pfEta",500,-5,5);
	pfPhi        = fs_->make<TH1F>("pfPhi","pfPhi",500,-6.3,6.3);
	pfchs0Pt     = fs_->make<TH1F>("pfchs0Pt","pfchs0Pt",500,0,500);
	pfchs0Eta    = fs_->make<TH1F>("pfchs0Eta","pfchs0Eta",500,-5,5);
	pfchs0Phi    = fs_->make<TH1F>("pfchs0Phi","pfchs0Phi",500,-6.3,6.3);
	pfchsLegPt   = fs_->make<TH1F>("pfchsLegPt","pfchsLegPt",500,0,500);
	pfchsLegEta  = fs_->make<TH1F>("pfchsLegEta","pfchsLegEta",500,-5,5);
	pfchsLegPhi  = fs_->make<TH1F>("pfchsLegPhi","pfchsLegPhi",500,-6.3,6.3);
	puppi0Pt     = fs_->make<TH1F>("puppi0Pt","puppi0Pt",500,0,500);
	puppi0Eta    = fs_->make<TH1F>("puppi0Eta","puppi0Eta",500,-5,5);
	puppi0Phi    = fs_->make<TH1F>("puppi0Phi","puppi0Phi",500,-6.3,6.3);
	puppiLegPt   = fs_->make<TH1F>("puppiLegPt","puppiLegPt",500,0,500);
	puppiLegEta  = fs_->make<TH1F>("puppiLegEta","puppiLegEta",500,-5,5);
	puppiLegPhi  = fs_->make<TH1F>("puppiLegPhi","puppiLegPhi",500,-6.3,6.3);

pfPt       ->Sumw2(); 
pfEta       ->Sumw2(); 
pfPhi       ->Sumw2(); 
pfchs0Pt    ->Sumw2(); 
pfchs0Eta   ->Sumw2(); 
pfchs0Phi   ->Sumw2(); 
pfchsLegPt  ->Sumw2(); 
pfchsLegEta ->Sumw2(); 
pfchsLegPhi ->Sumw2(); 
puppi0Pt    ->Sumw2(); 
puppi0Eta   ->Sumw2(); 
puppi0Phi   	->Sumw2(); 
puppiLegPt  ->Sumw2();
puppiLegEta->Sumw2();
puppiLegPhi ->Sumw2();

} 
void 
FlashggPFCollAnalyzer::endJob(){

/*pfPt       ->Write(); 
pfEta       ->Write(); 
pfPhi       ->Write(); 
pfchs0Pt    ->Write(); 
pfchs0Eta   ->Write(); 
pfchs0Phi   ->Write(); 
pfchsLegPt  ->Write(); 
pfchsLegEta ->Write(); 
pfchsLegPhi ->Write(); 
puppi0Pt    ->Write(); 
puppi0Eta   ->Write(); 
puppi0Phi   	->Write(); 
puppiLegPt  ->Write();
puppiLegEta->Write();
puppiLegPhi ->Write();*/

} 
void
FlashggPFCollAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

typedef FlashggPFCollAnalyzer FlashggFlashggPFCollAnalyzer;
DEFINE_FWK_MODULE(FlashggFlashggPFCollAnalyzer);
