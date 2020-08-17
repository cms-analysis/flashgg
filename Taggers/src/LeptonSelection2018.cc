#include "flashgg/Taggers/interface/LeptonSelection2018.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


namespace flashgg {



    std::vector<edm::Ptr<flashgg::Electron> > selectElectrons( const std::vector<edm::Ptr<flashgg::Electron> > Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, std::vector<double> EleEtaCuts, double ElePhotonDrCut, double ElePhotonZMassCut, double DeltaRTrkEle, bool debug_, int id)
{
    assert(EleEtaCuts.size()==3);
    std::vector<edm::Ptr<flashgg::Electron> > output;

    for(unsigned int l1=0; l1<Ele.size(); ++l1)
    {
        if (id == 1) {
            if(!Ele[l1]->passMVALooseId()) continue;
        }
        else if (id == 2) {
          if(!Ele[l1]->passMVAMediumId()) continue;
        }
        else if (id == 3) {
          if (!Ele[l1]->passMVATightId()) continue;
        }

        if( Ele[l1]->pt()<ElePtCut) continue;
        if( fabs(Ele[l1]->eta()) > EleEtaCuts[2] || ( fabs(Ele[l1]->eta()) > EleEtaCuts[0] && fabs(Ele[l1]->eta()) < EleEtaCuts[1] ) ) continue; 
        if( Ele[l1]->hasMatchedConversion() ) continue; 

        float dRPhoLeadEle = deltaR( Ele[l1]->eta(), Ele[l1]->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
        float dRPhoSubLeadEle = deltaR( Ele[l1]->eta(), Ele[l1]->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ); 
        if( dRPhoLeadEle < ElePhotonDrCut || dRPhoSubLeadEle < ElePhotonDrCut) continue;
        //float dRPhoLeadEleSC = deltaR( Ele[l1]->superCluster()->eta(), Ele[l1]->superCluster()->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
        //float dRPhoSubLeadEleSC = deltaR( Ele[l1]->superCluster()->eta(), Ele[l1]->superCluster()->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ); 
        //if( dRPhoLeadEleSC < ElePhotonDrCut || dRPhoSubLeadEleSC < ElePhotonDrCut) continue;

        TLorentzVector Electron;
        Electron.SetPtEtaPhiE(Ele[l1]->pt(), Ele[l1]->eta(), Ele[l1]->phi(), Ele[l1]->energy());
        //ElectronSC.SetXYZT(Ele[l1]->superCluster()->position().x(), Ele[l1]->superCluster()->position().y(), Ele[l1]->superCluster()->position().z(), Ele[l1]->ecalEnergy()); 
        TLorentzVector Ph1, Ph2;
        Ph1.SetPtEtaPhiE(dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), dipho->leadingPhoton()->energy());
        Ph2.SetPtEtaPhiE(dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), dipho->subLeadingPhoton()->energy());         

        if( fabs((Electron+Ph1).M() - 91.187) < ElePhotonZMassCut) continue;
        if( fabs((Electron+Ph2).M() - 91.187) < ElePhotonZMassCut) continue;
        //if( fabs((ElectronSC+Ph1).M() - 91.187) < ElePhotonZMassCut) continue;
        //if( fabs((ElectronSC+Ph2).M() - 91.187) < ElePhotonZMassCut) continue;
 
        //float TrkElecSCDeltaR = sqrt( Ele[l1]->deltaEtaSuperClusterTrackAtVtx() * Ele[l1]->deltaEtaSuperClusterTrackAtVtx() + Ele[l1]->deltaPhiSuperClusterTrackAtVtx() * Ele[l1]->deltaPhiSuperClusterTrackAtVtx() );
        //if( TrkElecSCDeltaR > DeltaRTrkEle ) continue;
        
        output.push_back(Ele[l1]);

        if(debug_)
            cout << "Selected electron with pT = " << Ele[l1]->pt() << ", eta = " << Ele[l1]->eta() << ", DR with photons = " <<  dRPhoLeadEle << " and " << dRPhoSubLeadEle << ", DMass with Z = " << fabs((Electron+Ph1).M() - 91.187) << " and " <<  fabs((Electron+Ph2).M() - 91.187) << " , Ele pass MVAMediumId " << Ele[l1]->passMVAMediumId() <<  endl;
    }

     return output;
}
    
    std::vector<edm::Ptr<flashgg::Muon    > > selectMuons( const std::vector<edm::Ptr<flashgg::Muon> > Muons, Ptr<flashgg::DiPhotonCandidate> dipho, const std::vector<edm::Ptr<reco::Vertex>> vtx, double MuonPtCut, double MuonEtaCut, double MuonMiniIsoCut, double MuonPhotonDrCut, bool debug_, int id)
{
     std::vector<edm::Ptr<flashgg::Muon> > output;

    for(unsigned int l1=0; l1<Muons.size(); ++l1)
    {
        if(Muons[l1]->pt()<MuonPtCut) continue;
        if(fabs(Muons[l1]->eta()) > MuonEtaCut) continue;
        if( !Muons[l1]->innerTrack() ) continue; 

        if (id == 1) {
            if( !Muons[l1]->isLooseMuon() ) continue;
        }
        else if (id == 2) {
            if( !Muons[l1]->isMediumMuon() ) continue; 
        }
        else if (id == 3) {
            if( !Muons[l1]->isTightMuon(*vtx[0]) ) continue;
        }

        float Iso = Muons[l1]->pfIsolationR04().sumChargedHadronPt + max(0., Muons[l1]->pfIsolationR04().sumNeutralHadronEt + Muons[l1]->pfIsolationR04().sumPhotonEt - 0.5*Muons[l1]->pfIsolationR04().sumPUPt);

        if(Iso/Muons[l1]->pt() > MuonMiniIsoCut) continue;

        float dRPhoLeadMuon = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
        float dRPhoSubLeadMuon = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
        if( dRPhoLeadMuon < MuonPhotonDrCut || dRPhoSubLeadMuon < MuonPhotonDrCut ) continue; 

        output.push_back(Muons[l1]);

        if(debug_)
            cout << "Selected muon with pT = " << Muons[l1]->pt() << ", eta = " << Muons[l1]->eta() << ", DR with photons = " <<  dRPhoLeadMuon << " and " << dRPhoSubLeadMuon << ", Iso= " << Iso/Muons[l1]->pt() << " , Muon pass MediumId " << Muons[l1]->isMediumMuon() << endl;

    }

     return output;
}





    std::vector<pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Muon>>> selectDiMuons(const std::vector<edm::Ptr<flashgg::Muon>> Muons, Ptr<flashgg::DiPhotonCandidate> dipho, double MuonEtaCut, double MuonPtCut, double MuonPhotonDrCut, double MuonsDrCut, double MuonsZMassCut, double LeadingLeptonPtCut)
{

    std::vector<pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Muon>>> output;
    for(unsigned int l1=0; l1<Muons.size(); ++l1)
    {
        for(unsigned int l2=l1+1; l2<Muons.size(); ++l2)
        {
            //if(Muons[l1]->charge() == Muons[l2]->charge()) continue;
            if(!Muons[l1]->isMediumMuon()) continue;
            if(!Muons[l2]->isMediumMuon()) continue;
            if(Muons[l1]->pt()<MuonPtCut) continue;
            if(Muons[l2]->pt()<MuonPtCut) continue;
            if(max(Muons[l1]->pt(), Muons[l2]->pt()) < LeadingLeptonPtCut) continue;
            if(fabs(Muons[l1]->eta()) > MuonEtaCut) continue;
            if(fabs(Muons[l2]->eta()) > MuonEtaCut) continue;

            float dRPhoLeadMuon1 = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadMuon1 = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ) ;
            float dRPhoLeadMuon2 = deltaR( Muons[l2]->eta(), Muons[l2]->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ); 
            float dRPhoSubLeadMuon2 = deltaR( Muons[l2]->eta(), Muons[l2]->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() );
            if( dRPhoLeadMuon1 < MuonPhotonDrCut || dRPhoSubLeadMuon1 < MuonPhotonDrCut ||  dRPhoLeadMuon2 < MuonPhotonDrCut || dRPhoSubLeadMuon2 < MuonPhotonDrCut ) continue; 

            float dRMuons = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), Muons[l2]->eta(), Muons[l2]->phi() ) ;
            if( dRMuons < MuonsDrCut) continue;

            TLorentzVector Mu1, Mu2;
            Mu1.SetPtEtaPhiE(Muons[l1]->pt(), Muons[l1]->eta(), Muons[l1]->phi(), Muons[l1]->energy());
            Mu2.SetPtEtaPhiE(Muons[l2]->pt(), Muons[l2]->eta(), Muons[l2]->phi(), Muons[l2]->energy());
            if( fabs((Mu1+Mu2).M() - 91.187) < MuonsZMassCut) continue;

            pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Muon>> thePair(Muons[l1], Muons[l2]);
            output.push_back(thePair);
        }
    }
//cout << "Is DiMuon " << endl;
    return output;
}

    std::vector<pair<edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Electron>>> selectDiElectrons(const std::vector<edm::Ptr<flashgg::Electron>> Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, std::vector<double> EleEtaCuts, double ElePhotonDrCut, double ElesDrCut, double ElePhotonZMassCut, double ElesZMassCut, double LeadingLeptonPtCut)
{

    assert(EleEtaCuts.size()==3);
    std::vector<pair<edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Electron>>> output;
    for(unsigned int l1=0; l1<Ele.size(); ++l1)
    {
        for(unsigned int l2=l1+1; l2<Ele.size(); ++l2)
        {
            //if(Ele[l1]->charge() == Ele[l2]->charge()) continue;
            if(!Ele[l1]->passMVALooseId()) continue;
            if(!Ele[l2]->passMVALooseId()) continue;
            if(Ele[l1]->pt()<ElePtCut) continue;
            if(Ele[l2]->pt()<ElePtCut) continue;
            if(max(Ele[l1]->pt(), Ele[l2]->pt()) < LeadingLeptonPtCut) continue;
            if( fabs(Ele[l1]->eta()) > EleEtaCuts[2] || ( fabs(Ele[l1]->eta()) > EleEtaCuts[0] && fabs(Ele[l1]->eta()) < EleEtaCuts[1] ) ) continue; 
            if( fabs(Ele[l2]->eta()) > EleEtaCuts[2] || ( fabs(Ele[l2]->eta()) > EleEtaCuts[0] && fabs(Ele[l2]->eta()) < EleEtaCuts[1] ) ) continue; 

            float dRPhoLeadEle1 = deltaR( Ele[l1]->eta(), Ele[l1]->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadEle1 = deltaR( Ele[l1]->eta(), Ele[l1]->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ); 
            float dRPhoLeadEle2 = deltaR( Ele[l2]->eta(), Ele[l2]->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadEle2 = deltaR( Ele[l2]->eta(), Ele[l2]->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ); 
            if( dRPhoLeadEle1 < ElePhotonDrCut || dRPhoSubLeadEle1 < ElePhotonDrCut || dRPhoLeadEle2 < ElePhotonDrCut || dRPhoSubLeadEle2 < ElePhotonDrCut) continue;

            float dREle = deltaR( Ele[l1]->eta(), Ele[l1]->phi(), Ele[l2]->eta(), Ele[l2]->phi() ) ;
            if( dREle < ElesDrCut) continue;

            TLorentzVector Ele1, Ele2;
            Ele1.SetPtEtaPhiE(Ele[l1]->pt(), Ele[l1]->eta(), Ele[l1]->phi(), Ele[l1]->energy());
            Ele2.SetPtEtaPhiE(Ele[l2]->pt(), Ele[l2]->eta(), Ele[l2]->phi(), Ele[l2]->energy());
            if( fabs((Ele1+Ele2).M() - 91.187) < ElesZMassCut) continue;

            TLorentzVector Ph1, Ph2;
            Ph1.SetPtEtaPhiE(dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi(), dipho->leadingPhoton()->energy());
            Ph2.SetPtEtaPhiE(dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi(), dipho->subLeadingPhoton()->energy());         

            if( fabs((Ele1+Ph1).M() - 91.187) < ElePhotonZMassCut) continue;
            if( fabs((Ele2+Ph1).M() - 91.187) < ElePhotonZMassCut) continue;
            if( fabs((Ele1+Ph2).M() - 91.187) < ElePhotonZMassCut) continue;
            if( fabs((Ele2+Ph2).M() - 91.187) < ElePhotonZMassCut) continue;

            pair<edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Electron>> thePair(Ele[l1], Ele[l2]);
            output.push_back(thePair);
        }
    }
//cout << "Is DiElectron " << endl;
    return output;
}


    std::vector<pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Electron>>> selectEleMuon(const std::vector<edm::Ptr<flashgg::Muon>> Muons, const std::vector<edm::Ptr<flashgg::Electron>> Ele, Ptr<flashgg::DiPhotonCandidate> dipho, double ElePtCut, double MuonPtCut, std::vector<double> EleEtaCuts, double MuonEtaCut, double MuonPhotonDrCut, double ElePhotonDrCut, double EleMuDrCut, double LeadingPtLeptonCut)
{
    assert(EleEtaCuts.size()==3);
    std::vector<pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Electron>>> output;
    for(unsigned int l1=0; l1<Muons.size(); ++l1)
    {
        for(unsigned int l2=0; l2<Ele.size(); ++l2)
        {
            if(Muons[l1]->charge() == Ele[l2]->charge()) continue;
            if(!Muons[l1]->isMediumMuon()) continue;
            if(!Ele[l2]->passMVALooseId()) continue;
            if(Muons[l1]->pt()<MuonPtCut) continue;
            if(Ele[l2]->pt()<ElePtCut) continue;
            if(max(Muons[l1]->pt(), Ele[l2]->pt()) < LeadingPtLeptonCut) continue;
            if(fabs(Muons[l1]->eta()) > MuonEtaCut) continue;
            if( fabs(Ele[l2]->eta()) > EleEtaCuts[2] || ( fabs(Ele[l2]->eta()) > EleEtaCuts[0] && fabs(Ele[l2]->eta()) < EleEtaCuts[1] ) ) continue; 

            float dRPhoLeadMuon = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadMuon = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ) ;
            float dRPhoLeadEle = deltaR( Ele[l2]->eta(), Ele[l2]->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadEle = deltaR( Ele[l2]->eta(), Ele[l2]->phi(), dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi() ); 
            if( dRPhoLeadMuon < MuonPhotonDrCut || dRPhoSubLeadMuon < MuonPhotonDrCut ||  dRPhoLeadEle < ElePhotonDrCut || dRPhoSubLeadEle < ElePhotonDrCut ) continue; 

            float dREleMu = deltaR( Muons[l1]->eta(), Muons[l1]->phi(), Ele[l2]->eta(), Ele[l2]->phi() ) ;
            if( dREleMu < EleMuDrCut) continue;

            pair<edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Electron>> thePair(Muons[l1], Ele[l2]);
            output.push_back(thePair);
        }
    }
//cout << "Is Mixed " << endl;
    return output;
}



}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

