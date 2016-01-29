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

    // FIXME JM : this will go away when egamma recipes will be directly implemented in flashgg::electrons
    
    vector<bool> EgammaIDs(const edm::Ptr<flashgg::Electron>& elec, const std::vector<edm::Ptr<reco::Vertex> > &pvPointers ){

        vector<bool> IDs;

        bool doPassLoose=false;
        bool doPassMedium=false;
        bool doPassMVA90=false;
        bool doPassMVA80=false;
        
        bool isEB=false;
        
        float eleta=elec->superCluster()->eta();
        if(fabs(eleta) <= 1.479 ) isEB = true;    
        
        double vtx_dz = 1000000.;
        unsigned int min_dz_vtx = -1;
        
        for( unsigned int vtxi = 0; vtxi < pvPointers.size(); vtxi++ ) {            
            Ptr<reco::Vertex> vtx = pvPointers[vtxi];            
            if( vtx_dz > fabs(elec->gsfTrack()->dz( vtx->position() )) ) {                
                vtx_dz = fabs( elec->gsfTrack()->dz( vtx->position() ) );
                min_dz_vtx = vtxi;
            }
        }
        
        Ptr<reco::Vertex> best_vtx_elec = pvPointers[min_dz_vtx];
        
        float elfull5x5_sigmaIetaIeta = elec->full5x5_sigmaIetaIeta();
        float eldEtaIn = elec->deltaEtaSuperClusterTrackAtVtx();
        float eldPhiIn = elec->deltaPhiSuperClusterTrackAtVtx();
        float elhOverE = elec->hcalOverEcal();
        float elRelIsoEA = elec->standardHggIso()/elec->pt();
        
        float elooEmooP =-999 ; 
        
        float elNonTrigMVA = elec->nonTrigMVA();
        
        
        if( elec->ecalEnergy() == 0 ){
            elooEmooP = 1e30;
        }else if( !std::isfinite(elec->ecalEnergy())){	    
            elooEmooP = 1e30;
        }else{
            elooEmooP = fabs(1.0/elec->ecalEnergy() - elec->eSuperClusterOverP()/elec->ecalEnergy() );
        }

        float elDxy = fabs( elec->gsfTrack()->dxy( best_vtx_elec->position()) ) ;
        float elDz = fabs( elec->gsfTrack()->dz( best_vtx_elec->position())) ;
        int elMissedHits = elec->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS);

	    if(isEB){

            if(fabs(eleta) < 0.8 && elNonTrigMVA >= 0.913286) doPassMVA90 = true;
            if(fabs(eleta) >= 0.8 && elNonTrigMVA >= 0.805013) doPassMVA90 = true;
            
            if(fabs(eleta) < 0.8 && elNonTrigMVA >= 0.967083) doPassMVA80 = true;
            if(fabs(eleta) >= 0.8 && elNonTrigMVA >= 0.929117) doPassMVA80 = true;

            if(
               elfull5x5_sigmaIetaIeta < 0.0103
               && fabs(eldEtaIn) < 0.0105
               && fabs(eldPhiIn) < 0.115
               && elhOverE < 0.104
               && elRelIsoEA < 0.0893
               && elooEmooP < 0.102
               && fabs(elDxy) < 0.0261
               && fabs(elDz) < 0.41
               && elMissedHits <=	2 
               ) doPassLoose = true;
            
            if( 
               elfull5x5_sigmaIetaIeta < 0.0101
               && fabs(eldEtaIn) < 0.0103
               && fabs(eldPhiIn) < 0.0336
               && elhOverE < 0.0876
               && elRelIsoEA < 0.0766
               && elooEmooP < 0.0174
               && fabs(elDxy) < 0.0118
               && fabs(elDz) < 0.373
               && elMissedHits <=	2 
                ) doPassMedium = true;
            
        }else{

	    if(elNonTrigMVA >= 0.358969) doPassMVA90 = true;	    
	    if(elNonTrigMVA >= 0.726311) doPassMVA80 = true;

            if( 
               elfull5x5_sigmaIetaIeta < 0.0301
               && fabs(eldEtaIn) < 0.00814
               && fabs(eldPhiIn) < 0.182
               && elhOverE < 0.0897
               && elRelIsoEA < 0.121
               && elooEmooP < 0.126
               && fabs(elDxy) < 0.118
               && fabs(elDz) < 0.822
               && elMissedHits <=	1 
                ) doPassLoose = true;
            
            if( 
               elfull5x5_sigmaIetaIeta < 0.0283
               && fabs(eldEtaIn) < 0.00733
               && fabs(eldPhiIn) < 0.114
               && elhOverE < 0.0678
               && elRelIsoEA < 0.0678
               && elooEmooP < 0.0898
               && fabs(elDxy) < 0.0739
               && fabs(elDz) < 0.602
               && elMissedHits <=	1 
                ) doPassMedium = true;
            }

        IDs.push_back(doPassLoose);
        IDs.push_back(doPassMedium);
        IDs.push_back(doPassMVA90);
        IDs.push_back(doPassMVA80);

        return IDs;
        
    }

    std::vector<edm::Ptr<Electron> > selectStdAllElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
                                                            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers,  double ElectronPtThreshold,
                                                        vector<double> EtaCuts , bool useMVARecipe, bool useLooseID){
                                                         
        
        assert(EtaCuts.size()==3);
        int idIndex=0;
        if (useLooseID) idIndex=0;
        else idIndex=1;
        if(useMVARecipe) idIndex+=2;
        

        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;
        
        for( unsigned int ElectronIndex = 0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ) {

            Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];
            float Electron_eta = fabs( Electron->superCluster()->eta() );
            
            if( Electron_eta > EtaCuts[2] || ( Electron_eta > EtaCuts[0] && Electron_eta < EtaCuts[1] ) )  continue;             
            if( Electron->pt() < ElectronPtThreshold ) continue; 
            
            vector<bool> IDs=EgammaIDs(Electron, vertexPointers );
            if(!IDs[idIndex]) continue;

            if( Electron->hasMatchedConversion() ) continue; 

            goodElectrons.push_back( Electron );
        }
        
        return goodElectrons;
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
        
        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;
        std::vector<edm::Ptr<flashgg::Electron> > allGoodElectrons=selectAllElectrons( ElectronPointers, vertexPointers , ElectronPtThreshold, 
                                                                                       TransverseImpactParam, LongitudinalImpactParam, 
                                                                                       NonTrigMVAThresholds,  NonTrigEtaCuts, IsoThreshold, NumOfMissingHitsThreshold, EtaCuts);
        
        for( unsigned int ElectronIndex = 0; ElectronIndex < allGoodElectrons.size(); ElectronIndex++ ) {
            Ptr<flashgg::Electron> electron = allGoodElectrons[ElectronIndex];
            bool photon_veto = phoVeto(electron, dipho,  deltaRPhoElectronThreshold,  DeltaRTrkElec, deltaMassElectronZThreshold);
            if(!photon_veto) goodElectrons.push_back( electron );
        }
        
        return goodElectrons;
    }

    std::vector<edm::Ptr<Electron> > selectStdElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
                                                      const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,  vector<double> EtaCuts,
                                                         bool useMVARecipe, bool useLooseID,
                                                         double deltaRPhoElectronThreshold, double DeltaRTrkElec, double deltaMassElectronZThreshold ){
 
        std::vector<const flashgg::Photon *> photons;        
        photons.push_back( dipho->leadingPhoton() );
        photons.push_back( dipho->subLeadingPhoton() );
        
        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;        
        std::vector<edm::Ptr<flashgg::Electron> > allGoodElectrons=selectStdAllElectrons( ElectronPointers, vertexPointers , 
                                                                                          ElectronPtThreshold, EtaCuts, useMVARecipe, useLooseID);
        
        for( unsigned int ElectronIndex = 0; ElectronIndex < allGoodElectrons.size(); ElectronIndex++ ) {
            
            Ptr<flashgg::Electron> electron = allGoodElectrons[ElectronIndex];            
            bool photon_veto = phoVeto(electron, dipho,  deltaRPhoElectronThreshold,  DeltaRTrkElec, deltaMassElectronZThreshold);                        
            if(!photon_veto) goodElectrons.push_back( electron );
        }
        
        return goodElectrons;
    }

    std::vector<edm::Ptr<Electron> > selectElectronsNotMatchingPhotons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, const Ptr<flashgg::DiPhotonCandidate> dipho){

        // for ttH hadronic tag zero leptons cut
        std::vector<const flashgg::Photon *> photons;        
        photons.push_back( dipho->leadingPhoton() );
        photons.push_back( dipho->subLeadingPhoton() );
        
        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;

        for( unsigned int ElectronIndex = 0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ) {
            
            Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];
            bool veto=false;            
            for( unsigned int phoIndex = 0; phoIndex <photons.size(); phoIndex++ ) {                                
                if( &( *photons.at( phoIndex )->superCluster() ) == &( *Electron->superCluster() ) ) veto=true; 
            }
            
            if(!veto) goodElectrons.push_back( Electron );
        }
        
        return goodElectrons;

    }
    
    bool phoVeto(  const Ptr<flashgg::Electron> electron , const Ptr<flashgg::DiPhotonCandidate> diphoton , float deltaRPhoElectronThreshold, float DeltaRTrkElec, float deltaMassElectronZThreshold ){
    
        
        double Zmass_ = 91.9;
        
        std::vector<const flashgg::Photon *> photons;        
        photons.push_back( diphoton->leadingPhoton() );
        photons.push_back( diphoton->subLeadingPhoton() );
        
        TLorentzVector elec_p4;
        elec_p4.SetXYZT( electron->px(), electron->py(), electron->pz(), electron->energy() );
        
        TLorentzVector elec_superClusterVect;
        elec_superClusterVect.SetXYZT(electron->superCluster()->position().x(),electron->superCluster()->position().y(),
                                       electron->superCluster()->position().z(),electron->ecalEnergy());            

        bool photon_veto = false;
        
        for( unsigned int phoIndex = 0; phoIndex <photons.size(); phoIndex++ ) {

            float drPhoElec=deltaR( electron->eta(), electron->phi(), photons.at( phoIndex )->superCluster()->eta(),  photons.at( phoIndex )->superCluster()->phi() ); 
            if( drPhoElec < deltaRPhoElectronThreshold ) photon_veto=true;
            
            TLorentzVector p;
            p.SetXYZT( photons.at( phoIndex )->px(), photons.at( phoIndex )->py(), photons.at( phoIndex )->pz(), photons.at( phoIndex )->energy() );
            
            if( p.DeltaR( elec_superClusterVect ) < deltaRPhoElectronThreshold ) photon_veto=true;
            
            if( &( *photons.at( phoIndex )->superCluster() ) == &( *electron->superCluster() ) ) {
                float TrkElecSCDeltaR = sqrt( electron->deltaEtaSuperClusterTrackAtVtx() * electron->deltaEtaSuperClusterTrackAtVtx() +
                                              electron->deltaPhiSuperClusterTrackAtVtx() * electron->deltaPhiSuperClusterTrackAtVtx() );
                if( TrkElecSCDeltaR < DeltaRTrkElec ) photon_veto=true;               
            }
            
            if( p.DeltaR( elec_p4 ) < deltaRPhoElectronThreshold ) photon_veto=true;
            
            TLorentzVector elep = elec_p4 + p;
            if( fabs( elep.M() - Zmass_ ) < deltaMassElectronZThreshold ) photon_veto=true;   
        }
        
        return photon_veto;
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

