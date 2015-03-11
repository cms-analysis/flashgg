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

#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class FlashggTreeMakerWithTags : public edm::EDAnalyzer {
	public:
		explicit FlashggTreeMakerWithTags(const edm::ParameterSet&);
		~FlashggTreeMakerWithTags();

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
		EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> >  diPhotonToken_; //
		EDGetTokenT<edm::View<pat::MET> >  METToken_; // LDC work-in-progress adding this!
		EDGetTokenT<edm::View<PileupSummaryInfo> >  PileUpToken_; // LDC work-in-progress adding this!
		edm::InputTag rhoFixedGrid_;

		TTree *flashggTreeWithTags;

		Int_t run;
		Int_t lumis;
		Int_t event;
		Float_t dRphojet1;
		Float_t dRphojet2;
		Float_t njets10;
		Float_t njets15;
		Float_t njets20;
		Float_t itype;
		Float_t nvtx;
		Float_t rho;
		Float_t xsec_weight;
		Float_t full_weight;
		Float_t pu_weight;
		Float_t pu_n;
		Float_t mass;
		Float_t dipho_pt;
		Float_t full_cat;
		Float_t et1;
		Float_t et2;
		Float_t eta1;
		Float_t eta2;
		Float_t r91;
		Float_t r92;
		Float_t sieie1;
		Float_t sieie2;
		Float_t hoe1;
		Float_t hoe2;
		Float_t sigmaEoE1;
		Float_t sigmaEoE2;
		Float_t ptoM1;
		Float_t ptoM2;
		Float_t isEB1;
		Float_t isEB2;
		Float_t chiso1;
		Float_t chiso2;
		Float_t chisow1;
		Float_t chisow2;
		Float_t phoiso1;
		Float_t phoiso2;
		Float_t phoiso041;
		Float_t phoiso042;
		Float_t ecaliso03_1;
		Float_t ecaliso03_2;
		Float_t hcaliso03_1;
		Float_t hcaliso03_2;
		Float_t trkiso03_1;
		Float_t trkiso03_2;
		Float_t pfchiso2_1;
		Float_t pfchiso2_2;
		Float_t sieip1;
		Float_t sieip2;
		Float_t etawidth1;
		Float_t phiwidth1;
		Float_t etawidth2;
		Float_t phiwidth2;
		Float_t regrerr1;
		Float_t regrerr2;
		Float_t cosphi;
		Float_t genmatch1;
		Float_t genmatch2;
		Float_t cicpf4cutlevel1;
		Float_t cicpf4cutlevel2;
		Float_t idmva1;
		Float_t idmva2;
		Float_t vbfcat;
		Float_t MET;
		Float_t MET_phi;
		Float_t isorv1;
		Float_t isowv1;
		Float_t isorv2;
		Float_t isowv2;
		Float_t s4ratio1;
		Float_t s4ratio2;
		Float_t effSigma1;
		Float_t effSigma2;
		Float_t scraw1;
		Float_t scraw2;
		Float_t vtx_x;
		Float_t vtx_y;
		Float_t vtx_z;
		Float_t gv_x;
		Float_t gv_y;
		Float_t gv_z;
		Float_t dijet_leadEta;
		Float_t dijet_subleadEta;
		Float_t dijet_LeadJPt;
		Float_t dijet_SubJPt;
		Float_t dijet_dEta;
		Float_t dijet_Zep;
		Float_t dijet_dPhi;
		Float_t dijet_Mjj;
		Float_t dijet_MVA;
		Float_t bdt_combined;
		Float_t issyst;
		Float_t name1;
		Float_t sigmaMrvoM;
		Float_t sigmaMwvoM;
		Float_t vtxprob;
		Float_t ptbal;
		Float_t ptasym;
		Float_t logspt2;
		Float_t p2conv;
		Float_t nconv;
		Float_t vtxmva;
		Float_t vtxdz;
		Float_t dipho_mva;
		Float_t dipho_mva_cat;
		
		//Tag Categories
		Int_t flash_Untagged_Category;
		Int_t flash_VBFTag_Category;

		edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_; // SCZ work-in-progress adding this!
		edm::EDGetTokenT<edm::View<flashgg::DiPhotonUntaggedCategory> > DiPhotonUntaggedToken_;
		edm::EDGetTokenT<edm::View<flashgg::VBFTag> > VBFTagToken_;
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
FlashggTreeMakerWithTags::FlashggTreeMakerWithTags(const edm::ParameterSet& iConfig):
	vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
	genParticleToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
	vertexCandidateMapTokenDz_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagDz"))),
	vertexCandidateMapTokenAOD_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagAOD"))),
	jetTokenDz_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz"))),
	// jetTokenRecoBasedMap_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagRecoBasedMap"))),
	diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
	METToken_(consumes<View<pat::MET> >(iConfig.getUntrackedParameter<InputTag> ("METTag", InputTag("slimmedMETs")))),
	PileUpToken_(consumes<View<PileupSummaryInfo> >(iConfig.getUntrackedParameter<InputTag> ("PileUpTag", InputTag("addPileupInfo")))),
  DiPhotonUntaggedToken_(consumes<View<flashgg::DiPhotonUntaggedCategory> >(iConfig.getUntrackedParameter<InputTag> ("UntaggedTag", InputTag("flashggUntaggedCategory")))),
  VBFTagToken_(consumes<View<flashgg::VBFTag> >(iConfig.getUntrackedParameter<InputTag> ("VBFTag", InputTag("flashggVBFTag"))))
	//  jetTokenReco_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagReco")))
{
	rhoFixedGrid_ = iConfig.getParameter<edm::InputTag>("rhoFixedGridCollection");
//	DiphotonMva_initialized = false;
//	VbfMva_initialized = false;
//	VbfDiphotonMva_initialized = false;
}

FlashggTreeMakerWithTags::~FlashggTreeMakerWithTags()
{

}

void
FlashggTreeMakerWithTags::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

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

	Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
	iEvent.getByToken(diPhotonToken_,diPhotons);
	const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

	Handle<View<pat::MET> > METs;
	iEvent.getByToken(METToken_,METs);
	const PtrVector<pat::MET>& METPointers = METs->ptrVector();

	Handle<View< PileupSummaryInfo> > PileupInfos;
	iEvent.getByToken(PileUpToken_,PileupInfos);
	const PtrVector<PileupSummaryInfo>& PileupInfoPointers = PileupInfos->ptrVector();

	Handle<double> rhoHandle; // the old way for now...move to getbytoken?
	iEvent.getByLabel(rhoFixedGrid_, rhoHandle );

	Handle<View<flashgg::DiPhotonUntaggedCategory> > UntaggedCategory;
	iEvent.getByToken(DiPhotonUntaggedToken_,UntaggedCategory);
	const PtrVector<flashgg::DiPhotonUntaggedCategory>& UntaggedCategoryPointers = UntaggedCategory->ptrVector();

	Handle<View<flashgg::VBFTag> > VBFTag;
	iEvent.getByToken(VBFTagToken_,VBFTag);
	const PtrVector<flashgg::VBFTag>& VBFTagPointers = VBFTag->ptrVector();
	

	//---------> njetsxx = number of jets with et > xx
	njets10 = 0.;
	njets15 = 0.;
	njets20 = 0.;
	for (UInt_t jetLoop =0; jetLoop < jetPointersDz.size() ; jetLoop++){

		Float_t et = jetPointersDz[jetLoop]->et();

		if( et > 10.){
			njets10 = njets10 + 1.;
		}
		if( et > 15.){
			njets15 = njets15 + 1.;
		}
		if( et > 20.){
			njets20 = njets20 + 1.;
		}
	}

	//------>dRphojet ?? need to clarify meaning of var
	//Need ti implement event categorisation first, ie is it VBF


	//------->event info
	run = iEvent.eventAuxiliary().run(); 
	event = iEvent.eventAuxiliary().event(); 
	lumis = iEvent.eventAuxiliary().luminosityBlock(); 

	//------>itype ?? need to determine how bets to implement this FIXME.
	itype = -1 ;// placeholder. Need to be able to access this one the fly based on the input file or config.
	// itype <0, Signal MC
	// itype =0, data
	// itype >0, background MC
	//----> nvtx, numver of primary vertices
	nvtx = vtxs.size();

	//-----> rho = energy density

	rho = *(rhoHandle.product());

	//------> weights and PU , leaving empty for now.. FIXME


	if (itype !=0 ){
		xsec_weight = 0;
		full_weight = 0;
		pu_weight = -1;
		pu_n = 0;
		gv_x = -9999.;
		gv_y = -9999.;
		gv_z = -9999.;

		for( unsigned int PVI = 0; PVI < PileupInfoPointers.size(); ++PVI) {	
			Int_t pu_bunchcrossing = PileupInfoPointers[PVI]->getBunchCrossing();
			if (pu_bunchcrossing ==0) {
				pu_n = PileupInfoPointers[PVI]->getPU_NumInteractions();
			}
		}

		for( unsigned int genLoop =0 ; genLoop < gens.size(); genLoop++){
			if( gens[genLoop]->pdgId() == 25) { //might need to be changed for background MC samples...
				gv_x = gens[genLoop]->vx();
				gv_y = gens[genLoop]->vy();
				gv_z = gens[genLoop]->vz();
			}

			break;
		}
	}

	else{
		xsec_weight = -1;
		full_weight = -1;
		pu_weight = -1;
		pu_n = -1;
		gv_x = -9999.;
		gv_y = -9999.;
		gv_z = -9999.;
	}


	//------> higgs diphoton candidate properties
	// We have a problem here, because until we are sure that there is consistently one higgs diphoton candiate, we 
	// will either need to select one of the multiple diphoton pairs. The following does that, by choosing the one closets to 125GeV..	
	float higgsMassDifference =9999.; 
	float higgsMass =125. ;
	Int_t higgsCandPresent = 0; //Is 0 is there are no candidates in event, set to 1 if there is at least one candidate pair.
	Int_t candIndex = 9999; //This int will store the index of the best higgs diphoton candidate...

	for (unsigned int diphotonlooper =0; diphotonlooper < diPhotonPointers.size() ; diphotonlooper++){
		if  (fabs(diPhotonPointers[diphotonlooper]->mass() - higgsMass) < higgsMassDifference){
			higgsMassDifference = fabs(diPhotonPointers[diphotonlooper]->mass() - higgsMass);
			candIndex = diphotonlooper;
			higgsCandPresent=1;
		}
	}

	if(higgsCandPresent){
		mass = diPhotonPointers[candIndex]->mass();
		dipho_pt = diPhotonPointers[candIndex]->pt();
	}
	else{
		mass = 0;
		dipho_pt = 0;
	}



	//------->full_cat FIXME leaving blank for now, need to implement if/when events are categoriesed. Discuss event interpretatrion.
	full_cat =0;

	//------>MET info
	if (METPointers.size() != 1) { std::cout << "WARNING - #MET is not 1" << std::endl;}
	MET = METPointers[0]->pt();
	MET_phi = METPointers[0]->phi();


	//-------> individual photon properties
	if(higgsCandPresent){
		//PHOTON1
		et1 = diPhotonPointers[candIndex]->leadingPhoton()->et();
		eta1 = diPhotonPointers[candIndex]->leadingPhoton()->eta();
	//	float phi1 = diPhotonPointers[candIndex]->leadingPhoton()->phi();
		r91 = diPhotonPointers[candIndex]->leadingPhoton()->r9();
		sieie1 = diPhotonPointers[candIndex]->leadingPhoton()->sigmaIetaIeta();
		hoe1 = diPhotonPointers[candIndex]->leadingPhoton()->hadronicOverEm();
		sigmaEoE1 = diPhotonPointers[candIndex]->leadingPhoton()->getSigEOverE();
		ptoM1 = diPhotonPointers[candIndex]->leadingPhoton()->pt()/mass;
		//---> Isolation variables, unsure if correct methods used...
		//Mthods from reco::photon
		//chiso1 = diPhotonPointers[candIndex]->leadingPhoton()->chargedHadronIso();
		//chisow1 = diPhotonPointers[candIndex]->leadingPhoton()->chargedHadronIsoWrongVtx();
		//phoiso1 = diPhotonPointers[candIndex]->leadingPhoton()->photonIso();
		//phoiso041 = diPhotonPointers[candIndex]->leadingPhoton()->photonIso(); //how does this differ??? FIXME
		//Methods from flashgg::photon
		chiso1 = diPhotonPointers[candIndex]->leadingPhoton()->getpfChgIso03WrtVtx(diPhotonPointers[candIndex]->getVertex());
		chisow1 = diPhotonPointers[candIndex]->leadingPhoton()->getpfChgIsoWrtWorstVtx04();//no flashgg method for come radius 04... ok to use 0.3?
		phoiso1 = diPhotonPointers[candIndex]->leadingPhoton()->getpfPhoIso03();
		phoiso041 = diPhotonPointers[candIndex]->leadingPhoton()->getpfPhoIso04(); //unsure of default radius?
		ecaliso03_1 = diPhotonPointers[candIndex]->leadingPhoton()->ecalRecHitSumEtConeDR03();
		hcaliso03_1 = diPhotonPointers[candIndex]->leadingPhoton()->hcalTowerSumEtConeDR03();
		trkiso03_1 = diPhotonPointers[candIndex]->leadingPhoton()->trkSumPtHollowConeDR03();
		pfchiso2_1 = diPhotonPointers[candIndex]->leadingPhoton()->getpfChgIso02WrtVtx(diPhotonPointers[candIndex]->getVertex());
		isorv1 = (chiso1 + phoiso1 + 2.5 - rho*0.09)*(50./et1); //used in cic analysis, might not be useful for us but we have what we need in hand so adding anyway.
		isowv1 = ( phoiso1 + chisow1 + 2.5 - rho*0.23)*(50./et1);


		isEB1 = diPhotonPointers[candIndex]->leadingPhoton()->isEB();
		sieip1 = diPhotonPointers[candIndex]->leadingPhoton()->getSieip();
		etawidth1 = diPhotonPointers[candIndex]->leadingPhoton()->superCluster()->etaWidth();
		phiwidth1 = diPhotonPointers[candIndex]->leadingPhoton()->superCluster()->phiWidth();
		regrerr1 = sigmaEoE1 * diPhotonPointers[candIndex]->leadingPhoton()->energy();
		idmva1 = diPhotonPointers[candIndex]->leadingPhoton()->getPhoIdMvaDWrtVtx(diPhotonPointers[candIndex]->getVertex());
		s4ratio1 =  diPhotonPointers[candIndex]->leadingPhoton()->getS4();
		effSigma1 =  diPhotonPointers[candIndex]->leadingPhoton()->getESEffSigmaRR();
		scraw1 =  diPhotonPointers[candIndex]->leadingPhoton()->superCluster()->rawEnergy();

		//PHOTON 2
		et2 = diPhotonPointers[candIndex]->subLeadingPhoton()->et();
		eta2 = diPhotonPointers[candIndex]->subLeadingPhoton()->eta();
	//	float phi2 = diPhotonPointers[candIndex]->subLeadingPhoton()->phi();
		r92 = diPhotonPointers[candIndex]->subLeadingPhoton()->r9();
		sieie2 = diPhotonPointers[candIndex]->subLeadingPhoton()->sigmaIetaIeta();
		hoe2 = diPhotonPointers[candIndex]->subLeadingPhoton()->hadronicOverEm();
		sigmaEoE2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getSigEOverE();
		ptoM2 = diPhotonPointers[candIndex]->subLeadingPhoton()->pt()/mass;
		isEB2 = diPhotonPointers[candIndex]->subLeadingPhoton()->isEB();
		//---> Isolation variables, unsure if correct methods used...
		//Mthods from reco::photon
		//chiso2 = diPhotonPointers[candIndex]->subLeadingPhoton()->chargedHadronIso();
		//chisow2 = diPhotonPointers[candIndex]->subLeadingPhoton()->chargedHadronIsoWrongVtx();
		//phoiso2 = diPhotonPointers[candIndex]->subLeadingPhoton()->photonIso();
		//phoiso042 = diPhotonPointers[candIndex]->subLeadingPhoton()->photonIso(); //how does this differ??? FIXME
		//Methods from flashgg::photon
		chiso2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfChgIso03WrtVtx(diPhotonPointers[candIndex]->getVertex());
		chisow2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfChgIsoWrtWorstVtx04();//no flashgg method for come radius 04... ok to use 0.3?
		phoiso2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfPhoIso03();


		phoiso042 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfPhoIso04(); //unsure of default radius?
		ecaliso03_2 = diPhotonPointers[candIndex]->subLeadingPhoton()->ecalRecHitSumEtConeDR03();
		hcaliso03_2 = diPhotonPointers[candIndex]->subLeadingPhoton()->hcalTowerSumEtConeDR03();
		trkiso03_2 = diPhotonPointers[candIndex]->subLeadingPhoton()->trkSumPtHollowConeDR03();
		pfchiso2_2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfChgIso02WrtVtx(diPhotonPointers[candIndex]->getVertex());
		isorv2 = (chiso2 + phoiso2 + 2.5 - rho*0.09)*(50./et2);
		isowv2 = ( phoiso2 + chisow2 + 2.5 - rho*0.23)*(50./et2);

		sieip2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getSieip();
		etawidth2 = diPhotonPointers[candIndex]->subLeadingPhoton()->superCluster()->etaWidth();
		phiwidth2 = diPhotonPointers[candIndex]->subLeadingPhoton()->superCluster()->phiWidth();
		regrerr2 = sigmaEoE1 * diPhotonPointers[candIndex]->subLeadingPhoton()->energy();
		idmva2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getPhoIdMvaDWrtVtx(diPhotonPointers[candIndex]->getVertex());
		s4ratio2 =  diPhotonPointers[candIndex]->subLeadingPhoton()->getS4();
		effSigma2 =  diPhotonPointers[candIndex]->subLeadingPhoton()->getESEffSigmaRR();
		scraw2 =  diPhotonPointers[candIndex]->subLeadingPhoton()->superCluster()->rawEnergy();

		//-----> Cos of photon delta phi
		cosphi = cos(diPhotonPointers[candIndex]->leadingPhoton()->phi()- diPhotonPointers[candIndex]->subLeadingPhoton()->phi());

		//-------> vtx info
		vtx_x= diPhotonPointers[candIndex]->getVertex()->x();
		vtx_y= diPhotonPointers[candIndex]->getVertex()->y();
		vtx_z= diPhotonPointers[candIndex]->getVertex()->z();


		// ---------> Gen match?
		//
		genmatch1= 0;
		genmatch2= 0;
		if(itype!=0){


			for(unsigned int ip=0;ip<gens.size();++ip) {
				if( gens[ip]->status() != 1 || gens[ip]->pdgId() != 22 ) {
					continue;
				}
				if( diPhotonPointers[candIndex]->leadingPhoton()->et()< 20. || fabs(diPhotonPointers[candIndex]->leadingPhoton()->eta()) > 3. ) { continue; }
				if( gens[ip]->motherRef(0)->pdgId() <= 25 ) {
					float deta =  diPhotonPointers[candIndex]->leadingPhoton()->eta() - gens[ip]->eta();
					float dphi =  deltaPhi(diPhotonPointers[candIndex]->leadingPhoton()->phi(),gens[ip]->phi());
					float dr = sqrt(deta*deta + dphi*dphi);
					float pt_change = (diPhotonPointers[candIndex]->leadingPhoton()->et() - gens[ip]->et())/gens[ip]->et();
					if (dr<0.3 && fabs(pt_change) < 0.5) {
						genmatch1 = true;
						break;
					}
				}
			}


			for(unsigned int ip=0;ip<gens.size();++ip) {
				if( gens[ip]->status() != 1 || gens[ip]->pdgId() != 22 ) {
					continue;
				}
				if( diPhotonPointers[candIndex]->subLeadingPhoton()->et()< 20. || fabs(diPhotonPointers[candIndex]->subLeadingPhoton()->eta()) > 3. ) { continue; }
				if( gens[ip]->motherRef(0)->pdgId() <= 25 ) {
					float deta =  diPhotonPointers[candIndex]->subLeadingPhoton()->eta() - gens[ip]->eta();
					float dphi =  deltaPhi(diPhotonPointers[candIndex]->subLeadingPhoton()->phi(),gens[ip]->phi());
					float dr = sqrt(deta*deta + dphi*dphi);
					float pt_change = (diPhotonPointers[candIndex]->subLeadingPhoton()->et() - gens[ip]->et())/gens[ip]->et();
					if (dr<0.3 && fabs(pt_change) < 0.5) {
						genmatch1 = true;
						break;
					}
				}
			}
		}


		 flash_Untagged_Category= -1; // so that there is at least some value to fill even if not part of category
		for (unsigned int untaggedLooper =0; untaggedLooper < UntaggedCategoryPointers.size(); untaggedLooper++)
		{
		if (UntaggedCategoryPointers[untaggedLooper]->getDiPhotonIndex() != candIndex) continue;
		
		sigmaMrvoM = UntaggedCategoryPointers[untaggedLooper]->diPhotonMVA().sigmarv;// Not sure if these variables are being filled properly... 
		sigmaMwvoM = UntaggedCategoryPointers[untaggedLooper]->diPhotonMVA().sigmawv; 
		vtxprob = UntaggedCategoryPointers[untaggedLooper]->diPhotonMVA().vtxprob ;
		dipho_mva = UntaggedCategoryPointers[untaggedLooper]-> diPhotonMVA().getMVAValue()  ;
		flash_Untagged_Category =  UntaggedCategoryPointers[untaggedLooper]->getCategoryNumber();
		dipho_mva_cat =(float) flash_Untagged_Category;

/*		std::cout << "DiPho Debug"<< std::endl;
		std::cout << sigmaMrvoM << std::endl;
		std::cout << sigmaMwvoM <<std::endl;
		std::cout << vtxprob <<std::endl;
		std::cout << dipho_mva <<std::endl;
		std::cout 	<< flash_Untagged_Category <<std::endl; //for debug
		std::cout << dipho_mva_cat <<std::endl;*/
	break;
	}

		ptbal = diPhotonPointers[candIndex]->getPtBal();
		ptasym = diPhotonPointers[candIndex]->getPtAsym();
		logspt2 = diPhotonPointers[candIndex]->getLogSumPt2();
		p2conv = diPhotonPointers[candIndex]->getPullConv(); 
		nconv = diPhotonPointers[candIndex]->getNConv(); 
		vtxmva = diPhotonPointers[candIndex]->getVtxProbMVA();
		vtxdz = diPhotonPointers[candIndex]->getDZ1(); 




		flash_VBFTag_Category =-1;// so that there is at least some value to fill even if untagged

		for (unsigned int vbfLooper =0; vbfLooper < VBFTagPointers.size(); vbfLooper++)
		{
	//	std::cout << std::endl<< "candIndex " << candIndex << "	vbfLoop " << vbfLooper << " diPhoIndex " << 	VBFTagPointers[vbfLooper]->getDiPhotonIndex() << std::endl ; 
		if (VBFTagPointers[vbfLooper]->getDiPhotonIndex()  != candIndex) continue;

		dijet_leadEta = VBFTagPointers[vbfLooper]->VBFMVA().dijet_leadEta; 
		dijet_subleadEta =VBFTagPointers[vbfLooper]->VBFMVA().dijet_subleadEta; 
		dijet_LeadJPt =VBFTagPointers[vbfLooper]->VBFMVA().   dijet_LeadJPt; 
		dijet_SubJPt = VBFTagPointers[vbfLooper]->VBFMVA().   dijet_SubJPt; 
		dijet_Zep = VBFTagPointers[vbfLooper]->VBFMVA().      dijet_Zep; 
		dijet_Mjj = VBFTagPointers[vbfLooper]->VBFMVA().      dijet_Mjj; 
		dijet_MVA = VBFTagPointers[vbfLooper]->VBFMVA().      VBFMVAValue();
		bdt_combined =VBFTagPointers[vbfLooper]->VBFDiPhoDiJetMVA().VBFDiPhoDiJetMVAValue();     
		flash_VBFTag_Category = VBFTagPointers[vbfLooper]->getCategoryNumber();
		

	//	std::cout<< std::endl << "VBFDebug"<< std::endl;
	//	std::cout << dijet_leadEta <<std::endl;
	if (flash_VBFTag_Category >2 )
		std::cout << "VBF cat " << flash_VBFTag_Category  <<std::endl;
	//	std::cout << bdt_combined << std::endl;
	//	std::cout << "Jets test " <<  VBFTagPointers[vbfLooper]->leadingJet().pt() << "	"<<   VBFTagPointers[vbfLooper]->subLeadingJet().eta() << std::endl; 
		break;
		}

		}
		else{

			et1 =  -999.;
			eta1 = -999.;
			r91 =  -999.;
			sieie1 =-999.; 
			hoe1 =-999.;
			sigmaEoE1 =-999.; 
			ptoM1 =-999.;
			isEB1 =-999.;
			chiso1 =-999.;
			chisow1 =-999.; 
			phoiso1 = -999.;
			phoiso041 =-999.; 
			ecaliso03_1 =-999.;
			hcaliso03_1 =-999.;
			trkiso03_1 = -999.;
			pfchiso2_1 = -999.;
			isorv1 = -999.;
			isowv1 = -999.;
			sieip1 = -999.;
			etawidth1 =-999.; 
			phiwidth1 = -999.;
			regrerr1 = -999.;
			idmva1 = -999.;
			s4ratio1 =-999.;  
			effSigma1 =-999.;  
			scraw1 =  -999.;
			et2 =  -999.;
			eta2 = -999.;
			r92 =-999.;
			sieie2 =-999.; 
			hoe2 =-999.;
			sigmaEoE2 =-999.; 
			ptoM2 =-999.;
			isEB2 =-999.;
			isorv2 = -999.;
			isowv2 = -999.;
			chiso2 =-999.;
			chisow2 =-999.; 
			phoiso2 = -999.;
			phoiso042 =-999.; 
			ecaliso03_2 =-999.;
			hcaliso03_2 =-999.;
			trkiso03_2 = -999.;
			pfchiso2_2 = -999.;
			sieip2 = -999.;
			etawidth2 =-999.; 
			phiwidth2 = -999.;
			regrerr2 = -999.;
			idmva2 = -999.;
			s4ratio2 =-999.;  
			effSigma2 =-999.;  
			scraw2 =  -999.;
			cosphi = -999.;
			vtx_x = -999.;
			vtx_y = -999.;
			vtx_z =-999.;
			genmatch1 = -999.;
			genmatch2 = -999.;
			sigmaMrvoM = -999.;
			sigmaMwvoM = -999.;
			vtxprob =-999.;
			ptbal =-999.;
			ptasym = -999.;
			logspt2 = -999.;
			p2conv =-999. ;
			nconv =-999. ;
			vtxmva =-999.;
			vtxdz =-999.;
			dipho_mva =-999.;

		}

		flashggTreeWithTags->Fill(); 
	}
	void 
		FlashggTreeMakerWithTags::beginJob()
		{
			flashggTreeWithTags = fs_->make<TTree>("flashggTreeWithTags","flashgg tree with Tags");
			flashggTreeWithTags->Branch("njets10", &njets10, "njets10/F");
			flashggTreeWithTags->Branch("njets15", &njets15, "njets15/F");
			flashggTreeWithTags->Branch("njets20", &njets20, "njets20/F");
			flashggTreeWithTags->Branch("dRphojet1", &dRphojet1, "dRphojet1/F");
			flashggTreeWithTags->Branch("dRphojet2", &dRphojet2, "dRphojet2/F");
			flashggTreeWithTags->Branch("run", &run, "runNumber/I");
			flashggTreeWithTags->Branch("lumis", &lumis, "lumiSection/I");
			flashggTreeWithTags->Branch("event", &run, "eventNumber/I");
			flashggTreeWithTags->Branch("nvtx", &nvtx, "nvtx/F");
			flashggTreeWithTags->Branch("rho", &rho, "rho/F");
			flashggTreeWithTags->Branch("xsec_weight", &xsec_weight, "xsec_weight/F");
			flashggTreeWithTags->Branch("full_weight", &full_weight, "full_weight/F");
			flashggTreeWithTags->Branch("pu_weight", &pu_weight, "pu_weight/F");
			flashggTreeWithTags->Branch("pu_n", &pu_n, "pu_n/F");
			flashggTreeWithTags->Branch("mass", &mass, "mass/F");
			flashggTreeWithTags->Branch("dipho_pt", &dipho_pt, "dipho_pt/F");
			flashggTreeWithTags->Branch("full_cat", &full_cat, "full_cat/F");
			flashggTreeWithTags->Branch("et1", &et1, "et1/F");
			flashggTreeWithTags->Branch("et2", &et2, "et2/F");
			flashggTreeWithTags->Branch("eta1", &eta1, "eta1/F");
			flashggTreeWithTags->Branch("eta2", &eta2, "eta2/F");
			flashggTreeWithTags->Branch("r91", &r91, "r91/F");
			flashggTreeWithTags->Branch("r92", &r92, "r92/F");
			flashggTreeWithTags->Branch("sieie1", &sieie1, "sieie1/F");
			flashggTreeWithTags->Branch("sieie2", &sieie2, "sieie2/F");
			flashggTreeWithTags->Branch("hoe1", &hoe1, "hoe1/F");
			flashggTreeWithTags->Branch("hoe2", &hoe2, "hoe2/F");
			flashggTreeWithTags->Branch("sigmaEoE1", &sigmaEoE1, "sigmaEoE1/F");
			flashggTreeWithTags->Branch("sigmaEoE2", &sigmaEoE2, "sigmaEoE2/F");
			flashggTreeWithTags->Branch("ptoM1", &ptoM1, "ptoM1/F");
			flashggTreeWithTags->Branch("ptoM2", &ptoM2, "ptoM2/F");
			flashggTreeWithTags->Branch("isEB1", &isEB1, "isEB1/F");
			flashggTreeWithTags->Branch("isEB2", &isEB2, "isEB2/F");
			flashggTreeWithTags->Branch("chiso1", &chiso1, "chiso1/F");
			flashggTreeWithTags->Branch("chiso2", &chiso2, "chiso2/F");
			flashggTreeWithTags->Branch("chisow1", &chisow1, "chisow1/F");
			flashggTreeWithTags->Branch("chisow2", &chisow2, "chisow2/F");
			flashggTreeWithTags->Branch("phoiso1", &phoiso1, "phoiso1/F");
			flashggTreeWithTags->Branch("phoiso2", &phoiso2, "phoiso2/F");
			flashggTreeWithTags->Branch("phoiso041", &phoiso041, "phoiso041/F");
			flashggTreeWithTags->Branch("phoiso042", &phoiso042, "phoiso042/F");
			flashggTreeWithTags->Branch("ecaliso03_1", &ecaliso03_1, "ecaliso03_1/F");
			flashggTreeWithTags->Branch("ecaliso03_2", &ecaliso03_2, "ecaliso03_2/F");
			flashggTreeWithTags->Branch("hcaliso03_1", &hcaliso03_1, "hcaliso03_1/F");
			flashggTreeWithTags->Branch("hcaliso03_2", &hcaliso03_2, "hcaliso03_2/F");
			flashggTreeWithTags->Branch("trkiso03_1", &trkiso03_1, "trkiso03_1/F");
			flashggTreeWithTags->Branch("trkiso03_2", &trkiso03_2, "trkiso03_2/F");
			flashggTreeWithTags->Branch("pfchiso2_1", &pfchiso2_1, "pfchiso2_1/F");
			flashggTreeWithTags->Branch("pfchiso2_2", &pfchiso2_2, "pfchiso2_2/F");
			flashggTreeWithTags->Branch("sieip1", &sieip1, "sieip1/F");
			flashggTreeWithTags->Branch("sieip2", &sieip2, "sieip2/F");
			flashggTreeWithTags->Branch("etawidth1", &etawidth1, "etawidth1/F");
			flashggTreeWithTags->Branch("phiwidth1", &phiwidth1, "phiwidth1/F");
			flashggTreeWithTags->Branch("etawidth2", &etawidth2, "etawidth2/F");
			flashggTreeWithTags->Branch("phiwidth2", &phiwidth2, "phiwidth2/F");
			flashggTreeWithTags->Branch("regrerr1", &regrerr1, "regrerr1/F");
			flashggTreeWithTags->Branch("regrerr2", &regrerr2, "regrerr2/F");
			flashggTreeWithTags->Branch("cosphi", &cosphi, "cosphi/F");
			flashggTreeWithTags->Branch("genmatch1", &genmatch1, "genmatch1/F");
			flashggTreeWithTags->Branch("genmatch2", &genmatch2, "genmatch2/F");
			flashggTreeWithTags->Branch("idmva1", &idmva1, "idmva1/F");
			flashggTreeWithTags->Branch("idmva2", &idmva2, "idmva2/F");
			flashggTreeWithTags->Branch("vbfcat", &vbfcat, "vbfcat/F");
			flashggTreeWithTags->Branch("MET", &MET, "MET/F");
			flashggTreeWithTags->Branch("MET_phi", &MET_phi, "MET_phi/F");
			flashggTreeWithTags->Branch("isorv1", &isorv1, "isorv1/F");
			flashggTreeWithTags->Branch("isowv1", &isowv1, "isowv1/F");
			flashggTreeWithTags->Branch("isorv2", &isorv2, "isorv2/F");
			flashggTreeWithTags->Branch("isowv2", &isowv2, "isowv2/F");
			flashggTreeWithTags->Branch("s4ratio1", &s4ratio1, "s4ratio1/F");
			flashggTreeWithTags->Branch("s4ratio2", &s4ratio2, "s4ratio2/F");
			flashggTreeWithTags->Branch("effSigma1", &effSigma1, "effSigma1/F");
			flashggTreeWithTags->Branch("effSigma2", &effSigma2, "effSigma2/F");
			flashggTreeWithTags->Branch("scraw1", &scraw1, "scraw1/F");
			flashggTreeWithTags->Branch("scraw2", &scraw2, "scraw2/F");
			flashggTreeWithTags->Branch("vtx_x", &vtx_x, "vtx_x/F");
			flashggTreeWithTags->Branch("vtx_y", &vtx_y, "vtx_y/F");
			flashggTreeWithTags->Branch("vtx_z", &vtx_z, "vtx_z/F");
			flashggTreeWithTags->Branch("gv_x", &gv_x, "gv_x/F");
			flashggTreeWithTags->Branch("gv_y", &gv_y, "gv_y/F");
			flashggTreeWithTags->Branch("gv_z", &gv_z, "gv_z/F");
			flashggTreeWithTags->Branch("dijet_leadEta", &dijet_leadEta, "dijet_leadEta/F");
			flashggTreeWithTags->Branch("dijet_subleadEta", &dijet_subleadEta, "dijet_subleadEta/F");
			flashggTreeWithTags->Branch("dijet_LeadJPt", &dijet_LeadJPt, "dijet_LeadJPt/F");
			flashggTreeWithTags->Branch("dijet_SubJPt", &dijet_SubJPt, "dijet_SubJPt/F");
			flashggTreeWithTags->Branch("dijet_dEta", &dijet_dEta, "dijet_dEta/F");
			flashggTreeWithTags->Branch("dijet_Zep", &dijet_Zep, "dijet_Zep/F");
			flashggTreeWithTags->Branch("dijet_dPhi", &dijet_dPhi, "dijet_dPhi/F");
			flashggTreeWithTags->Branch("dijet_Mjj", &dijet_Mjj, "dijet_Mjj/F");
			flashggTreeWithTags->Branch("dijet_MVA", &dijet_MVA, "dijet_MVA/F");
			flashggTreeWithTags->Branch("bdt_combined", &bdt_combined, "bdt_combined/F");
			flashggTreeWithTags->Branch("issyst", &issyst, "issyst/F");
			flashggTreeWithTags->Branch("name1", &name1, "name1/F");
			flashggTreeWithTags->Branch("sigmaMrvoM", &sigmaMrvoM, "sigmaMrvoM/F");
			flashggTreeWithTags->Branch("sigmaMwvoM", &sigmaMwvoM, "sigmaMwvoM/F");
			flashggTreeWithTags->Branch("vtxprob", &vtxprob, "vtxprob/F");
			flashggTreeWithTags->Branch("ptbal", &ptbal, "ptbal/F");
			flashggTreeWithTags->Branch("ptasym", &ptasym, "ptasym/F");
			flashggTreeWithTags->Branch("logspt2", &logspt2, "logspt2/F");
			flashggTreeWithTags->Branch("p2conv", &p2conv, "p2conv/F");
			flashggTreeWithTags->Branch("nconv", &nconv, "nconv/F");
			flashggTreeWithTags->Branch("vtxmva", &vtxmva, "vtxmva/F");
			flashggTreeWithTags->Branch("vtxdz", &vtxdz, "vtxdz/F");
			flashggTreeWithTags->Branch("dipho_mva", &dipho_mva, "dipho_mva/F");
			flashggTreeWithTags->Branch("dipho_mva_cat", &dipho_mva_cat, "dipho_mva_cat/F");
			flashggTreeWithTags->Branch("flash_Untagged_Category", &flash_Untagged_Category, "flash_Untagged_Category/I");
			flashggTreeWithTags->Branch("flash_VBFTag_Category", &flash_VBFTag_Category, "flash_VBFTag_Category/I");
		}

	void 
		FlashggTreeMakerWithTags::endJob() 
		{
		}


	void
		FlashggTreeMakerWithTags::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
			//The following says we do not know what parameters are allowed so do no validation
			// Please change this to state exactly what you do use, even if it is no parameters
			edm::ParameterSetDescription desc;
			desc.setUnknown();
			descriptions.addDefault(desc);
		}

	typedef FlashggTreeMakerWithTags FlashggFlashggTreeMakerWithTags;
	DEFINE_FWK_MODULE(FlashggFlashggTreeMakerWithTags);
