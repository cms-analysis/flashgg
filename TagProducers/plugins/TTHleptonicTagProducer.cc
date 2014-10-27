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
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "leptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>

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
			EDGetTokenT<View<pat::Electron> > electronToken_;
			EDGetTokenT<View<pat::Muon> > muonToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

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
			double PuIDCutoffThreshold_;


	};

	TTHleptonicTagProducer::TTHleptonicTagProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		thejetToken_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("TTHJetTag",InputTag("flashggJets")))),
		electronToken_(consumes<View<pat::Electron> >(iConfig.getUntrackedParameter<InputTag> ("TTHElecTag", InputTag("slimmedElectrons")))),
		muonToken_(consumes<View<pat::Muon> >(iConfig.getUntrackedParameter<InputTag>("TTHMuonTag",InputTag("slimmedMuons")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA"))))
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
		PuIDCutoffThreshold_ = iConfig.getUntrackedParameter<double>("PuIDCutoffThreshold",default_PuIDCutoffThreshold_);

		produces<vector<TTHleptonicTag> >(); 
	}

	void TTHleptonicTagProducer::produce( Event & evt, const EventSetup & )

{

 		std::cout << "[NEW EVENT] "<< std::endl;

		Handle<View<flashgg::Jet> > theJets;
		evt.getByToken(thejetToken_,theJets);
		const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();

		Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
		evt.getByToken(diPhotonToken_,diPhotons);
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

		Handle<View<pat::Muon> > theMuons;
		evt.getByToken(muonToken_,theMuons);
		const PtrVector<pat::Muon>& muonPointers = theMuons->ptrVector();

		Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    		evt.getByToken(mvaResultToken_,mvaResults);
    		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();
		std::auto_ptr<vector<TTHleptonicTag> > tthltags(new vector<TTHleptonicTag>);

 		assert(diPhotonPointers.size() == mvaResultPointers.size());

		int count = 0;
		bool photonSelection = false;
		
			for(unsigned int diphoIndex = 0; diphoIndex < diPhotonPointers.size(); diphoIndex++ )
			{
//continue - if statement is true, omit everything after it, going to the beginning of the loop.
				edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[diphoIndex];

				edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[diphoIndex];
 
 std::cout << "dipho->leadingPhoton()->pt() is "<<dipho->leadingPhoton()->pt()<< std::endl;
 std::cout << "(dipho->mass())*leadPhoOverMassThreshold_ is "<<(dipho->mass())*leadPhoOverMassThreshold_<< std::endl;

				if(dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) continue;

 std::cout << "dipho->subLeadingPhoton()->pt() is "<<dipho->subLeadingPhoton()->pt()<< std::endl;
 std::cout << "(dipho->mass())*subleadPhoOverMassThreshold_ is "<<(dipho->mass())*subleadPhoOverMassThreshold_<< std::endl;

				if(dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) continue;

				
std::cout << "mvares->result is "<<mvares->result<< std::endl;
				if(mvares->result < MVAThreshold_) continue;

		
				photonSelection = true;
			
				PtrVector<pat::Muon> goodMuons = selectMuons(muonPointers,dipho, leptonEtaThreshold_ ,leptonPtThreshold_,muPFIsoSumRelThreshold_,deltaRLepPhoThreshold_,deltaRLepPhoThreshold_);

				if (!goodMuons) continue;

				cout << "goodMuons.size() is "<<goodMuons.size()<< endl;


			for(unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++)
			{

					cout << "inside muon loop " << endl;

					Ptr<pat::Muon> muon = goodMuons[muonIndex];
					cout << "muon->pt() is "<< muon->pt() << endl;


					int jetcount = 0; 
					int njets_btagloose = 0;
					int njets_btagmedium = 0;
					int bjetcount = 0;
					int deltaRMuonJetcount = 0;
					bool muonSelection = false;
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

					muonSelection =true;

					jetcount++; 
	
					bDiscriminatorValue = thejet->bDiscriminator(bTag_.c_str());

					if(bDiscriminatorValue>bDiscriminator_[0]) 
					{
						njets_btagloose++;
						if (bDiscriminatorValue<=bDiscriminator_[1])
						{
							bjetcount++;
						}
					}
					if(bDiscriminatorValue>bDiscriminator_[1]) 
					{
						njets_btagmedium++;
						bjetcount++;
					}

std::cout << "njets_btagloose is "<< njets_btagloose << std::endl;
std::cout << "njets_btagmedium is "<< njets_btagmedium << std::endl;

std::cout << "[TTHleptonic] MVA is "<< mvares->result << " and bjetcount is " << bjetcount << "and jetcount is "<< jetcount << std::endl;
				//end of jets loop 
				}

					TTHleptonicTag tthltags_obj(muon);

					if(njets_btagmedium >= bjetsNumberThreshold_ && jetcount >= jetsNumberThreshold_ && photonSelection && muonSelection){
						tthltags->push_back(tthltags_obj);
						count++;

std::cout << "[TTHleptonic] tagged count: "<<count<< std::endl;
					}
			//end of muons loop
			}
//diPho loop end
}
//after all loops
std::cout << "after all loops "<< std::endl;
		evt.put(tthltags);
 std::cout << "All [TTHleptonic] tagged count: "<<count<< std::endl;

	}

}
typedef flashgg::TTHleptonicTagProducer FlashggTTHleptonicTagProducer;
DEFINE_FWK_MODULE(FlashggTTHleptonicTagProducer);
