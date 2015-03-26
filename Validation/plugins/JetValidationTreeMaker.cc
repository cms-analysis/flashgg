// -----------------------
// By Y.Haddad & L.Corpe  12/2014
// 
// Jet validation analyzer: tree maker for the jet studies in flashgg
// -----------------------

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
//#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/DataFormats/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"


#include "TTree.h"

// **********************************************************************

struct eventInfo {
  
  float genVertexZ;
  float zerothVertexZ;
  float diphotonVertexZ;
  int legacyEqZeroth;
  int nDiphotons;
  
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
  int legacyEqZeroth;
  int nDiphotons;
  float PUJetID_betaStar;
  float PUJetID_rms;
  int passesPUJetID;
  

  int   photonMatch;
  float photondRmin;
  float GenPhotonPt;
  
  
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
  float PUJetID_betaStar;
  float PUJetID_rms;
  int passesPUJetID;
  int legacyEqZeroth;
  int nDiphotons;
  
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
  
  //EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenDz_;
  //EDGetTokenT< VertexCandidateMap > vertexCandidateMapTokenAOD_;
  
  EDGetTokenT< edm::View<reco::GenParticle> > genPartToken_;
  EDGetTokenT< edm::View<reco::GenJet> >      genJetToken_;
  EDGetTokenT< edm::View<flashgg::Jet> >      jetDzToken_;
  EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diPhotonToken_;
  EDGetTokenT< View<reco::Vertex> > vertexToken_;
  
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
  
  bool        usePUJetID;
  
};

JetValidationTreeMaker::JetValidationTreeMaker(const edm::ParameterSet& iConfig):
  genPartToken_ (consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
  genJetToken_  (consumes<View<reco::GenJet> >(iConfig.getUntrackedParameter<InputTag> ("GenJetTag", InputTag("slimmedGenJets")))),
  jetDzToken_   (consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz"))),
  diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
  vertexToken_  (consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
  usePUJetID    (iConfig.getUntrackedParameter<bool>("UsePUJetID",false))
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


  
  Handle<View<reco::Vertex> > primaryVertices;
  iEvent.getByToken(vertexToken_,primaryVertices);
//  const PtrVector<reco::Vertex>& vtxs = primaryVertices->ptrVector();

  Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
  iEvent.getByToken(diPhotonToken_,diPhotons);
//  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();
  
  Handle<View<reco::GenParticle> > genParticles;
  iEvent.getByToken(genPartToken_,genParticles);
//  const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();
  
  Handle<View<reco::GenJet> > genJet;
  iEvent.getByToken(genJetToken_,genJet);
//  const PtrVector<reco::GenJet>& genJets = genJet->ptrVector();
  
  Handle<View<flashgg::Jet> > jetsDz;
  iEvent.getByToken(jetDzToken_,jetsDz);
//  const PtrVector<flashgg::Jet>& jetsDzPointers = jetsDz->ptrVector();
  
  int legacyEqZeroth =0;
  int nDiphotons =0;

  //std::cout <<" DEBUG 1"<<std::endl;
  nDiphotons = diPhotons->size();
  if(diPhotons->size()==0){
    legacyEqZeroth =1; //if there is no diphoton, we use 0th vertex anyway.
  } else {
    if(fabs(diPhotons->ptrAt(0)->vtx()->z() - primaryVertices->ptrAt(0)->z())<0.01){
      legacyEqZeroth =1;
    }
  }

  eInfo.nDiphotons = nDiphotons;
  eInfo.legacyEqZeroth = legacyEqZeroth;
  
  initEventStructure();
  
  eInfo.nJet = jetsDz->size();
  eInfo.nPV  = primaryVertices->size();
  eInfo.nSV  = 0;
  
  // ++  finding the photon-jet overlaping
  std::map<unsigned int, GenPhotonInfo> photonJet_id;
  
  for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
    genInfo.pt     = genParticles->ptrAt(genLoop)->pt() ;
    genInfo.eta    = genParticles->ptrAt(genLoop)->eta();
    genInfo.phi    = genParticles->ptrAt(genLoop)->phi();
    genInfo.status = genParticles->ptrAt(genLoop)->status();
    genInfo.pdgid  = int(genParticles->ptrAt(genLoop)->pdgId());
    
    std::map<float,unsigned int> minim;
    std::map<unsigned int,GenPhotonInfo> minim_info;
    float DeltaRmin=999.;
    
    if (genParticles->ptrAt(genLoop)->pdgId() == 22  && genParticles->ptrAt(genLoop)->numberOfDaughters() == 0){
      for( unsigned int jetLoop =0 ; jetLoop < jetsDz->size(); jetLoop++){
	GenPhotonInfo tmp_info;
	
	float dphi  = deltaPhi(jetsDz->ptrAt(jetLoop)->phi(),genParticles->ptrAt(genLoop)->phi());
	float deta  = jetsDz->ptrAt(jetLoop)->eta() -  genParticles->ptrAt(genLoop)->eta();
	float dr    =  std::sqrt(deta*deta + dphi*dphi);
	
	DeltaRmin = std::min (dr, DeltaRmin);
	minim[dr] = jetLoop; 
	
	// fill the info
	tmp_info.pt     = genParticles->ptrAt(genLoop)->pt ();
	tmp_info.eta    = genParticles->ptrAt(genLoop)->eta();
	tmp_info.phi    = genParticles->ptrAt(genLoop)->phi();	
	tmp_info.DRmin  = DeltaRmin;
	
	minim_info[jetLoop] = tmp_info;
      }
      unsigned int bestjetid = minim.find(DeltaRmin)->second;
      photonJet_id[bestjetid] = minim_info[bestjetid];
    }
    genPartTree->Fill();
  }
  
  std::map<unsigned int, jetInfo> recojetmap;
  for (unsigned int jdz = 0 ; jdz < jetsDz->size() ; jdz++) {
    
    jInfo.photondRmin = 999.;
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
    
    jInfo.pt            = jetsDz->ptrAt(jdz)->pt();
    jInfo.rawPt         = jetsDz->ptrAt(jdz)->correctedJet("Uncorrected").pt() ;
    
    if(jetCollectionName.find("PPI")>1 && jetCollectionName.find("PPI")<jetCollectionName.size()){
      jInfo.bestPt      = jetsDz->ptrAt(jdz)->correctedJet("Uncorrected").pt() ;
    } else {
      jInfo.bestPt      = jetsDz->ptrAt(jdz)->pt() ;
    }
    
    if( jetsDz->ptrAt(jdz)->genJet()){
      jInfo.genJetMatch           = 1;
      jInfo.genJetPt              = jetsDz->ptrAt(jdz)->genJet()->pt();
      jInfo.genJetEta             = jetsDz->ptrAt(jdz)->genJet()->eta();
      
    } else {
      jInfo.genJetPt              = -9999.;
      jInfo.genJetEta             = -9999.;
      jInfo.genJetMatch           = 0;
    }
    
    if( jetsDz->ptrAt(jdz)->genParton()){
      jInfo.genQuarkMatch           = 1;
      jInfo.genQuarkPt              = jetsDz->ptrAt(jdz)->genParton()->pt();
      jInfo.genQuarkEta             = jetsDz->ptrAt(jdz)->genParton()->eta();
      jInfo.genQuarkPdgId           = jetsDz->ptrAt(jdz)->genParton()->pdgId();
      
    } else {
      jInfo.genQuarkPt              = -9999.;
      jInfo.genQuarkEta             = -9999.;
      jInfo.genQuarkMatch           = 0;
      jInfo.genQuarkPdgId           = -9999;
    }
    //----------------------
    jInfo.energy           = jetsDz->ptrAt(jdz)->energy() ;
    jInfo.mass             = jetsDz->ptrAt(jdz)->mass() ;
    jInfo.eta              = jetsDz->ptrAt(jdz)->eta();
    jInfo.phi              = jetsDz->ptrAt(jdz)->phi();
    jInfo.area             = jetsDz->ptrAt(jdz)->jetArea();
    
    if(!(jetCollectionName.find("PPI")>1 && jetCollectionName.find("PPI")<jetCollectionName.size()) )
      {
	// use the di-photon vertex if the di-photon existe otherwise use the Vtx0
	
	if((diPhotons->size() > 0 ) && 
	   ((jetCollectionName.find("Leg")>1 && jetCollectionName.find("Leg")<jetCollectionName.size())||(jetCollectionName.length()   <3 )) ) { // for PF 

	  jInfo.PUJetID_betaStar        = jetsDz->ptrAt(jdz)->betaStar(diPhotons->ptrAt(0)->vtx());
	  jInfo.PUJetID_rms             = jetsDz->ptrAt(jdz)->rms(diPhotons->ptrAt(0)->vtx());
	  jInfo.passesPUJetID           = jetsDz->ptrAt(jdz)->passesPuJetId(diPhotons->ptrAt(0)->vtx());

	} else {

	  jInfo.PUJetID_betaStar        = jetsDz->ptrAt(jdz)->betaStar(primaryVertices->ptrAt(0));
	  jInfo.PUJetID_rms             = jetsDz->ptrAt(jdz)->rms(primaryVertices->ptrAt(0));
	  jInfo.passesPUJetID           = jetsDz->ptrAt(jdz)->passesPuJetId(primaryVertices->ptrAt(0));

	}
      } else {

      jInfo.PUJetID_betaStar        = -999.;
      jInfo.PUJetID_rms             = -999.;
      jInfo.passesPUJetID           = -999;
    }

    jInfo.nDiphotons                = nDiphotons;
    jInfo.legacyEqZeroth            = legacyEqZeroth;

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
    //for (unsigned int i = 0 ; i < jetsDz->ptrAt(jdz)->jetConstituentsQuick().size() ; i++){
    //
    //  const reco::Candidate* icand = jetsDz->ptrAt(jdz)->jetConstituentsQuick()[i];
    //  float candPt                 = icand->pt();
    //  float candCharge             = icand->charge();
    //
    //  jInfo.npart_0++;
    //  if(candCharge != 0) jInfo.ncharged_0++;
    //  if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_0++;
    //
    //  if(candPt > 5){
    //	jInfo.npart_5++;
    //	if(candCharge != 0) jInfo.ncharged_5++;
    //	if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_5++;
    //  }
    //  if(candPt > 10){
    //	jInfo.npart_10++;
    //	if(candCharge != 0) jInfo.ncharged_10++;
    //	if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_10++;
    //  }
    //  if(candPt > 20){
    //	jInfo.npart_20++;
    //	if(candCharge != 0) jInfo.ncharged_20++;
    //	if(candCharge == 0 && icand->pdgId() == 22) jInfo.nphoton_20++;
    //  }
    //}

    recojetmap[jdz] = jInfo;
    jetTree->Fill();
  }

  for( unsigned int genLoop =0 ; genLoop < genJet->size(); genLoop++){

    genJetInfo.recoJetPt       = -999.;
    genJetInfo.recoJetRawPt    = -999.;
    genJetInfo.recoJetBestPt   = -999.;
    genJetInfo.recoJetMatch    =  0   ;
    genJetInfo.recoJetEta      = -999.;
    genJetInfo.dR              = -999.;
    genJetInfo.pt              = -999.;
    genJetInfo.eta             = -999.;
    genJetInfo.phi             = -999.;
    genJetInfo.PUJetID_betaStar= -999.;
    genJetInfo.PUJetID_rms     = -999.;
    genJetInfo.passesPUJetID   = -999;
    genJetInfo.nDiphotons      = nDiphotons;

    if (genJet->ptrAt(genLoop)->pt() <20) { continue;}

    genJetInfo.pt     = genJet->ptrAt(genLoop)->pt() ;
    genJetInfo.eta    = genJet->ptrAt(genLoop)->eta();
    genJetInfo.phi    = genJet->ptrAt(genLoop)->phi();

    float deta;
    float dphi;
    float dr  ;

    for (unsigned int recoLoop=0; recoLoop <  jetsDz->size(); recoLoop++){


      if(jetsDz->ptrAt(recoLoop)->pt() < 5) continue;

      deta= jetsDz->ptrAt(recoLoop)->eta() - 	 genJet->ptrAt(genLoop)->eta();
      dphi= deltaPhi(jetsDz->ptrAt(recoLoop)->phi(),genJet->ptrAt(genLoop)->phi());
      dr = std::sqrt(deta*deta + dphi*dphi);

      if (dr < 0.4 ) {
	genJetInfo.dR      =  dr;
	genJetInfo.recoJetPt       = jetsDz->ptrAt(recoLoop)->pt() ;
	genJetInfo.recoJetRawPt    = jetsDz->ptrAt(recoLoop)->correctedJet("Uncorrected").pt()  ;

	// add the photon overlaping info 
	jetInfo tmpjetinfo = recojetmap[recoLoop];
	genJetInfo.photonMatch     = tmpjetinfo.photonMatch;
	genJetInfo.GenPhotonPt     = tmpjetinfo.GenPhotonPt;//recojetmap[recojetmap].GenPhotonPt  ;
	genJetInfo.photondRmin     = tmpjetinfo.photondRmin;//recojetmap[recojetmap].photondRmin  ;



	if(!(jetCollectionName.find("PPI")>1 && jetCollectionName.find("PPI")<jetCollectionName.size())){
	  genJetInfo.recoJetBestPt   =  jetsDz->ptrAt(recoLoop)->correctedJet("Uncorrected").pt() ;
	  if( (diPhotons->size() > 0 ) && 
	      ((jetCollectionName.find("Leg")>1 && jetCollectionName.find("Leg")<jetCollectionName.size())||(jetCollectionName.length()   <3 )) ) { // for PF

	    genJetInfo.PUJetID_betaStar        = jetsDz->ptrAt(recoLoop)->betaStar(diPhotons->ptrAt(0)->vtx());
	    genJetInfo.PUJetID_rms             = jetsDz->ptrAt(recoLoop)->rms(diPhotons->ptrAt(0)->vtx());
	    genJetInfo.passesPUJetID           = jetsDz->ptrAt(recoLoop)->passesPuJetId(diPhotons->ptrAt(0)->vtx());

	  } else {

	    genJetInfo.PUJetID_betaStar        = jetsDz->ptrAt(recoLoop)->betaStar(primaryVertices->ptrAt(0));
	    genJetInfo.PUJetID_rms             = jetsDz->ptrAt(recoLoop)->rms(primaryVertices->ptrAt(0));
	    genJetInfo.passesPUJetID           = jetsDz->ptrAt(recoLoop)->passesPuJetId(primaryVertices->ptrAt(0));

	  }
	} else {

	  genJetInfo.recoJetBestPt   = jetsDz->ptrAt(recoLoop)->pt()  ;
	  genJetInfo.PUJetID_betaStar  = -999.;
	  genJetInfo.PUJetID_rms       = -999.;
	  genJetInfo.passesPUJetID     = -999;

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
  // +++ trees 
  std::string type("eventTree_");
  type += jetCollectionName;

  eventTree = fs_->make<TTree>(type.c_str(),jetCollectionName.c_str());
  eventTree->Branch("eventBranch",&eInfo.legacyEqZeroth,"legacyEqZeroth/I:nDiphotons/I");

  std::string typeJet("jetTree_");
  typeJet += jetCollectionName;
  jetTree = fs_->make<TTree>(typeJet.c_str(),jetCollectionName.c_str());
  jetTree->Branch("pt"              ,&jInfo.pt                ,"pt/F" );
  jetTree->Branch("rawPt"           ,&jInfo.rawPt             ,"rawPt/F" );
  jetTree->Branch("bestPt"          ,&jInfo.bestPt            ,"bestPt/F" );
  jetTree->Branch("energy"          ,&jInfo.energy            ,"energy/F" );
  jetTree->Branch("mass"            ,&jInfo.mass              ,"mass/F" );
  jetTree->Branch("eta"             ,&jInfo.eta               ,"eta/F");
  jetTree->Branch("phi"             ,&jInfo.phi               ,"phi/F");
  jetTree->Branch("PUJetID_betaStar",&jInfo.PUJetID_betaStar  ,"PUJetID_betaStar/F");
  jetTree->Branch("PUJetID_rms"     ,&jInfo.PUJetID_rms       ,"PUJetID_rms/F");
  jetTree->Branch("passesPUJetID"   ,&jInfo.passesPUJetID     ,"passesPUJetID/I");
  jetTree->Branch("JetArea"         ,&jInfo.area              ,"area/F");
  jetTree->Branch("nDiphotons"      ,&jInfo.nDiphotons        ,"nDiphotons/I");

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

  genJetTree->Branch("recoJetPt"        ,&genJetInfo.recoJetPt        ,"recoJetPt/F" );
  genJetTree->Branch("recoJetRawPt"     ,&genJetInfo.recoJetRawPt     ,"recoJetRawPt/F" );
  genJetTree->Branch("recoJetBestPt"    ,&genJetInfo.recoJetBestPt    ,"recoJetBestPt/F");
  genJetTree->Branch("recoJetMatch"     ,&genJetInfo.recoJetMatch     ,"recoJetMatch/I");
  genJetTree->Branch("recoJetEta"       ,&genJetInfo.recoJetEta       ,"recoJetEta/F" );
  genJetTree->Branch("dR"               ,&genJetInfo.dR               ,"dR/F" );
  genJetTree->Branch("PUJetID_betaStar" ,&genJetInfo.PUJetID_betaStar ,"PUJetID_betaStar/F");
  genJetTree->Branch("PUJetID_rms"      ,&genJetInfo.PUJetID_rms      ,"PUJetID_rms/F");
  genJetTree->Branch("passesPUJetID"    ,&genJetInfo.passesPUJetID    ,"passesPUJetID/I");
  genJetTree->Branch("nDiphotons"       ,&genJetInfo.nDiphotons        ,"nDiphotons/I");

  genJetTree->Branch("photonMatch"      ,&genJetInfo.photonMatch  ,"photonMatch/I" );
  genJetTree->Branch("photondRmin"      ,&genJetInfo.photondRmin  ,"photondRmin/F" );
  genJetTree->Branch("GenPhotonPt"      ,&genJetInfo.GenPhotonPt  ,"GenPhotonPt/F" );

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
