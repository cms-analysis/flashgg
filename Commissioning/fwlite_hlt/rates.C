{
  // Set up FW Lite for automatic loading of CMS libraries
  // and data formats.   As you may have other user-defined setup
  // in your rootlogon.C, the CMS setup is executed only if the CMS
  // environment is set up.
  //

  using namespace std;

  TString cmsswbase = getenv("CMSSW_BASE");
  if (cmsswbase.Length() > 0) {
    //
    // The CMSSW environment is defined (this is true even for FW Lite)
    // so set up the rest.
    //
    cout << "Loading FW Lite setup." << endl;
    gSystem->Load("libFWCoreFWLite.so");
    AutoLibraryLoader::enable();
    gSystem->Load("libDataFormatsFWLite.so");
    gSystem->Load("libDataFormatsPatCandidates.so");
    gSystem->Load("libflashggMicroAODFormats.so"); 
    
  }
  
  //  TFile f("/afs/cern.ch/user/c/carrillo/flashgg/CMSSW_7_0_7_patch1/src/flashgg/MicroAODProducers/test/myOutputFile.root");
  //  TFile f("/afs/cern.ch/user/c/carrillo/my_production_microAOD_hlt/output.root");
  TFile f("/afs/cern.ch/user/c/carrillo/eoscarrillo/low_mass_hlt/output125.root");
  //TFile f("/tmp/carrillo/myOutputFileBig.root");
  TTree *Events = f.Get("Events");
  //Events->Print();
  Events->SetScanField(0);
  //Events->Scan("flashggDiPhotonCandidates_flashggDiPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.zernike42:flashggPhotons_flashggPhotons__FLASHggTEST.obj.hcalTowersBehindClusters[0]");

  
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "flashgg/MicroAODFormats/interface/DiphotonCandidate.h"  
#include "flashgg/MicroAODFormats/interface/Photon.h"  
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#endif
  
  gStyle->SetOptStat(0);

  TH1F * studied_bit_histo;
  TH1F * allpaths;
  int cut_index;
  theFileOut = new TFile("rates.root", "RECREATE");
  TString name;

  const int N=10;

  studied_bit_histo = new TH1F ("studiedbit","studiedbit",5  ,-0.5,4.5);
  allpaths = new TH1F ("allpaths","allpaths",N,0,N);

  fwlite::Event ev(&f);

  int iEvent=0;

  for( ev.toBegin(); ! ev.atEnd(); ++ev) {
    iEvent++;
    cout<<iEvent<<endl;
    fwlite::Handle<edm::TriggerResults> hTriggerResults;
    hTriggerResults.getByLabel(ev,"TriggerResults","","TEST");
    edm::TriggerNames const&  triggerNames = ev.triggerNames(*hTriggerResults);
    vector<std::string> const& names = triggerNames.triggerNames();
    if(iEvent==1){
      for (unsigned index = 0; index < triggerNames.size(); ++index) {
	cout << index << " " << triggerNames.triggerName(index) << " "<< hTriggerResults->accept(index)<<endl;
	if(index==0){
	  allpaths->GetXaxis()->SetBinLabel(index+1,"all_events");
	}else{
	  allpaths->GetXaxis()->SetBinLabel(index+1,triggerNames.triggerName(index).c_str());
	}
      }
      cout << "size = " << triggerNames.size() << endl;
    }

    allpaths->Fill(0);
    for(unsigned int l=1;l<triggerNames.size();l++){
      if(hTriggerResults->accept(l)){
	allpaths->Fill(l);
      }
    }

    unsigned studied_bit=8;
    bool othersOR=false;
    int pho_bits[2]={1,3};
    
    for(l=0;l<2;l++){
      othersOR=othersOR||hTriggerResults->accept(pho_bits[l]);
    }

    studied_bit_histo->Fill(0);
    if(hTriggerResults->accept(studied_bit)){
      studied_bit_histo->Fill(1);
    }

    if(othersOR){
      studied_bit_histo->Fill(2);
    }

    if(othersOR||hTriggerResults->accept(studied_bit)){
      studied_bit_histo->Fill(3);
    }

    if(othersOR&&hTriggerResults->accept(studied_bit)){
      studied_bit_histo->Fill(4);
    }
  }
  
  //endjob
  cout<<"beginning endJob"<<endl;
  theFileOut->cd();
  
  studiedbit->GetXaxis()->SetBinLabel(1,"events");
  studiedbit->GetXaxis()->SetBinLabel(2,"studied bit");
  studiedbit->GetXaxis()->SetBinLabel(3,"othersOR");
  studiedbit->GetXaxis()->SetBinLabel(4,"othersOR || studied bit");
  studiedbit->GetXaxis()->SetBinLabel(5,"othersOR && studied bit");
  //studiedbit->Draw("");
  //studiedbit->Draw("htextsame");
  studiedbit->SetFillColor(kYellow);
  studiedbit->Write();
  
  //allpaths->Draw("");
  //allpaths->Draw("htextsame");
  allpaths->SetFillColor(kRed);
  allpaths->Write();
  
  theFileOut->Close();
  exit(0);
}


