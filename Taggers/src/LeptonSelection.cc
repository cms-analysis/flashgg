#include "flashgg/Taggers/interface/LeptonSelection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


namespace flashgg {

    std::vector<edm::Ptr<flashgg::Muon> > selectAllMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, 
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold )
    {

        std::vector<edm::Ptr<flashgg::Muon> > goodMuons;
        for( unsigned int muonIndex = 0; muonIndex < muonPointers.size(); muonIndex++ ) {
            Ptr<flashgg::Muon> muon = muonPointers[muonIndex];
            
            /*
            std::cout << " Muon index " << muonIndex << " has pt eta weight: "
                      << muon->pt() << " " << muon->eta() << " "
                      << muon->centralWeight() << std::endl;
            auto weightList = muon->weightList();
            for( unsigned int i = 0 ; i < weightList.size() ; i++ ) {
                std::cout << "    " << weightList[i] << " " << muon->weight( weightList[i] );
            }
            std::cout << std::endl;
            */

            if( fabs( muon->eta() ) > muonEtaThreshold ) continue; 
            if( muon->pt() < muonPtThreshold ) continue; 

            int vtxInd = 0;
            double dzmin = 9999;
            for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {
                Ptr<reco::Vertex> vtx = vertexPointers[ivtx];
                if( !muon->innerTrack() ) continue; 
                if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {                    
                    dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );
                    vtxInd = ivtx;
                }
            }

            Ptr<reco::Vertex> best_vtx = vertexPointers[vtxInd];            

            if( !muon::isTightMuon( *muon, *best_vtx ) ) continue; 
            
            double muPFIsoSumRel = ( muon->pfIsolationR04().sumChargedHadronPt 
                                     + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt 
                                            + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );

            if( muPFIsoSumRel > muPFIsoSumRelThreshold ) continue; 
            
            goodMuons.push_back( muon );
        }
        return goodMuons;
    }

    std::vector<edm::Ptr<flashgg::Muon> > selectMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold )
    {

        std::vector<edm::Ptr<flashgg::Muon> > goodMuons;
        std::vector<edm::Ptr<flashgg::Muon> > allGoodMuons= selectAllMuons( muonPointers, vertexPointers, muonEtaThreshold, muonPtThreshold, muPFIsoSumRelThreshold );

        for( unsigned int muonIndex = 0; muonIndex <allGoodMuons.size(); muonIndex++ ) {
            Ptr<flashgg::Muon> muon = allGoodMuons[muonIndex];
            float dRPhoLeadMuon = deltaR( muon->eta(), muon->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadMuon = deltaR( muon->eta(), muon->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ); 

            if( dRPhoLeadMuon < dRPhoLeadMuonThreshold || dRPhoSubLeadMuon < dRPhoSubLeadMuonThreshold ) continue; 
            
            goodMuons.push_back( muon );
        }
        return goodMuons;
    }

    std::vector<edm::Ptr<Electron> > selectAllElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
                                                      const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,
                                                         double TransverseImpactParam, double LongitudinalImpactParam, vector<double> NonTrigMVAThresholds, 
                                                         vector<double> NonTrigEtaCuts, double IsoThreshold,
                                                         double NumOfMissingHitsThreshold, vector<double> EtaCuts )
    {



        assert(NonTrigMVAThresholds.size()==3);
        assert(NonTrigEtaCuts.size()==3);
        assert(EtaCuts.size()==3);


        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;
        
        for( unsigned int ElectronIndex = 0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ) {

            Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];
            float Electron_eta = fabs( Electron->superCluster()->eta() );
            Ptr<reco::Vertex> Electron_vtx = chooseElectronVertex( Electron, vertexPointers );
            
            float dxy = Electron->gsfTrack()->dxy( Electron_vtx->position() );
            float dz = Electron->gsfTrack()->dz( Electron_vtx->position() );
            
            if( Electron_eta > EtaCuts[2] || ( Electron_eta > EtaCuts[0] && Electron_eta < EtaCuts[1] ) )  continue; 
            
            if( Electron->pt() < ElectronPtThreshold ) continue; 

            int iDet=0;

            if(fabs( Electron->superCluster()->eta()<NonTrigEtaCuts[0] )) iDet=0;
            else if((fabs( Electron->superCluster()->eta()))>NonTrigEtaCuts[0] && (fabs( Electron->superCluster()->eta()))<NonTrigEtaCuts[1]) iDet=1;
            else if((fabs( Electron->superCluster()->eta()))>NonTrigEtaCuts[1] && (fabs( Electron->superCluster()->eta()))<NonTrigEtaCuts[2]) iDet=2;
            
            double  NonTrigMVAThreshold=NonTrigMVAThresholds[iDet];
            if( Electron->nonTrigMVA() < NonTrigMVAThreshold ) continue; 

            // Put 3 different thresholds as recommended by egamma POG
            
            if( Electron->standardHggIso() / Electron->pt() > IsoThreshold ) continue; 

            if( Electron->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS ) > NumOfMissingHitsThreshold ) { continue; }
            //std::cout << "[DEBUG] passed  missing hits cut." << std::endl;

            if( Electron->hasMatchedConversion() ) continue; 
            //std::cout << "[DEBUG] passed hasConversion." << std::endl;

            if( fabs(dxy) > TransverseImpactParam ) continue; 
            //std::cout << "[DEBUG] passed dxy cut " << std::endl;
            if( fabs(dz) > LongitudinalImpactParam ) continue; 
            //std::cout << "[DEBUG] passed dz cut." << std::endl;

            //std::cout << "[DEBUG] LeptonSelection.c   ... pushing back Electron index " << ElectronIndex << std::endl;
            goodElectrons.push_back( Electron );
        }

        return goodElectrons;
    }
    std::vector<edm::Ptr<Electron> > selectElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
                                                      const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,  
                                                      double TransverseImpactParam, double LongitudinalImpactParam, vector<double> NonTrigMVAThresholds, 
                                                      vector<double> NonTrigEtaCuts, double IsoThreshold,
                                                      double NumOfMissingHitsThreshold, vector<double> EtaCuts, double deltaRPhoElectronThreshold, double DeltaRTrkElec,
                                                      double deltaMassElectronZThreshold ){
 
        std::vector<const flashgg::Photon *> photons;        
        photons.push_back( dipho->leadingPhoton() );
        photons.push_back( dipho->subLeadingPhoton() );
        
        double Zmass_ = 91.9;

        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;
        
        std::vector<edm::Ptr<flashgg::Electron> > allGoodElectrons=selectAllElectrons( ElectronPointers, vertexPointers , ElectronPtThreshold, 
                                                                                       TransverseImpactParam, LongitudinalImpactParam, 
                                                                                       NonTrigMVAThresholds,  NonTrigEtaCuts, IsoThreshold, NumOfMissingHitsThreshold, EtaCuts);
        
        for( unsigned int ElectronIndex = 0; ElectronIndex < allGoodElectrons.size(); ElectronIndex++ ) {
            
            Ptr<flashgg::Electron> Electron = allGoodElectrons[ElectronIndex];
            
            TLorentzVector elec_p4;
            elec_p4.SetXYZT( Electron->px(), Electron->py(), Electron->pz(), Electron->energy() );
            
            float phi = Electron->superCluster()->phi();
            float theta = ( 2 * atan( exp( -Electron->superCluster()->eta() ) ) );
            float energy = Electron->ecalEnergy();
            float px = energy * sin( theta ) * cos( phi );
            float py = energy * sin( theta ) * sin( phi );
            float pz = energy * cos( theta );
            
            TLorentzVector elec_superClusterVect;
            elec_superClusterVect.SetXYZT( px, py, pz, energy );
            
            bool photon_veto = false;
            
            for( unsigned int phoIndex = 0; phoIndex <photons.size(); phoIndex++ ) {
                
                TLorentzVector p;
                p.SetXYZT( photons.at( phoIndex )->px(), photons.at( phoIndex )->py(), photons.at( phoIndex )->pz(), photons.at( phoIndex )->energy() );
                if( p.DeltaR( elec_superClusterVect ) < 1. ) photon_veto=true;
                if( &( *photons.at( phoIndex )->superCluster() ) == &( *Electron->superCluster() ) ) {
                    float TrkElecSCDeltaR = sqrt( Electron->deltaEtaSuperClusterTrackAtVtx() * Electron->deltaEtaSuperClusterTrackAtVtx() +
                                                  Electron->deltaPhiSuperClusterTrackAtVtx() * Electron->deltaPhiSuperClusterTrackAtVtx() );
                    if( TrkElecSCDeltaR < DeltaRTrkElec ) photon_veto=true;
                }
                
                if( p.DeltaR( elec_p4 ) <= deltaRPhoElectronThreshold ) photon_veto=true;
                TLorentzVector elep = elec_p4 + p;
                if( fabs( elep.M() - Zmass_ ) <= deltaMassElectronZThreshold ) photon_veto=true;   
            }
                        
            if(!photon_veto) goodElectrons.push_back( Electron );
        }
        
        return goodElectrons;
    }

    Ptr<reco::Vertex>  chooseElectronVertex( Ptr<flashgg::Electron> &elec, const std::vector<edm::Ptr<reco::Vertex> > &vertices )
    {

        double vtx_dz = 1000000;
        unsigned int min_dz_vtx = -1;
        for( unsigned int vtxi = 0; vtxi < vertices.size(); vtxi++ ) {

            Ptr<reco::Vertex> vtx = vertices[vtxi];
            
            if( fabs( vtx_dz ) > fabs( elec->gsfTrack()->dz( vtx->position() ) ) ) {
                
                vtx_dz = fabs(elec->gsfTrack()->dz( vtx->position() ));
                min_dz_vtx = vtxi;
            }
        }
        return vertices[min_dz_vtx];
    }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

