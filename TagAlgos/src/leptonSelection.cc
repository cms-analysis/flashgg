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



	PtrVector<flashgg::Electron> selectElectrons(const PtrVector<flashgg::Electron>& ElectronPointers,Ptr<flashgg::DiPhotonCandidate> dipho,PtrVector<flashgg::Photon>& pPointers, math::XYZPoint & beamSpot){

		float TrkSCDeltaR = 1.;

		PtrVector<flashgg::Electron> goodElectrons;

		Ptr<reco::Vertex> vtx = dipho->getVertex();

		for(unsigned int pIndex=0; pIndex < pPointers.size(); pIndex++ ){		
			
			Ptr<flashgg::Photon> p = pPointers[pIndex];

			TLorentzVector pho_p4; 
			
		 pho_p4.SetXYZT(p->px(),p->py(),p->pz(),p->energy()); 

			for(unsigned int ElectronIndex=0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ){

				Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];

				//D0
				float dxy = ((vtx->position().x() - beamSpot.x())*Electron->py() - (vtx->position().y() - beamSpot.y()))/Electron->pt();											//DZ
				float dz = vtx->position().z() - ((vtx->position().x()*Electron->px()+vtx->position().y()*Electron->py())*Electron->pz())/(Electron->pt()*Electron->pt()); 

				if(dxy>0.02)continue;
				if(dz>0.2)continue;


				float phi = Electron->superCluster()->phi();	
				float theta = (2*atan(exp(-Electron->superCluster()->eta())));
				float energy = Electron->ecalEnergy();
				float px = energy*sin(theta)*cos(phi);
				float py = energy*sin(theta)*sin(phi);
				float pz = energy*cos(theta);

				TLorentzVector elec_superclusterVect;

				 elec_superclusterVect.SetXYZT(px,py,pz,energy);

				if(pho_p4.DeltaR(elec_superclusterVect)){
					goodElectrons.push_back(Electron);
				}

				else if(!(&(*p->superCluster())==&(*Electron->superCluster()))){	

					float TrkElecSCDeltaR = sqrt(Electron->deltaEtaSuperClusterTrackAtVtx()*Electron->deltaEtaSuperClusterTrackAtVtx()+Electron->deltaPhiSuperClusterTrackAtVtx()*Electron->deltaPhiSuperClusterTrackAtVtx());

					if(TrkElecSCDeltaR < TrkSCDeltaR){

						goodElectrons.push_back( Electron );
					}

				}
			}
		}
		return goodElectrons;
	}
}
