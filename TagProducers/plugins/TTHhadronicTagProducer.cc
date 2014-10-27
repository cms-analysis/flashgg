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
#include "flashgg/TagFormats/interface/TTHhadronicTag.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>

using namespace std;
using namespace edm;


namespace flashgg {
	class TTHhadronicTagProducer : public EDProducer {

		public:
			TTHhadronicTagProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<View<Jet> > thejetToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

			//Thresholds
			double leadPhoOverMassThreshold_;
			double subleadPhoOverMassThreshold_;
			double MVAThreshold_;
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

	TTHhadronicTagProducer::TTHhadronicTagProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		thejetToken_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("TTHJetTag",InputTag("flashggJets")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA"))))
	{

			double default_leadPhoOverMassThreshold_ = 0.5;
			double default_subleadPhoOverMassThreshold_ =0.25;
			double default_MVAThreshold_ = -0.2;
			double default_jetsNumberThreshold_ = 5;
			double default_bjetsNumberThreshold_ = 1;
			double default_jetPtThreshold_ = 30.;
			double default_jetEtaThreshold_ = 2.4;
			vector<double> default_bDiscriminator_;
    			default_bDiscriminator_.push_back(0.244);
    			default_bDiscriminator_.push_back(0.679);
			string default_bTag_ = "combinedSecondaryVertexBJetTags";
			double default_PuIDCutoffThreshold_ = 0.8;

		leadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>("leadPhoOverMassThreshold",default_leadPhoOverMassThreshold_);
		subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>("subleadPhoOverMassThreshold",default_subleadPhoOverMassThreshold_);
		MVAThreshold_ = iConfig.getUntrackedParameter<double>("MVAThreshold",default_MVAThreshold_);
		jetsNumberThreshold_ = iConfig.getUntrackedParameter<double>("jetsNumberThreshold",default_jetsNumberThreshold_);
		bjetsNumberThreshold_ = iConfig.getUntrackedParameter<double>("bjetsNumberThreshold",default_bjetsNumberThreshold_);
		jetPtThreshold_ = iConfig.getUntrackedParameter<double>("jetPtThreshold",default_jetPtThreshold_);
		jetEtaThreshold_ = iConfig.getUntrackedParameter<double>("jetEtaThreshold",default_jetEtaThreshold_);
 		bDiscriminator_ = iConfig.getUntrackedParameter<vector<double > >("bDiscriminator",default_bDiscriminator_);
		bTag_ = iConfig.getUntrackedParameter<string>("bTag",default_bTag_);
		PuIDCutoffThreshold_ = iConfig.getUntrackedParameter<double>("PuIDCutoffThreshold",default_PuIDCutoffThreshold_);

		produces<vector<TTHhadronicTag> >(); 
	}

	void TTHhadronicTagProducer::produce( Event & evt, const EventSetup & )

{

 		std::cout << "[NEW EVENT] "<< std::endl;

		Handle<View<flashgg::Jet> > theJets;
		evt.getByToken(thejetToken_,theJets);
		const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();

		Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
		evt.getByToken(diPhotonToken_,diPhotons);
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

		Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    		evt.getByToken(mvaResultToken_,mvaResults);
    		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();
		std::auto_ptr<vector<TTHhadronicTag> > tthhtags(new vector<TTHhadronicTag>);

 		assert(diPhotonPointers.size() == mvaResultPointers.size());

		int count = 0;
		bool photonSelection = false;
		
			for(unsigned int diphoIndex = 0; diphoIndex < diPhotonPointers.size(); diphoIndex++ )
			{
//continue - if statement is true, omit everything after it, going to the beginning of the loop.
				edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[diphoIndex];

				edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[diphoIndex];
 
				if(dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) continue;
 std::cout << "dipho->leadingPhoton()->pt() is "<<dipho->leadingPhoton()->pt()<< std::endl;
 std::cout << "(dipho->mass())*leadPhoOverMassThreshold_ is "<<(dipho->mass())*leadPhoOverMassThreshold_<< std::endl;

				if(dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) continue;
 std::cout << "dipho->subLeadingPhoton()->pt() is "<<dipho->subLeadingPhoton()->pt()<< std::endl;
 std::cout << "(dipho->mass())*subleadPhoOverMassThreshold_ is "<<(dipho->mass())*subleadPhoOverMassThreshold_<< std::endl;

				

				if(mvares->result < MVAThreshold_) continue;
std::cout << "mvares->result is "<<mvares->result<< std::endl;
		
					photonSelection = true;
			
					int jetcount = 0; 
					int njets_btagloose = 0;
					int njets_btagmedium = 0;
					int bjetcount = 0;
					double bDiscriminatorValue = -999.;
					

			for (unsigned int candIndex_outer =0; candIndex_outer < jetPointers.size() ; candIndex_outer++)
				{
					edm::Ptr<flashgg::Jet> thejet = jetPointers[candIndex_outer];
					
					if (thejet->getPuJetId(dipho) <  PuIDCutoffThreshold_) continue;

					//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5367
					if(fabs(thejet->eta()) > jetEtaThreshold_) continue; 

					//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5371
					if(thejet->pt() < jetPtThreshold_) continue;

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

std::cout << "[TTHhadronic] diPho MVA is "<< mvares->result << " and bjetcount is " << bjetcount << "and jetcount is "<< jetcount << std::endl;
				

					TTHhadronicTag tthhtags_obj(thejet);

					if(njets_btagmedium >= bjetsNumberThreshold_ && jetcount >= jetsNumberThreshold_ && photonSelection){
						tthhtags->push_back(tthhtags_obj);
						count++;

std::cout << "[TTHhadronic] tagged count: "<<count<< std::endl;
					}
//end of jets loop 
				}
//diPho loop end
}
//after all loops
std::cout << "after all loops "<< std::endl;
		evt.put(tthhtags);
 std::cout << "All [TTHhadronic] tagged count: "<<count<< std::endl;

	}

}
typedef flashgg::TTHhadronicTagProducer FlashggTTHhadronicTagProducer;
DEFINE_FWK_MODULE(FlashggTTHhadronicTagProducer);
