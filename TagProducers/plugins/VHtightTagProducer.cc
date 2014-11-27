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
#include "flashgg/TagFormats/interface/VHtightTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/TagAlgos/interface/leptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <math.h> 

using namespace std;
using namespace edm;


namespace flashgg {
	class VHtightTagProducer : public EDProducer {

		public:
			VHtightTagProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<View<Jet> > thejetToken_;
			EDGetTokenT<View<pat::Electron> > electronToken_;
			EDGetTokenT<View<pat::Muon> > muonToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
			EDGetTokenT<View<pat::MET> > METToken_;

			//Thresholds
 			double leptonPtThreshold_;
			double leptonEtaThreshold_;
			double leadPhoOverMassThreshold_;
			double subleadPhoOverMassThreshold_;
			double MVAThreshold_;
			double deltaRLepPhoThreshold_;
			double jetsNumberThreshold_;
			double jetPtThreshold_;
			double jetEtaThreshold_;
			double muPFIsoSumRelThreshold_;
			double PuIDCutoffThreshold_;
			double PhoMVAThreshold_;
			double METThreshold_;

			double deltaRJetMuonThreshold_;
			double invMassLepLowThreshold_;
			double invMassLepHighThreshold_;
			double numberOfLowPtMuonsThreshold_;
			double numberOfHighPtMuonsThreshold_;
			double leptonLowPtThreshold_;
			double deltaRLowPtMuonPhoThreshold_;

			double deltaRPhoLeadJet_;
			double deltaRPhoSubLeadJet_;


	};

	VHtightTagProducer::VHtightTagProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		thejetToken_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("VHtightJetTag",InputTag("flashggJets")))),
		electronToken_(consumes<View<pat::Electron> >(iConfig.getUntrackedParameter<InputTag> ("ElectronTag", InputTag("slimmedElectrons")))),
		muonToken_(consumes<View<pat::Muon> >(iConfig.getUntrackedParameter<InputTag>("MuonTag",InputTag("slimmedMuons")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA")))),
		METToken_(consumes<View<pat::MET> >(iConfig.getUntrackedParameter<InputTag> ("METTag", InputTag("slimmedMETs"))))
	{

 			double default_leptonPtThreshold_ = 20.;
			double default_leptonEtaThreshold_ = 2.4;
			double default_leadPhoOverMassThreshold_ = 0.375;
			double default_subleadPhoOverMassThreshold_ =0.25;
			double default_MVAThreshold_ = -0.6;
			double default_deltaRLepPhoThreshold_ = 1;
			double default_jetsNumberThreshold_ = 3;
			double default_jetPtThreshold_ = 30.;
			double default_jetEtaThreshold_ = 2.4;
			double default_muPFIsoSumRelThreshold_ = 0.2;
			double default_PuIDCutoffThreshold_ = 0.8;
			double default_PhoMVAThreshold_ = -0.2;
			double default_METThreshold_=45.;
			double default_deltaRJetMuonThreshold_ = 0.5;
			double default_invMassLepLowThreshold_ = 70.;
			double default_invMassLepHighThreshold_ = 110.;
			double default_numberOfLowPtMuonsThreshold_ = 2.;
			double default_numberOfHighPtMuonsThreshold_ = 1.;
			double default_leptonLowPtThreshold_ = 10.;
			double default_deltaRLowPtMuonPhoThreshold_ = 0.5;

			double default_deltaRPhoLeadJet_= 0.5;
			double default_deltaRPhoSubLeadJet_= 0.5;

 		leptonPtThreshold_ = iConfig.getUntrackedParameter<double>("leptonPtThreshold",default_leptonPtThreshold_);
		leptonEtaThreshold_ = iConfig.getUntrackedParameter<double>("leptonEtaThreshold",default_leptonEtaThreshold_);
		leadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>("leadPhoOverMassThreshold",default_leadPhoOverMassThreshold_);
		subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>("subleadPhoOverMassThreshold",default_subleadPhoOverMassThreshold_);
		MVAThreshold_ = iConfig.getUntrackedParameter<double>("MVAThreshold",default_MVAThreshold_);
		deltaRLepPhoThreshold_ = iConfig.getUntrackedParameter<double>("deltaRLepPhoThreshold",default_deltaRLepPhoThreshold_);
		jetsNumberThreshold_ = iConfig.getUntrackedParameter<double>("jetsNumberThreshold",default_jetsNumberThreshold_);
		jetPtThreshold_ = iConfig.getUntrackedParameter<double>("jetPtThreshold",default_jetPtThreshold_);
		jetEtaThreshold_ = iConfig.getUntrackedParameter<double>("jetEtaThreshold",default_jetEtaThreshold_);
		muPFIsoSumRelThreshold_ = iConfig.getUntrackedParameter<double>("muPFIsoSumRelThreshold",default_muPFIsoSumRelThreshold_); 
		PuIDCutoffThreshold_ = iConfig.getUntrackedParameter<double>("PuIDCutoffThreshold",default_PuIDCutoffThreshold_);
		PhoMVAThreshold_ = iConfig.getUntrackedParameter<double>("PhoMVAThreshold",default_PhoMVAThreshold_);
		METThreshold_ = iConfig.getUntrackedParameter<double>("METThreshold",default_METThreshold_);

		deltaRJetMuonThreshold_ = iConfig.getUntrackedParameter<double>("deltaRJetMuonThreshold",default_deltaRJetMuonThreshold_);
		invMassLepLowThreshold_ = iConfig.getUntrackedParameter<double>("invMassLepLowThreshold",default_invMassLepLowThreshold_);
		invMassLepHighThreshold_ = iConfig.getUntrackedParameter<double>("invMassLepHighThreshold",default_invMassLepHighThreshold_);
		numberOfLowPtMuonsThreshold_ = iConfig.getUntrackedParameter<double>("numberOfLowPtMuonsThreshold",default_numberOfLowPtMuonsThreshold_);
		numberOfHighPtMuonsThreshold_ = iConfig.getUntrackedParameter<double>("numberOfHighPtMuonsThreshold",default_numberOfHighPtMuonsThreshold_);
		leptonLowPtThreshold_ = iConfig.getUntrackedParameter<double>("leptonLowPtThreshold",default_leptonLowPtThreshold_);
		deltaRLowPtMuonPhoThreshold_ = iConfig.getUntrackedParameter<double>("deltaRLowPtMuonPhoThreshold",default_deltaRLowPtMuonPhoThreshold_);
		deltaRPhoLeadJet_ = iConfig.getUntrackedParameter<double>("deltaRPhoLeadJet",default_deltaRPhoLeadJet_);
		deltaRPhoSubLeadJet_ = iConfig.getUntrackedParameter<double>("deltaRPhoSubLeadJet",default_deltaRPhoSubLeadJet_);

		produces<vector<VHtightTag> >(); 
	}

	void VHtightTagProducer::produce( Event & evt, const EventSetup & )

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

		Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    		evt.getByToken(mvaResultToken_,mvaResults);
    		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();
		std::auto_ptr<vector<VHtightTag> > VHtightTags(new vector<VHtightTag>);

		Handle<View<pat::MET> > METs;
		evt.getByToken(METToken_,METs);
		const PtrVector<pat::MET>& METPointers = METs->ptrVector();

 		assert(diPhotonPointers.size() == mvaResultPointers.size());

		bool photonSelection = false;
		double idmva1 = 0.;
		double idmva2 = 0.;

		bool isMuonHighPt = false;
		bool isMuonLowPt = false;
		bool isInvMassOK = false;

			for(unsigned int diphoIndex = 0; diphoIndex < diPhotonPointers.size(); diphoIndex++ )
			{
				

				PtrVector<pat::Muon> tagMuons_highPt;
				PtrVector<pat::Muon> tagMuons_lowPt;
				PtrVector<pat::Muon> tagMuons;

				isMuonHighPt = false;
				isMuonLowPt = false;
				isInvMassOK = false;

				PtrVector<Jet> tagJets;
				PtrVector<pat::MET> tagMETs;

				edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[diphoIndex];
				edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[diphoIndex];

				VHtightTag VHtightTags_obj(dipho, mvares);

				if(dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) continue;

				if(dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) continue;

				idmva1 = dipho->leadingPhoton()->getPhoIdMvaDWrtVtx(dipho->getVertex());
				idmva2 = dipho->subLeadingPhoton()->getPhoIdMvaDWrtVtx(dipho->getVertex());
				if(idmva1 <= PhoMVAThreshold_|| idmva2 <= PhoMVAThreshold_) continue;

				if(mvares->result < MVAThreshold_) continue;
		
				photonSelection = true;

				tagMuons_highPt = selectMuons(muonPointers,dipho, leptonEtaThreshold_,leptonPtThreshold_,muPFIsoSumRelThreshold_,deltaRLepPhoThreshold_,deltaRLepPhoThreshold_);
				tagMuons_lowPt = selectMuons(muonPointers,dipho,leptonEtaThreshold_,leptonLowPtThreshold_,muPFIsoSumRelThreshold_,deltaRLowPtMuonPhoThreshold_,deltaRLowPtMuonPhoThreshold_);
				if(!tagMuons_highPt && !tagMuons_lowPt) continue;

				if (!tagMuons_highPt)
				{

					if (tagMuons_lowPt.size() >= 2)
					{

						isMuonLowPt = true;

						float invMass2 = 2*tagMuons_lowPt[0]->pt()*tagMuons_lowPt[1]->pt()*(cosh(tagMuons_lowPt[0]->eta()-tagMuons_lowPt[1]->eta())-cos(tagMuons_lowPt[0]->phi()-tagMuons_lowPt[1]->phi()));

						if(sqrt(invMass2) < numberOfHighPtMuonsThreshold_ && sqrt(invMass2) > invMassLepLowThreshold_) 
							{
								tagMuons = tagMuons_lowPt; 
								
								isInvMassOK = true;
							}

					}

				}

				else
				{
					if (tagMuons_highPt.size() == 1) 
					{
						tagMuons = tagMuons_highPt; 
						isMuonHighPt = true;
					}
					if (tagMuons_highPt.size() >= 2) 
					{ 
					//to put this event to VT tight with 2 muons; to avoid cut on MET
					isMuonLowPt = true; 

					float invMass2_high = 2*tagMuons_highPt[0]->pt()*tagMuons_highPt[1]->pt()*(cosh(tagMuons_highPt[0]->eta()-tagMuons_highPt[1]->eta())-cos(tagMuons_highPt[0]->phi()-tagMuons_highPt[1]->phi()));

					if(sqrt(invMass2_high) < numberOfHighPtMuonsThreshold_ && sqrt(invMass2_high) > invMassLepLowThreshold_) 
						{
							tagMuons = tagMuons_highPt;
							
							isInvMassOK = true;
						
						}



 					}
				}


			for(unsigned int muonIndex = 0; muonIndex < tagMuons.size(); muonIndex++)
			{

					Ptr<pat::Muon> muon = tagMuons[muonIndex];
					
			for (unsigned int candIndex_outer =0; candIndex_outer < jetPointers.size() ; candIndex_outer++)
				{
					edm::Ptr<flashgg::Jet> thejet = jetPointers[candIndex_outer];
					
					if (thejet->getPuJetId(dipho) <  PuIDCutoffThreshold_) continue;

					if(fabs(thejet->eta()) > jetEtaThreshold_) continue; 

					if(thejet->pt() < jetPtThreshold_) continue;

					float dRJetMuon =deltaR(thejet->eta(),thejet->phi(), muon->eta(),muon->phi()) ;
					if(dRJetMuon < deltaRJetMuonThreshold_) continue;
					
					float dRPhoLeadJet =deltaR(thejet->eta(),thejet->phi(),dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi()) ;
					float dRPhoSubLeadJet = deltaR(thejet->eta(),thejet->phi(),dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi());	

					if(dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_) continue;
		 
					tagJets.push_back(thejet);

				//end of jets loop 
				}

			//end of muons loop 
			}



			//------>MET info
			if (METPointers.size() != 1) { std::cout << "WARNING - #MET is not 1" << std::endl;}
			Ptr<pat::MET> theMET = METPointers[0];
			if(isMuonHighPt && theMET->pt() > METThreshold_) 
			{
				tagMETs.push_back(theMET);
			}

			if( photonSelection && ((isMuonHighPt && tagMETs.size()>0 && tagJets.size() < jetsNumberThreshold_) || (isMuonLowPt && isInvMassOK)))
			{

				VHtightTags_obj.setJets(tagJets);
				VHtightTags_obj.setMuons(tagMuons);
				VHtightTags_obj.setMET(tagMETs);
				VHtightTags_obj.setDiPhotonIndex(diphoIndex);
				VHtightTags->push_back(VHtightTags_obj);
			}

			//diPho loop end
			}
			evt.put(VHtightTags);

}

}
typedef flashgg::VHtightTagProducer FlashggVHtightTagProducer;
DEFINE_FWK_MODULE(FlashggVHtightTagProducer);
