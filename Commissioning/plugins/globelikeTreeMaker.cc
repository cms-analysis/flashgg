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

#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"

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
		EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> >  diPhotonToken_; //
		EDGetTokenT<edm::View<pat::MET> >  METToken_; // LDC work-in-progress adding this!
		EDGetTokenT<edm::View<PileupSummaryInfo> >  PileUpToken_; // LDC work-in-progress adding this!
	
		edm::InputTag rhoFixedGrid_;

		TTree *flashggTree;

  TMVA::Reader * DiphotonMva_;                                                                                                                                                      
  bool DiphotonMva_initialized;
  edm::FileInPath diphotonMVAweightfile_;

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




		edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_; // SCZ work-in-progress adding this!
  edm::EDGetTokenT<edm::View<reco::BeamSpot> > beamSpotToken_;
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
	METToken_(consumes<View<pat::MET> >(iConfig.getUntrackedParameter<InputTag> ("METTag", InputTag("slimmedMETs")))),
	PileUpToken_(consumes<View<PileupSummaryInfo> >(iConfig.getUntrackedParameter<InputTag> ("PileUpTag", InputTag("addPileupInfo")))),
	photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons")))),
	beamSpotToken_(consumes<View<reco::BeamSpot> >(iConfig.getUntrackedParameter<InputTag>("BeamSpotTag",InputTag("offlineBeamSpot"))))
	//  jetTokenReco_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagReco")))
{
	rhoFixedGrid_ = iConfig.getParameter<edm::InputTag>("rhoFixedGridCollection");
	diphotonMVAweightfile_ = iConfig.getParameter<edm::FileInPath>("diphotonMVAweightfile");
	DiphotonMva_initialized = false;
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

	Handle<View<reco::GenParticle> > genParticles;
	iEvent.getByToken(genParticleToken_,genParticles);
	const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();
	float a = gens.size();

	Handle<View<flashgg::Jet> > jetsDz;
	iEvent.getByToken(jetTokenDz_,jetsDz);
	const PtrVector<flashgg::Jet>& jetPointersDz = jetsDz->ptrVector();
	a = jetPointersDz.size();

	//Handle<View<flashgg::Jet> > jetsRecoBasedMap;
	//iEvent.getByToken(jetTokenRecoBasedMap_,jetsRecoBasedMap);
	// const PtrVector<flashgg::Jet>& jetPointersRecoBasedMap = jetsRecoBasedMap->ptrVector();
	//a = jetPointersRecoBasedMap.size();
	//
	Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
	iEvent.getByToken(diPhotonToken_,diPhotons);
	const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

	Handle<View<pat::MET> > METs;
	iEvent.getByToken(METToken_,METs);
	const PtrVector<pat::MET>& METPointers = METs->ptrVector();
	
	Handle<View< PileupSummaryInfo> > PileupInfos;
	iEvent.getByToken(PileUpToken_,PileupInfos);
const PtrVector<PileupSummaryInfo>& PileupInfoPointers = PileupInfos->ptrVector();
	
	Handle<View<flashgg::Photon> > photons;
	iEvent.getByToken(photonToken_,photons);
	const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();
	a = photonPointers.size();

	Handle<double> rhoHandle; // the old way for now...move to getbytoken?
	iEvent.getByLabel(rhoFixedGrid_, rhoHandle );

	a=a+1;


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


	if (itype !=0 )
	{
		xsec_weight = 0;
		full_weight = 0;
		pu_weight = -1;
		pu_n = 0;

		for( unsigned int PVI = 0; PVI < PileupInfoPointers.size(); ++PVI) {	
				Int_t pu_bunchcrossing = PileupInfoPointers[PVI]->getBunchCrossing();
				if (pu_bunchcrossing ==0) {
					pu_n = PileupInfoPointers[PVI]->getPU_NumInteractions();
				}
			}

		}

		else

		{
			xsec_weight = -1;
			full_weight = -1;
			pu_weight = -1;
			pu_n = -1;
		}


		//------> higgs diphoton candidate properties
		// We have a problem here, because until we are sure that there is consistently one higgs diphoton candiate, we 
		// will either need to select one of the multiple diphoton pairs. The following does that, by choosing the one closets to 125GeV...
		float higgsMassDifference =9999.; 
		float higgsMass =125. ;
		Int_t higgsCandPresent = 0; //Is 0 is there are no candidates in event, set to 1 if there is at least one candidate pair.
		Int_t candIndex = 9999; //This int will store the index of the best higgs diphoton candidate...

		for (unsigned int diphotonlooper =0; diphotonlooper < diPhotonPointers.size() ; diphotonlooper++)
		{
			if  (fabs(diPhotonPointers[diphotonlooper]->mass() - higgsMass) < higgsMassDifference)
			{
				higgsMassDifference = fabs(diPhotonPointers[diphotonlooper]->mass() - higgsMass);
				candIndex = diphotonlooper;
				higgsCandPresent=1;
			}
		}

		if(higgsCandPresent)
		{
			mass = diPhotonPointers[candIndex]->mass();
			dipho_pt = diPhotonPointers[candIndex]->pt();
		}
		else
		{
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
		if(higgsCandPresent)
		{
			//PHOTON1
			et1 = diPhotonPointers[candIndex]->leadingPhoton()->et();
			eta1 = diPhotonPointers[candIndex]->leadingPhoton()->eta();
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
			chisow1 = diPhotonPointers[candIndex]->leadingPhoton()->getpfChgIsoWrtWorstVtx03();//no flashgg method for come radius 04... ok to use 0.3?
			phoiso1 = diPhotonPointers[candIndex]->leadingPhoton()->getpfPhoIso03();
			phoiso041 = diPhotonPointers[candIndex]->leadingPhoton()->photonIso(); //unsure of default radius?
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
			chisow2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfChgIsoWrtWorstVtx03();//no flashgg method for come radius 04... ok to use 0.3?
			phoiso2 = diPhotonPointers[candIndex]->subLeadingPhoton()->getpfPhoIso03();
			
			std:: cout << diPhotonPointers[candIndex]->subLeadingPhoton()->chargedHadronIso() << "	" << chiso2 << std::endl;

			phoiso042 = diPhotonPointers[candIndex]->subLeadingPhoton()->photonIso(); //unsure of default radius?
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

			// Diphoton calculations
			// Moved from LegacyVertexSelector (M. Olmedo) to here by SZ

			edm::Ptr<reco::Vertex> vtx = diPhotonPointers[candIndex]->getVertex();
			const flashgg::Photon* g1 = diPhotonPointers[candIndex]->leadingPhoton();
			const flashgg::Photon* g2 = diPhotonPointers[candIndex]->subLeadingPhoton();
			TVector3 Photon1Dir, Photon2Dir;
			Photon1Dir.SetXYZ(g1->superCluster()->position().x() - vtx->position().x(),
					g1->superCluster()->position().y() - vtx->position().y(),
					g1->superCluster()->position().z() - vtx->position().z());
			Photon2Dir.SetXYZ(g2->superCluster()->position().x() - vtx->position().x(),
					g2->superCluster()->position().y() - vtx->position().y(),
					g2->superCluster()->position().z() - vtx->position().z());
			//		Photon1Dir_uv = Photon1Dir.Unit()*g1->superCluster()->rawEnergy();
			//		Photon2Dir_uv = Photon2Dir.Unit()*g2->superCluster()->rawEnergy();
			//		p14.SetPxPyPzE(Photon1Dir_uv.x(),Photon1Dir_uv.y(),Photon1Dir_uv.z(),g1->superCluster()->rawEnergy());
			//		p24.SetPxPyPzE(Photon2Dir_uv.x(),Photon2Dir_uv.y(),Photon2Dir_uv.z(),g2->superCluster()->rawEnergy());

			Handle<reco::BeamSpot> recoBeamSpotHandle;
			iEvent.getByToken(beamSpotToken_,recoBeamSpotHandle);
			float beamsig;
			if (recoBeamSpotHandle.isValid()){
				beamsig = recoBeamSpotHandle->sigmaZ();
			} else {
				beamsig = -9999; // I hope this never happens!"
			}

			float r1 = TMath::Sqrt(Photon1Dir.X()*Photon1Dir.X()+Photon1Dir.Y()*Photon1Dir.Y()+Photon1Dir.Z()*Photon1Dir.Z());
			float r2 = TMath::Sqrt(Photon2Dir.X()*Photon2Dir.X()+Photon2Dir.Y()*Photon2Dir.Y()+Photon2Dir.Z()*Photon2Dir.Z());
			float cos_term = TMath::Cos(g1->phi()-g2->phi());
			float sech1 = 1.0/TMath::CosH(g1->eta());
			float sech2 = 1.0/TMath::CosH(g2->eta());
			float tanh1 = 1.0/TMath::TanH(g1->phi());
			float tanh2 = 1.0/TMath::TanH(g2->phi());
			float numerator1 = sech1*(sech1*tanh2-tanh1*sech2*cos_term);
			float numerator2 = sech2*(sech2*tanh1-tanh2*sech1*cos_term);
			float denominator = 1. - tanh1*tanh2 - sech1*sech2*cos_term;
			float angleResolution = ((-1.*beamsig*TMath::Sqrt(.2))/denominator)*(numerator1/r1 + numerator2/r2);
			float alpha_sig = 0.5*angleResolution;
			float SigmaM = TMath::Sqrt(g1->getSigEOverE()*g1->getSigEOverE() + g2->getSigEOverE()*g2->getSigEOverE());
			float MassResolutionWrongVtx = TMath::Sqrt((SigmaM*SigmaM)+(alpha_sig*alpha_sig));

			float leadptom_       = g1->pt()/(diPhotonPointers[candIndex]->mass());
			float subleadptom_    = g2->pt()/(diPhotonPointers[candIndex]->mass());
			float subleadmva_     = g2->getPhoIdMvaDWrtVtx(vtx);
			float leadmva_        = g1->getPhoIdMvaDWrtVtx(vtx);
			float leadeta_        = g2->eta();
			float subleadeta_     = g1->eta();
			float sigmarv_        = .5*sqrt((g1->getSigEOverE())*(g1->getSigEOverE()) + (g2->getSigEOverE())*(g2->getSigEOverE()));
			float sigmawv_        = MassResolutionWrongVtx;
			float CosPhi_         = TMath::Cos(g1->phi()-g2->phi());
			float vtxprob_        =  1.-.49*(1+diPhotonPointers[candIndex]->getVtxProbMVA());

			if (!DiphotonMva_initialized) {
				DiphotonMva_initialized = true;
				DiphotonMva_ = new TMVA::Reader("!Color:Silent");
				DiphotonMva_->AddVariable("masserrsmeared/mass",&sigmarv_);
				DiphotonMva_->AddVariable("masserrsmearedwrongvtx/mass",&sigmawv_);
				DiphotonMva_->AddVariable("vtxprob",&vtxprob_);
				DiphotonMva_->AddVariable("ph1.pt/mass",&leadptom_);
				DiphotonMva_->AddVariable("ph2.pt/mass",&subleadptom_);
				DiphotonMva_->AddVariable("ph1.eta",&leadeta_);
				DiphotonMva_->AddVariable("ph2.eta",&subleadeta_);
				DiphotonMva_->AddVariable("TMath::Cos(ph1.phi-ph2.phi)",&CosPhi_);
				DiphotonMva_->AddVariable("ph1.idmva",&leadmva_);
				DiphotonMva_->AddVariable("ph2.idmva",&subleadmva_);
				DiphotonMva_->BookMVA("BDT",diphotonMVAweightfile_.fullPath());
			}

			float diphomva_ = DiphotonMva_->EvaluateMVA("BDT");

			// Storing these to the variables used in the tree
			sigmaMrvoM = sigmarv_; 
			sigmaMwvoM = sigmawv_; 
			vtxprob = vtxprob_;
			ptbal = diPhotonPointers[candIndex]->getPtBal();
			ptasym = diPhotonPointers[candIndex]->getPtAsym();
			logspt2 = diPhotonPointers[candIndex]->getLogSumPt2();
			p2conv = diPhotonPointers[candIndex]->getPullConv(); 
			nconv = diPhotonPointers[candIndex]->getNConv(); 
			vtxmva = diPhotonPointers[candIndex]->getVtxProbMVA();
			vtxdz = diPhotonPointers[candIndex]->getDZ1(); 
			dipho_mva = diphomva_;


			//		std::cout << "higgsCandidatePResent in globelikeTreeMaker" << std::endl;
		}
		else
		{

			et1 =  0;
			eta1 = 0;
			r91 =  0;
			sieie1 =0; 
			hoe1 =0;
			sigmaEoE1 =0; 
			ptoM1 =0;
			isEB1 =0;
			chiso1 =0;
			chisow1 =0; 
			phoiso1 = 0;
			phoiso041 =0; 
			ecaliso03_1 =0;
			hcaliso03_1 =0;
			trkiso03_1 = 0;
			pfchiso2_1 = 0;
			sieip1 = 0;
			etawidth1 =0; 
			phiwidth1 = 0;
			regrerr1 = 0;
			idmva1 = 0;
			s4ratio1 =0;  
			effSigma1 =0;  
			scraw1 =  0;

			et2 =  0;
			eta2 = 0;
			r92 =0;
			sieie2 =0; 
			hoe2 =0;
			sigmaEoE2 =0; 
			ptoM2 =0;
			isEB2 =0;
			chiso2 =0;
			chisow2 =0; 
			phoiso2 = 0;
			phoiso042 =0; 
			ecaliso03_2 =0;
			hcaliso03_2 =0;
			trkiso03_2 = 0;
			pfchiso2_2 = 0;
			sieip2 = 0;
			etawidth2 =0; 
			phiwidth2 = 0;
			regrerr2 = 0;
			idmva2 = 0;
			s4ratio2 =0;  
			effSigma2 =0;  
			scraw2 =  0;
		}

		flashggTree->Fill(); 
}
	void 
FlashggTreeMaker::beginJob()
{
	flashggTree = fs_->make<TTree>("flashggTree","flashgg tree");
	flashggTree->Branch("njets10", &njets10, "njets10/F");
	flashggTree->Branch("njets15", &njets15, "njets15/F");
	flashggTree->Branch("njets20", &njets20, "njets20/F");
	flashggTree->Branch("dRphojet1", &dRphojet1, "dRphojet1/F");
	flashggTree->Branch("dRphojet2", &dRphojet2, "dRphojet2/F");
	flashggTree->Branch("run", &run, "runNumber/I");
	flashggTree->Branch("lumis", &lumis, "lumiSection/I");
	flashggTree->Branch("event", &run, "eventNumber/I");
	flashggTree->Branch("nvtx", &nvtx, "nvtx/F");
	flashggTree->Branch("rho", &rho, "rho/F");
	flashggTree->Branch("xsec_weight", &xsec_weight, "xsec_weight/F");
	flashggTree->Branch("full_weight", &full_weight, "full_weight/F");
	flashggTree->Branch("pu_weight", &pu_weight, "pu_weight/F");
	flashggTree->Branch("pu_n", &pu_n, "pu_n/F");
	flashggTree->Branch("mass", &mass, "mass/F");
	flashggTree->Branch("dipho_pt", &dipho_pt, "dipho_pt/F");
	flashggTree->Branch("full_cat", &full_cat, "full_cat/F");
	flashggTree->Branch("et1", &et1, "et1/F");
	flashggTree->Branch("et2", &et2, "et2/F");
	flashggTree->Branch("eta1", &eta1, "eta1/F");
	flashggTree->Branch("eta2", &eta2, "eta2/F");
	flashggTree->Branch("r91", &r91, "r91/F");
	flashggTree->Branch("r92", &r92, "r92/F");
	flashggTree->Branch("sieie1", &sieie1, "sieie1/F");
	flashggTree->Branch("sieie2", &sieie2, "sieie2/F");
	flashggTree->Branch("hoe1", &hoe1, "hoe1/F");
	flashggTree->Branch("hoe2", &hoe2, "hoe2/F");
	flashggTree->Branch("sigmaEoE1", &sigmaEoE1, "sigmaEoE1/F");
	flashggTree->Branch("sigmaEoE2", &sigmaEoE2, "sigmaEoE2/F");
	flashggTree->Branch("ptoM1", &ptoM1, "ptoM1/F");
	flashggTree->Branch("ptoM2", &ptoM2, "ptoM2/F");
	flashggTree->Branch("isEB1", &isEB1, "isEB1/F");
	flashggTree->Branch("isEB2", &isEB2, "isEB2/F");
	flashggTree->Branch("chiso1", &chiso1, "chiso1/F");
	flashggTree->Branch("chiso2", &chiso2, "chiso2/F");
	flashggTree->Branch("chisow1", &chisow1, "chisow1/F");
	flashggTree->Branch("chisow2", &chisow2, "chisow2/F");
	flashggTree->Branch("phoiso1", &phoiso1, "phoiso1/F");
	flashggTree->Branch("phoiso2", &phoiso2, "phoiso2/F");
	flashggTree->Branch("phoiso041", &phoiso041, "phoiso041/F");
	flashggTree->Branch("phoiso042", &phoiso042, "phoiso042/F");
	flashggTree->Branch("ecaliso03_1", &ecaliso03_1, "ecaliso03_1/F");
	flashggTree->Branch("ecaliso03_2", &ecaliso03_2, "ecaliso03_2/F");
	flashggTree->Branch("hcaliso03_1", &hcaliso03_1, "hcaliso03_1/F");
	flashggTree->Branch("hcaliso03_2", &hcaliso03_2, "hcaliso03_2/F");
	flashggTree->Branch("trkiso03_1", &trkiso03_1, "trkiso03_1/F");
	flashggTree->Branch("trkiso03_2", &trkiso03_2, "trkiso03_2/F");
	flashggTree->Branch("pfchiso2_1", &pfchiso2_1, "pfchiso2_1/F");
	flashggTree->Branch("pfchiso2_2", &pfchiso2_2, "pfchiso2_2/F");
	flashggTree->Branch("sieip1", &sieip1, "sieip1/F");
	flashggTree->Branch("sieip2", &sieip2, "sieip2/F");
	flashggTree->Branch("etawidth1", &etawidth1, "etawidth1/F");
	flashggTree->Branch("phiwidth1", &phiwidth1, "phiwidth1/F");
	flashggTree->Branch("etawidth2", &etawidth2, "etawidth2/F");
	flashggTree->Branch("phiwidth2", &phiwidth2, "phiwidth2/F");
	flashggTree->Branch("regrerr1", &regrerr1, "regrerr1/F");
	flashggTree->Branch("regrerr2", &regrerr2, "regrerr2/F");
	flashggTree->Branch("cosphi", &cosphi, "cosphi/F");
	flashggTree->Branch("genmatch1", &genmatch1, "genmatch1/F");
	flashggTree->Branch("genmatch2", &genmatch2, "genmatch2/F");
	flashggTree->Branch("idmva1", &idmva1, "idmva1/F");
	flashggTree->Branch("idmva2", &idmva2, "idmva2/F");
	flashggTree->Branch("vbfcat", &vbfcat, "vbfcat/F");
	flashggTree->Branch("MET", &MET, "MET/F");
	flashggTree->Branch("MET_phi", &MET_phi, "MET_phi/F");
	flashggTree->Branch("isorv1", &isorv1, "isorv1/F");
	flashggTree->Branch("isowv1", &isowv1, "isowv1/F");
	flashggTree->Branch("isorv2", &isorv2, "isorv2/F");
	flashggTree->Branch("isowv2", &isowv2, "isowv2/F");
	flashggTree->Branch("s4ratio1", &s4ratio1, "s4ratio1/F");
	flashggTree->Branch("s4ratio2", &s4ratio2, "s4ratio2/F");
	flashggTree->Branch("effSigma1", &effSigma1, "effSigma1/F");
	flashggTree->Branch("effSigma2", &effSigma2, "effSigma2/F");
	flashggTree->Branch("scraw1", &scraw1, "scraw1/F");
	flashggTree->Branch("scraw2", &scraw2, "scraw2/F");
	flashggTree->Branch("vtx_x", &vtx_x, "vtx_x/F");
	flashggTree->Branch("vtx_y", &vtx_y, "vtx_y/F");
	flashggTree->Branch("vtx_z", &vtx_z, "vtx_z/F");
	flashggTree->Branch("gv_x", &gv_x, "gv_x/F");
	flashggTree->Branch("gv_y", &gv_y, "gv_y/F");
	flashggTree->Branch("gv_z", &gv_z, "gv_z/F");
	flashggTree->Branch("dijet_leadEta", &dijet_leadEta, "dijet_leadEta/F");
	flashggTree->Branch("dijet_subleadEta", &dijet_subleadEta, "dijet_subleadEta/F");
	flashggTree->Branch("dijet_LeadJPt", &dijet_LeadJPt, "dijet_LeadJPt/F");
	flashggTree->Branch("dijet_SubJPt", &dijet_SubJPt, "dijet_SubJPt/F");
	flashggTree->Branch("dijet_dEta", &dijet_dEta, "dijet_dEta/F");
	flashggTree->Branch("dijet_Zep", &dijet_Zep, "dijet_Zep/F");
	flashggTree->Branch("dijet_dPhi", &dijet_dPhi, "dijet_dPhi/F");
	flashggTree->Branch("dijet_Mjj", &dijet_Mjj, "dijet_Mjj/F");
	flashggTree->Branch("dijet_MVA", &dijet_MVA, "dijet_MVA/F");
	flashggTree->Branch("bdt_combined", &bdt_combined, "bdt_combined/F");
	flashggTree->Branch("issyst", &issyst, "issyst/F");
	flashggTree->Branch("name1", &name1, "name1/F");
	flashggTree->Branch("sigmaMrvoM", &sigmaMrvoM, "sigmaMrvoM/F");
	flashggTree->Branch("sigmaMwvoM", &sigmaMwvoM, "sigmaMwvoM/F");
	flashggTree->Branch("vtxprob", &vtxprob, "vtxprob/F");
	flashggTree->Branch("ptbal", &ptbal, "ptbal/F");
	flashggTree->Branch("ptasym", &ptasym, "ptasym/F");
	flashggTree->Branch("logspt2", &logspt2, "logspt2/F");
	flashggTree->Branch("p2conv", &p2conv, "p2conv/F");
	flashggTree->Branch("nconv", &nconv, "nconv/F");
	flashggTree->Branch("vtxmva", &vtxmva, "vtxmva/F");
	flashggTree->Branch("vtxdz", &vtxdz, "vtxdz/F");
	flashggTree->Branch("dipho_mva", &dipho_mva, "dipho_mva/F");
	flashggTree->Branch("dipho_mva_cat", &dipho_mva_cat, "dipho_mva_cat/F");
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
