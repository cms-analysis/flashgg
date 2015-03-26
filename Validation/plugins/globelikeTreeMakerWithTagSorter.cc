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
//#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/DataFormats/interface/Jet.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"


#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class FlashggTreeMakerWithTagSorter : public edm::EDAnalyzer {
	public:
		explicit FlashggTreeMakerWithTagSorter(const edm::ParameterSet&);
		~FlashggTreeMakerWithTagSorter();

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
		EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> >  diPhotonToken_; 
		EDGetTokenT<edm::View<pat::MET> >  METToken_; 
		EDGetTokenT<edm::View<PileupSummaryInfo> >  PileUpToken_;
		edm::InputTag rhoFixedGrid_;

		TTree *flashggTreeWithTagSorter;

		// Variables to fill
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
		Float_t dipho_PToM;
		//Tag Categories
		Int_t flash_Untagged_Category;
		Int_t flash_VBFTag_Category;
		Int_t leadjet_genmatch;
		Int_t subljet_genmatch;


		edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_; // SCZ work-in-progress adding this!
		edm::EDGetTokenT<edm::OwnVector<flashgg::DiPhotonTagBase> > TagSorterToken_;
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
FlashggTreeMakerWithTagSorter::FlashggTreeMakerWithTagSorter(const edm::ParameterSet& iConfig):
	vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
	genParticleToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles")))),
	vertexCandidateMapTokenDz_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagDz"))),
	vertexCandidateMapTokenAOD_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagAOD"))),
	jetTokenDz_(consumes<View<flashgg::Jet> >(iConfig.getParameter<InputTag>("JetTagDz"))),
	diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
	METToken_(consumes<View<pat::MET> >(iConfig.getUntrackedParameter<InputTag> ("METTag", InputTag("slimmedMETs")))),
	PileUpToken_(consumes<View<PileupSummaryInfo> >(iConfig.getUntrackedParameter<InputTag> ("PileUpTag", InputTag("addPileupInfo")))),
	TagSorterToken_(consumes<edm::OwnVector<flashgg::DiPhotonTagBase> >(iConfig.getUntrackedParameter<InputTag> ("TagSorter", InputTag("flashggTagSorter"))))
{
	rhoFixedGrid_ = iConfig.getParameter<edm::InputTag>("rhoFixedGridCollection");
}

FlashggTreeMakerWithTagSorter::~FlashggTreeMakerWithTagSorter()
{

}

void
FlashggTreeMakerWithTagSorter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

	// ********************************************************************************
	// access edm objects

	Handle<VertexCandidateMap> vertexCandidateMapDz;
	iEvent.getByToken(vertexCandidateMapTokenDz_,vertexCandidateMapDz);

	Handle<VertexCandidateMap> vertexCandidateMapAOD;
	iEvent.getByToken(vertexCandidateMapTokenAOD_,vertexCandidateMapAOD);

	Handle<View<reco::Vertex> > primaryVertices;
	iEvent.getByToken(vertexToken_,primaryVertices);
//	const PtrVector<reco::Vertex>& vtxs = primaryVertices->ptrVector();

	Handle<View<reco::GenParticle> > genParticles;
	iEvent.getByToken(genParticleToken_,genParticles);
//	const PtrVector<reco::GenParticle>& gens = genParticles->ptrVector();

	Handle<View<flashgg::Jet> > jetsDz;
	iEvent.getByToken(jetTokenDz_,jetsDz);
//	const PtrVector<flashgg::Jet>& jetPointersDz = jetsDz->ptrVector();

	Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
	iEvent.getByToken(diPhotonToken_,diPhotons);
//	const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

	Handle<View<pat::MET> > METs;
	iEvent.getByToken(METToken_,METs);
//	const PtrVector<pat::MET>& METPointers = METs->ptrVector();

	Handle<View< PileupSummaryInfo> > PileupInfos;
	iEvent.getByToken(PileUpToken_,PileupInfos);
//	const PtrVector<PileupSummaryInfo>& PileupInfoPointers = PileupInfos->ptrVector();

	Handle<double> rhoHandle; // the old way for now...move to getbytoken?
	iEvent.getByLabel(rhoFixedGrid_, rhoHandle );

	//Slightly unusal way of accessing selected Tag from TagSorter, since a pointer is saved rather than a vector.
	Handle<edm::OwnVector<flashgg::DiPhotonTagBase> > TagSorter;
	iEvent.getByToken(TagSorterToken_,TagSorter);
	//	std::cout << " TagSorter->get() = " << TagSorter->get() << std::endl;

	flash_Untagged_Category= -1; // so that there is at least some value to fill even if not part of category
	flash_VBFTag_Category =-1;// so that there is at least some value to fill even if untagged
	leadjet_genmatch =-1;
	subljet_genmatch =-1;
	//	int hasTag =0;

	//-----------> Determine if there is a Tag, and select it! This givens diPhoton candIndex

	int candIndex = -1; // now given as the index of the diPhoton par selected by the tag.
	if (TagSorter.product()->size() > 0 ) //make sure TagSorter is not a null pointer
	{

		const flashgg::DiPhotonTagBase* chosenTag = &*(TagSorter.product()->begin());

		candIndex = (chosenTag->diPhotonIndex()); //should exist regardless of tag type.

		const	DiPhotonUntaggedCategory *untagged = dynamic_cast<const DiPhotonUntaggedCategory*>(chosenTag);

		//if(untagged == NULL) std::cout << "NOT UNTAGGED" <<std::endl;
		if(untagged != NULL) {
			std::cout << "[UNTAGGED] category " << untagged->categoryNumber() << std::endl;
			flash_Untagged_Category= untagged->categoryNumber() ;
			//	hasTag=1;
		}

		const	VBFTag *vbftag = dynamic_cast<const VBFTag*>(chosenTag);
		//if(vbftag == NULL) std::cout << "NOT VBF" <<std::endl;
		if(vbftag != NULL) {
			std::cout << "[VBF] Category " << vbftag->categoryNumber() <<std::endl;
			flash_VBFTag_Category =vbftag->categoryNumber() ;
			//	hasTag=1;
			//	std::cout << "VBF lead Jet eta" <<vbftag->leadingJet().eta() <<std::endl; //debug variables to see if correct info is being saved...
			//	std::cout << "VBF sublead Jet phi" <<vbftag->leadingJet().phi() <<std::endl;
		}

                const   TTHHadronicTag *tthhadronictag = dynamic_cast<const TTHHadronicTag*>(chosenTag);
                //if(tthhadronictag == NULL) std::cout << "NOT TTHhadronic" <<std::endl;                                                                                                             
                if(tthhadronictag != NULL) {
		  std::cout << "[TTHhadronic] Category " << tthhadronictag->categoryNumber() <<std::endl;
                }

                const   TTHLeptonicTag *tthleptonictag = dynamic_cast<const TTHLeptonicTag*>(chosenTag);
		//if(tthleptonictag == NULL) std::cout << "NOT TTHleptonic" <<std::endl;                                                                                            

                if(tthleptonictag != NULL) {
		  std::cout << "[TTHleptonic] Category " << tthleptonictag->categoryNumber() 
			    << " nelectrons=" << tthleptonictag->electrons().size() 
			    << " nmuons=" << tthleptonictag->muons().size() 
			    << std::endl;
		}

		// IMPORTANT: All future Tags must be added in the way of untagged and vbftag.	

		if (untagged == NULL && vbftag == NULL && tthhadronictag == NULL && tthleptonictag == NULL ) {
		  std::cout << "[FAILED TO CONVERT TAG] with SumPt " << chosenTag->sumPt() << std::endl;
		}


		//--------------> Tag selected, now fill tree with relevant properties!



		//---------> njetsxx = number of jets with et > xx
		njets10 = 0.;
		njets15 = 0.;
		njets20 = 0.;

		for (UInt_t jetLoop =0; jetLoop < jetsDz->size() ; jetLoop++){

			Float_t et = jetsDz->ptrAt(jetLoop)->et();

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
		nvtx = primaryVertices->size();

		//-----> rho = energy density
		rho = *(rhoHandle.product());


		//------> weights and PU and gen vertex and match information 
		genmatch1= 0;
		genmatch2= 0;
		xsec_weight = -1;
		full_weight = -1;
		pu_weight = -1;
		pu_n = -1;
		gv_x = -9999.;
		gv_y = -9999.;
		gv_z = -9999.;

		if (itype !=0 ){
			xsec_weight = 0; //probably can be deleted as initialised above... 
			full_weight = 0;
			pu_weight = -1;
			pu_n = 0;

			// pileup info
			for( unsigned int PVI = 0; PVI < PileupInfos->size(); ++PVI) {	
				Int_t pu_bunchcrossing = PileupInfos->ptrAt(PVI)->getBunchCrossing();
				if (pu_bunchcrossing ==0) {
					pu_n = PileupInfos->ptrAt(PVI)->getPU_NumInteractions();
				}
			}

			// gen vertex location
			for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
				if( genParticles->ptrAt(genLoop)->pdgId() == 25) { //might need to be changed for background MC samples...
					gv_x = genParticles->ptrAt(genLoop)->vx();
					gv_y = genParticles->ptrAt(genLoop)->vy();
					gv_z = genParticles->ptrAt(genLoop)->vz();
				}

				break;
			}

			//	int hasQuarks =0;

			for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
		//		std::cout << "[TREE] PDG: "<< genParticles->ptrAt(genLoop)->pdgId() << "	, status " << genParticles->ptrAt(genLoop)->status() << std::endl;
			}

			std::cout << std::endl;
			std::vector<int> vbfQuarkIndices;
			bool count =0;
			for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
				if (genParticles->ptrAt(genLoop)->status() !=3) continue;
				if (genParticles->ptrAt(genLoop)->pdgId() ==25) {count =1 ;continue ;}
				if ((fabs(genParticles->ptrAt(genLoop)->pdgId()) >8) && (count ==1)) {count =0; break;}
				if(count) {
					vbfQuarkIndices.push_back(genLoop);
			//		std::cout << "[GEN] PDG: "<< genParticles->ptrAt(genLoop)->pdgId() << "	, status " << genParticles->ptrAt(genLoop)->status() << std::endl;
				}
			}
			
//std::cout << "[DEBUG] " << vbfQuarkIndices.size() << std::endl;
			leadjet_genmatch =0;
			subljet_genmatch =0;
			if(vbftag != NULL) {
				for (unsigned int i =0 ; 	i< vbfQuarkIndices.size(); i++)
				{
				//	std::cout << "quark index " << vbfQuarkIndices[i] << std::endl;
					float deta =  vbftag->leadingJet().eta() - genParticles->ptrAt(vbfQuarkIndices[i])->eta();
					float dphi =  deltaPhi(vbftag->leadingJet().phi(),genParticles->ptrAt(vbfQuarkIndices[i])->phi());
					float dr = sqrt(deta*deta + dphi*dphi);
					if(fabs(dr) <0.5){
						leadjet_genmatch =1;
						break;
					}
				}

				for (unsigned int i =0 ; 	i< vbfQuarkIndices.size(); i++)
				{
					float deta =  vbftag->subLeadingJet().eta() - genParticles->ptrAt(vbfQuarkIndices[i])->eta();
					float dphi =  deltaPhi(vbftag->subLeadingJet().phi(),genParticles->ptrAt(vbfQuarkIndices[i])->phi());
					float dr = sqrt(deta*deta + dphi*dphi);
					if(fabs(dr) <0.5){
						subljet_genmatch =1;	
						break;
					}
				}
	//		std::cout << leadjet_genmatch << "	" << subljet_genmatch << std::endl;
			}


			// gen match leading pho 
			for(unsigned int ip=0;ip<genParticles->size();++ip) {
			  //			  std::cout << "GMLP " << genParticles->ptrAt(ip)->status() << " "  << genParticles->ptrAt(ip)->pdgId() << " " << genParticles->ptrAt(ip)->mother(0) << std::endl;
				if( genParticles->ptrAt(ip)->status() != 1 || genParticles->ptrAt(ip)->pdgId() != 22 ) {
					continue;
				}
				if( diPhotons->ptrAt(candIndex)->leadingPhoton()->et()< 20. || fabs(diPhotons->ptrAt(candIndex)->leadingPhoton()->eta()) > 3. ) { continue; }
				if (genParticles->ptrAt(ip)->mother(0) != NULL && genParticles->ptrAt(ip)->mother(0)->pdgId() <= 25) {
					float deta =  diPhotons->ptrAt(candIndex)->leadingPhoton()->eta() - genParticles->ptrAt(ip)->eta();
					float dphi =  deltaPhi(diPhotons->ptrAt(candIndex)->leadingPhoton()->phi(),genParticles->ptrAt(ip)->phi());
					float dr = sqrt(deta*deta + dphi*dphi);
					float pt_change = (diPhotons->ptrAt(candIndex)->leadingPhoton()->et() - genParticles->ptrAt(ip)->et())/genParticles->ptrAt(ip)->et();
					if (dr<0.3 && fabs(pt_change) < 0.5) {
						genmatch1 = true;
						break;
					}
				}
			}
			// gen match subleading pho
			for(unsigned int ip=0;ip<genParticles->size();++ip) {
			  //			  std::cout << "GMSLP " << genParticles->ptrAt(ip)->status() << " "  << genParticles->ptrAt(ip)->pdgId() << " " << genParticles->ptrAt(ip)->mother(0) << std::endl;
				if( genParticles->ptrAt(ip)->status() != 1 || genParticles->ptrAt(ip)->pdgId() != 22 ) {
					continue;
				}
				if( diPhotons->ptrAt(candIndex)->subLeadingPhoton()->et()< 20. || fabs(diPhotons->ptrAt(candIndex)->subLeadingPhoton()->eta()) > 3. ) { continue; }
				if ( genParticles->ptrAt(ip)->mother(0) != NULL && genParticles->ptrAt(ip)->mother(0)->pdgId() <= 25) {
					float deta =  diPhotons->ptrAt(candIndex)->subLeadingPhoton()->eta() - genParticles->ptrAt(ip)->eta();
					float dphi =  deltaPhi(diPhotons->ptrAt(candIndex)->subLeadingPhoton()->phi(),genParticles->ptrAt(ip)->phi());
					float dr = sqrt(deta*deta + dphi*dphi);
					float pt_change = (diPhotons->ptrAt(candIndex)->subLeadingPhoton()->et() - genParticles->ptrAt(ip)->et())/genParticles->ptrAt(ip)->et();
					if (dr<0.3 && fabs(pt_change) < 0.5) {
						genmatch1 = true;
						break;
					}
				}
			}
		}

		/* NB: This entire section has been superseded by the TagSorter, which now selects the best diPhoton.
			 Will remove in next iteration.

		//------> higgs diphoton candidate properties
		// We have a problem here, because until we are sure that there is consistently one higgs diphoton candiate, we 
		// will either need to select one of the multiple diphoton pairs. The following does that, by choosing the one closets to 125GeV..	
		float higgsMassDifference =9999.; 
		float higgsMass =125. ;
		Int_t higgsCandPresent = 0; //Is 0 is there are no candidates in event, set to 1 if there is at least one candidate pair.
		Int_t candIndex = 9999; //This int will store the index of the best higgs diphoton candidate...

		for (unsigned int diphotonlooper =0; diphotonlooper < diPhotons->size() ; diphotonlooper++){
		if  (fabs(diPhotons[diphotonlooper]->mass() - higgsMass) < higgsMassDifference){
		higgsMassDifference = fabs(diPhotons[diphotonlooper]->mass() - higgsMass);
		candIndex = diphotonlooper;
		higgsCandPresent=1;
		}
		}
		*/

		mass = diPhotons->ptrAt(candIndex)->mass();
		dipho_pt = diPhotons->ptrAt(candIndex)->pt();
		dipho_PToM = dipho_pt/mass;
		//------->full_cat FIXME leaving blank for now, need to implement if/when events are categoriesed. Discuss event interpretatrion.
		full_cat =0;

		//------>MET info
		if (METs->size() != 1) { std::cout << "WARNING - #MET is not 1" << std::endl;}
		MET = METs->ptrAt(0)->pt();
		MET_phi = METs->ptrAt(0)->phi();


		//-------> individual photon properties
		//PHOTON1
		et1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->et();
		eta1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->eta();
		float phi1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->phi();
		r91 = diPhotons->ptrAt(candIndex)->leadingPhoton()->r9();
		sieie1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->sigmaIetaIeta();
		hoe1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->hadronicOverEm();
		sigmaEoE1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->sigEOverE();
		ptoM1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->pt()/mass;
		//---> Isolation variables, unsure if correct methods used...
		chiso1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->pfChgIso03WrtVtx(diPhotons->ptrAt(candIndex)->vtx());
		chisow1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->pfChgIsoWrtWorstVtx04();//no flashgg method for come radius 04... ok to use 0.3?
		phoiso1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->pfPhoIso03();
		phoiso041 = diPhotons->ptrAt(candIndex)->leadingPhoton()->pfPhoIso04(); //unsure of default radius?
		ecaliso03_1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->ecalRecHitSumEtConeDR03();
		hcaliso03_1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->hcalTowerSumEtConeDR03();
		trkiso03_1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->trkSumPtHollowConeDR03();
		pfchiso2_1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->pfChgIso02WrtVtx(diPhotons->ptrAt(candIndex)->vtx());
		isorv1 = (chiso1 + phoiso1 + 2.5 - rho*0.09)*(50./et1); //used in cic analysis, might not be useful for us but we have what we need in hand so adding anyway.
		isowv1 = ( phoiso1 + chisow1 + 2.5 - rho*0.23)*(50./et1);

		isEB1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->isEB();
		sieip1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->sieip();
		etawidth1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->superCluster()->etaWidth();
		phiwidth1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->superCluster()->phiWidth();
		regrerr1 = sigmaEoE1 * diPhotons->ptrAt(candIndex)->leadingPhoton()->energy();
		idmva1 = diPhotons->ptrAt(candIndex)->leadingPhoton()->phoIdMvaDWrtVtx(diPhotons->ptrAt(candIndex)->vtx());
		s4ratio1 =  diPhotons->ptrAt(candIndex)->leadingPhoton()->s4();
		effSigma1 =  diPhotons->ptrAt(candIndex)->leadingPhoton()->esEffSigmaRR();
		scraw1 =  diPhotons->ptrAt(candIndex)->leadingPhoton()->superCluster()->rawEnergy();

		//PHOTON 2
		et2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->et();
		eta2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->eta();
		float phi2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->phi();
		r92 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->r9();
		sieie2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->sigmaIetaIeta();
		hoe2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->hadronicOverEm();
		sigmaEoE2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->sigEOverE();
		ptoM2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pt()/mass;
		isEB2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->isEB();
		//---> Isolation variables, unsure if correct methods used...
		//Methods from flashgg::photon
		chiso2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pfChgIso03WrtVtx(diPhotons->ptrAt(candIndex)->vtx());
		chisow2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pfChgIsoWrtWorstVtx04();//no flashgg method for come radius 04... ok to use 0.3?
		phoiso2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pfPhoIso03();


		phoiso042 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pfPhoIso04(); 
		ecaliso03_2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->ecalRecHitSumEtConeDR03();
		hcaliso03_2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->hcalTowerSumEtConeDR03();
		trkiso03_2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->trkSumPtHollowConeDR03();
		pfchiso2_2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->pfChgIso02WrtVtx(diPhotons->ptrAt(candIndex)->vtx());
		isorv2 = (chiso2 + phoiso2 + 2.5 - rho*0.09)*(50./et2);
		isowv2 = ( phoiso2 + chisow2 + 2.5 - rho*0.23)*(50./et2);

		sieip2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->sieip();
		etawidth2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->superCluster()->etaWidth();
		phiwidth2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->superCluster()->phiWidth();
		regrerr2 = sigmaEoE1 * diPhotons->ptrAt(candIndex)->subLeadingPhoton()->energy();
		idmva2 = diPhotons->ptrAt(candIndex)->subLeadingPhoton()->phoIdMvaDWrtVtx(diPhotons->ptrAt(candIndex)->vtx());
		s4ratio2 =  diPhotons->ptrAt(candIndex)->subLeadingPhoton()->s4();
		effSigma2 =  diPhotons->ptrAt(candIndex)->subLeadingPhoton()->esEffSigmaRR();
		scraw2 =  diPhotons->ptrAt(candIndex)->subLeadingPhoton()->superCluster()->rawEnergy();

		//-----> Cos of photon delta phi
		cosphi = cos(diPhotons->ptrAt(candIndex)->leadingPhoton()->phi()- diPhotons->ptrAt(candIndex)->subLeadingPhoton()->phi());

		//-------> vtx info
		vtx_x= diPhotons->ptrAt(candIndex)->vtx()->x();
		vtx_y= diPhotons->ptrAt(candIndex)->vtx()->y();
		vtx_z= diPhotons->ptrAt(candIndex)->vtx()->z();

		//------>VBF information
		dRphojet1 = -1;
		dRphojet2 = -1;
		vbfcat=-9999;
		dijet_leadEta = -9999;
		dijet_subleadEta= -9999;
		dijet_LeadJPt = -9999;
		dijet_SubJPt= -9999;
		dijet_dEta= -9999;
		dijet_Zep= -9999;
		dijet_dPhi= -9999;
		dijet_Mjj= -9999;
		dijet_MVA= -9999;
		bdt_combined =-9999;
		if(vbftag != NULL)
                {
                    
                    float dRLeadPho = deltaR(vbftag->leadingJet().eta(), vbftag->leadingJet().phi(),
                                             eta1, phi1); 
                    float dRSublPho = deltaR(vbftag->leadingJet().eta(), vbftag->leadingJet().phi(),
                                             eta2, phi2); 
                    dRphojet1 = min(dRSublPho, dRLeadPho); //distance of jet 1 to closest photon
                    dRLeadPho = deltaR(vbftag->subLeadingJet().eta(), vbftag->subLeadingJet().phi(),
                                       eta1, phi1); 
                    dRSublPho = deltaR(vbftag->subLeadingJet().eta(), vbftag->subLeadingJet().phi(),
                                       eta2, phi2);
                    dRphojet2 = min(dRSublPho, dRLeadPho); //distance of jet 2 to closest photon

                    TLorentzVector leadingJet, subLeadingJet, dijet;
                    leadingJet.SetPtEtaPhiE(vbftag->leadingJet().pt(),vbftag->leadingJet().eta(),vbftag->leadingJet().phi(),vbftag->leadingJet().energy());
                    subLeadingJet.SetPtEtaPhiE(vbftag->subLeadingJet().pt(),vbftag->subLeadingJet().eta(),vbftag->subLeadingJet().phi(),vbftag->subLeadingJet().energy());

                    dijet = leadingJet + subLeadingJet;
                    vbfcat = flash_VBFTag_Category;
                    dijet_leadEta = vbftag->VBFMVA().dijet_leadEta; 
                    dijet_subleadEta = vbftag->VBFMVA().dijet_subleadEta ;
                    dijet_LeadJPt = vbftag->VBFMVA().dijet_LeadJPt ;
                    dijet_SubJPt= vbftag->VBFMVA().dijet_SubJPt ;
                    dijet_dEta= fabs(vbftag->leadingJet().eta() - vbftag->subLeadingJet().eta()) ;
                    dijet_dPhi= deltaPhi(dijet.Phi(),diPhotons->ptrAt(candIndex)->phi());
                    dijet_Zep= vbftag->VBFMVA().dijet_Zep ;
                    dijet_Mjj= vbftag->VBFMVA().dijet_Mjj ;
                    dijet_MVA= vbftag->VBFMVA().VBFMVAValue() ;
                    bdt_combined= vbftag->VBFDiPhoDiJetMVA().vbfDiPhoDiJetMvaResult;
		}

		sigmaMrvoM = chosenTag->diPhotonMVA().sigmarv;
		sigmaMwvoM =chosenTag->diPhotonMVA().sigmawv;
		vtxprob =chosenTag->diPhotonMVA().vtxprob;
		ptbal = diPhotons->ptrAt(candIndex)->ptBal(0);
		ptasym = diPhotons->ptrAt(candIndex)->ptAsym(0);
		logspt2 = diPhotons->ptrAt(candIndex)->logSumPt2(0);
		p2conv = diPhotons->ptrAt(candIndex)->pullConv(0); 
		nconv = diPhotons->ptrAt(candIndex)->nConv(0); 
		vtxmva = -1.*diPhotons->ptrAt(candIndex)->vtxProbMVA();
		vtxdz = diPhotons->ptrAt(candIndex)->dZ1(); 
		dipho_mva = chosenTag->diPhotonMVA().mvaValue();

		dipho_mva_cat = flash_Untagged_Category;		

		flashggTreeWithTagSorter->Fill(); 
	} else { //case where TagSorter[0] doesn't exist
		std::cout << "[NO TAG]" <<std::endl;
	}

}
	void 
FlashggTreeMakerWithTagSorter::beginJob()
{
	flashggTreeWithTagSorter = fs_->make<TTree>("flashggTreeWithTagSorter","flashgg tree with Tags");
	flashggTreeWithTagSorter->Branch("njets10", &njets10, "njets10/F");
	flashggTreeWithTagSorter->Branch("njets15", &njets15, "njets15/F");
	flashggTreeWithTagSorter->Branch("njets20", &njets20, "njets20/F");
	flashggTreeWithTagSorter->Branch("dRphojet1", &dRphojet1, "dRphojet1/F");
	flashggTreeWithTagSorter->Branch("dRphojet2", &dRphojet2, "dRphojet2/F");
	flashggTreeWithTagSorter->Branch("run", &run, "runNumber/I");
	flashggTreeWithTagSorter->Branch("lumis", &lumis, "lumiSection/I");
	flashggTreeWithTagSorter->Branch("event", &run, "eventNumber/I");
	flashggTreeWithTagSorter->Branch("nvtx", &nvtx, "nvtx/F");
	flashggTreeWithTagSorter->Branch("rho", &rho, "rho/F");
	flashggTreeWithTagSorter->Branch("xsec_weight", &xsec_weight, "xsec_weight/F");
	flashggTreeWithTagSorter->Branch("full_weight", &full_weight, "full_weight/F");
	flashggTreeWithTagSorter->Branch("pu_weight", &pu_weight, "pu_weight/F");
	flashggTreeWithTagSorter->Branch("pu_n", &pu_n, "pu_n/F");
	flashggTreeWithTagSorter->Branch("mass", &mass, "mass/F");
	flashggTreeWithTagSorter->Branch("dipho_pt", &dipho_pt, "dipho_pt/F");
	flashggTreeWithTagSorter->Branch("full_cat", &full_cat, "full_cat/F");
	flashggTreeWithTagSorter->Branch("et1", &et1, "et1/F");
	flashggTreeWithTagSorter->Branch("et2", &et2, "et2/F");
	flashggTreeWithTagSorter->Branch("eta1", &eta1, "eta1/F");
	flashggTreeWithTagSorter->Branch("eta2", &eta2, "eta2/F");
	flashggTreeWithTagSorter->Branch("r91", &r91, "r91/F");
	flashggTreeWithTagSorter->Branch("r92", &r92, "r92/F");
	flashggTreeWithTagSorter->Branch("sieie1", &sieie1, "sieie1/F");
	flashggTreeWithTagSorter->Branch("sieie2", &sieie2, "sieie2/F");
	flashggTreeWithTagSorter->Branch("hoe1", &hoe1, "hoe1/F");
	flashggTreeWithTagSorter->Branch("hoe2", &hoe2, "hoe2/F");
	flashggTreeWithTagSorter->Branch("sigmaEoE1", &sigmaEoE1, "sigmaEoE1/F");
	flashggTreeWithTagSorter->Branch("sigmaEoE2", &sigmaEoE2, "sigmaEoE2/F");
	flashggTreeWithTagSorter->Branch("ptoM1", &ptoM1, "ptoM1/F");
	flashggTreeWithTagSorter->Branch("ptoM2", &ptoM2, "ptoM2/F");
	flashggTreeWithTagSorter->Branch("isEB1", &isEB1, "isEB1/F");
	flashggTreeWithTagSorter->Branch("isEB2", &isEB2, "isEB2/F");
	flashggTreeWithTagSorter->Branch("chiso1", &chiso1, "chiso1/F");
	flashggTreeWithTagSorter->Branch("chiso2", &chiso2, "chiso2/F");
	flashggTreeWithTagSorter->Branch("chisow1", &chisow1, "chisow1/F");
	flashggTreeWithTagSorter->Branch("chisow2", &chisow2, "chisow2/F");
	flashggTreeWithTagSorter->Branch("phoiso1", &phoiso1, "phoiso1/F");
	flashggTreeWithTagSorter->Branch("phoiso2", &phoiso2, "phoiso2/F");
	flashggTreeWithTagSorter->Branch("phoiso041", &phoiso041, "phoiso041/F");
	flashggTreeWithTagSorter->Branch("phoiso042", &phoiso042, "phoiso042/F");
	flashggTreeWithTagSorter->Branch("ecaliso03_1", &ecaliso03_1, "ecaliso03_1/F");
	flashggTreeWithTagSorter->Branch("ecaliso03_2", &ecaliso03_2, "ecaliso03_2/F");
	flashggTreeWithTagSorter->Branch("hcaliso03_1", &hcaliso03_1, "hcaliso03_1/F");
	flashggTreeWithTagSorter->Branch("hcaliso03_2", &hcaliso03_2, "hcaliso03_2/F");
	flashggTreeWithTagSorter->Branch("trkiso03_1", &trkiso03_1, "trkiso03_1/F");
	flashggTreeWithTagSorter->Branch("trkiso03_2", &trkiso03_2, "trkiso03_2/F");
	flashggTreeWithTagSorter->Branch("pfchiso2_1", &pfchiso2_1, "pfchiso2_1/F");
	flashggTreeWithTagSorter->Branch("pfchiso2_2", &pfchiso2_2, "pfchiso2_2/F");
	flashggTreeWithTagSorter->Branch("sieip1", &sieip1, "sieip1/F");
	flashggTreeWithTagSorter->Branch("sieip2", &sieip2, "sieip2/F");
	flashggTreeWithTagSorter->Branch("etawidth1", &etawidth1, "etawidth1/F");
	flashggTreeWithTagSorter->Branch("phiwidth1", &phiwidth1, "phiwidth1/F");
	flashggTreeWithTagSorter->Branch("etawidth2", &etawidth2, "etawidth2/F");
	flashggTreeWithTagSorter->Branch("phiwidth2", &phiwidth2, "phiwidth2/F");
	flashggTreeWithTagSorter->Branch("regrerr1", &regrerr1, "regrerr1/F");
	flashggTreeWithTagSorter->Branch("regrerr2", &regrerr2, "regrerr2/F");
	flashggTreeWithTagSorter->Branch("cosphi", &cosphi, "cosphi/F");
	flashggTreeWithTagSorter->Branch("genmatch1", &genmatch1, "genmatch1/F");
	flashggTreeWithTagSorter->Branch("genmatch2", &genmatch2, "genmatch2/F");
	flashggTreeWithTagSorter->Branch("idmva1", &idmva1, "idmva1/F");
	flashggTreeWithTagSorter->Branch("idmva2", &idmva2, "idmva2/F");
	flashggTreeWithTagSorter->Branch("vbfcat", &vbfcat, "vbfcat/F");
	flashggTreeWithTagSorter->Branch("MET", &MET, "MET/F");
	flashggTreeWithTagSorter->Branch("MET_phi", &MET_phi, "MET_phi/F");
	flashggTreeWithTagSorter->Branch("isorv1", &isorv1, "isorv1/F");
	flashggTreeWithTagSorter->Branch("isowv1", &isowv1, "isowv1/F");
	flashggTreeWithTagSorter->Branch("isorv2", &isorv2, "isorv2/F");
	flashggTreeWithTagSorter->Branch("isowv2", &isowv2, "isowv2/F");
	flashggTreeWithTagSorter->Branch("s4ratio1", &s4ratio1, "s4ratio1/F");
	flashggTreeWithTagSorter->Branch("s4ratio2", &s4ratio2, "s4ratio2/F");
	flashggTreeWithTagSorter->Branch("effSigma1", &effSigma1, "effSigma1/F");
	flashggTreeWithTagSorter->Branch("effSigma2", &effSigma2, "effSigma2/F");
	flashggTreeWithTagSorter->Branch("scraw1", &scraw1, "scraw1/F");
	flashggTreeWithTagSorter->Branch("scraw2", &scraw2, "scraw2/F");
	flashggTreeWithTagSorter->Branch("vtx_x", &vtx_x, "vtx_x/F");
	flashggTreeWithTagSorter->Branch("vtx_y", &vtx_y, "vtx_y/F");
	flashggTreeWithTagSorter->Branch("vtx_z", &vtx_z, "vtx_z/F");
	flashggTreeWithTagSorter->Branch("gv_x", &gv_x, "gv_x/F");
	flashggTreeWithTagSorter->Branch("gv_y", &gv_y, "gv_y/F");
	flashggTreeWithTagSorter->Branch("gv_z", &gv_z, "gv_z/F");
	flashggTreeWithTagSorter->Branch("dijet_leadEta", &dijet_leadEta, "dijet_leadEta/F");
	flashggTreeWithTagSorter->Branch("dijet_subleadEta", &dijet_subleadEta, "dijet_subleadEta/F");
	flashggTreeWithTagSorter->Branch("dijet_LeadJPt", &dijet_LeadJPt, "dijet_LeadJPt/F");
	flashggTreeWithTagSorter->Branch("dijet_SubJPt", &dijet_SubJPt, "dijet_SubJPt/F");
	flashggTreeWithTagSorter->Branch("dijet_dEta", &dijet_dEta, "dijet_dEta/F");
	flashggTreeWithTagSorter->Branch("dijet_Zep", &dijet_Zep, "dijet_Zep/F");
	flashggTreeWithTagSorter->Branch("dijet_dPhi", &dijet_dPhi, "dijet_dPhi/F");
	flashggTreeWithTagSorter->Branch("dijet_Mjj", &dijet_Mjj, "dijet_Mjj/F");
	flashggTreeWithTagSorter->Branch("dijet_MVA", &dijet_MVA, "dijet_MVA/F");
	flashggTreeWithTagSorter->Branch("bdt_combined", &bdt_combined, "bdt_combined/F");
	flashggTreeWithTagSorter->Branch("issyst", &issyst, "issyst/F");
	flashggTreeWithTagSorter->Branch("name1", &name1, "name1/F");
	flashggTreeWithTagSorter->Branch("sigmaMrvoM", &sigmaMrvoM, "sigmaMrvoM/F");
	flashggTreeWithTagSorter->Branch("sigmaMwvoM", &sigmaMwvoM, "sigmaMwvoM/F");
	flashggTreeWithTagSorter->Branch("vtxprob", &vtxprob, "vtxprob/F");
	flashggTreeWithTagSorter->Branch("ptbal", &ptbal, "ptbal/F");
	flashggTreeWithTagSorter->Branch("ptasym", &ptasym, "ptasym/F");
	flashggTreeWithTagSorter->Branch("logspt2", &logspt2, "logspt2/F");
	flashggTreeWithTagSorter->Branch("p2conv", &p2conv, "p2conv/F");
	flashggTreeWithTagSorter->Branch("nconv", &nconv, "nconv/F");
	flashggTreeWithTagSorter->Branch("vtxmva", &vtxmva, "vtxmva/F");
	flashggTreeWithTagSorter->Branch("vtxdz", &vtxdz, "vtxdz/F");
	flashggTreeWithTagSorter->Branch("dipho_mva", &dipho_mva, "dipho_mva/F");
	flashggTreeWithTagSorter->Branch("dipho_mva_cat", &dipho_mva_cat, "dipho_mva_cat/F");
	flashggTreeWithTagSorter->Branch("dipho_PToM", &dipho_PToM, "dipho_PToM/F");
	flashggTreeWithTagSorter->Branch("flash_Untagged_Category", &flash_Untagged_Category, "flash_Untagged_Category/I");
	flashggTreeWithTagSorter->Branch("flash_VBFTag_Category", &flash_VBFTag_Category, "flash_VBFTag_Category/I");
	flashggTreeWithTagSorter->Branch("subljet_genmatch", &subljet_genmatch, "subljet_genmatch/I");
	flashggTreeWithTagSorter->Branch("leadjet_genmatch", &leadjet_genmatch, "leadjet_genmatch/I");
}

	void 
FlashggTreeMakerWithTagSorter::endJob() 
{
}


void
FlashggTreeMakerWithTagSorter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

typedef FlashggTreeMakerWithTagSorter FlashggFlashggTreeMakerWithTagSorter;
DEFINE_FWK_MODULE(FlashggFlashggTreeMakerWithTagSorter);
