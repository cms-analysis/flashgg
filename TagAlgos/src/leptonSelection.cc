#include "flashgg/TagAlgos/interface/leptonSelection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h" 

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


PtrVector<pat::Electron> selectElectrons(const PtrVector<pat::Electron>& elecPointers,Ptr<flashgg::DiPhotonCandidate> dipho, double rho, double elecPtThreshold) {
  
  PtrVector<pat::Electron> goodElectrons;
  
  Ptr<reco::Vertex> vtx = dipho->getVertex();

	for(unsigned int electronIndex = 0; electronIndex < elecPointers.size(); electronIndex++ )
 		{
    			Ptr<pat::Electron> elec = elecPointers[electronIndex];

			float elecEta = elec->superCluster()->eta(); 			
    
			if(fabs(elecEta) >  2.5) continue;
			if(fabs(elecEta) > 1.442 && fabs(elecEta) < 1.566 ) continue;

			float elecPt = elec->pt();

			if(elecPt < 20) continue;

cout << "elec->superCluster()->pt() is "<< elecPt << endl;

//https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon and https://cmssdt.cern.ch/SDT/lxr/source/RecoBTag/SoftLepton/plugins/SoftPFMuonTagInfoProducer.cc#0135

    float EffectiveArea =0;
    if      (fabs(elecEta) >= 0.000 && fabs(elecEta) < 1.000 ) EffectiveArea = 0.013 + 0.122;
    else if (fabs(elecEta) >= 1.000 && fabs(elecEta) < 1.479 ) EffectiveArea = 0.021 + 0.147;
    else if (fabs(elecEta) >= 1.479 && fabs(elecEta) < 2.000 ) EffectiveArea = 0.013 + 0.055;
    else if (fabs(elecEta) >= 2.000 && fabs(elecEta) < 2.200 ) EffectiveArea = 0.010 + 0.106;
    else if (fabs(elecEta) >= 2.200 && fabs(elecEta) < 2.300 ) EffectiveArea = 0.024 + 0.138;
    else if (fabs(elecEta) >= 2.300 && fabs(elecEta) < 2.400 ) EffectiveArea = 0.020 + 0.221;
    else                                         EffectiveArea = 0.019 + 0.211;



//https://cmssdt.cern.ch/SDT/lxr/source/DataFormats/EgammaCandidates/interface/GsfElectron.h
//https://cmssdt.cern.ch/SDT/doxygen/CMSSW_4_4_2/doc/html/d3/df3/structreco_1_1GsfElectron_1_1PflowIsolationVariables.html
//I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt

//rho2012 ???
//Rho is neutral rho, defined in full tracker acceptance, with a 0.5 GeV threshold on neutrals. This can be taken, starting from 50X, from the event directly (double_kt6PFJetsCentralNeutral_rho_RECO.obj) For its exact definition see [2].
//rho=process.kt6PFJets.clone( Rho_EtaMax = cms.double(2.5), Ghost_EtaMax = cms.double(2.5) )

//https://cmssdt.cern.ch/SDT/lxr/source/DataFormats/EgammaCandidates/interface/GsfElectron.h#517
reco::GsfElectron::PflowIsolationVariables pfIso = elec->pfIsolationVariables();


float elePFIsoSumRel = (pfIso.sumChargedHadronPt + TMath::Max(float(0), pfIso.sumPhotonEt + pfIso.sumNeutralHadronEt) - EffectiveArea*rho)/(elecPt);

if( elePFIsoSumRel > 0.15 ) continue;

//https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID
double eleMissHits = elec->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
if( eleMissHits >    1 ) continue;

double eleD0Vtx = elec->gsfTrack()->dxy(vtx->position());
double eleDzVtx = elec->gsfTrack()->dz(vtx->position());

    if( fabs(eleD0Vtx) > 0.02 ) continue;
    if( fabs(eleDzVtx) > 0.20 ) continue;

//https://twiki.cern.ch/twiki/bin/view/Main/HVVElectronId2012
//https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideElectronID

//https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID => !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*

 std::cout << "Electron ID: 95relIso=" << myElec->electronID("simpleEleId95relIso")  
             << " 90relIso=" << myElec->electronID("simpleEleId90relIso") 
           << " 85relIso=" << myElec->electronID("simpleEleId85relIso") 
           << " 80relIso=" << myElec->electronID("simpleEleId80relIso") 
           << " 70relIso=" << myElec->electronID("simpleEleId70relIso") 
           << " 60relIso=" << myElec->electronID("simpleEleId60relIso")  << ...... <<
           << std::endl;
The value map returns a double with the following meaning:
0: fails
1: passes electron ID only
2: passes electron Isolation only
3: passes electron ID and Isolation only
4: passes conversion rejection
5: passes conversion rejection and ID
6: passes conversion rejection and Isolation
7: passes the whole selection

*/

int elecID = elec->electronID("simpleEleId95relIso") ;
if (elecID!=7) continue;
/*
Minimum distance between conversion tracks:
electron->convDist()
$ \Delta cot \theta $ between conversion tracks at conversion vertex:
electron->convDcot()
*/

double eleIDMVANonTrig = elec->electronID("mvaNonTrigV0");
if (eleIDMVANonTrig<0.9)   continue;

  //if( (*eleConvVtxFit)[i] == 1 ) continue;

//bool passConversionVeto = elec->sourcePtr()->get()->passConversionVeto(); 
//passConversionVeto_ = !ConversionTools::hasMatchedConversion
//check this:
bool passConversionVeto = elec->passConversionVeto(); 
if(!passConversionVeto) continue;

//https://twiki.cern.ch/twiki/bin/viewauth/CMS/ConversionBackgroundRejection ???
//passConversionVeto () const - 	vertex fit combined with missing number of hits method 
    //if( (*eleConvVtxFit)[i] == 1 ) continue; ???

/*
(*) 
bool passConversionVeto = electron.sourcePtr()->get()->passConversionVeto(); 
(**) 
bool passConversionVetoFromCMG = electron.passConversionVeto(); 

https://savannah.cern.ch/task/?40657
Hi guys, 
If you use the cmg::Electron::passConversionVeto() flag in your analysis please read. 
While synching my analysis with other groups I found there is a problem in the way we fill this flag during the cmgTuple production. Currently we do this: 
output->passConversionVeto_ = !ConversionTools::hasMatchedConversion(*input, hConversions, beamspot.position (), true, 2.0, 1e-06, 0) 
where input is the pat::Electron. This needs to be changed to this: 
output->passConversionVeto_ = !ConversionTools::hasMatchedConversion( 
dynamic_cast<reco::GsfElectron const&>(*(input->originalObjectRef())), 
hConversions, beamspot.position (), true, 2.0, 1e-06, 0) ; 
The reason is that in the first case the GsfElectron is not properly retrieved from the pat::Electron and the matching to the conversion collection doesn't work in the same way. 
I will commit this fix in the next CMGTools release. 
Jose 
*/

/*
[ikucher@lxplus0048 plugins]$ grep eleConvVtxFit 
../interface/ggNtuplizer.h:  vector<int>    eleConvVtxFit_;
../plugins/ggNtuplizer.cc:  tree_->Branch("eleConvVtxFit", &eleConvVtxFit_);
../plugins/ggNtuplizer.cc:  eleConvVtxFit_.clear();
../plugins/ggNtuplizer.cc:      eleConvVtxFit_ .push_back((int) ConversionTools::hasMatchedConversion(*recoElectron, convH_, beamSpotHandle_->position(), true, 2.0, 1e-6, 0));  

*/
		 

  		goodElectrons.push_back( elec );
 	}
  return goodElectrons;
}

PtrVector<pat::Electron> selectElectronsAwayFromTwoPhotons(const PtrVector<pat::Electron>& elecPointers,Ptr<flashgg::DiPhotonCandidate> dipho){
  
  PtrVector<pat::Electron> selectedElectrons;

  for(unsigned elecIndex=0;elecIndex<elecPointers.size();elecIndex++){
    
	Ptr<pat::Electron> electron = elecPointers[elecIndex];
    
    double dR1 = deltaR(dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi(), electron->superCluster()->eta(), electron->superCluster()->phi());
    double dR2 = deltaR(dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi(), electron->superCluster()->eta(), electron->superCluster()->phi());
    double dRmin = min(dR1,dR2);

/*
 if (ele.gsfTrack()->trackerExpectedHitsInner().numberOfHits() > maxMissingHits) continue;
    //if (ele.pt() < minPt) continue;                                                                                                                                              

    if (fabs(ele.superCluster()->eta() - pho.superCluster()->eta()) == 0.)
      if (fabs(ele.superCluster()->phi() - pho.superCluster()->phi()) == 0.) {
        dr = sqrt(pow(ele.deltaEtaSuperClusterTrackAtVtx(),2)+pow(ele.deltaPhiSuperClusterTrackAtVtx(),2));
      }
    double dRTrk1=(*phoCiCdRtoTrk)[ilead];
    double dRTrk2=(*phoCiCdRtoTrk)[itrail];
    double dRminTrk = min(dRTrk1,dRTrk2);
*/

    if( dRmin < 1.0 ) continue;

 if (electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() > 0) continue;   

double dRTrk1 = 0;    
double dRTrk2 = 0;                                                                                                                            

    if (fabs(electron->superCluster()->eta() - dipho->leadingPhoton()->superCluster()->eta()) == 0.)
      if (fabs(electron->superCluster()->phi() - dipho->leadingPhoton()->superCluster()->phi()) == 0.) {
        dRTrk1 = sqrt(pow(electron->deltaEtaSuperClusterTrackAtVtx(),2)+pow(electron->deltaPhiSuperClusterTrackAtVtx(),2));
      }

    if (fabs(electron->superCluster()->eta() - dipho->subLeadingPhoton()->superCluster()->eta()) == 0.)
      if (fabs(electron->superCluster()->phi() - dipho->subLeadingPhoton()->superCluster()->phi()) == 0.) {
        dRTrk2 = sqrt(pow(electron->deltaEtaSuperClusterTrackAtVtx(),2)+pow(electron->deltaPhiSuperClusterTrackAtVtx(),2));
      }

    double dRminTrk = min(dRTrk1,dRTrk2);
    if( dRminTrk < 1.0 ) continue;

    selectedElectrons.push_back(electron);
  }
  return selectedElectrons;  
}

/*

phoCiCdRtoTrk_.push_back(getPhotondRtoTrk(gsfElectronHandle_, *iPho, 2.5, 0));

float ggNtuplizer::getPhotondRtoTrk(edm::Handle<reco::GsfElectronCollection> gsfHandle, const pat::Photon & pho, float minPt, int maxMissingHits) {

  float dr = 99;
  for (reco::GsfElectronCollection::const_iterator igsf = gsfHandle->begin(); igsf != gsfHandle->end(); ++igsf) {

    reco::GsfElectron ele = reco::GsfElectron(*igsf);

    if (ele.gsfTrack()->trackerExpectedHitsInner().numberOfHits() > maxMissingHits) continue;
    //if (ele.pt() < minPt) continue;                                                                                                                                              

    if (fabs(ele.superCluster()->eta() - pho.superCluster()->eta()) == 0.)
      if (fabs(ele.superCluster()->phi() - pho.superCluster()->phi()) == 0.) {
        dr = sqrt(pow(ele.deltaEtaSuperClusterTrackAtVtx(),2)+pow(ele.deltaPhiSuperClusterTrackAtVtx(),2));
      }
  }

  return dr;
}


vector<int> xAna::selectElectronsHCP2012( int &vtxLep , double ptcut ) { 
  // JM default ptcut is 20, ptcut added for VH dilep
 

    /// fill vertex lepton index for each lepton
    float dzmin = 9999;
    for( int ivtx = 0 ; ivtx < nVtxBS; ivtx++ )
      if(  fabs( (*eleVtx_z)[i] - (*vtxbs_z)[ivtx] ) < dzmin ) {
	dzmin = fabs( (*eleVtx_z)[i] - (*vtxbs_z)[ivtx] );
	vtxLepIndex[i] = ivtx;
      }
    if( vtxLepIndex[i] == -1 ) vtxLepIndex[i] = 0;
	
    vtxInd = vtxLepIndex[i];
    if( elePFIsoSumRel > 0.15 ) continue;
    if( (*eleMissHits)[i] >    1 ) continue;
    if( fabs((*eleD0Vtx)[i][vtxInd] ) > 0.02 ) continue;
    if( fabs((*eleDzVtx)[i][vtxInd] ) > 0.20 ) continue;
    if( (*eleIDMVANonTrig)[i] < 0.90 ) continue;

    if( (*eleConvVtxFit)[i] == 1 ) continue;

    goodElectronIndex.insert( make_pair( (*eleIDMVANonTrig)[i], i ) );
  }
  
  return out;
}


********************************************8

vector<int> xAna::selectElectronsAwayFromTwoPhotons(int i, int j, vector<int> elecIndex ){
  
  vector<int> selElecIndex;
  int ilead = i, itrail = j;

  for(unsigned ie=0;ie<elecIndex.size();ie++){
    
    int iel=elecIndex[ie];
    
    double dR1 = deltaR((*phoEta)[ilead], (*phoPhi)[ilead], (*eleEta)[iel], (*elePhi)[iel]);
    double dR2 = deltaR((*phoEta)[itrail], (*phoPhi)[itrail], (*eleEta)[iel], (*elePhi)[iel]);
    double dRmin = min(dR1,dR2);

    double dRTrk1=(*phoCiCdRtoTrk)[ilead];
    double dRTrk2=(*phoCiCdRtoTrk)[itrail];
    double dRminTrk = min(dRTrk1,dRTrk2);

    
    //cout<<" JM DEBUG ========= dR1="<<dR1<<" dR2="<< dR2<< endl;

    if( dRmin < 1.0 ) continue;
    if( dRminTrk < 1.0 ) continue;

    selElecIndex.push_back(iel);
  }
  return selElecIndex;  
}

*/

}
