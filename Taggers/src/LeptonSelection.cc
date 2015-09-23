#include "flashgg/Taggers/interface/LeptonSelection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


namespace flashgg {

    std::vector<edm::Ptr<flashgg::Muon> > selectMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold )
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

            if( fabs( muon->eta() ) > muonEtaThreshold ) { continue; }
            if( muon->pt() < muonPtThreshold ) { continue; }

            int vtxInd = 0;
            double dzmin = 9999;

            for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {

                Ptr<reco::Vertex> vtx = vertexPointers[ivtx];

                if( !muon->innerTrack() ) { continue; }

                if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {

                    dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );

                    vtxInd = ivtx;
                }

            }

            Ptr<reco::Vertex> best_vtx = vertexPointers[vtxInd];

            //https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon and https://cmssdt.cern.ch/SDT/lxr/source/RecoBTag/SoftLepton/plugins/SoftPFMuonTagInfoProducer.cc#0135

            if( !muon::isTightMuon( *muon, *best_vtx ) ) { continue; }

            //I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt
            //https://cmssdt.cern.ch/SDT/doxygen/CMSSW_5_3_14/doc/html/df/d33/structreco_1_1MuonPFIsolation.html
            // deltaBeta correction

            double muPFIsoSumRel = ( muon->pfIsolationR04().sumChargedHadronPt + max( 0.,
                                     muon->pfIsolationR04().sumNeutralHadronEt + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );
            if( muPFIsoSumRel > muPFIsoSumRelThreshold ) { continue; }

            float dRPhoLeadMuon = deltaR( muon->eta(), muon->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadMuon = deltaR( muon->eta(), muon->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() );

            //https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5369
            if( dRPhoLeadMuon < dRPhoLeadMuonThreshold || dRPhoSubLeadMuon < dRPhoSubLeadMuonThreshold ) { continue; }

            goodMuons.push_back( muon );
        }
        return goodMuons;
    }

    std::vector<edm::Ptr<Electron> > selectElectrons( const std::vector<edm::Ptr<flashgg::Electron> > &ElectronPointers,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers , double ElectronPtThreshold, double DeltaRTrkElec, double TransverseImpactParam,
            double LongitudinalImapctParam, double NonTrigMVAThreshold, double IsoThreshold, double NumOfMissingHitsThreshold, vector<double> EtaCuts )
    {

        std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;

        std::cout << " LC DEBUG (LeptonSlection.cc) nElectrons " << ElectronPointers.size() << std::endl;
        for( unsigned int ElectronIndex = 0; ElectronIndex < ElectronPointers.size(); ElectronIndex++ ) {

            Ptr<flashgg::Electron> Electron = ElectronPointers[ElectronIndex];

            /*
            std::cout << " LeptonSelection Electron index " << ElectronIndex << " has pt eta weight: "
                      << Electron->pt() << " " << Electron->eta() << " "
                      << Electron->centralWeight() << std::endl;
            auto weightList = Electron->weightList();
            for( unsigned int i = 0 ; i < weightList.size() ; i++ ) {
                std::cout << "    " << weightList[i] << " " << Electron->weight( weightList[i] );
            }
            std::cout << std::endl;
            */

            float Electron_eta = fabs( Electron->superCluster()->eta() );
            Ptr<reco::Vertex> Electron_vtx = chooseElectronVertex( Electron, vertexPointers );
            float dxy = Electron->gsfTrack()->dxy( Electron_vtx->position() );
            float dz = Electron->gsfTrack()->dz( Electron_vtx->position() );
            //std::cout << "[DEBUG] LeptonSelection.cc Electron eta " << Electron_eta <<  "  -- cuts <"<< EtaCuts[0] << ", <" << EtaCuts[1] << ", >" << EtaCuts[2] << std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc Electron pt " << Electron->pt() <<", pt cut  >" << ElectronPtThreshold<<  std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc non trig MVA " << Electron->nonTrigMVA() <<", mva cut  >" << NonTrigMVAThreshold <<  std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc iso" << Electron->standardHggIso()  <<", iso cut  <" << IsoThreshold <<  std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc missing Hits" << Electron->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS )  <<", cut  <" << NumOfMissingHitsThreshold <<  std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc hasConversion " << Electron->hasMatchedConversion() << " needed 0  to pass " << std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc dxy " << dxy << " -- cut  < "<<TransverseImpactParam << std::endl;
            //std::cout << "[DEBUG] LeptonSelection.cc dz " << dz << " -- cut  < "<< LongitudinalImapctParam << std::endl;

            if( Electron_eta > EtaCuts[2] && ( Electron_eta > EtaCuts[0] && Electron_eta < EtaCuts[1] ) ) { continue; }
            //fixed the ||  to become and && in above, otherwise all electrons above EtaCuts[2] (which is ~1.44) are thrown out.
            //ie it would have ignored all electrons on the endcaps.. 
            //std::cout << "[DEBUG] passed eta cuts. " << std::endl;
            if( Electron->pt() < ElectronPtThreshold ) { continue; }
            //std::cout << "[DEBUG] passed pt cuts." << std::endl;



            if( Electron->nonTrigMVA() < NonTrigMVAThreshold ) { continue; }
            //std::cout << "[DEBUG] passed non trig mva." << std::endl;
         
            //if( Electron->standardHggIso() > IsoThreshold ) { continue; } //FIXME
            //std::cout << "[DEBUG] passed iso." << std::endl; //FIXME
            //The default standardHggIso is delivering values which are an order or magnitude larger than the cut. Maybe need to add rho correction? 
            std::cout << "[WARNING] Isolation cut for flashggElectrons temporarily disabled pending optimisation." << std::endl; //FIXME


            if( Electron->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS ) > NumOfMissingHitsThreshold ) { continue; }
            //std::cout << "[DEBUG] passed  missing hits cut." << std::endl;

            if( Electron->hasMatchedConversion() ) { continue; }
            //std::cout << "[DEBUG] passed hasConversion." << std::endl;

            if( dxy > TransverseImpactParam ) { continue; }
            //std::cout << "[DEBUG] passed dxy cut " << std::endl;
            if( dz > LongitudinalImapctParam ) { continue; }
            //std::cout << "[DEBUG] passed dz cut." << std::endl;

            //std::cout << "[DEBUG] LeptonSelection.c   ... pushing back Electron index " << ElectronIndex << std::endl;
            goodElectrons.push_back( Electron );
        }

        return goodElectrons;
    }


    Ptr<reco::Vertex>  chooseElectronVertex( Ptr<flashgg::Electron> &elec, const std::vector<edm::Ptr<reco::Vertex> > &vertices )
    {

        double vtx_dz = 1000000;
        unsigned int min_dz_vtx = -1;
        for( unsigned int vtxi = 0; vtxi < vertices.size(); vtxi++ ) {

            Ptr<reco::Vertex> vtx = vertices[vtxi];

            if( vtx_dz > elec->gsfTrack()->dz( vtx->position() ) ) {

                vtx_dz = elec->gsfTrack()->dz( vtx->position() );
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

