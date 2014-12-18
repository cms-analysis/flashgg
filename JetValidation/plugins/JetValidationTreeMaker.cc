// By Y.Haddad & L.Corpe
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
  
  unsigned int nJet;
  unsigned int nPV;
  unsigned int nSV;
  
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

// per-genJet tree
struct GenJetInfo {
  float pt;
  float eta;
  float phi;
  float recoJetPt   ; 
  float recoJetRawPt ;
  float recoJetBestPt;
  int   recoJetMatch ;
  float recoJetEta   ;
  float dR   ;
  
  void init(){
    pt  =-999;
    eta =-999; 
  }
};
// per recoJet tree
struct jetInfo {
  float pt;
  float rawPt;
  float bestPt;
  float energy;
  float mass;
  float area;
  float eta;
  float phi;
  
  int   npart_0 ;    // number of particles at pt>0
  int   npart_5 ;    // number of particles at pt>5
  int   npart_10;    // number of particles at pt>10
  int   npart_20;    // number of particles at pt>20
  
  int   ncharged_0 ; // number of particles at pt>0
  int   ncharged_5 ; // number of particles at pt>5
  int   ncharged_10; // number of particles at pt>10
  int   ncharged_20; // number of particles at pt>20
  
  int   nphoton_0 ;  // number of particles at pt>0
  int   nphoton_5 ;  // number of particles at pt>5
  int   nphoton_10;  // number of particles at pt>10
  int   nphoton_20;  // number of particles at pt>20
  
  float genJetPt;
  float genJetEta;
  int   genJetMatch;
  float genQuarkPt;
  float genQuarkEta;
  int   genQuarkMatch;
  int   genQuarkPdgId;
  void init(){
    pt  =-999;
    eta =-999; 
  }
  

  // photon matching part
  int   photonMatch;
  float photondRmin;
  float GenPhotonEta;
  float GenPhotonPhi;
  float GenPhotonPt;
  
};  

struct GenPhotonInfo{
  float pt;
  float phi;
  float eta;
  float DRmin;
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
  
  
  // Additional methods
  void initEventStructure();
  
  //EDGetTokenT< View<reco::Vertex> > vertexToken_;
  //EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenDz_;
  //EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenAOD_;
  
  EDGetTokenT< edm::View<reco::GenParticle> > genPartToken_;
  EDGetTokenT< edm::View<reco::GenJet> >      genJetToken_;
  EDGetTokenT< edm::View<flashgg::Jet> >      jetDzToken_;
  
  TTree*     eventTree;
  TTree*     jetTree;
  TTree*     genPartTree;
  TTree*     genJetTree;
  
  eventInfo   eInfo;
  jetInfo     jInfo;
  GenPartInfo genInfo;
  GenJetInfo  genJetInfo;
  Int_t       event_number;
  std::string jetCollectionName; 
  
  GenPartInfo jGenPhotonInfo;
  //----------------------
  
  TH1F *h_jetPt ;      
  TH1F *h_jetRawPt ;      
  TH1F *h_jetEta ;     
  TH1F *h_jetPhi  ;    
  
  TH1F *h_genPt ;      
  TH1F *h_genEta ;     
  TH1F *h_genPhi  ;    
  
};

JetValidationTreeMaker::JetValidationTreeMaker(const edm::ParameterSet& iConfig):
  genPartToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
  genJetToken_(consumes<View<reco::GenJet> >(iConfig.getUntrackedParameter<InputTag> ("GenJetTag", InputTag("slimmedGenJets")))),
  jetDzToken_ (consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz")))
{
  event_number = 0;
  jetCollectionName = iConfig.getParameter<string>("StringTag");
}

JetValidationTreeMaker::~JetValidationTreeMaker()
{
  event_number = 0;
}

void
JetValidationTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  
  //Handle<VertexCandidateMap> vertexCandidateMapDz;
  //iEvent.getByToken(vertexCandidateMapTokenDz_,vertexCandidateMapDz);
  //
  //Handle<VertexCandidateMap> vertexCandidateMapAOD;
  //iEvent.getByToken(vertexCandidateMapTokenAOD_,vertexCandidateMapAOD);
  //
  //Handle<View<reco::Vertex> > primaryVertices;
  //iEvent.getByToken(vertexToken_,primaryVertices);
  //const PtrVector<reco::Vertex>& vtxs = primaryVertices->ptrVector();
  
  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken(genPartToken_,genParticles);
  const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();
  
  Handle<View<reco::GenJet> > genJet;
  iEvent.getByToken(genJetToken_,genJet);
  const PtrVector<reco::GenJet>& genJets = genJet->ptrVector();
  
  Handle<View<flashgg::Jet> > jetsDz;
  iEvent.getByToken(jetDzToken_,jetsDz);
  const PtrVector<flashgg::Jet>& jetsDzPointers = jetsDz->ptrVector();
  
  initEventStructure();
  
  //std::cout << " Event Number : " <<  event_number << std::endl;
  
  std::map<unsigned int, GenPhotonInfo> photonJet_id;
  
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
    }
    
    //----------------------
    // matching with photons
    //std::cout << "gens.size() == " << gens.size() << std::endl;
    std::map<float,unsigned int> minim;
    std::map<unsigned int,GenPhotonInfo> minim_info;
    float DeltaRmin=999.;
    
    
    if (gens[genLoop]->pdgId() == 22  && gens[genLoop]->numberOfDaughters() == 0){
      for( unsigned int jetLoop =0 ; jetLoop < jetsDzPointers.size(); jetLoop++){
	GenPhotonInfo tmp_info;
	
	float dphi  = jetsDzPointers[jetLoop]->phi() -  gens[genLoop]->phi();
	float deta  = jetsDzPointers[jetLoop]->eta() -  gens[genLoop]->eta();
	float dr    =  std::sqrt(deta*deta + dphi*dphi);
	
	DeltaRmin = std::min (dr, DeltaRmin);
	minim[dr] = jetLoop; 
	
	// fill the info
	tmp_info.pt     = gens[genLoop]->pt ();
	tmp_info.eta    = gens[genLoop]->eta();
	tmp_info.phi    = gens[genLoop]->phi();	
	tmp_info.DRmin  = DeltaRmin;
	
	minim_info[jetLoop] = tmp_info;
      }
      
      unsigned int bestjetid = minim.find(DeltaRmin)->second;
      photonJet_id[bestjetid] = minim_info[bestjetid];
      //----------------------
      //std::cout << genLoop <<"  "
      //		<< minim.find(DeltaRmin)->second 
      //	<<" DeltaMin to photon == "<< DeltaRmin << std::endl;
    }
    genPartTree->Fill();
  }
  
  //std::cout << "number of matched photon == " << photonJet_id.size() << std::endl;
  
  for (unsigned int jdz = 0 ; jdz < jetsDzPointers.size() ; jdz++) {
    
    if( photonJet_id.find(jdz) != photonJet_id.end()){
      GenPhotonInfo tmp_info = photonJet_id.find(jdz)->second; // call find ones 
      jInfo.photondRmin  = tmp_info.DRmin;
      jInfo.GenPhotonPt  = tmp_info.pt   ;
      jInfo.GenPhotonEta = tmp_info.eta  ;
      jInfo.GenPhotonPhi = tmp_info.phi  ;
      jInfo.photonMatch  = 1;
    }else{
      jInfo.photondRmin  = -999.;
      jInfo.GenPhotonPt  = -999.;
      jInfo.GenPhotonEta = -999.;
      jInfo.GenPhotonPhi = -999.;
      jInfo.photonMatch  =  0   ;
      jInfo.photondRmin  = -999.;
    }
    
    //if(jInfo.photonMatch == 1) std::cout << "pt == " <<  jInfo.GenPhotonPt << "\t match==" << jInfo.photonMatch <<std::endl;
    
    jInfo.pt            = jetsDzPointers[jdz]->pt();
    jInfo.rawPt         = jetsDzPointers[jdz]->correctedJet("Uncorrected").pt() ;
    
    if(jetCollectionName.find("PPI")>1 && jetCollectionName.find("PPI")<jetCollectionName.size()){
      jInfo.bestPt      = jetsDzPointers[jdz]->correctedJet("Uncorrected").pt() ;
    } else {
      jInfo.bestPt      = jetsDzPointers[jdz]->pt() ;
    }
    
    if( jetsDzPointers[jdz]->genJet()){
      jInfo.genJetMatch           = 1;
      jInfo.genJetPt              = jetsDzPointers[jdz]->genJet()->pt();
      jInfo.genJetEta             = jetsDzPointers[jdz]->genJet()->eta();
      
    } else {
      jInfo.genJetPt              = -9999.;
      jInfo.genJetEta             = -9999.;
      jInfo.genJetMatch           = 0;
    }
    
    if( jetsDzPointers[jdz]->genParton()){
      jInfo.genQuarkMatch           = 1;
      jInfo.genQuarkPt              = jetsDzPointers[jdz]->genParton()->pt();
      jInfo.genQuarkEta             = jetsDzPointers[jdz]->genParton()->eta();
      jInfo.genQuarkPdgId           = jetsDzPointers[jdz]->genParton()->pdgId();
      
    } else {
      jInfo.genQuarkPt              = -9999.;
      jInfo.genQuarkEta             = -9999.;
      jInfo.genQuarkMatch           = 0;
      jInfo.genQuarkPdgId           = -9999;
    }
    //----------------------
    jInfo.energy           = jetsDzPointers[jdz]->energy() ;
    jInfo.mass             = jetsDzPointers[jdz]->mass() ;
    jInfo.eta              = jetsDzPointers[jdz]->eta();
    jInfo.phi              = jetsDzPointers[jdz]->phi();
    jInfo.area             = jetsDzPointers[jdz]->jetArea();
    // Get constituants information
    jInfo.npart_0  = 0; //
    jInfo.npart_5  = 0; //
    jInfo.npart_10 = 0; //
    jInfo.npart_20 = 0; //

    jInfo.ncharged_0  = 0; //
    jInfo.ncharged_5  = 0; //
    jInfo.ncharged_10 = 0; //
    jInfo.ncharged_20 = 0; //

    jInfo.nphoton_0  = 0; //
    jInfo.nphoton_5  = 0; //
    jInfo.nphoton_10 = 0; //
    jInfo.nphoton_20 = 0; //
    // loop over consitutuants
    
    for (unsigned int i = 0 ; i < jetsDzPointers[jdz]->getJetConstituentsQuick().size() ; i++){
      //1edm::Ptr<pat::PackedCandidate> icand = edm::Ptr<pat::PackedCandidate> ( jetsDzPointers[jdz]->daughterPtr(i) );
      const reco::Candidate* icand = jetsDzPointers[jdz]->getJetConstituentsQuick()[i];
      float candPt     = icand->pt();
      float candCharge = icand->charge();
      
      jInfo.npart_0++;
      if(candCharge != 0) jInfo.ncharged_0++;
      if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_0++;
      
      if(candPt > 5){
    	jInfo.npart_5++;
    	if(candCharge != 0) jInfo.ncharged_5++;
    	if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_5++;
      }
      if(candPt > 10){
    	jInfo.npart_10++;
    	if(candCharge != 0) jInfo.ncharged_10++;
    	if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_10++;
      }
      if(candPt > 20){
    	jInfo.npart_20++;
    	if(candCharge != 0) jInfo.ncharged_20++;
    	if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_20++;
      }
    }
    // histogramming
    h_jetPt    ->Fill(jetsDzPointers[jdz]->pt() );	
    h_jetRawPt ->Fill(jetsDzPointers[jdz]->correctedJet("Uncorrected").pt() );	
    h_jetEta   ->Fill(jetsDzPointers[jdz]->eta());	
    h_jetPhi   ->Fill(jetsDzPointers[jdz]->phi());	
    //
    jetTree->Fill();
  }
  
  // fill genJet tree
  //
  //std::cout << "DEBUG - gen Jets size " << genJets.size() << std::endl;
  for( unsigned int genLoop =0 ; genLoop < genJets.size(); genLoop++){
    
    genJetInfo.recoJetPt       = -999.;
    genJetInfo.recoJetRawPt    = -999.;
    genJetInfo.recoJetBestPt   = -999.;
    genJetInfo.recoJetMatch    =  0   ;
    genJetInfo.recoJetEta      = -999.;
    genJetInfo.dR              = -999.;
    genJetInfo.pt              = -999.;
    genJetInfo.eta             = -999.;
    genJetInfo.phi             = -999.;

    if (genJets[genLoop]->pt() <20) { continue;}

    genJetInfo.pt     = genJets[genLoop]->pt() ;
    //	std::cout << "DEBUG " << genJets[genLoop]->pt() << std::endl;
    genJetInfo.eta    = genJets[genLoop]->eta();
    genJetInfo.phi    = genJets[genLoop]->phi();
    
    float deta;
    float dphi;
    float dr  ;
    //loop over reco to do dR match
    for (unsigned int recoLoop=0; recoLoop <  jetsDzPointers.size(); recoLoop++){

      if(jetsDzPointers[recoLoop]->pt() < 5) continue;

      deta= jetsDzPointers[recoLoop]->eta() - 	 genJets[genLoop]->eta();
      dphi= jetsDzPointers[recoLoop]->phi() - 	 genJets[genLoop]->phi();
      dr = std::sqrt(deta*deta + dphi*dphi);

      if (dr < 0.4 ) {

	genJetInfo.dR      =  dr;
	genJetInfo.recoJetPt       = jetsDzPointers[recoLoop]->pt() ;
	genJetInfo.recoJetRawPt    = jetsDzPointers[recoLoop]->correctedJet("Uncorrected").pt()  ;

	if(jetCollectionName.find("PPI")>1 && jetCollectionName.find("PPI")<jetCollectionName.size()){
	  genJetInfo.recoJetBestPt   =  jetsDzPointers[recoLoop]->correctedJet("Uncorrected").pt() ;
	} else{  
	  genJetInfo.recoJetBestPt   = jetsDzPointers[recoLoop]->pt()  ;
	}
	genJetInfo.recoJetMatch    = 1 ;
	break;

      }
    }

    genJetTree->Fill();
  }
  
  eventTree->Fill();
  event_number++;
}


void 
JetValidationTreeMaker::beginJob()
{
  // +++ histograming
  h_jetPt    = fs_->make<TH1F>("h_jetPt"    ,"flashgg jets;P_{t}[GeV]"   ,500,0,500);
  h_jetRawPt = fs_->make<TH1F>("h_jetRawPt" ,"flashgg jets;rawP_{t}[GeV]",500,0,500);
  h_jetEta   = fs_->make<TH1F>("h_jetEta"   ,"flashgg jets;#eta"         ,500,-5,5);
  h_jetPhi   = fs_->make<TH1F>("h_jetPhi"   ,"flashgg jets;#phi"         ,500,-6.3,6.3);
  
  h_genPt    = fs_->make<TH1F>("h_genPt" ,"gen quarks only;P_{t}[GeV]",500,0,500);
  h_genEta   = fs_->make<TH1F>("h_genEta","gen quarks only;#eta"      ,500,-5,5);
  h_genPhi   = fs_->make<TH1F>("h_genPhi","gen quarks only;#phi"      ,500,-6.3,6.3);
  
  // +++ trees 
  std::string type("eventTree_");
  type += jetCollectionName;
  eventTree = fs_->make<TTree>(type.c_str(),jetCollectionName.c_str());
  eventTree->Branch("eventBranch",&eInfo.genVertexZ,"gen_vertex_z/F:zeroth_vertex_z/F:diphotonVertexZ/F:higgs_pt/F");
  
  std::string typeJet("jetTree_");
  typeJet += jetCollectionName;
  jetTree = fs_->make<TTree>(typeJet.c_str(),jetCollectionName.c_str());
  jetTree->Branch("pt"             ,&jInfo.pt           ,"pt/F" );
  jetTree->Branch("rawPt"          ,&jInfo.rawPt        ,"rawPt/F" );
  jetTree->Branch("bestPt"         ,&jInfo.bestPt       ,"bestPt/F" );
  jetTree->Branch("energy"         ,&jInfo.energy       ,"energy/F" );
  jetTree->Branch("mass"           ,&jInfo.mass         ,"mass/F" );
  jetTree->Branch("eta"            ,&jInfo.eta          ,"eta/F");
  jetTree->Branch("phi"            ,&jInfo.phi          ,"phi/F");
  jetTree->Branch("JetArea"        ,&jInfo.area         ,"area/F");
  
  jetTree->Branch("npart_0"        ,&jInfo.npart_0      ,"npart_0/I");
  jetTree->Branch("npart_5"        ,&jInfo.npart_5      ,"npart_5/I");
  jetTree->Branch("npart_10"       ,&jInfo.npart_10     ,"npart_10/I");
  jetTree->Branch("npart_20"       ,&jInfo.npart_20     ,"npart_20/I");
  
  jetTree->Branch("ncharged_0"     ,&jInfo.ncharged_0   ,"ncharged_0/I");
  jetTree->Branch("ncharged_5"     ,&jInfo.ncharged_5   ,"ncharged_5/I");
  jetTree->Branch("ncharged_10"    ,&jInfo.ncharged_10  ,"ncharged_10/I");
  jetTree->Branch("ncharged_20"    ,&jInfo.ncharged_20  ,"ncharged_20/I");
  
  jetTree->Branch("photon_0"       ,&jInfo.nphoton_0    ,"nphoton_0/I");
  jetTree->Branch("photon_5"       ,&jInfo.nphoton_5    ,"nphoton_5/I");
  jetTree->Branch("photon_10"      ,&jInfo.nphoton_10   ,"nphoton_10/I");
  jetTree->Branch("photon_20"      ,&jInfo.nphoton_20   ,"nphoton_20/I");
  
  jetTree->Branch("genJetPt"       ,&jInfo.genJetPt     ,"genJetPt/F" );
  jetTree->Branch("genJetEta"      ,&jInfo.genJetEta    ,"genJetEta/F" );
  jetTree->Branch("genJetMatch"    ,&jInfo.genJetMatch  ,"genJetMatch/I" );
  jetTree->Branch("genQuarkPt"     ,&jInfo.genQuarkPt   ,"genQuarkPt/F" );
  jetTree->Branch("genQuarkEta"    ,&jInfo.genQuarkEta  ,"genQuarkEta/F" );
  jetTree->Branch("genQuarkMatch"  ,&jInfo.genQuarkMatch,"genQuarkMatch/I" );
  jetTree->Branch("genQuarkPdgId"  ,&jInfo.genQuarkPdgId,"genQuarkPdfId/I" );
  
  jetTree->Branch("photonMatch"    ,&jInfo.photonMatch  ,"photonMatch/I" );
  jetTree->Branch("photondRmin"    ,&jInfo.photondRmin  ,"photondRmin/F" );
  
  jetTree->Branch("GenPhotonPt"    ,&jInfo.GenPhotonPt   ,"GenPhotonPt/F" );
  jetTree->Branch("GenPhotonEta"   ,&jInfo.GenPhotonEta  ,"GenPhotonEta/F" );
  jetTree->Branch("GenPhotonPhi"   ,&jInfo.GenPhotonPhi  ,"GenPhotonPhi/F" );
  
  genPartTree = fs_->make<TTree>("genPartTree","Check per-jet tree");
  genPartTree->Branch("pt"     ,&genInfo.pt      ,"pt/F" );
  genPartTree->Branch("eta"    ,&genInfo.eta     ,"eta/F");
  genPartTree->Branch("phi"    ,&genInfo.phi     ,"phi/F");
  genPartTree->Branch("status" ,&genInfo.status  ,"status/I" );
  genPartTree->Branch("pdgid"  ,&genInfo.pdgid   ,"pdgid/I");
  
  std::string typeGenJet("genJetTree_");
  typeGenJet += jetCollectionName;
  genJetTree = fs_->make<TTree>(typeGenJet.c_str(),jetCollectionName.c_str());
  genJetTree->Branch("pt"     ,&genJetInfo.pt      ,"pt/F" );
  genJetTree->Branch("eta"    ,&genJetInfo.eta     ,"eta/F");
  genJetTree->Branch("phi"    ,&genJetInfo.phi     ,"phi/F");
  
  genJetTree->Branch("recoJetPt"        ,&genJetInfo.recoJetPt      ,"recoJetPt/F" );
  genJetTree->Branch("recoJetRawPt"     ,&genJetInfo.recoJetRawPt   ,"recoJetRawPt/F" );
  genJetTree->Branch("recoJetBestPt"    ,&genJetInfo.recoJetBestPt  ,"recoJetBestPt/F");
  genJetTree->Branch("recoJetMatch"     ,&genJetInfo.recoJetMatch   ,"recoJetMatch/I");
  genJetTree->Branch("recoJetEta"       ,&genJetInfo.recoJetEta     ,"recoJetEta/F" );
  genJetTree->Branch("dR"               ,&genJetInfo.dR             ,"dR/F" );
}

void JetValidationTreeMaker::endJob() 
{

}

void JetValidationTreeMaker::initEventStructure() 
{
  
}


void JetValidationTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

typedef JetValidationTreeMaker FlashggJetValidationTreeMaker;
DEFINE_FWK_MODULE(FlashggJetValidationTreeMaker);
