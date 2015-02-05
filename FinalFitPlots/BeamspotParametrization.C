#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TAxis.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooBifurGauss.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooFitResult.h"
#include "RooAbsArg.h"
#include "RooArgList.h"
#include "RooPlot.h"
#include "RooWorkspace.h"

using namespace std;
using namespace RooFit;

void BeamspotParametrization(string filename_newBS, string filename_oldBS){
 
  gROOT->SetBatch();

  TFile *newFile = TFile::Open(filename_newBS.c_str());
  TFile *oldFile = TFile::Open(filename_oldBS.c_str());
  TFile *outFile = new TFile("BSplots/bs_fits.root","RECREATE");
  RooWorkspace *work = new RooWorkspace("cms_hgg_workspace");
 
  // BS variable
  RooRealVar *deltaZ = new RooRealVar("ZfromGenToChosen","ZfromGenToChosen",-30.,30.);
  RooRealVar *deltaZtoBest = new RooRealVar("ZfromGenToBest","ZfromGenToBest",-0.02,0.02);
  RooRealVar *bdtoutput = new RooRealVar("bdtoutput","bdtoutput",-1,1.);
  RooRealVar *weight = new RooRealVar("weight","weight",0.,100.);
  
  // get trees
  map<string,TTree*> trees;
  trees.insert(pair<string,TTree*>("ggh_m125_newBS",(TTree*)newFile->Get("ggh_m125_8TeV_newBS")));
  trees.insert(pair<string,TTree*>("ggh_m125_oldBS",(TTree*)oldFile->Get("ggh_m125_8TeV")));

  // set up double gaussian
  // norm terms
  RooRealVar *norm0 = new RooRealVar("n0","n0",500,0,1e3);
  RooRealVar *norm1 = new RooRealVar("n1","n1",500,0,1e3);

  // mean terms
  RooRealVar *mean0 = new RooRealVar("m0","m0",0.,-0.1,0.1);
  RooRealVar *mean1 = new RooRealVar("m1","m1",0.,-0.5,0.5);

  // sigma terms
  RooRealVar *sigma0 = new RooRealVar("s0","s0",0.2,0.,2.);
  RooRealVar *sigma1 = new RooRealVar("s1","s1",6.,0.,10.);

  // gaussians
  RooGaussian *gaus0 = new RooGaussian("g0","g0",*deltaZ,*mean0,*sigma0);
  RooGaussian *gaus1 = new RooGaussian("g1","g1",*deltaZ,*mean1,*sigma1);
  
  // the extended counterparts
  RooExtendPdf *ext0 = new RooExtendPdf("e0","e0",*gaus0,*norm0);
  RooExtendPdf *ext1 = new RooExtendPdf("e1","e1",*gaus1,*norm1);
  
  // the double gaussian
  RooAddPdf *doubleGaus = new RooAddPdf("doubleGaus","doubleGaus",RooArgList(*ext0,*ext1));

  map<string,RooFitResult*> fitResults;

  // narrow range
  deltaZ->setRange("narrow",-0.02,0.02);
  
  // double gaussian fit range
  deltaZ->setRange("mid_low",-30.,-0.4);
  deltaZ->setRange("mid_high",0.4,30.);
  
  double ymax=2.;
  TCanvas *canv = new TCanvas();

  deltaZ->setRange(-0.02,0.02);
  RooPlot *plot = deltaZ->frame();
  deltaZ->setRange(-30.,30.);
  deltaZ->setBins(150);

  RooPlot *plotB = deltaZtoBest->frame();
  deltaZtoBest->setRange(-30.,30.);
  RooPlot *plotBW = deltaZtoBest->frame();

  // loop trees and fit gaussian
  int color=2;
  for (map<string,TTree*>::iterator mapIt=trees.begin(); mapIt!=trees.end(); mapIt++){
   
    cout << (mapIt->second)->GetName() << " " << (mapIt->second)->GetEntries() << endl;

    // make datasets
    RooDataSet *data = new RooDataSet((mapIt->first).c_str(),(mapIt->first).c_str(),RooArgSet(*deltaZ,*weight),Import(*(mapIt->second)),WeightVar(*weight));
    RooDataSet *best = new RooDataSet(Form("best_%s",mapIt->first.c_str()),mapIt->first.c_str(),RooArgSet(*deltaZtoBest,*weight),Import(*(mapIt->second)),WeightVar(*weight));

    // plot some close ups
    data->plotOn(plot,MarkerColor(color),LineColor(color));
    best->plotOn(plotB,MarkerColor(color),LineColor(color));
    best->plotOn(plotBW,MarkerColor(color),LineColor(color));
    color+=2;
    
    double normNarrow = data->sumEntries("TMath::Abs(ZfromGenToChosen)<50.","narrow");
    double normMid = data->sumEntries("TMath::Abs(ZfromGenToChosen)<50.","mid_low,mid_high");
  
    cout << "----- " << data->sumEntries() << " -----" << endl;
    cout << "---- " << normMid << " [-30.,-0.4][0.4,30.] " << normNarrow << " [-0.02,0.02]" << endl;
 
    // plot Data
    RooPlot *plotA = deltaZ->frame();
    plotA->SetTitle((mapIt->first).c_str());
    data->plotOn(plotA);
    plotA->Draw();
    canv->Print(Form("BSplots/all_%s.pdf",(mapIt->first).c_str()));

    // fit middle gaussian
    int ntries=0;
    RooFitResult *fitRes = doubleGaus->fitTo(*data,Range("mid_low","mid_high"),Save());
    while (fitRes->covQual()!=3 && ntries<5.){
      fitRes = doubleGaus->fitTo(*data,Range("mid_low","mid_high"),Save());
      ntries++;
    }
    
    // save params
    fitResults.insert(pair<string,RooFitResult*>(mapIt->first,fitRes));

    // plot fit result
    RooPlot *plotM = deltaZ->frame();
    plotM->SetTitle((mapIt->first).c_str());
    data->plotOn(plotM);
    doubleGaus->plotOn(plotM,Range("mid_low,mid_high"),Normalization(normMid,RooAbsReal::NumEvent));
    plotM->Draw();
    plotM->GetYaxis()->SetRangeUser(0,ymax);
    canv->Print(Form("BSplots/mid_%s.pdf",(mapIt->first).c_str()));

    work->import(*data);
    work->import(*doubleGaus);
    work->import(*fitRes);
  }
  plot->Draw();
  canv->Print("BSplots/together.pdf");
  plotB->Draw();
  canv->Print("BSplots/tobest.pdf");
  plotBW->GetYaxis()->SetRangeUser(0.,.2);
  plotBW->Draw();
  canv->Print("BSplots/tobestwide.pdf");
  
  for (map<string,RooFitResult*>::iterator iter=fitResults.begin(); iter!=fitResults.end(); iter++){
    
    cout << "--------------------------------- " << endl;
    cout << "--- FIT RESULTS --- " << iter->first << endl;
    cout << "--------------------------------- " << endl;
    (iter->second)->Print();
  }
  
  work->Write(); 
  newFile->Close();
  oldFile->Close();
  outFile->Close();

}
