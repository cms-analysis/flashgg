#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/TTHleptonicTag.h"
#include "flashgg/MicroAODFormats/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/TagAlgos/interface/leptonSelection.h"
#include "flashgg/TagAlgos/interface/electronSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"

using namespace std;
using namespace edm;


namespace flashgg {
	class TTHleptonicTagProducer : public EDProducer {

		public:
			TTHleptonicTagProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<View<Jet> > thejetToken_;
			EDGetTokenT<View<Electron> > electronToken_;
			EDGetTokenT<View<pat::Muon> > muonToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
			EDGetTokenT<View<Photon> > photonToken_;
			EDGetTokenT<reco::BeamSpot>  beamSpotToken_;

			//Thresholds
			double leptonPtThreshold_;
			double leptonEtaThreshold_;
			double leadPhoOverMassThreshold_;
			double subleadPhoOverMassThreshold_;
			double MVAThreshold_;
			double deltaRLepPhoThreshold_;
			double deltaRJetLepThreshold_;
			double jetsNumberThreshold_;
			double bjetsNumberThreshold_;
			double jetPtThreshold_;
			double jetEtaThreshold_;

			vector<double> bDiscriminator_;
			string bTag_;	
			double muPFIsoSumRelThreshold_;
			double deltaRMuonJetcountThreshold_;
			double deltaRElectronJetcountThreshold_;
			double PuIDCutoffThreshold_;
			double PhoMVAThreshold_;


	};

	TTHleptonicTagProducer::TTHleptonicTagProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		thejetToken_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("TTHJetTag",InputTag("flashggJets")))),
		electronToken_(consumes<View<flashgg::Electron> >(iConfig.getUntrackedParameter<InputTag> ("TTHElecTag", InputTag("flashggElectrons")))),
		muonToken_(consumes<View<pat::Muon> >(iConfig.getUntrackedParameter<InputTag>("TTHMuonTag",InputTag("slimmedMuons")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA")))),
		photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhoton")))),
		beamSpotToken_(consumes<reco::BeamSpot >(iConfig.getUntrackedParameter<InputTag>("BeamSpotTag",InputTag("offlineBeamSpot"))))
	{

		double default_leptonPtThreshold_ = 20.;
		double default_leptonEtaThreshold_ = 2.4;
		double default_leadPhoOverMassThreshold_ = 0.5;
		double default_subleadPhoOverMassThreshold_ =0.25;
		double default_MVAThreshold_ = -0.6;
		double default_deltaRLepPhoThreshold_ = 0.5;
		double default_deltaRJetLepThreshold_ = 0.5;
		double default_jetsNumberThreshold_ = 2;
		double default_bjetsNumberThreshold_ = 1;
		double default_jetPtThreshold_ = 30.;
		double default_jetEtaThreshold_ = 2.4;

		vector<double> default_bDiscriminator_;
		default_bDiscriminator_.push_back(0.244);
		default_bDiscriminator_.push_back(0.679);

		string default_bTag_ = "combinedSecondaryVertexBJetTags";
		double default_muPFIsoSumRelThreshold_ = 0.2;
		double default_deltaRMuonJetcountThreshold_ = 2.;
		double default_PuIDCutoffThreshold_ = 0.8;
		double default_PhoMVAThreshold_ = -0.2;

		leptonPtThreshold_ = iConfig.getUntrackedParameter<double>("leptonPtThreshold",default_leptonPtThreshold_);
		leptonEtaThreshold_ = iConfig.getUntrackedParameter<double>("leptonEtaThreshold",default_leptonEtaThreshold_);
		leadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>("leadPhoOverMassThreshold",default_leadPhoOverMassThreshold_);
		subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>("subleadPhoOverMassThreshold",default_subleadPhoOverMassThreshold_);
		MVAThreshold_ = iConfig.getUntrackedParameter<double>("MVAThreshold",default_MVAThreshold_);
		deltaRLepPhoThreshold_ = iConfig.getUntrackedParameter<double>("deltaRLepPhoThreshold",default_deltaRLepPhoThreshold_);
		deltaRJetLepThreshold_ = iConfig.getUntrackedParameter<double>("deltaRJetLepThreshold",default_deltaRJetLepThreshold_);
		jetsNumberThreshold_ = iConfig.getUntrackedParameter<double>("jetsNumberThreshold",default_jetsNumberThreshold_);
		bjetsNumberThreshold_ = iConfig.getUntrackedParameter<double>("bjetsNumberThreshold",default_bjetsNumberThreshold_);
		jetPtThreshold_ = iConfig.getUntrackedParameter<double>("jetPtThreshold",default_jetPtThreshold_);
		jetEtaThreshold_ = iConfig.getUntrackedParameter<double>("jetEtaThreshold",default_jetEtaThreshold_);

		bDiscriminator_ = iConfig.getUntrackedParameter<vector<double > >("bDiscriminator",default_bDiscriminator_);
		bTag_ = iConfig.getUntrackedParameter<string>("bTag",default_bTag_);

		muPFIsoSumRelThreshold_ = iConfig.getUntrackedParameter<double>("muPFIsoSumRelThreshold",default_muPFIsoSumRelThreshold_); 
		deltaRMuonJetcountThreshold_=iConfig.getUntrackedParameter<double>("deltaRMuonJetcountThreshold",default_deltaRMuonJetcountThreshold_);
		deltaRElectronJetcountThreshold_=iConfig.getUntrackedParameter<double>("deltaRMuonJetcountThreshold",default_deltaRMuonJetcountThreshold_);
		PuIDCutoffThreshold_ = iConfig.getUntrackedParameter<double>("PuIDCutoffThreshold",default_PuIDCutoffThreshold_);
		PhoMVAThreshold_ = iConfig.getUntrackedParameter<double>("PhoMVAThreshold",default_PhoMVAThreshold_);

		produces<vector<TTHleptonicTag> >(); 
	}

	void TTHleptonicTagProducer::produce( Event & evt, const EventSetup & )

	{

		Handle<View<flashgg::Jet> > theJets;
		evt.getByToken(thejetToken_,theJets);
		const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();

		Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
		evt.getByToken(diPhotonToken_,diPhotons);
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

		Handle<View<pat::Muon> > theMuons;
		evt.getByToken(muonToken_,theMuons);
		const PtrVector<pat::Muon>& muonPointers = theMuons->ptrVector();

		Handle<View<flashgg::Electron> > theElectrons;
		evt.getByToken(electronToken_,theElectrons);
		const PtrVector<flashgg::Electron>& electronPointers = theElectrons->ptrVector();

		Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
		evt.getByToken(mvaResultToken_,mvaResults);
		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();
		std::auto_ptr<vector<TTHleptonicTag> > tthltags(new vector<TTHleptonicTag>);

		Handle<View<flashgg::Photon> > thePhotons;
		evt.getByToken(photonToken_,thePhotons);
		const PtrVector<flashgg::Photon>& photonPointers = thePhotons->ptrVector();

		Handle<reco::BeamSpot> recoBeamSpotHandle;
		evt.getByToken(beamSpotToken_,recoBeamSpotHandle);
		math::XYZPoint beamSpot;
		beamSpot = recoBeamSpotHandle->position();

		assert(diPhotonPointers.size() == mvaResultPointers.size());

		bool tagged = false;
		bool photonSelection = false;
		double idmva1 = 0.;
		double idmva2 = 0.;

		for(unsigned int diphoIndex = 0; diphoIndex < diPhotonPointers.size(); diphoIndex++ )
		{


			PtrVector<pat::Muon> tagMuons;
			PtrVector<Electron> tagElectrons;
			PtrVector<Jet> tagJets;
			PtrVector<Jet> tagBJets;

			edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[diphoIndex];
			edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[diphoIndex];

			TTHleptonicTag tthltags_obj(dipho,mvares);

			if(dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) continue;

			if(dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) continue;

			idmva1 = dipho->leadingPhoton()->getPhoIdMvaDWrtVtx(dipho->getVertex());
			idmva2 = dipho->subLeadingPhoton()->getPhoIdMvaDWrtVtx(dipho->getVertex());
			if(idmva1 <= PhoMVAThreshold_|| idmva2 <= PhoMVAThreshold_) continue;

			if(mvares->result < MVAThreshold_) continue;

			photonSelection = true;

			PtrVector<pat::Muon> goodMuons = selectMuons(muonPointers,dipho, leptonEtaThreshold_ ,leptonPtThreshold_,muPFIsoSumRelThreshold_,deltaRLepPhoThreshold_,deltaRLepPhoThreshold_);

			PtrVector<Electron> goodElectrons = selectElectrons(electronPointers,dipho,photonPointers,beamSpot);

			if (!goodMuons || !goodElectrons) continue;

			for(unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++)
			{

				Ptr<pat::Muon> muon = goodMuons[muonIndex];

				int deltaRMuonJetcount = 0;
				double bDiscriminatorValue = -999.;


				for (unsigned int candIndex_outer =0; candIndex_outer < jetPointers.size() ; candIndex_outer++)
				{
					edm::Ptr<flashgg::Jet> thejet = jetPointers[candIndex_outer];

					if (thejet->getPuJetId(dipho) <  PuIDCutoffThreshold_) continue;

					//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5367
					if(fabs(thejet->eta()) > jetEtaThreshold_) continue; 

					//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5371
					if(thejet->pt() < jetPtThreshold_) continue;

					float dRJetMuon =deltaR(thejet->eta(),thejet->phi(), muon->eta(),muon->phi()) ;

					//https://github.com/njets_btagmediumh2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5370
					if(dRJetMuon < deltaRJetLepThreshold_) continue; 
					deltaRMuonJetcount++;

					if(deltaRMuonJetcount<deltaRMuonJetcountThreshold_) continue;

					tagJets.push_back(thejet);

					bDiscriminatorValue = thejet->bDiscriminator(bTag_.c_str());

					if(bDiscriminatorValue>bDiscriminator_[1]) 
					{
						tagBJets.push_back(thejet);
					}

					//end of jets loop 
				}

				tagMuons.push_back(muon);
				//end of muons loop
			}

			for(unsigned int ElectronIndex = 0; ElectronIndex < goodElectrons.size(); ElectronIndex++)
			{

				Ptr<Electron> Electron = goodElectrons[ElectronIndex];
				TLorentzVector elec_p4;
				elec_p4.SetXYZT(Electron->px(),Electron->py(),Electron->pz(),Electron->energy()); 
				TLorentzVector leadp;
				leadp.SetXYZT(dipho->leadingPhoton()->px(),dipho->leadingPhoton()->py(),dipho->leadingPhoton()->pz(),dipho->leadingPhoton()->energy());	
				TLorentzVector subleadp;
				subleadp.SetXYZT(dipho->subLeadingPhoton()->px(),dipho->subLeadingPhoton()->py(),dipho->subLeadingPhoton()->pz(),dipho->subLeadingPhoton()->energy());	
				if( leadp.DeltaR(elec_p4) <= 1. )continue;
				if( subleadp.DeltaR(elec_p4) <= 1. )continue;
				TLorentzVector eleleadp = elec_p4+leadp;
				TLorentzVector elesubleadp = elec_p4+subleadp;
				if( fabs(eleleadp.M()-91.9)<=10)continue;
				if( fabs(elesubleadp.M()-91.1)<=10)continue;

				int deltaRElectronJetcount = 0;
				double bDiscriminatorValue = -999.;

				for (unsigned int candIndex_outer =0; candIndex_outer < jetPointers.size() ; candIndex_outer++)
				{
					edm::Ptr<flashgg::Jet> thejet = jetPointers[candIndex_outer];

					if (thejet->getPuJetId(dipho) <  PuIDCutoffThreshold_) continue;

					//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5367
					if(fabs(thejet->eta()) > jetEtaThreshold_) continue; 

					//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5371
					if(thejet->pt() < jetPtThreshold_) continue;

					float dRJetElectron =deltaR(thejet->eta(),thejet->phi(), Electron->eta(),Electron->phi()) ;

					//https://github.com/njets_btagmediumh2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5370
					if(dRJetElectron < deltaRJetLepThreshold_) continue; 
					deltaRElectronJetcount++;

					if(deltaRElectronJetcount<deltaRElectronJetcountThreshold_) continue;

					if(tagJets.size()==0){

						tagJets.push_back(thejet);	

						bDiscriminatorValue = thejet->bDiscriminator(bTag_.c_str());

						if(bDiscriminatorValue>bDiscriminator_[1]) 
						{
							tagBJets.push_back(thejet);
						}
					}
					//end of jets loop 
				}

				tagElectrons.push_back(Electron);
				//end of electron loop
			}

			if(tagBJets.size() >= bjetsNumberThreshold_ && tagJets.size() >= jetsNumberThreshold_ && photonSelection && (tagMuons.size()>0 || tagElectrons.size()>0) )
			{

				tthltags_obj.setJets(tagJets);
				tthltags_obj.setBJets(tagBJets);
				tthltags_obj.setMuons(tagMuons);
				tthltags_obj.setElectrons(tagElectrons);	


				tthltags->push_back(tthltags_obj);
				tagged = true;
			}

			//diPho loop end
		}
		evt.put(tthltags);

		std::cout << "event is tagged : "<< tagged << std::endl;

	}

}
typedef flashgg::TTHleptonicTagProducer FlashggTTHleptonicTagProducer;
DEFINE_FWK_MODULE(FlashggTTHleptonicTagProducer);
