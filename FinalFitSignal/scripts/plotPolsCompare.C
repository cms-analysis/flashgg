#include <iostream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TMath.h"
#include "TPaveText.h"

using namespace std;

void plotComparePols(string proc="ggh", int cat=0, int nGaus=3, int dmOrdMax=2, int sigOrdMax=2, int fracOrdMax=2){

  gROOT->SetBatch();

  system(Form("mkdir -p plots/comparePols/%s_cat%d",proc.c_str(),cat));
  int colors[3] = {kBlue,kRed,kGreen+2};
  int styles[3] = {1,2,7};
  
  string plots[3] = {"dm","sigma","frac"};
  int ordMax[3] = {dmOrdMax,sigOrdMax,fracOrdMax};
  int ordComp[3] = {1,2,1};

  TFile *tempFile = TFile::Open(Form("extra_files/CMS-HGG_extra_%s_cat%d_121.root",proc.c_str(),cat));
  
  for (int itype=0; itype<3; itype++){
    string type=plots[itype];
    if (itype==2) nGaus--; 
    for (int g=0; g<nGaus; g++){
      TCanvas *canv = new TCanvas();
      TPaveText *pt = new TPaveText(0.73,0.82,0.89,0.89,"NDC");
      pt->SetFillStyle(0);
      pt->AddText(Form("%s g%d",plots[itype].c_str(),g));
      TLegend *leg = new TLegend(0.11,0.69,0.4,0.89);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      TH1F *hist = (TH1F*)tempFile->Get(Form("th_%s_g%d",type.c_str(),g));
      hist->Sumw2();
      hist->SetTitle("");
      hist->GetXaxis()->SetTitle("m_{H}");
      hist->SetLineColor(kBlack);
      hist->SetMarkerColor(kBlack);
      vector<TGraph*> graphs;
      leg->AddEntry(hist,"Raw fit value","LEP");
      for (int ord=0; ord<=ordMax[itype]; ord++){
        TFile *funcFile;
        if (itype==0) funcFile = TFile::Open(Form("extra_files/CMS-HGG_extra_%s_cat%d_%d%d%d.root",proc.c_str(),cat,ord,ordComp[1],ordComp[2]));
        else if (itype==1) funcFile = TFile::Open(Form("extra_files/CMS-HGG_extra_%s_cat%d_%d%d%d.root",proc.c_str(),cat,ordComp[0],ord,ordComp[2]));
        else if (itype==2) funcFile = TFile::Open(Form("extra_files/CMS-HGG_extra_%s_cat%d_%d%d%d.root",proc.c_str(),cat,ordComp[0],ordComp[1],ord));

        TGraph *graph = (TGraph*)funcFile->Get(Form("tg_%s_g%d_%s_cat%d",type.c_str(),g,proc.c_str(),cat));
        graph->SetLineColor(colors[ord]);
        graph->SetLineStyle(styles[ord]);
        graphs.push_back(graph);
        leg->AddEntry(graph,Form("Pol%d fit shape",ord),"L");
      }
      hist->SetStats(false);
      hist->GetYaxis()->SetRangeUser(-0.5,1);
      hist->Draw();
      for (unsigned int o=0; o<graphs.size(); o++) graphs[o]->Draw("CSAME");
      leg->Draw("SAME");
      pt->SetTextSize(0.05);
      pt->Draw("SAME");
      canv->Print(Form("plots/comparePols/%s_cat%d/%s_g%d.pdf",proc.c_str(),cat,type.c_str(),g));
      canv->Print(Form("plots/comparePols/%s_cat%d/%s_g%d.png",proc.c_str(),cat,type.c_str(),g));
      graphs.clear();
      delete leg;
      delete pt;
    }
  }
  delete tempFile;

}
