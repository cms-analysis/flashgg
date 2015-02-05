#include <iostream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"

using namespace std;

void plotSpinSignal(string filename, int ncats=20, string folder="spinSignalPlots", float mass=125, bool norm=false){

  gROOT->SetBatch();
  gStyle->SetOptStat(0);

  system(Form("mkdir -p %s",folder.c_str()));

  TFile *tFile = TFile::Open(filename.c_str());

  for (int cat=0; cat<ncats; cat++){
    TH1F *gghSig = (TH1F*)tFile->Get(Form("th1f_sig_ggh_mass_m%3.1f_cat%d",mass,cat));
    TH1F *vbfSig = (TH1F*)tFile->Get(Form("th1f_sig_vbf_mass_m%3.1f_cat%d",mass,cat));
    TH1F *wzhSig = (TH1F*)tFile->Get(Form("th1f_sig_wzh_mass_m%3.1f_cat%d",mass,cat));
    TH1F *tthSig = (TH1F*)tFile->Get(Form("th1f_sig_tth_mass_m%3.1f_cat%d",mass,cat));
    TH1F *ggh_gravSig = (TH1F*)tFile->Get(Form("th1f_sig_gg_grav_mass_m%3.1f_cat%d",mass,cat));
    TH1F *qqh_gravSig = (TH1F*)tFile->Get(Form("th1f_sig_qq_grav_mass_m%3.1f_cat%d",mass,cat));
    
    TH1F *smSig = (TH1F*)gghSig->Clone(Form("th1f_sig_sm_mass_m%3.1f_cat%d",mass,cat));
    smSig->Add(vbfSig);
    smSig->Add(wzhSig);
    smSig->Add(tthSig);

    if (norm){
      smSig->Scale(1./smSig->Integral());
      ggh_gravSig->Scale(1./ggh_gravSig->Integral());
      qqh_gravSig->Scale(1./qqh_gravSig->Integral());
    }

    smSig->SetLineColor(4);
    smSig->SetFillColor(38);
    smSig->SetFillStyle(3001);
    smSig->SetLineWidth(2);
    ggh_gravSig->SetLineColor(kRed);
    ggh_gravSig->SetFillColor(kRed);
    ggh_gravSig->SetFillStyle(3004);
    ggh_gravSig->SetLineWidth(2);
    qqh_gravSig->SetLineColor(kGreen+1);
    qqh_gravSig->SetLineWidth(2);
    
    TCanvas *canv = new TCanvas();
    smSig->GetYaxis()->SetRangeUser(0.,1.1*TMath::Max(smSig->GetMaximum(),ggh_gravSig->GetMaximum()));
    smSig->SetTitle("");
    TLegend *leg = new TLegend(0.6,0.7,0.89,0.89);
    leg->SetFillColor(0);
    leg->AddEntry(smSig,"X#rightarrow#gamma#gamma 0^{+}","LF");
    leg->AddEntry(ggh_gravSig,"X#rightarrow#gamma#gamma 2^{+}_{m} (gg)","LF");
    leg->AddEntry(qqh_gravSig,"X#rightarrow#gamma#gamma 2^{+}_{m} (qq)","LF");
    smSig->Draw("HISTF");
    ggh_gravSig->Draw("HISTFsame");
    qqh_gravSig->Draw("HISTsame");
    leg->Draw("same");
    canv->Print(Form("%s/m%3.1f_cat%d.pdf",folder.c_str(),mass,cat));
    canv->Print(Form("%s/m%3.1f_cat%d.png",folder.c_str(),mass,cat));
  
  }
   
}


