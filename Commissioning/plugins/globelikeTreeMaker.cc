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
#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAODAlgos/interface/PhotonIdUtils.h"

#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"


#include "TTree.h"

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class FlashggTreeMaker : public edm::EDAnalyzer {
   public:
      explicit FlashggTreeMaker(const edm::ParameterSet&);
      ~FlashggTreeMaker();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:

      edm::Service<TFileService> fs_;

      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //void initEventStructure();

  EDGetTokenT<View<reco::Vertex> > vertexToken_;
  EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
	EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenDz_;
  EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenAOD_;
  EDGetTokenT<View<flashgg::Jet> > jetTokenDz_;
 // EDGetTokenT<View<flashgg::Jet> > jetTokenRecoBasedMap_;
  //EDGetTokenT<View<flashgg::Jet> > jetTokenReco_;
  EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> >  diPhotonToken_; // LDC work-in-progress adding this!


TTree *flashggTree;

Int_t run;
Int_t lumis;
Int_t event;
Float_t njets10;
Float_t njets15;
Float_t njets20;

  

  edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_; // SCZ work-in-progress adding this!
  //  edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;
  //      edm::EDGetTokenT<edm::View<reco::Vertex> >               vertexToken_; 
  //      edm::EDGetTokenT<edm::View<pat::PackedCandidate> >       pfcandidateToken_;

  //      TTree* photonTree; 
  //      photonInfo phoInfo;
      // add all variables as private members
      
  //      flashgg::PhotonIdUtils phou;
      
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
FlashggTreeMaker::FlashggTreeMaker(const edm::ParameterSet& iConfig):
  vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
  genParticleToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
  vertexCandidateMapTokenDz_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagDz"))),
  vertexCandidateMapTokenAOD_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagAOD"))),
  jetTokenDz_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz"))),
 // jetTokenRecoBasedMap_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagRecoBasedMap"))),
  diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
  photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons"))))
  //  jetTokenReco_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagReco")))
{
 
}

FlashggTreeMaker::~FlashggTreeMaker()
{
 
}



void
FlashggTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // ********************************************************************************

  // access edm objects

  Handle<VertexCandidateMap> vertexCandidateMapDz;
  iEvent.getByToken(vertexCandidateMapTokenDz_,vertexCandidateMapDz);

  Handle<VertexCandidateMap> vertexCandidateMapAOD;
  iEvent.getByToken(vertexCandidateMapTokenAOD_,vertexCandidateMapAOD);

  Handle<View<reco::Vertex> > primaryVertices;
  iEvent.getByToken(vertexToken_,primaryVertices);
  const PtrVector<reco::Vertex>& vtxs = primaryVertices->ptrVector();
	int a = vtxs.size();
  
  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken(genParticleToken_,genParticles);
  const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();
	a = gens.size();

  Handle<View<flashgg::Jet> > jetsDz;
  iEvent.getByToken(jetTokenDz_,jetsDz);
  const PtrVector<flashgg::Jet>& jetPointersDz = jetsDz->ptrVector();
	a = jetPointersDz.size();

  //Handle<View<flashgg::Jet> > jetsRecoBasedMap;
  //iEvent.getByToken(jetTokenRecoBasedMap_,jetsRecoBasedMap);
 // const PtrVector<flashgg::Jet>& jetPointersRecoBasedMap = jetsRecoBasedMap->ptrVector();
//a = jetPointersRecoBasedMap.size();
  Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
  iEvent.getByToken(diPhotonToken_,diPhotons);
  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();
a = diPhotonPointers.size();
  Handle<View<flashgg::Photon> > photons;
  iEvent.getByToken(photonToken_,photons);
  const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();
a = photonPointers.size();

a=a+1;

	njets10 = 0.;
	njets15 = 0.;
	njets20 = 0.;
	for (UInt_t jetLoop =0; jetLoop < jetPointersDz.size() ; jetLoop++){
		Float_t et = jetPointersDz[jetLoop]->et();
		if( et > 10.){
			njets10=+1.;
		}
		if( et > 15.){
			njets15=+1.;
		}
		if( et > 20.){
			njets20=+1.;
		}

	}


std::cout << njets10 << "	" << njets15 << "	" << njets20 << std::endl; 


run = iEvent.eventAuxiliary().run(); 
	event = iEvent.eventAuxiliary().event(); 
	lumis = iEvent.eventAuxiliary().luminosityBlock(); 

	flashggTree->Fill();

}
	void 
FlashggTreeMaker::beginJob()
{
	flashggTree = fs_->make<TTree>("flashggTree","flashgg tree");
	flashggTree->Branch("njets10", &njets10, "njets10/F");
	flashggTree->Branch("njets15", &njets15, "njets15/F");
	flashggTree->Branch("njets20", &njets20, "njets20/F");
	flashggTree->Branch("run", &run, "runNumber/I");
	flashggTree->Branch("lumis", &lumis, "lumiSection/I");
	flashggTree->Branch("event", &run, "eventNumber/I");
}

	void 
FlashggTreeMaker::endJob() 
{
}


void
FlashggTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

typedef FlashggTreeMaker FlashggFlashggTreeMaker;
DEFINE_FWK_MODULE(FlashggFlashggTreeMaker);
