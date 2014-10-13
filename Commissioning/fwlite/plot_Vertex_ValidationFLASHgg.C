{
  // Set up FW Lite for automatic loading of CMS libraries
  // and data formats.   As you may have other user-defined setup
  // in your rootlogon.C, the CMS setup is executed only if the CMS
  // environment is set up.
  //
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
  
  TFile f("../../MicroAODProducers/test/myOutputFile.root");
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
#endif
  
  gStyle->SetOptStat(0);
  
  TH1F * histo_sumpt2 = new TH1F("sumpt2 in","sumpt2",100,0.,15.);
  TH1F * histo_ptbal = new TH1F("ptbal normalized","ptbal normalized",100,-200.,350.);
  TH1F * histo_ptasym = new TH1F("ptasym normalized","ptasym normalized",100,-1,1);
  TH1F * histo_pull_conv = new TH1F("pull conv","pull conv",100,0.,11.);
  TH1F * histo_vtxprobmva = new TH1F("vtxprobmva","vtxprobmva",20,-1.,1.);
  
  TH1F * histo_gen_matched_sumpt2 = new TH1F("sumpt2","sumpt2",100,0.,15.);
  TH1F * histo_gen_matched_ptbal = new TH1F("ptbal normalized","ptbal  normalized",100,-200.,350.);
  TH1F * histo_gen_matched_ptasym = new TH1F("ptasym normalized","ptasym normalized",100,-1.,1.);
  TH1F * histo_gen_matched_pull_conv = new TH1F("pull conv","pull conv",100,0.,11.);
  TH1F * histo_gen_matched_vtxprobmva = new TH1F("vtxprobmva","vtxprobmva",20,-1.,1.);

  fwlite::Event ev(&f);

  int count=0;

  for( ev.toBegin(); ! ev.atEnd(); ++ev) {
    if(count==1000) break;
    count++;
    fwlite::Handle<std::vector<flashgg::Photon> > objs_pho;
    fwlite::Handle<std::vector<flashgg::DiPhotonCandidate> > objs_dipho;
    fwlite::Handle<std::vector<reco::Vertex> > objs_vertex;
    fwlite::Handle<std::vector<reco::GenParticle> > objs_genpart;
    
    objs_pho.getByLabel(ev,"flashggPhotons");
    objs_dipho.getByLabel(ev,"flashggDiPhotons");
    objs_vertex.getByLabel(ev,"offlineSlimmedPrimaryVertices"); 
    objs_genpart.getByLabel(ev,"prunedGenParticles"); 
    
    std::vector<reco::GenParticle> const & genpart = *objs_genpart;
    
    float z_higgs=0;
    for (int i=0; i < objs_genpart.ptr()->size();i++ ){
      if(genpart[i].pdgId() == 25) z_higgs = genpart[i].vz();
    }
    
    std::vector<flashgg::DiPhotonCandidate> const & dipho = *objs_dipho;
    for (int i=0; i < objs_dipho.ptr()->size();i++ ){
      bool gen_matched = (fabs(dipho[i].getVertex()->position().z()-z_higgs)<1.);//we consider gen-matched when less than 1cm
    
      histo_vtxprobmva->Fill(dipho[i].getVtxProbMVA());
      if(gen_matched){
	histo_gen_matched_sumpt2->Fill(dipho[i].getLogSumPt2());
	histo_gen_matched_ptbal->Fill(dipho[i].getPtBal());
	histo_gen_matched_ptasym->Fill(dipho[i].getPtAsym());
	if(dipho[i].getNConv()!=0){
	  histo_gen_matched_pull_conv->Fill(dipho[i].getPullConv());
	}
	histo_gen_matched_vtxprobmva->Fill(dipho[i].getVtxProbMVA());
      }else{
	histo_sumpt2->Fill(dipho[i].getLogSumPt2());
	histo_ptbal->Fill(dipho[i].getPtBal());
	histo_ptasym->Fill(dipho[i].getPtAsym());
	if(dipho[i].getNConv()!=0){
	  histo_pull_conv->Fill(dipho[i].getPullConv());
	}
      }
    }
  }

  histo_gen_matched_sumpt2->SetLineColor(kBlue);
  histo_gen_matched_ptbal->SetLineColor(kBlue);
  histo_gen_matched_ptasym->SetLineColor(kBlue);
  histo_gen_matched_pull_conv->SetLineColor(kBlue);
  
  histo_sumpt2->SetLineColor(kBlack);
  histo_ptbal->SetLineColor(kBlack);
  histo_ptasym->SetLineColor(kBlack);
  histo_pull_conv->SetLineColor(kBlack);

  histo_gen_matched_sumpt2->SetLineWidth(3);
  histo_gen_matched_ptbal->SetLineWidth(3);
  histo_gen_matched_ptasym->SetLineWidth(3);
  histo_gen_matched_pull_conv->SetLineWidth(3);
  
  histo_sumpt2->SetLineWidth(3);
  histo_ptbal->SetLineWidth(3);
  histo_ptasym->SetLineWidth(3);
  histo_pull_conv->SetLineWidth(3);
  
  TLegend *leg = new TLegend(0.55,0.88,0.98,0.70);
  leg->AddEntry(histo_gen_matched_sumpt2,"higgs vertex match (dz<1.cm)","lp");
  leg->AddEntry(histo_sumpt2,"the rest of vertices","lp");
  leg->SetFillColor(0);
  
  TCanvas * Ca0 = new TCanvas("Ca0","Canvas",1200,800);  Ca0->Divide(2,2); 
  
  Ca0_1->cd();  
  histo_sumpt2->GetXaxis()->SetTitle("log sumpt2"); 
  histo_gen_matched_sumpt2->GetXaxis()->SetTitle("log sumpt2"); 
  histo_sumpt2->Draw(); 
  histo_gen_matched_sumpt2->Draw("same"); 
  leg->Draw("same");
  Ca0_1->SetLogy();
  
  Ca0_2->cd(); 
  histo_ptbal->GetXaxis()->SetTitle("GeV"); 
  histo_gen_matched_ptbal->GetXaxis()->SetTitle("GeV");
  histo_ptbal->DrawNormalized();  
  histo_gen_matched_ptbal->DrawNormalized("same"); 
  leg->Draw("same");
  Ca0_2->SetLogy();
  
  Ca0_3->cd(); 
  histo_ptasym->GetXaxis()->SetTitle("ptasym");
  histo_gen_matched_ptasym->GetXaxis()->SetTitle("ptasym");
  histo_gen_matched_ptasym->DrawNormalized("");
  histo_ptasym->DrawNormalized("same"); 
  leg->Draw("same");
  //Ca0_3->SetLogy();
  
  Ca0_4->cd(); 
  histo_gen_matched_pull_conv->Draw();
  histo_pull_conv->Draw("same"); 
  leg->Draw("same");
  histo_pull_conv->GetXaxis()->SetTitle("pull_conv");
  histo_gen_matched_pull_conv->GetXaxis()->SetTitle("pull_conv");  
  Ca0_4->SetLogy();

  Ca0->SaveAs("mva_plots.png");
  
  histo_gen_matched_sumpt2->Delete();
  histo_gen_matched_ptbal->Delete();
  histo_gen_matched_ptasym->Delete();
  histo_gen_matched_pull_conv->Delete();
  histo_sumpt2->Delete();
  histo_ptbal->Delete();
  histo_ptasym->Delete();
  histo_pull_conv->Delete();
  
  TCanvas * Ca1 = new TCanvas("Ca1","Canvas",1200,800); 
  TGraphAsymmErrors * efficiency =new TGraphAsymmErrors(histo_gen_matched_vtxprobmva,histo_vtxprobmva);
  efficiency->SetTitle("Vertex Assignment probability < 1.cm w.r.t GEN higgs vs vertex probability MVA");
  efficiency->GetXaxis()->SetTitle("MVA_{prob}");
  efficiency->GetYaxis()->SetTitle("#epsilon");
  efficiency->SetMarkerColor(kRed);
  efficiency->SetMarkerStyle(23);
  efficiency->SetMarkerSize(2);
  efficiency->GetXaxis()->SetRangeUser(-1.,1.);
  efficiency->GetYaxis()->SetRangeUser(0.,1.);
  TF1 * line=new TF1("line","[1]*x+[0]",-1,1);
  efficiency->Fit("line");
  efficiency->Draw("AP");
  line->Draw("same");
  Ca1->SaveAs("vtxprobmva.png");
  
  exit(0);

}


