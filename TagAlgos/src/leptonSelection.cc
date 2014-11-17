#include "flashgg/TagAlgos/interface/leptonSelection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h" 
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


namespace flashgg {

	PtrVector<pat::Muon> selectMuons(const PtrVector<pat::Muon>& muonPointers,Ptr<flashgg::DiPhotonCandidate> dipho, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold, double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold) {

		PtrVector<pat::Muon> goodMuons;

		Ptr<reco::Vertex> vtx = dipho->getVertex();

		for(unsigned int muonIndex = 0; muonIndex < muonPointers.size(); muonIndex++ )
		{
			Ptr<pat::Muon> muon = muonPointers[muonIndex]; 			

			if(fabs(muon->eta()) > muonEtaThreshold) continue;
			if(muon->pt() < muonPtThreshold) continue;

			//https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon and https://cmssdt.cern.ch/SDT/lxr/source/RecoBTag/SoftLepton/plugins/SoftPFMuonTagInfoProducer.cc#0135

			if(!muon::isTightMuon(*muon,*vtx)) continue;

			//I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt
			//https://cmssdt.cern.ch/SDT/doxygen/CMSSW_5_3_14/doc/html/df/d33/structreco_1_1MuonPFIsolation.html
			// deltaBeta correction

			double muPFIsoSumRel =(muon->pfIsolationR04().sumChargedHadronPt+max(0.,muon->pfIsolationR04().sumNeutralHadronEt+muon->pfIsolationR04().sumPhotonEt-0.5 *muon->pfIsolationR04().sumPUPt))/(muon->pt());   
			if(muPFIsoSumRel>muPFIsoSumRelThreshold)continue;	

			float dRPhoLeadMuon =deltaR(muon->eta(),muon->phi(),dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi()) ;
			float dRPhoSubLeadMuon = deltaR(muon->eta(),muon->phi(),dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi());	

			//https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5369
			if(dRPhoLeadMuon < dRPhoLeadMuonThreshold || dRPhoSubLeadMuon < dRPhoSubLeadMuonThreshold) continue;		 

			goodMuons.push_back( muon );
		}
		return goodMuons;
	}

	PtrVector<Electron> selectElectrons(const PtrVector<flashgg::Electron>& ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho, const PtrVector<reco::Vertex>& vertexPointers ,double ElectronPtThreshold,double DeltaRTrkElec, double TransverseImpactParam, double LongitudinalImapctParam){

		PtrVector<flashgg::Electron> goodElectrons;

		TLorentzVector phoLead_p4;

		TLorentzVector phoSublead_p4;

		phoLead_p4.SetXYZT(dipho->leadingPhoton()->px(),dipho->leadingPhoton()->py(),dipho->leadingPhoton()->pz(),dipho->leadingPhoton()->energy()); 

		phoSublead_p4.SetXYZT(dipho->subLeadingPhoton()->px(),dipho->subLeadingPhoton()->py(),dipho->subLeadingPhoton()->pz(),dipho->subLeadingPhoton()->energy()); 

		for(unsigned int ElectronIndex=0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ){

			Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];

		        Ptr<reco::Vertex> Electron_vtx = ChooseElectronVertex(Electron,vertexPointers);

			if(Electron->pt()<ElectronPtThreshold)continue;

			float dxy = Electron->gsfTrack()->dxy(Electron_vtx->position());							  
			float dz = Electron->gsfTrack()->dz(Electron_vtx->position()); 
			if(dxy>TransverseImpactParam)continue;
			if(dz>LongitudinalImapctParam)continue;

			if(!(&(*dipho->leadingPhoton()->superCluster())==&(*Electron->superCluster())) || !(&(*dipho->leadingPhoton()->superCluster())==&(*Electron->superCluster())) ){	
				float TrkElecSCDeltaR = sqrt(Electron->deltaEtaSuperClusterTrackAtVtx()*Electron->deltaEtaSuperClusterTrackAtVtx()+Electron->deltaPhiSuperClusterTrackAtVtx()*Electron->deltaPhiSuperClusterTrackAtVtx());

				if(TrkElecSCDeltaR < DeltaRTrkElec){

					goodElectrons.push_back( Electron );
				}

			}

		}

		return goodElectrons;
	}

	
	Ptr<reco::Vertex>  ChooseElectronVertex(Ptr<flashgg::Electron> & elec,const PtrVector<reco::Vertex> & vertices){

		double vtx_dz = 1000000;
		unsigned int min_dz_vtx = -1;
		for(unsigned int vtxi=0; vtxi<vertices.size();vtxi++){

			Ptr<reco::Vertex> vtx = vertices[vtxi];	
			
			if(vtx_dz > elec->gsfTrack()->dz(vtx->position())){

				vtx_dz=elec->gsfTrack()->dz(vtx->position());
				min_dz_vtx = vtxi;
			}
		}
		return vertices[min_dz_vtx]; 
	}
}
