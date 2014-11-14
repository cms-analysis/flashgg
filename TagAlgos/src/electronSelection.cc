#include "flashgg/TagAlgos/interface/electronSelection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h" 
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


namespace flashgg {

	PtrVector<Electron> selectElectrons(const PtrVector<flashgg::Electron>& ElectronPointers,Ptr<flashgg::DiPhotonCandidate> dipho,PtrVector<flashgg::Photon>& pPointers, math::XYZPoint & beamSpot){

		PtrVector<flashgg::Electron> goodElectrons;

		Ptr<reco::Vertex> vtx = dipho->getVertex();

		for(unsigned int pIndex=0; pIndex < pPointers.size(); pIndex++ ){		

			Ptr<flashgg::Photon> p = pPointers[pIndex];

			TLorentzVector pho_p4; 

			pho_p4.SetXYZT(p->px(),p->py(),p->pz(),p->energy()); 

			for(unsigned int ElectronIndex=0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ){

				Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];

				//D0
				float dxy = ((vtx->position().x() - beamSpot.x())*Electron->py() - (vtx->position().y() - beamSpot.y()))/Electron->pt();							  //DZ
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

				if( pho_p4.DeltaR(elec_superclusterVect) > 1. ){
					goodElectrons.push_back(Electron);
				}

				else if(!(&(*p->superCluster())==&(*Electron->superCluster()))){	
					float TrkElecSCDeltaR = sqrt(Electron->deltaEtaSuperClusterTrackAtVtx()*Electron->deltaEtaSuperClusterTrackAtVtx()+Electron->deltaPhiSuperClusterTrackAtVtx()*Electron->deltaPhiSuperClusterTrackAtVtx());

					if(TrkElecSCDeltaR > 1.){

						goodElectrons.push_back( Electron );
					}
				}
			}
		}
		return goodElectrons;
	}
}
