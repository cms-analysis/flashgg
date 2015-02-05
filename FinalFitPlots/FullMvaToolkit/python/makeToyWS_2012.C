#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooKeysPdf.h"
#include "RooGenericPdf.h"

using namespace std;
using namespace RooFit;

void printTree(TTree *tree){
  
  float bdtoutput;
  float mgg;
  float weight;
	int category;

  tree->SetBranchAddress("bdtoutput",&bdtoutput);
  tree->SetBranchAddress("CMS_hgg_mass",&mgg);
  tree->SetBranchAddress("weight",&weight);
  tree->SetBranchAddress("category",&category);

  for (int e=0; e<tree->GetEntries(); e++){
    tree->GetEntry(e);
    cout << Form("M: %3.1f -- B: %1.3f -- W: %1.3f -- V: %d",mgg,bdtoutput,weight,category) << endl;
  }

}

void makeToyWS(string inFileName, string outFileName){

  gROOT->SetBatch();

  const bool doBkg=false;

  TFile *outFile = new TFile(outFileName.c_str(),"RECREATE");
  RooWorkspace *outWS = new RooWorkspace("fits_workspace");

  system("mkdir PdfPlots");

  TFile *treeFile = TFile::Open(inFileName.c_str());
  TTree *dataTree = (TTree*)treeFile->Get("Data");
  TTree *gghTree = (TTree*)treeFile->Get("ggh_m125_8TeV");
  TTree *vbfTree = (TTree*)treeFile->Get("vbf_m125_8TeV");
  TTree *wzhTree = (TTree*)treeFile->Get("wzh_m125_8TeV");
  TTree *tthTree = (TTree*)treeFile->Get("tth_m125_8TeV");

	TList *treeList = new TList();
	treeList->Add(gghTree);
	treeList->Add(vbfTree);
	treeList->Add(wzhTree);
	treeList->Add(tthTree);
	dataTree->SetName("dataTree");
  TTree *bkgTree;
  if (doBkg) bkgTree = (TTree*)treeFile->Get("bkgTree");

  // real vars
  RooRealVar *mass = new RooRealVar("CMS_hgg_mass","CMS_hgg_mass",100,180);
  RooRealVar *bdtoutput = new RooRealVar("bdtoutput","bdtoutput",-1,1);
  RooRealVar *category = new RooRealVar("category","category",0,5);
  RooRealVar *weight = new RooRealVar("weight","weight",0,20);

  RooDataSet *data_forkeyspdf = new RooDataSet("data_forkeyspdf","data_bdt",RooArgSet(*bdtoutput,*mass,*category),Import(*dataTree),Cut("category<4"));
  RooKeysPdf *data_pdf = new RooKeysPdf("data_pdf","data_pdf",*bdtoutput,*data_forkeyspdf);

  RooDataSet *bkg_forkeyspdf;
  RooKeysPdf *bkg_pdf;
  if (doBkg){
    bkg_forkeyspdf = new RooDataSet("bkg_forkeyspdf","bkg_bdt",RooArgSet(*bdtoutput,*mass,*category),Import(*bkgTree),Cut("category<4"));
    bkg_pdf = new RooKeysPdf("bkg_pdf","bkg_pdf",*bdtoutput,*bkg_forkeyspdf);
    outWS->import(*bkg_forkeyspdf);
    outWS->import(*bkg_pdf);
  }

  outWS->import(*data_forkeyspdf);
  outWS->import(*data_pdf);

  const int massBins=80;
  const int bdtBins=38;
  const double bLow=0.05;
  mass->setBins(massBins);
  bdtoutput->setBins(bdtBins);
  bdtoutput->setRange(bLow,1.);
  
  // data datasets
  RooDataSet *data_novbf = new RooDataSet("data_bdt_novbf","data_bdt",RooArgSet(*bdtoutput,*mass,*category),Import(*dataTree),Cut("category<4 && bdtoutput>=-0.05"));
  RooDataSet *data_vbf = new RooDataSet("data_bdt_vbf","data_bdt",RooArgSet(*bdtoutput,*mass,*category),Import(*dataTree),Cut("category==5 && bdtoutput>=-0.05"));
  RooDataSet *data_vbf_tight = new RooDataSet("data_bdt_vbf_tight","data_bdt",RooArgSet(*bdtoutput,*mass,*category),Import(*dataTree),Cut("category==4 && bdtoutput>=-0.05"));

  // data pdfs
  RooDataHist *data_hist = new RooDataHist("data_bdt_hist","data_bdt",RooArgSet(*bdtoutput,*mass),*data_novbf);
  RooHistPdf *data_hist_pdf = new RooHistPdf("data_hist_pdf","data_hist_pdf",RooArgSet(*bdtoutput,*mass),*data_hist);

  // ic data pdf
  RooDataSet *data_formass_all = new RooDataSet("data_bdt_cut_all","data_bdt",RooArgSet(*bdtoutput,*mass,*category),Import(*dataTree),Cut("bdtoutput>=-0.05"));
  RooRealVar *r1 = new RooRealVar("r1","r1",-8.,-50.,0.); 
  RooRealVar *r2 = new RooRealVar("r2","r2",-1.,-50.,0.); 
  RooRealVar *f1 = new RooRealVar("f1","f1",0.5,0.,1.); 
  RooGenericPdf *fit  = new RooGenericPdf("data_pow_model","data_pow_model","(1-@3)*TMath::Power(@0,@1) + @3*TMath::Power(@0,@2)",RooArgList(*mass,*r1,*r2,*f1));
  fit->fitTo(*data_formass_all);

  TCanvas *canv = new TCanvas();
  // data plots
  RooPlot *datB = bdtoutput->frame();
  data_novbf->plotOn(datB,Binning(bdtBins));
  data_pdf->plotOn(datB);
  datB->Draw();
  canv->SaveAs("PdfPlots/datBkeys.pdf");

  RooPlot *datBh = bdtoutput->frame();
  data_novbf->plotOn(datBh,Binning(bdtBins));
  data_hist_pdf->plotOn(datBh);
  datBh->Draw();
  canv->SaveAs("PdfPlots/datB.pdf");

  RooPlot *datM = mass->frame();
  data_novbf->plotOn(datM,Binning(massBins));
  data_hist_pdf->plotOn(datM);
  datM->Draw();
  canv->SaveAs("PdfPlots/datM.pdf");

  RooPlot *datMc = mass->frame();
  data_formass_all->plotOn(datMc,Binning(massBins));
  fit->plotOn(datMc,LineColor(kMagenta));
  datMc->Draw();
  canv->SaveAs("PdfPlots/datMfit.pdf");

  
  if (doBkg){
    // background datasets
    RooDataSet *bkg_novbf = new RooDataSet("bkg_bdt_novbf","bkg_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*bkgTree),Cut("category<4 && bdtoutput>=-0.05"),WeightVar(*weight));
    RooDataSet *bkg_vbf = new RooDataSet("bkg_bdt_vbf","bkg_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*bkgTree),Cut("category==5 && bdtoutput>=-0.05"),WeightVar(*weight));
    RooDataSet *bkg_vbf_tight = new RooDataSet("bkg_bdt_vbf_tight","bkg_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*bkgTree),Cut("category==4 && bdtoutput>=-0.05"),WeightVar(*weight));
    outWS->import(*bkg_novbf);
    outWS->import(*bkg_vbf);
    outWS->import(*bkg_vbf_tight);

    // bkg pdfs
    RooDataHist *bkg_hist = new RooDataHist("bkg_bdt_hist","bkg_bdt",RooArgSet(*bdtoutput,*mass),*bkg_novbf);
    RooHistPdf *bkg_hist_pdf = new RooHistPdf("bkg_hist_pdf","bkg_hist_pdf",RooArgSet(*bdtoutput,*mass),*bkg_hist);

    outWS->import(*bkg_pdf);
    outWS->import(*bkg_hist);
    outWS->import(*bkg_hist_pdf);
    
    // bkg plots
    RooPlot *bkgBk = bdtoutput->frame();
    bkg_novbf->plotOn(bkgBk,Binning(bdtBins));
    bkg_pdf->plotOn(bkgBk);
    bkgBk->Draw();
    canv->SaveAs("PdfPlots/bkgBkeys.pdf");
    
    RooPlot *bkgB = bdtoutput->frame();
    bkg_novbf->plotOn(bkgB,Binning(bdtBins));
    bkg_hist_pdf->plotOn(bkgB);
    bkgB->Draw();
    canv->SaveAs("PdfPlots/bkgB.pdf");

    RooPlot *bkgM = mass->frame();
    bkg_novbf->plotOn(bkgM,Binning(massBins));
    bkg_hist_pdf->plotOn(bkgM);
    bkgM->Draw();
    canv->SaveAs("PdfPlots/bkgM.pdf");
  }

  // signal datasets
  
	RooDataSet *ggh_novbf = new RooDataSet("ggh_bdt_novbf","ggh_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*gghTree),Cut("category<4 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *vbf_novbf = new RooDataSet("vbf_bdt_novbf","vbf_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*vbfTree),Cut("category<4 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *wzh_novbf = new RooDataSet("wzh_bdt_novbf","wzh_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*wzhTree),Cut("category<4 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *tth_novbf = new RooDataSet("tth_bdt_novbf","tth_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*tthTree),Cut("category<4 && bdtoutput>=-0.05"),WeightVar(*weight));

	RooDataSet *ggh_vbf = new RooDataSet("ggh_bdt_vbf","ggh_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*gghTree),Cut("category==5 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *vbf_vbf = new RooDataSet("vbf_bdt_vbf","vbf_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*vbfTree),Cut("category==5 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *wzh_vbf = new RooDataSet("wzh_bdt_vbf","wzh_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*wzhTree),Cut("category==5 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *tth_vbf = new RooDataSet("tth_bdt_vbf","tth_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*tthTree),Cut("category==5 && bdtoutput>=-0.05"),WeightVar(*weight));

	RooDataSet *ggh_vbf_tight = new RooDataSet("ggh_bdt_vbf_tight","ggh_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*gghTree),Cut("category==4 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *vbf_vbf_tight = new RooDataSet("vbf_bdt_vbf_tight","vbf_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*vbfTree),Cut("category==4 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *wzh_vbf_tight = new RooDataSet("wzh_bdt_vbf_tight","wzh_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*wzhTree),Cut("category==4 && bdtoutput>=-0.05"),WeightVar(*weight));
  RooDataSet *tth_vbf_tight = new RooDataSet("tth_bdt_vbf_tight","tth_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*tthTree),Cut("category==4 && bdtoutput>=-0.05"),WeightVar(*weight));

	RooDataSet *sig_novbf = (RooDataSet*)ggh_novbf->Clone("sig_bdt_novbf");
	sig_novbf->append(*vbf_novbf);
	sig_novbf->append(*wzh_novbf);
	sig_novbf->append(*tth_novbf);
	RooDataSet *sig_vbf = (RooDataSet*)ggh_vbf->Clone("sig_bdt_vbf");
	sig_vbf->append(*vbf_vbf);
	sig_vbf->append(*wzh_vbf);
	sig_vbf->append(*tth_vbf);
	RooDataSet *sig_vbf_tight = (RooDataSet*)ggh_vbf_tight->Clone("sig_bdt_vbf_tight");
	sig_vbf_tight->append(*vbf_vbf_tight);
	sig_vbf_tight->append(*wzh_vbf_tight);
	sig_vbf_tight->append(*tth_vbf_tight);
  //RooDataSet *sig_novbf = new RooDataSet("sig_bdt_novbf","sig_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*sigTree),Cut("category<4 && bdtoutput>=0.05"),WeightVar(*weight));
  //RooDataSet *sig_vbf = new RooDataSet("sig_bdt_vbf","sig_bdt",RooArgSet(*bdtoutput,*mass,*category,*weight),Import(*sigTree),Cut("category>3 && bdtoutput>=0.05"),WeightVar(*weight));
  
  // signal pdfs
  RooDataHist *sig_hist = new RooDataHist("sig_bdt_hist","sig_bdt",RooArgSet(*bdtoutput,*mass),*sig_novbf);
  RooHistPdf *sig_pdf = new RooHistPdf("sig_pdf","sig_pdf",RooArgSet(*bdtoutput,*mass),*sig_hist);

  RooPlot *sigB = bdtoutput->frame();
  sig_novbf->plotOn(sigB,Binning(bdtBins));
  sig_pdf->plotOn(sigB);
  sigB->Draw();
  canv->SaveAs("PdfPlots/sigB.pdf");

  RooPlot *sigM = mass->frame();
  sig_novbf->plotOn(sigM,Binning(massBins));
  sig_pdf->plotOn(sigM);
  sigM->Draw();
  canv->SaveAs("PdfPlots/sigM.pdf");
  

  outWS->import(*data_novbf);
  outWS->import(*data_vbf);
  outWS->import(*data_vbf_tight);
  outWS->import(*data_pdf);
  outWS->import(*data_formass_all);
  outWS->import(*data_hist);
  outWS->import(*data_hist_pdf);
  outWS->import(*fit);
  outWS->import(*sig_novbf);
  outWS->import(*sig_vbf);
  outWS->import(*sig_vbf_tight);
  outWS->import(*sig_pdf);

  outFile->cd();
  outWS->Write();
  gghTree->Write();
  vbfTree->Write();
  wzhTree->Write();
  tthTree->Write();
  dataTree->Write();
  if (doBkg) bkgTree->Write();

  outFile->Close();
  treeFile->Close();



}
