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

void checkSystematics(string filename, int ncats=9, bool isCutBased=false, string folder="systematicPlots", bool interpolated=true){

  gROOT->SetBatch();
  gStyle->SetOptStat(0);

  TFile *tFile = TFile::Open(filename.c_str());
   
  vector<string> processes;
  processes.push_back("ggh");
  processes.push_back("vbf");
  processes.push_back("wzh");
  processes.push_back("tth");
  processes.push_back("gg_grav");
  processes.push_back("qq_grav");

  vector<string> systematics;
  systematics.push_back("E_res");
  systematics.push_back("E_scale");
  systematics.push_back("idEff");
  systematics.push_back("triggerEff");
  systematics.push_back("vtxEff");
  if (!isCutBased) {
    systematics.push_back("phoIdMva");
    systematics.push_back("regSig");
    //systematics.push_back("pdfWeight");
  } 
  else {
    systematics.push_back("r9Eff");
    systematics.push_back("ptSpin");
  }
  
  system(Form("mkdir %s",folder.c_str()));

  // can only do integer masses
  vector<int> masses;
  masses.push_back(125);
  /*
  if (interpolated){
    for (int m=110; m<=150; m++){
      masses.push_back(m);
    }
  }
  else {
    for (int m=110; m<=150; m+=5){
      masses.push_back(m);
    }
  }
  */

  for (unsigned int i=0; i<masses.size(); i++){
    int m=masses[i];
    for (vector<string>::iterator proc=processes.begin(); proc!=processes.end(); proc++) {
      for (int cat=0; cat<ncats; cat++){

        TH1F *nomSig = (TH1F*)tFile->Get(Form("th1f_sig_%s_mass_m%3d_cat%d",proc->c_str(),m,cat)); 
        TH1F *sigRV = (TH1F*)tFile->Get(Form("th1f_sig_%s_mass_m%3d_rv_cat%d",proc->c_str(),m,cat)); 
        TH1F *sigWV = (TH1F*)tFile->Get(Form("th1f_sig_%s_mass_m%3d_wv_cat%d",proc->c_str(),m,cat)); 
        nomSig->SetStats(0);
        sigRV->SetStats(0);
        sigWV->SetStats(0);
        TCanvas *canv = new TCanvas();
        TLegend *leg = new TLegend(0.7,0.7,0.89,0.89);
        nomSig->SetLineWidth(2);
        nomSig->SetLineColor(kBlack);
        sigRV->SetLineColor(kBlue);
        sigRV->SetFillColor(kBlue);
        sigRV->SetFillStyle(3003);
        sigWV->SetLineColor(kRed);
        sigWV->SetFillColor(kRed);
        sigWV->SetFillStyle(3004);
        leg->AddEntry(nomSig,"Nominal Signal","f");
        leg->AddEntry(sigRV,"Right vertex","f");
        leg->AddEntry(sigWV,"Wrong vertex","f");
        nomSig->GetYaxis()->SetRangeUser(0.,1.3*TMath::Max(nomSig->GetMaximum(),TMath::Max(sigRV->GetMaximum(),sigWV->GetMaximum())));
        nomSig->SetTitle("");
        nomSig->GetXaxis()->SetTitle("m_{#gamma#gamma}");
        nomSig->Draw("hist");
        sigRV->Draw("histsame");
        sigWV->Draw("histsame");
        leg->Draw("same");
        canv->Print(Form("%s/rvwv_%s_m%d_cat%d.pdf",folder.c_str(),proc->c_str(),m,cat));
        canv->Print(Form("%s/rvwv_%s_m%d_cat%d.png",folder.c_str(),proc->c_str(),m,cat));
        delete leg;

        for (vector<string>::iterator syst=systematics.begin(); syst!=systematics.end(); syst++){
          
          TLegend *systLeg = new TLegend(0.7,0.7,0.89,0.89);
          TH1F *up = (TH1F*)tFile->Get(Form("th1f_sig_%s_mass_m%3d_cat%d_%sUp01_sigma",proc->c_str(),m,cat,syst->c_str()));
          TH1F *down = (TH1F*)tFile->Get(Form("th1f_sig_%s_mass_m%3d_cat%d_%sDown01_sigma",proc->c_str(),m,cat,syst->c_str()));
          up->SetStats(0);
          down->SetStats(0);
          up->SetLineColor(kRed);
          down->SetLineColor(kBlue);
          systLeg->AddEntry(nomSig,"Nominal Signal","f");
          systLeg->AddEntry(up,Form("%s Up",syst->c_str()),"f");
          systLeg->AddEntry(down,Form("%s Down",syst->c_str()),"f");
          nomSig->GetYaxis()->SetRangeUser(0.,1.3*TMath::Max(nomSig->GetMaximum(),TMath::Max(up->GetMaximum(),down->GetMaximum())));
          nomSig->Draw("hist");
          up->Draw("histsame");
          down->Draw("histsame");
          systLeg->Draw("same");
          canv->Print(Form("%s/%s_%s_m%d_cat%d.pdf",folder.c_str(),syst->c_str(),proc->c_str(),m,cat));
          canv->Print(Form("%s/%s_%s_m%d_cat%d.png",folder.c_str(),syst->c_str(),proc->c_str(),m,cat));
          delete systLeg;
        
        }
        delete canv;
      }
    }
  }

  system(Form("python make_syst_html.py %s/",folder.c_str()));

  cout << Form("Done. Please cp -r %s to some public web space to view files",folder.c_str()) << endl;

}
