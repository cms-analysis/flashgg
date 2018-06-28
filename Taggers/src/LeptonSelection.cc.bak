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

    std::vector<edm::Ptr<flashgg::Muon> > selectAllMuonsSum16( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, 
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muMiniIsoSumRelThreshold )
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
            
            double muMiniIsoSumRel = muon->fggMiniIsoSumRel();

            if( muMiniIsoSumRel > muMiniIsoSumRelThreshold ) continue; 
            
            goodMuons.push_back( muon );
        }
        return goodMuons;
    }

    std::vector<edm::Ptr<flashgg::Muon> > selectMuonsSum16( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muMiniIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold )
    {

        std::vector<edm::Ptr<flashgg::Muon> > goodMuons;
        std::vector<edm::Ptr<flashgg::Muon> > allGoodMuons= selectAllMuonsSum16( muonPointers, vertexPointers, muonEtaThreshold, muonPtThreshold, muMiniIsoSumRelThreshold );

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
    
    vector<bool> EgammaIDs(const edm::Ptr<flashgg::Electron>& elec, const std::vector<edm::Ptr<reco::Vertex> > &pvPointers, double rho ){

        vector<bool> IDs;

        bool doPassLoose=false;
        bool doPassMedium=false;
        bool doPassMVA90=false;
        bool doPassMVA80=false;
        
        bool isEB=false;
        
        float eleta=elec->superCluster()->eta();
        if(fabs(eleta) <= 1.479 ) isEB = true;    
        
        //if electron dxy and dz cuts are used : 
        // double vtx_dz = 1000000.;
        // unsigned int min_dz_vtx = -1;
        
        // for( unsigned int vtxi = 0; vtxi < pvPointers.size(); vtxi++ ) {            
        //     Ptr<reco::Vertex> vtx = pvPointers[vtxi];            
        //     if( vtx_dz > fabs(elec->gsfTrack()->dz( vtx->position() )) ) {                
        //         vtx_dz = fabs( elec->gsfTrack()->dz( vtx->position() ) );
        //         min_dz_vtx = vtxi;
        //     }
        // }
        
        // Ptr<reco::Vertex> best_vtx_elec = pvPointers[min_dz_vtx];

        // float elDxy = fabs( elec->gsfTrack()->dxy( best_vtx_elec->position()) ) ;
        // float elDz = fabs( elec->gsfTrack()->dz( best_vtx_elec->position())) ;
        
        float elfull5x5_sigmaIetaIeta = elec->full5x5_sigmaIetaIeta();
        //float eldEtaIn = elec->deltaEtaSuperClusterTrackAtVtx();
        float eldEtaInSeed = 999.;
        if(elec->superCluster().isNonnull() && elec->superCluster()->seed().isNonnull()) {
            eldEtaInSeed = elec->deltaEtaSuperClusterTrackAtVtx() - elec->superCluster()->eta() + elec->superCluster()->seed()->eta();
            //std::cout<<"In LeptonSelection.cc : for Recalculation of EGAMM IDs, eldEtaInSeed  " << std::endl;
        }

        float eldPhiIn = elec->deltaPhiSuperClusterTrackAtVtx();
        float elhOverE = elec->hadronicOverEm();//hcalOverEcal();
        float elRelIsoEA = elec->standardHggIso();//corrected below
        
        float elooEmooP =999. ;
        if(fabs(elec->ecalEnergy()) > 0.){
            float ecal_energy_inverse = 1.0/elec->ecalEnergy();
            float eSCoverP = elec->eSuperClusterOverP();
            elooEmooP = abs(1.0 - eSCoverP)*ecal_energy_inverse;
        }
        
        float elNonTrigMVA = elec->nonTrigMVA();
        bool passConversionVeto= elec->passConversionVeto();
        
        int elMissedHits = elec->gsfTrack()->hitPattern().numberOfAllHits( reco::HitPattern::MISSING_INNER_HITS);

        //for isolation recalculation        
        float Aeff = 0;
        //cmssw/RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt
        if( fabs(eleta) <= 1.0000 ){
            Aeff = 0.1703;
        } else if( fabs(eleta) <= 1.4790 ){
            Aeff = 0.1715;
        } else if( fabs(eleta) <= 2.0000 ){
            Aeff = 0.1213;
        } else if( fabs(eleta) <= 2.2000 ){
            Aeff = 0.1230;
        } else if( fabs(eleta) <= 2.3000 ){
            Aeff = 0.1635;
        } else if( fabs(eleta) <= 2.4000 ){
            Aeff = 0.1937;
        } else if( fabs(eleta) <= 5.0000 ){
            Aeff = 0.2393;
        }
        elRelIsoEA = ( elec->pfIsolationVariables().sumChargedHadronPt + std::max( 0.0d, elec->pfIsolationVariables().sumNeutralHadronEt + elec->pfIsolationVariables().sumPhotonEt - Aeff*rho) ) / elec->pt() ;

        //values modified for Moriond2017 recommendations
        //MVA : https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2#Recommended_MVA_recipes_for_2016
        //CutBased : https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for
	    if(isEB){

            if(fabs(eleta) < 0.8 && elNonTrigMVA >= 0.837) doPassMVA90 = true;
            if(fabs(eleta) >= 0.8 && elNonTrigMVA >= 0.715) doPassMVA90 = true;
            
            if(fabs(eleta) < 0.8 && elNonTrigMVA >= 0.941) doPassMVA80 = true;
            if(fabs(eleta) >= 0.8 && elNonTrigMVA >= 0.899) doPassMVA80 = true;

            if(
               elfull5x5_sigmaIetaIeta < 0.011
               && fabs(eldEtaInSeed) < 0.00477
               && fabs(eldPhiIn) < 0.222
               && elhOverE < 0.298
               && elRelIsoEA < 0.0994
               && fabs(elooEmooP) < 0.241
               //&& fabs(elDxy) < 0.0261
               //&& fabs(elDz) < 0.41
               && elMissedHits <=	1 
               && passConversionVeto
               ) doPassLoose = true;
            
            if( 
               elfull5x5_sigmaIetaIeta < 0.00998
               && fabs(eldEtaInSeed) < 0.00311
               && fabs(eldPhiIn) < 0.103
               && elhOverE < 0.253
               && elRelIsoEA < 0.0695
               && fabs(elooEmooP) < 0.134
               //&& fabs(elDxy) < 0.0261
               //&& fabs(elDz) < 0.41
               && elMissedHits <=	1 
               && passConversionVeto
                ) doPassMedium = true;
            
        }else{

	    if(elNonTrigMVA >= 0.357) doPassMVA90 = true;	    
	    if(elNonTrigMVA >= 0.758) doPassMVA80 = true;

            if(
               elfull5x5_sigmaIetaIeta < 0.0314
               && fabs(eldEtaInSeed) < 0.00868
               && fabs(eldPhiIn) < 0.213
               && elhOverE < 0.101
               && elRelIsoEA < 0.107
               && fabs(elooEmooP) < 0.14
               //&& fabs(elDxy) < 0.0261
               //&& fabs(elDz) < 0.41
               && elMissedHits <=	1 
               && passConversionVeto
               ) doPassLoose = true;
            
            if( 
               elfull5x5_sigmaIetaIeta < 0.0298
               && fabs(eldEtaInSeed) < 0.00609
               && fabs(eldPhiIn) < 0.045
               && elhOverE < 0.0878
               && elRelIsoEA < 0.0821
               && fabs(elooEmooP) < 0.13
               //&& fabs(elDxy) < 0.0261
               //&& fabs(elDz) < 0.41
               && elMissedHits <=	1 
               && passConversionVeto
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
                                                            vector<double> EtaCuts , bool useMVARecipe, bool useLooseID,
                                                            double rho, bool isData){
                                                         
        
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
            
            //vector<bool> IDs=EgammaIDs(Electron, vertexPointers, rho );
            vector<bool> IDs;
            //if(!isData){//for MC use the stored Egamma IDs // fixed with reminiAOD microAOD, no special case reqd for data
                IDs.clear();
                IDs.push_back(Electron->passLooseId());
                IDs.push_back(Electron->passMediumId());
                IDs.push_back(Electron->passMVAMediumId());
                IDs.push_back(Electron->passMVATightId());
                //} else {//for data use calculated IDs : FIXME saghosh : for data MicroAOD not containing latest egamma ids
                //IDs=EgammaIDs(Electron, vertexPointers, rho );
                //}
            
            if(!IDs[idIndex]) continue;

            if( Electron->hasMatchedConversion() ) continue; 

            goodElectrons.push_back( Electron );
        }
        
        return goodElectrons;
    }


    std::vector<edm::Ptr<Electron> > selectAllElectronsSum16( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
                                                            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers,  double ElectronPtThreshold,
                                                              vector<double> EtaCuts , bool useMVARecipe, bool useLooseID,
                                                              double elMiniIsoEBThreshold, double elMiniIsoEEThreshold,
                                                              double TransverseImpactParam_EB, double LongitudinalImpactParam_EB, double TransverseImpactParam_EE, double LongitudinalImpactParam_EE,
                                                              double rho, bool isData){


        assert(EtaCuts.size()==3);
        int idIndex=0;
        if (useLooseID) idIndex=0;
        else idIndex=1;
        if(useMVARecipe) idIndex+=2;


        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;

        for( unsigned int ElectronIndex = 0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ) {

            Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];
            float Electron_eta = fabs( Electron->superCluster()->eta() );

            //if electron dxy and dz cuts are used :
            // double vtx_dz = 1000000.;
            // unsigned int min_dz_vtx = -1;
            // for( unsigned int vtxi = 0; vtxi < vertexPointers.size(); vtxi++ ) {
            //     Ptr<reco::Vertex> vtx = vertexPointers[vtxi];
            //     if( vtx_dz > fabs( Electron->gsfTrack()->dz( vtx->position() )) ) {
            //         vtx_dz = fabs( Electron->gsfTrack()->dz( vtx->position() ) );
            //         min_dz_vtx = vtxi;
            //     }
            // }
            // Ptr<reco::Vertex> best_vtx_elec = vertexPointers[min_dz_vtx];

            // float Electron_dxy = fabs( Electron->gsfTrack()->dxy( best_vtx_elec->position()) ) ;
            // float Electron_dz = fabs( Electron->gsfTrack()->dz( best_vtx_elec->position())) ;

            if( Electron_eta > EtaCuts[2] || ( Electron_eta > EtaCuts[0] && Electron_eta < EtaCuts[1] ) )  continue;
            if( Electron->pt() < ElectronPtThreshold ) continue;

            //vector<bool> IDs=EgammaIDs(Electron, vertexPointers, rho ); 
            vector<bool> IDs;
            //if(!isData){//for MC use the stored Egamma IDs //fixed with reminiAOD microAOD, no special case reqd for data
                IDs.clear();
                IDs.push_back(Electron->passLooseId());
                IDs.push_back(Electron->passMediumId());
                IDs.push_back(Electron->passMVAMediumId());
                IDs.push_back(Electron->passMVATightId());
                //} else {//for data use calculated IDs : FIXME saghosh : for data MicroAOD not containing latest egamma ids
                //IDs=EgammaIDs(Electron, vertexPointers, rho );
                //}
            
            if(!IDs[idIndex]) continue;

            if( Electron->hasMatchedConversion() ) continue;

            if( fabs( Electron->superCluster()->eta() ) <= 1.479 ){
                if( Electron->fggMiniIsoSumRel() > elMiniIsoEBThreshold ) continue;
                //if( Electron_dxy > TransverseImpactParam_EB ) continue;
                //if( Electron_dz > LongitudinalImpactParam_EB ) continue;
            } else {
                if( Electron->fggMiniIsoSumRel() > elMiniIsoEEThreshold ) continue;
                //if( Electron_dxy > TransverseImpactParam_EE ) continue;
                //if( Electron_dz> LongitudinalImpactParam_EE ) continue;
            }

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
            
            if( Electron->standardHggIso() > IsoThreshold ) continue; 

            if( Electron->gsfTrack()->hitPattern().numberOfAllHits( reco::HitPattern::MISSING_INNER_HITS ) > NumOfMissingHitsThreshold ) { continue; }
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
                                                         double deltaRPhoElectronThreshold, double DeltaRTrkElec, double deltaMassElectronZThreshold,
                                                         double rho, bool isData){
 
        std::vector<const flashgg::Photon *> photons;        
        photons.push_back( dipho->leadingPhoton() );
        photons.push_back( dipho->subLeadingPhoton() );
        
        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;        
        std::vector<edm::Ptr<flashgg::Electron> > allGoodElectrons=selectStdAllElectrons( ElectronPointers, vertexPointers , 
                                                                                          ElectronPtThreshold, EtaCuts, useMVARecipe, useLooseID,
                                                                                          rho, isData);
        
        for( unsigned int ElectronIndex = 0; ElectronIndex < allGoodElectrons.size(); ElectronIndex++ ) {
            
            Ptr<flashgg::Electron> electron = allGoodElectrons[ElectronIndex];            
            bool photon_veto = phoVeto(electron, dipho,  deltaRPhoElectronThreshold,  DeltaRTrkElec, deltaMassElectronZThreshold);                        
            if(!photon_veto) goodElectrons.push_back( electron );
        }
        
        return goodElectrons;
    }

    std::vector<edm::Ptr<Electron> > selectElectronsSum16( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
                                                      const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold,  vector<double> EtaCuts,
                                                         double deltaRPhoElectronThreshold, double DeltaRTrkElec, double deltaMassElectronZThreshold, 
                                                           double elMiniIsoEBThreshold, double elMiniIsoEEThreshold,
                                                           double TransverseImpactParam_EB, double LongitudinalImpactParam_EB, double TransverseImpactParam_EE, double LongitudinalImpactParam_EE,
                                                           double rho, bool isData){
 
        std::vector<const flashgg::Photon *> photons;        
        photons.push_back( dipho->leadingPhoton() );
        photons.push_back( dipho->subLeadingPhoton() );
        
        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;        
        std::vector<edm::Ptr<flashgg::Electron> > allGoodElectrons=selectAllElectronsSum16( ElectronPointers, vertexPointers , 
                                                                                            ElectronPtThreshold, EtaCuts, true, true,
                                                                                            elMiniIsoEBThreshold, elMiniIsoEEThreshold,
                                                                                            TransverseImpactParam_EB, LongitudinalImpactParam_EB, TransverseImpactParam_EE, LongitudinalImpactParam_EE,
                                                                                            rho, isData);
        
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

