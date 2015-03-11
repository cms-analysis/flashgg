#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VHtightTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/DataFormats/interface/Electron.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/leptonSelection.h"

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
	class VHtightTagProducer : public EDProducer {

		public:
			VHtightTagProducer( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<View<Jet> > thejetToken_;
			EDGetTokenT<View<flashgg::Electron> > electronToken_;
			EDGetTokenT<View<pat::Muon> > muonToken_;
			EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
			EDGetTokenT<View<pat::MET> > METToken_;
			EDGetTokenT<View<reco::Vertex> > vertexToken_;

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
	  //			double PuIDCutoffThreshold_;
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

			double LowPtEtaPhoThreshold_;
			double MidPtEtaPhoThreshold_;
			double HighEtaPhoThreshold_;
                        double ElectronPtThreshold_;
                        double DeltaRTrkElec_;
                        double TransverseImpactParam_;
                        double LongitudinalImpactParam_;

                        double deltaRPhoElectronThreshold_;
                        double Zmass_;
                        double deltaMassElectronZThreshold_;

			bool hasGoodMuons_highPt = false;
                        bool hasGoodMuons_lowPt = false;

                        bool hasGoodElectrons_highPt = false;
                        bool hasGoodElectrons_lowPt = false;

                        double nonTrigMVAThreshold_;
                        double electronIsoThreshold_;
                        double electronNumOfHitsThreshold_;
			vector<double> EtaCuts_;

	};

	VHtightTagProducer::VHtightTagProducer(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
		thejetToken_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("VHtightJetTag",InputTag("flashggJets")))),
		electronToken_(consumes<View<flashgg::Electron> >(iConfig.getUntrackedParameter<InputTag> ("ElectronTag", InputTag("flashggElectrons")))),
		muonToken_(consumes<View<pat::Muon> >(iConfig.getUntrackedParameter<InputTag>("MuonTag",InputTag("slimmedMuons")))),
		mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA")))),
		METToken_(consumes<View<pat::MET> >(iConfig.getUntrackedParameter<InputTag> ("METTag", InputTag("slimmedMETs")))),
		vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag",InputTag("offlinePrimaryVertices"))))	
		
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

			double default_LowPtEtaPhoThreshold_ = 1.4447;
			double default_MidPtEtaPhoThreshold_ = 1.566;
			double default_HighEtaPhoThreshold_ = 2.5;

			double default_ElectronPtThreshold_ = 20.;
                        double default_DeltaRTrkElec_ = 1.;
                        double default_TransverseImpactParam_ = 0.02;
                        double default_LongitudinalImpactParam_ = 0.2;

                	double default_deltaRPhoElectronThreshold_ = 1.;
                	double default_Zmass_ = 91.9;
                	double default_deltaMassElectronZThreshold_ = 10.;

                        double default_nonTrigMVAThreshold_ = 0.9;
                        double default_electronIsoThreshold_ = 0.15;
                        double default_electronNumOfHitsThreshold_ = 1.;

                 	vector<double> default_EtaCuts_;
                	default_EtaCuts_.push_back(1.442);
                	default_EtaCuts_.push_back(1.566);
                	default_EtaCuts_.push_back(2.5);


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

		LowPtEtaPhoThreshold_ = iConfig.getUntrackedParameter<double>("LowPtEtaPhoThreshold",default_LowPtEtaPhoThreshold_);
		MidPtEtaPhoThreshold_ = iConfig.getUntrackedParameter<double>("MidPtEtaPhoThreshold",default_MidPtEtaPhoThreshold_);
		HighEtaPhoThreshold_ = iConfig.getUntrackedParameter<double>("HighEtaPhoThreshold",default_HighEtaPhoThreshold_);

		ElectronPtThreshold_ = iConfig.getUntrackedParameter<double>("ElectronPtThreshold",default_ElectronPtThreshold_);
                DeltaRTrkElec_ = iConfig.getUntrackedParameter<double>("DeltaRTrkElec",default_DeltaRTrkElec_);
                TransverseImpactParam_ = iConfig.getUntrackedParameter<double>("TransverseImpactParam",default_TransverseImpactParam_);
                LongitudinalImpactParam_ = iConfig.getUntrackedParameter<double>("LongitudinalImpactParam",default_LongitudinalImpactParam_);

                deltaRPhoElectronThreshold_ = iConfig.getUntrackedParameter<double>("deltaRPhoElectronThreshold",default_deltaRPhoElectronThreshold_);
                Zmass_ = iConfig.getUntrackedParameter<double>("Zmass_",default_Zmass_);
                deltaMassElectronZThreshold_ = iConfig.getUntrackedParameter<double>("deltaMassElectronZThreshold_",default_deltaMassElectronZThreshold_);

                nonTrigMVAThreshold_ = iConfig.getUntrackedParameter<double>("nonTrigMVAThreshold",default_nonTrigMVAThreshold_);
                electronIsoThreshold_ = iConfig.getUntrackedParameter<double>("electronIsoThreshold",default_electronIsoThreshold_);
                electronNumOfHitsThreshold_ = iConfig.getUntrackedParameter<double>("electronNumOfHitsThreshold",default_electronNumOfHitsThreshold_);

		EtaCuts_ = iConfig.getUntrackedParameter<vector<double > >("EtaCuts",default_EtaCuts_);

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

Handle<View<flashgg::Electron> > theElectrons;
evt.getByToken(electronToken_,theElectrons);
const PtrVector<flashgg::Electron>& electronPointers = theElectrons->ptrVector();

Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
evt.getByToken(mvaResultToken_,mvaResults);
const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();
std::auto_ptr<vector<VHtightTag> > VHtightTags(new vector<VHtightTag>);

Handle<View<pat::MET> > METs;
evt.getByToken(METToken_,METs);
const PtrVector<pat::MET>& METPointers = METs->ptrVector();

Handle<View<reco::Vertex> > vertices;
evt.getByToken(vertexToken_,vertices);
const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();

assert(diPhotonPointers.size() == mvaResultPointers.size());

bool photonSelection = false;
double idmva1 = 0.;
double idmva2 = 0.;

bool isMuonHighPt = false;
bool isMuonLowPt = false;
bool isInvMassOK = false;

bool isElectronHighPt = false;
bool isElectronLowPt = false;
bool isInvMassOK_elec = false;

for(unsigned int diphoIndex = 0; diphoIndex < diPhotonPointers.size(); diphoIndex++ )
 {
   hasGoodMuons_highPt = false;
   hasGoodMuons_lowPt = false;
   hasGoodElectrons_highPt = false;
   hasGoodElectrons_lowPt = false;
	
   PtrVector<pat::Muon> tagMuons_highPt;
   PtrVector<pat::Muon> tagMuons_lowPt;
   PtrVector<pat::Muon> tagMuons;
   PtrVector<Electron> tagElectrons_highPt;
   PtrVector<Electron> tagElectrons_lowPt;
   PtrVector<Electron> tagElectrons;

   isMuonHighPt = false;
   isMuonLowPt = false;
   isInvMassOK = false;

   isElectronHighPt = false;
   isElectronLowPt = false;
   isInvMassOK_elec = false;

   PtrVector<Jet> tagJets;
   PtrVector<pat::MET> tagMETs;
 
   edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[diphoIndex];
   edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[diphoIndex];

   VHtightTag VHtightTags_obj(dipho, mvares);
   if(dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) continue;

    if(dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) continue;
    if ((fabs(dipho->leadingPhoton()->superCluster()->eta()) > LowPtEtaPhoThreshold_ && fabs(dipho->leadingPhoton()->superCluster()->eta()) < MidPtEtaPhoThreshold_) || fabs(dipho->leadingPhoton()->superCluster()->eta()) > HighEtaPhoThreshold_) continue;
    if ((fabs(dipho->subLeadingPhoton()->superCluster()->eta()) > LowPtEtaPhoThreshold_ && fabs(dipho->subLeadingPhoton()->superCluster()->eta()) < MidPtEtaPhoThreshold_) || fabs(dipho->subLeadingPhoton()->superCluster()->eta()) > HighEtaPhoThreshold_) continue;
     
    idmva1 = dipho->leadingPhoton()->getPhoIdMvaDWrtVtx(dipho->getVertex());
    idmva2 = dipho->subLeadingPhoton()->getPhoIdMvaDWrtVtx(dipho->getVertex());
    if(idmva1 <= PhoMVAThreshold_|| idmva2 <= PhoMVAThreshold_) continue;
    if(mvares->result < MVAThreshold_) continue;
		
    photonSelection = true;

    tagMuons_highPt = selectMuons(muonPointers,dipho, vertexPointers, leptonEtaThreshold_,leptonPtThreshold_,muPFIsoSumRelThreshold_,deltaRLepPhoThreshold_,deltaRLepPhoThreshold_);
    tagMuons_lowPt = selectMuons(muonPointers,dipho, vertexPointers, leptonEtaThreshold_,leptonLowPtThreshold_,muPFIsoSumRelThreshold_,deltaRLowPtMuonPhoThreshold_,deltaRLowPtMuonPhoThreshold_);
    hasGoodMuons_highPt = (tagMuons_highPt.size()>0);
    hasGoodMuons_lowPt = (tagMuons_lowPt.size()>0);

    tagElectrons_highPt = selectElectrons(electronPointers,vertexPointers,leptonPtThreshold_,DeltaRTrkElec_,TransverseImpactParam_,LongitudinalImpactParam_,nonTrigMVAThreshold_,electronIsoThreshold_,electronNumOfHitsThreshold_,EtaCuts_);
    tagElectrons_lowPt = selectElectrons(electronPointers,vertexPointers,leptonLowPtThreshold_,DeltaRTrkElec_,TransverseImpactParam_,LongitudinalImpactParam_, nonTrigMVAThreshold_,electronIsoThreshold_,electronNumOfHitsThreshold_,EtaCuts_);
    hasGoodElectrons_highPt = (tagElectrons_highPt.size()>0);
    hasGoodElectrons_lowPt = (tagElectrons_lowPt.size()>0);

    if(!hasGoodMuons_highPt && !hasGoodMuons_lowPt && !hasGoodElectrons_highPt && !hasGoodElectrons_lowPt) continue;

    if (!hasGoodMuons_highPt && !hasGoodElectrons_highPt && !hasGoodElectrons_lowPt)
    {
     if (tagMuons_lowPt.size() >= 2)
     {
	isMuonLowPt = true;
	float invMass2 = 2*tagMuons_lowPt[0]->pt()*tagMuons_lowPt[1]->pt()*(cosh(tagMuons_lowPt[0]->eta()-tagMuons_lowPt[1]->eta())-cos(tagMuons_lowPt[0]->phi()-tagMuons_lowPt[1]->phi()));
	if(sqrt(invMass2) < invMassLepHighThreshold_  && sqrt(invMass2) > invMassLepLowThreshold_) 
	{
	 tagMuons = tagMuons_lowPt; 
	 isInvMassOK = true;
	}
      }
    }
  
   if(hasGoodMuons_highPt && !hasGoodElectrons_highPt && !hasGoodElectrons_lowPt)
   {
    if (tagMuons_highPt.size() == 1) 
    {
     tagMuons = tagMuons_highPt; 
     isMuonHighPt = true;
    }
   if (tagMuons_highPt.size() >= 2) 
   { 
     isMuonLowPt = true; 
     float invMass2_high = 2*tagMuons_highPt[0]->pt()*tagMuons_highPt[1]->pt()*(cosh(tagMuons_highPt[0]->eta()-tagMuons_highPt[1]->eta())-cos(tagMuons_highPt[0]->phi()-tagMuons_highPt[1]->phi()));
     if(sqrt(invMass2_high) < invMassLepHighThreshold_ && sqrt(invMass2_high) > invMassLepLowThreshold_) 
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
     if (! thejet->passesPuJetId(dipho)) continue;
     if(fabs(thejet->eta()) > jetEtaThreshold_) continue; 
     if(thejet->pt() < jetPtThreshold_) continue;
     float dRJetMuon =deltaR(thejet->eta(),thejet->phi(), muon->eta(),muon->phi()) ;
     if(dRJetMuon < deltaRJetMuonThreshold_) continue;
			
     float dRPhoLeadJet =deltaR(thejet->eta(),thejet->phi(),dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi()) ;
     float dRPhoSubLeadJet = deltaR(thejet->eta(),thejet->phi(),dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi());	
     if(dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_) continue;
     tagJets.push_back(thejet);
     }
   }


 if (!hasGoodElectrons_highPt && !hasGoodMuons_highPt && !hasGoodMuons_lowPt)
  {
   if (tagElectrons_lowPt.size() >= 2)
    {
      isElectronLowPt = true;
      float invMass2 = 2*tagElectrons_lowPt[0]->pt()*tagElectrons_lowPt[1]->pt()*(cosh(tagElectrons_highPt[0]->eta()-tagElectrons_highPt[1]->eta())-cos(tagElectrons_highPt[0]->phi()-tagElectrons_highPt[1]->phi()));
      if(sqrt(invMass2) < invMassLepHighThreshold_  && sqrt(invMass2) > invMassLepLowThreshold_)
       {
        tagElectrons = tagElectrons_lowPt;
        isInvMassOK_elec = true;
       }
    }
   }
 if(hasGoodElectrons_highPt && !hasGoodMuons_highPt && !hasGoodMuons_lowPt)
  {
    if (tagElectrons_highPt.size() == 1)
    {
     tagElectrons = tagElectrons_highPt;
     isElectronHighPt = true;
    }
    if (tagElectrons_highPt.size() >= 2)
    {
     isElectronLowPt = true;
     float invMass2_high = 2*tagElectrons_highPt[0]->pt()*tagElectrons_highPt[1]->pt()*(cosh(tagElectrons_highPt[0]->eta()-tagElectrons_highPt[1]->eta())-cos(tagElectrons_highPt[0]->phi()-tagElectrons_highPt[1]->phi()));
     if(sqrt(invMass2_high) < invMassLepHighThreshold_ && sqrt(invMass2_high) > invMassLepLowThreshold_)
      {
      tagElectrons = tagElectrons_highPt;
      isInvMassOK_elec = true;
      }
     }
                                }
  if ( tagElectrons.size() > 0  ){
   bool photon_veto = false;
   std::vector<const flashgg::Photon*> photons;

   for(unsigned int elecIndex = 0; elecIndex < tagElectrons.size(); elecIndex++)
   {
    Ptr<Electron> elec = tagElectrons[elecIndex];
    TLorentzVector elec_p4;
    elec_p4.SetXYZT(elec->px(),elec->py(),elec->pz(),elec->energy());
    float phi = elec->superCluster()->phi();
    float theta = (2*atan(exp(-elec->superCluster()->eta())));
    float energy = elec->ecalEnergy();
    float px = energy*sin(theta)*cos(phi);
    float py = energy*sin(theta)*sin(phi);
    float pz = energy*cos(theta);

    TLorentzVector elec_superClusterVect;
    elec_superClusterVect.SetXYZT(px,py,pz,energy);
    for(unsigned int phoIndex = 0; phoIndex < photons.size(); phoIndex++ ){
     TLorentzVector p;
     p.SetXYZT(photons.at(phoIndex)->px(),photons.at(phoIndex)->py(),photons.at(phoIndex)->pz(),photons.at(phoIndex)->energy());
     if( p.DeltaR(elec_superClusterVect)< 1.)continue;
     if(&(*photons.at(phoIndex)->superCluster())==&(*elec->superCluster())){
     float TrkElecSCDeltaR = sqrt(elec->deltaEtaSuperClusterTrackAtVtx()*elec->deltaEtaSuperClusterTrackAtVtx()+elec->deltaPhiSuperClusterTrackAtVtx()*elec->deltaPhiSuperClusterTrackAtVtx());
     if(TrkElecSCDeltaR < DeltaRTrkElec_)continue;
    }

    if( p.DeltaR(elec_p4) <= deltaRPhoElectronThreshold_ )continue;
    TLorentzVector elep = elec_p4+p;
    if( fabs(elep.M()-Zmass_)<=deltaMassElectronZThreshold_)continue;
    photon_veto = true;
    }

    if(!photon_veto) break;
    for (unsigned int candIndex_outer =0; candIndex_outer < jetPointers.size() ; candIndex_outer++)
     {
       edm::Ptr<flashgg::Jet> thejet = jetPointers[candIndex_outer];
       if (! thejet->passesPuJetId(dipho)) continue;
       if(fabs(thejet->eta()) > jetEtaThreshold_) continue;
       if(thejet->pt() < jetPtThreshold_) continue;
       float dRJetElec =deltaR(thejet->eta(),thejet->phi(),elec->eta(),elec->phi()) ;
       if(dRJetElec < deltaRJetMuonThreshold_) continue;
       float dRPhoLeadJet =deltaR(thejet->eta(),thejet->phi(),dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi()) ;
       float dRPhoSubLeadJet = deltaR(thejet->eta(),thejet->phi(),dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi());
       if(dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_) continue;
       tagJets.push_back(thejet);
     }			

     }

   }

 if (METPointers.size() != 1) { std::cout << "WARNING - #MET is not 1" << std::endl;}
 Ptr<pat::MET> theMET = METPointers[0];
 if((isMuonHighPt || isElectronHighPt) && theMET->pt() > METThreshold_) 
  {
   tagMETs.push_back(theMET);
  }
 if( photonSelection && (((isMuonHighPt && tagMETs.size()>0 && tagJets.size() < jetsNumberThreshold_) || (isMuonLowPt && isInvMassOK)) ||(isElectronHighPt && tagMETs.size()>0 && tagJets.size() < jetsNumberThreshold_) || (isElectronLowPt && isInvMassOK_elec)  ) )
  {
   VHtightTags_obj.setJets(tagJets);
   VHtightTags_obj.setMuons(tagMuons);
   VHtightTags_obj.setElectrons(tagElectrons);
   VHtightTags_obj.setMET(tagMETs);
   VHtightTags_obj.setDiPhotonIndex(diphoIndex);
   VHtightTags->push_back(VHtightTags_obj);
  }
}
evt.put(VHtightTags);

}

}
typedef flashgg::VHtightTagProducer FlashggVHtightTagProducer;
DEFINE_FWK_MODULE(FlashggVHtightTagProducer);
