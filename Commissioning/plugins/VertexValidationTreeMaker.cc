// By S. Zenz
// Cut and pasted from the flashggCommissioning tree maker code  by C. Favaro et al.

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
#include "TTree.h"

// **********************************************************************

// define the structures used to create tree branches and fill the trees 

// per-event tree
struct eventInfo {
 // float nvtx;
 // float ntrk_tot_AOD;
 // float ntrk_tot_Dz;
float genVertexZ;
float zerothVertexZ;
float higgsPt;
};

// per-vertex tree
struct vertexInfo {
  int ntrk_AOD;
  int ntrk_Dz;
  float sumptsq_AOD;
  float sumptsq_Dz;
};

// **********************************************************************

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class VertexValidationTreeMaker : public edm::EDAnalyzer {
   public:
      explicit VertexValidationTreeMaker(const edm::ParameterSet&);
      ~VertexValidationTreeMaker();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:

      edm::Service<TFileService> fs_;

      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      void initEventStructure();

  EDGetTokenT<View<reco::Vertex> > vertexToken_;
  EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
	EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenDz_;
  EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenAOD_;

  
  TTree* vertexTree;
  TTree* eventTree;
  vertexInfo vInfo;
  eventInfo eInfo;
  

  //      edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_;
  //      edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;
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
VertexValidationTreeMaker::VertexValidationTreeMaker(const edm::ParameterSet& iConfig):
  vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
  genParticleToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
  vertexCandidateMapTokenDz_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagDz"))),
  vertexCandidateMapTokenAOD_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagAOD")))
{
 
}

VertexValidationTreeMaker::~VertexValidationTreeMaker()
{
 
}



void
VertexValidationTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
  
	Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken(genParticleToken_,genParticles);
  const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();

  // cout << "size = " << pvPointers.size() << " " << pfCandPointers.size() << endl;
  
	// ********************************************************************************
	initEventStructure();

//	std::cout << " Number of genParticles : " <<  gens.size() << std::endl;
	for( unsigned int genLoop =0 ; genLoop < gens.size(); genLoop++)
	{
		std::cout << "genParticle " << genLoop << " has vertex at z= " << gens[genLoop]->vz() << " 	PDGID:" << gens[genLoop]->pdgId() << ", status" << gens[genLoop]->status()<< std::endl;

		if(  gens[genLoop]->pdgId() ==25)
		{
		eInfo.genVertexZ=gens[genLoop]->vz();
		eInfo.higgsPt=gens[genLoop]->pt();

		break;
		}
	}



	for (unsigned int i = 0 ; i < vtxs.size() ; i++) {


		edm::Ptr<reco::Vertex> vtx = vtxs[i];

		if(i==0)
		{
eInfo.zerothVertexZ = vtx->position().z(); 
		}
		std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
		if (! vertexCandidateMapDz->count(vtx) ) {
			std::cout << " Missing vertex from vertexCandidateMapDz - skipping" << std::endl;
			continue;
		}
		if (! vertexCandidateMapAOD->count(vtx) ) {
			std::cout << " Missing vertex from vertexCandidateMapAOD - skipping" << std::endl;
			continue;
		}
		vInfo.ntrk_AOD = vertexCandidateMapAOD->at(vtx).size() ;
		vInfo.ntrk_Dz = vertexCandidateMapDz->at(vtx).size() ;
		vInfo.sumptsq_AOD = 0.;
		vInfo.sumptsq_Dz = 0.;
		for (unsigned int j = 0 ; j < vertexCandidateMapDz->at(vtx).size() ; j++) {
			edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMapDz->at(vtx)[j];
			//std::cout << " Candidate " << j << " in vertex " << i << " (Dz Map) has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
			vInfo.sumptsq_Dz += (cand->pt()*cand->pt());
		}
		for (unsigned int j = 0 ; j < vertexCandidateMapAOD->at(vtx).size() ; j++) {
			edm::Ptr<pat::PackedCandidate> cand = vertexCandidateMapAOD->at(vtx)[j];
			//std::cout << " Candidate " << j << " in vertex " << i << " (AOD Map) has dz (w.r.t that vertex) of  " << cand->dz(vtx->position()) << std::endl;
			vInfo.sumptsq_AOD += (cand->pt()*cand->pt());
		}
		vertexTree->Fill();
	}

eventTree->Fill();

}


	void 
VertexValidationTreeMaker::beginJob()
{
	vertexTree = fs_->make<TTree>("vertexTree","per-vertex tree");
	vertexTree->Branch("vertexBranch",&vInfo.ntrk_AOD,"ntrk_AOD/I:ntrk_Dz/I:sumptsq_AOD/F:sumptsq_Dz/F");
	eventTree = fs_->make<TTree>("eventTree","per-event tree");
	eventTree->Branch("eventBranch",&eInfo.genVertexZ,"gen_vertex_z/F:zeroth_vertex_z/F:higgs_pt/F");
}

	void 
VertexValidationTreeMaker::endJob() 
{
}

	void
VertexValidationTreeMaker::initEventStructure() 
{
	vInfo.ntrk_AOD = -999;
	vInfo.ntrk_Dz = -999;
	vInfo.sumptsq_AOD = -999.;
	vInfo.sumptsq_Dz = -999.;

	eInfo.genVertexZ =-999.;
	eInfo.zerothVertexZ =-999.;
	eInfo.higgsPt = -999.;
}


void
VertexValidationTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

typedef VertexValidationTreeMaker FlashggVertexValidationTreeMaker;
DEFINE_FWK_MODULE(FlashggVertexValidationTreeMaker);
