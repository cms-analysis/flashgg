// By Y.Haddad
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
#include "flashgg/MicroAODFormats/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"


#include "TTree.h"

// **********************************************************************

struct eventInfo {
  float genVertexZ;
  float zerothVertexZ;
  float diphotonVertexZ;
  float higgsPt;
  
  void init()  { 
    genVertexZ      =-999.;
    zerothVertexZ   =-999.;
    diphotonVertexZ =-999.;
    higgsPt         =-999.;
  }
};

// per-vertex tree
struct GenPartInfo {
  float pt;
  float eta;
  float phi;
  int   pdgid;
  int   status;
  
  void init(){
    pt  =-999;
    eta =-999; 
  }
};

// per jet tree
struct jetInfo {
  float pt;
  float eta;
  float phi;
  int   npart;
  int   nphoton;
  int   ncharged;
  
  void init(){
    pt  =-999;
    eta =-999; 
  }
};

// **********************************************************************

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class JetValidationTreeMaker : public edm::EDAnalyzer {
public:
  explicit JetValidationTreeMaker(const edm::ParameterSet&);
  ~JetValidationTreeMaker();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  
  edm::Service<TFileService> fs_;
  
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  void initEventStructure();
  
  //EDGetTokenT< View<reco::Vertex> > vertexToken_;
  //EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenDz_;
  //EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenAOD_;
  
  EDGetTokenT< edm::View<reco::GenParticle> > genPartToken_;
  EDGetTokenT< edm::View<flashgg::Jet> >      jetDzToken_;
  
  
  TTree*     eventTree;
  TTree*     jetTree;
  TTree*     genPartTree;
  
  eventInfo   eInfo;
  jetInfo     jInfo;
  GenPartInfo genInfo;
  Int_t event_number;
  
  
  TH1F *h_jetPt ;      
  TH1F *h_jetEta ;     
  TH1F *h_jetPhi  ;    
  
  TH1F *h_genPt ;      
  TH1F *h_genEta ;     
  TH1F *h_genPhi  ;    
  
};

JetValidationTreeMaker::JetValidationTreeMaker(const edm::ParameterSet& iConfig):
  genPartToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
  jetDzToken_ (consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz")))
{
  event_number = 0;
}

JetValidationTreeMaker::~JetValidationTreeMaker()
{
  event_number = 0;
}



void
JetValidationTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  //Handle<View<reco::Vertex> > primaryVertices;
  //iEvent.getByToken(vertexToken_,primaryVertices);
  //const PtrVector<reco::Vertex>& vtxs = primaryVertices->ptrVector();
  
  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken(genPartToken_,genParticles);
  const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();
  
  Handle<View<flashgg::Jet> > jetsDz;
  iEvent.getByToken(jetDzToken_,jetsDz);
  const PtrVector<flashgg::Jet>& jetsDzPointers = jetsDz->ptrVector();
  
  
  initEventStructure();
  
  std::cout << " JET CHECKER ::  jetsDzCheckPointers.size() = " << jetsDzPointers.size() << std::endl;
  std::cout << " JET CHECKER ::  Event                      = " << event_number << std::endl;
  
  for (unsigned int jdz = 0 ; jdz < jetsDzPointers.size() ; jdz++) {
    std::cout << "\t JET CHECKER :: jet["<< jdz <<"] pt  =" <<jetsDzPointers[jdz]->pt() << std::endl;
    jInfo.pt                = jetsDzPointers[jdz]->pt();
    jInfo.eta               = jetsDzPointers[jdz]->eta();
    jInfo.phi               = jetsDzPointers[jdz]->phi();
    
    h_jetPt ->Fill(jetsDzPointers[jdz]->pt() );	
    h_jetEta->Fill(jetsDzPointers[jdz]->eta());	
    h_jetPhi->Fill(jetsDzPointers[jdz]->phi());	
    
    jetTree->Fill();
  }
    
  std::cout << " Number of genParticles : " <<  gens.size() << std::endl;
  for( unsigned int genLoop =0 ; genLoop < gens.size(); genLoop++){
    
    genInfo.pt     = gens[genLoop]->pt() ;
    genInfo.eta    = gens[genLoop]->eta();
    genInfo.phi    = gens[genLoop]->phi();
    genInfo.status = gens[genLoop]->status();
    genInfo.pdgid  = int(gens[genLoop]->pdgId());
    
    // takes only the light quarks pid < 6
    if ( fabs(gens[genLoop]->pdgId()) < 6 ){
      h_genPt ->Fill(gens[genLoop]->pt() );	
      h_genEta->Fill(gens[genLoop]->eta());	
      h_genPhi->Fill(gens[genLoop]->phi());	
      
      std::cout << "\t GenQuarks id::" << genLoop 
		<< "  pt::"         << genInfo.pt    
		<< "  pid::("       << genInfo.pdgid  << "," << gens[genLoop]->pdgId() << ")"
		<< "  sts::"        << genInfo.status 
		<< std::endl;
    }
    genPartTree->Fill();
  }
    
  eventTree->Fill();
  event_number++;
}


void 
JetValidationTreeMaker::beginJob()
{
  

  
  h_jetPt    = fs_->make<TH1F>("h_jetPt" ,"flashgg jets;P_{t}[GeV]",500,0,500);
  h_jetEta   = fs_->make<TH1F>("h_jetEta","flashgg jets;#eta"      ,500,-5,5);
  h_jetPhi   = fs_->make<TH1F>("h_jetPhi","flashgg jets;#phi"      ,500,-6.3,6.3);
  
  h_genPt    = fs_->make<TH1F>("h_genPt" ,"gen quarks only;P_{t}[GeV]",500,0,500);
  h_genEta   = fs_->make<TH1F>("h_genEta","gen quarks only;#eta"      ,500,-5,5);
  h_genPhi   = fs_->make<TH1F>("h_genPhi","gen quarks only;#phi"      ,500,-6.3,6.3);
  
  eventTree = fs_->make<TTree>("eventTree","per-event tree");
  eventTree->Branch("eventBranch",&eInfo.genVertexZ,"gen_vertex_z/F:zeroth_vertex_z/F:diphotonVertexZ/F:higgs_pt/F");
  
  jetTree = fs_->make<TTree>("jetTree","Check per-jet tree");
  jetTree->Branch("pt"   ,&jInfo.pt  ,"pt/F" );
  jetTree->Branch("eta"  ,&jInfo.eta ,"eta/F");
  jetTree->Branch("phi"  ,&jInfo.phi ,"phi/F");
  
  genPartTree = fs_->make<TTree>("genPartTree","Check per-jet tree");
  genPartTree->Branch("pt"     ,&genInfo.pt      ,"pt/F" );
  genPartTree->Branch("eta"    ,&genInfo.eta     ,"eta/F");
  genPartTree->Branch("phi"    ,&genInfo.phi     ,"phi/F");
  genPartTree->Branch("status" ,&genInfo.status  ,"status/I" );
  genPartTree->Branch("pdgid"  ,&genInfo.pdgid   ,"pdgid/I");

  
}

void 
JetValidationTreeMaker::endJob() 
{
  
}

void
JetValidationTreeMaker::initEventStructure() 
{
  
}


void 
JetValidationTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

typedef JetValidationTreeMaker FlashggJetValidationTreeMaker;
DEFINE_FWK_MODULE(FlashggJetValidationTreeMaker);
