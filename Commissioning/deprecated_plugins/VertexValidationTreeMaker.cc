// By S. Zenz
// Cut and pasted from the flashggCommissioning tree maker code  by C. Favaro et al.
//
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

// **********************************************************************

// define the structures used to create tree branches and fill the trees 

// per-event tree
struct eventInfo {
  // float nvtx;
  // float ntrk_tot_AOD;
  // float ntrk_tot_Dz;
  float genVertexZ;
  float zerothVertexZ;
  float diphotonVertexZ;
  float higgsPt;
};

// per-vertex tree
struct vertexInfo {
  int   ntrk_AOD;
  int   ntrk_Dz;
  float sumptsq_AOD;
  float sumptsq_Dz;
};

// per jet tree
struct jetInfo {
  float pt;
  float eta;
  float pujetid_dz;
  float pujetid_jrbm;
  float pujetid_jr;
  float overlap_photon_pt;
  int n_overlap_photon;
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

  EDGetTokenT<View<reco::Vertex> >      vertexToken_;
  EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
  EDGetTokenT< VertexCandidateMap >     vertexCandidateMapTokenDz_;
  EDGetTokenT< VertexCandidateMap >     vertexCandidateMapTokenAOD_;
  EDGetTokenT<View<flashgg::Jet> >      jetTokenDz_;
  EDGetTokenT<View<flashgg::Jet> >      jetTokenRecoBasedMap_;
  EDGetTokenT<View<flashgg::Jet> >      jetTokenReco_;
  EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> >  diPhotonToken_; // LDC work-in-progress adding this!

  
  TTree* vertexTree;
  TTree* eventTree;
  TTree *jetTree;
  vertexInfo vInfo;
  eventInfo eInfo;
  jetInfo jInfo;
  

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
VertexValidationTreeMaker::VertexValidationTreeMaker(const edm::ParameterSet& iConfig):
  vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
  genParticleToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
  vertexCandidateMapTokenDz_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagDz"))),
  vertexCandidateMapTokenAOD_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagAOD"))),
  jetTokenDz_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz"))),
  jetTokenRecoBasedMap_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagRecoBasedMap"))),
  diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
  photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons"))))
  //  jetTokenReco_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagReco")))
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

  Handle<View<flashgg::Jet> > jetsDz;
  iEvent.getByToken(jetTokenDz_,jetsDz);
  const PtrVector<flashgg::Jet>& jetPointersDz = jetsDz->ptrVector();

  Handle<View<flashgg::Jet> > jetsRecoBasedMap;
  iEvent.getByToken(jetTokenRecoBasedMap_,jetsRecoBasedMap);
  const PtrVector<flashgg::Jet>& jetPointersRecoBasedMap = jetsRecoBasedMap->ptrVector();

  Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
  iEvent.getByToken(diPhotonToken_,diPhotons);
  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

  Handle<View<flashgg::Photon> > photons;
  iEvent.getByToken(photonToken_,photons);
  const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();

  //  Handle<View<flashgg::Jet> > jetsReco;
  //  iEvent.getByToken(jetTokenReco_,jetsReco);
  //  const PtrVector<flashgg::Jet>& jetPointersReco = jetsReco->ptrVector();



  // cout << "size = " << pvPointers.size() << " " << pfCandPointers.size() << endl;
  
  // ********************************************************************************
  initEventStructure();

  //	std::cout << " SCZ jetPointersDz.size() = " << jetPointersDz.size() << std::endl;
  //	std::cout <<" SCZ jetPointersRecoBasedMap.size() = " << jetPointersRecoBasedMap.size() << std::endl;
  //	std::cout <<" SCZ jetPointersReco.size() = " << jetPointersReco.size() << std::endl;
  //	std::cout << " SCZ vtxs.size() = " << vtxs.size() << std::endl;

  for (unsigned int jdz = 0 ; jdz < jetPointersDz.size() ; jdz++) {
    //	  std::cout << "jdz=" << jdz << std::endl;
    //	  std::cout << "jdzpt=" <<jetPointersDz[jdz]->pt() << std::endl;
    if (jetPointersDz[jdz]->pt() > 20.) {
      //	    std::cout << " scz JetDz " << jdz << " Pt=" << jetPointersDz[jdz]->pt() 
      //		      << " Eta=" << jetPointersDz[jdz]->eta() 
      //		      << " pujetid=" << jetPointersDz[jdz]->getPuJetId(vtxs[0]) << std::endl;
    }
    jInfo.pt = jetPointersDz[jdz]->pt();
    jInfo.eta = jetPointersDz[jdz]->eta();
    jInfo.pujetid_dz = jetPointersDz[jdz]->getPuJetId(vtxs[0]);
    jInfo.pujetid_jr = jetPointersDz[jdz]->userFloat("pileupJetId:fullDiscriminant");
    jInfo.pujetid_jrbm = -99.;
    jInfo.overlap_photon_pt = -99.;
    jInfo.n_overlap_photon = 0;
    for (unsigned int jrbm = 0 ; jrbm < jetPointersRecoBasedMap.size() ; jrbm++) {
      //	    std::cout << "jrbm=" << jrbm << std::endl;
      if (fabs(jetPointersDz[jdz]->pt() - jetPointersRecoBasedMap[jrbm]->pt()) < 0.1 &&
	  fabs(jetPointersDz[jdz]->eta() -jetPointersRecoBasedMap[jrbm]->eta()) < 0.01) {
	jInfo.pujetid_jrbm = jetPointersRecoBasedMap[jrbm]->getPuJetId(vtxs[0]);
	break;
      }
    }
    for (unsigned int np = 0 ; np < photonPointers.size() ; np++) {
      if (deltaR(jetPointersDz[jdz]->eta(),jetPointersDz[jdz]->phi(),photonPointers[np]->eta(),photonPointers[np]->phi()) < 0.4) {
	jInfo.overlap_photon_pt = photonPointers[np]->pt();
	jInfo.n_overlap_photon++;
	//	      std::cout << " SCZ DEBUG photon pt=" << photonPointers[np]->pt() << " eta=" << photonPointers[np]->eta() << " phi=" << photonPointers[np]->phi() << std::endl;
      }
    }
	  
    //          for (unsigned int jr = 0 ; jr < jetPointersReco.size() ; jr++) {
    //	    std::cout << "jr=" << jr << std::endl;
    //            if (fabs(jetPointersDz[jdz]->pt() - jetPointersReco[jr]->pt()) < 5.0 &&
    //                fabs(jetPointersDz[jdz]->eta() -jetPointersReco[jr]->eta()) < 0.1) {
    //	      jInfo.pujetid_jr = jetPointersReco[jr]->getPuJetId(vtxs[0]);
    //	      break;
    //            }
    //          }
    jetTree->Fill();
  }

  /*
    for (unsigned int jrbm = 0 ; jrbm < jetPointersRecoBasedMap.size() ; jrbm++) {
    if (jetPointersRecoBasedMap[jrbm]->pt() > 20.) {
    std::cout << " scz JetRecoBasedMap " << jrbm << " Pt=" << jetPointersRecoBasedMap[jrbm]->pt() 
    << " Eta=" << jetPointersRecoBasedMap[jrbm]->eta()
    << " pujetid=" << jetPointersRecoBasedMap[jrbm]->getPuJetId(vtxs[0]) << std::endl;
    }
    }

    for (unsigned int jr = 0 ; jr < jetPointersReco.size() ; jr++) {
    if (jetPointersReco[jr]->pt() > 20.) {
    std::cout << " scz JetReco " << jr << " Pt=" << jetPointersReco[jr]->pt() 
    << " Eta=" << jetPointersReco[jr]->eta()
    << " pujetid=" << jetPointersReco[jr]->getPuJetId(vtxs[0]) << std::endl;
    }
    }
  */


  //	std::cout << " Number of genParticles : " <<  gens.size() << std::endl;
  for( unsigned int genLoop =0 ; genLoop < gens.size(); genLoop++)
    {
      //		std::cout << "genParticle " << genLoop << " has vertex at z= " << gens[genLoop]->vz() << " 	PDGID:" << gens[genLoop]->pdgId() << ", status" << gens[genLoop]->status()<< std::endl;

      if(  gens[genLoop]->pdgId() ==25)
	{
	  eInfo.genVertexZ=gens[genLoop]->vz();
	  eInfo.higgsPt=gens[genLoop]->pt();

	  break;
	}
    }


  //std::cout << "DiPhoton Candidates " << diPhotonPointers.size() << std::endl;

  //eInfo.diphotonVertexZ = vtx->position().z(); 
  eInfo.diphotonVertexZ = -9999; 
  float higgsMassDifference =9999;

  for (unsigned int diphotonlooper =0; diphotonlooper < diPhotonPointers.size() ; diphotonlooper++)
    {
      //std::cout << diPhotonPointers[diphotonlooper]->mass() << std::endl;
     
      if  (fabs(diPhotonPointers[diphotonlooper]->mass() - 125) < higgsMassDifference)
	{
	  eInfo.diphotonVertexZ = diPhotonPointers[diphotonlooper]->getVertex()->z();
	  higgsMassDifference = fabs(diPhotonPointers[diphotonlooper]->mass() - 125); 
	}
      //     std::cout << "closest mass diff " << higgsMassDifference << " vertex dz" << eInfo.diphotonVertexZ << std::endl;

    }

  for (unsigned int i = 0 ; i < vtxs.size() ; i++) {
   
   
    edm::Ptr<reco::Vertex> vtx = vtxs[i];
   
   
    if(i==0)
      {
	eInfo.zerothVertexZ = vtx->position().z(); 
      }
    //		std::cout << " On vertex " << i << " with z position " << vtx->position().z() << std::endl;
    if (! vertexCandidateMapDz->count(vtx) ) {
      //     std::cout << " Missing vertex from vertexCandidateMapDz - skipping" << std::endl;
      continue;
    }
    if (! vertexCandidateMapAOD->count(vtx) ) {
      //     std::cout << " Missing vertex from vertexCandidateMapAOD - skipping" << std::endl;
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
  eventTree->Branch("eventBranch",&eInfo.genVertexZ,"gen_vertex_z/F:zeroth_vertex_z/F:diphotonVertexZ/F:higgs_pt/F");
  jetTree = fs_->make<TTree>("jetTree","per-jet tree");
  jetTree->Branch("jetBranch",&jInfo.pt,"pt/F:eta/F:pujetid_dz/F:pujetid_jrbm/F:pujetid_jr/F:overlap_photon_pt/F:n_overlap_photon/I");
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
  eInfo.diphotonVertexZ =-999.;
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
