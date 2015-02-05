#include <iostream>

#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"

#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooArgList.h"

#include "RooPowerLawSum.h"
#include "RooExponentialSum.h"
#include "RooMsgService.h"

using namespace std;
using namespace RooFit;

int main(){
  
  RooMsgService::instance().setGlobalKillBelow(ERROR);
  
  TFile *bkgFile = TFile::Open("comb_svn/hgg.inputbkgdata_8TeV_MVA.root");
  RooWorkspace *bkgWS = (RooWorkspace*)bkgFile->Get("cms_hgg_workspace");
  RooRealVar *mass = (RooRealVar*)bkgWS->var("CMS_hgg_mass");
  RooDataSet *data = (RooDataSet*)bkgWS->data("data_mass_cat0");

  RooRealVar *p1 = new RooRealVar("p1","p1",-2.,-10.,0.);
  RooRealVar *p2 = new RooRealVar("p2","p2",-0.001,-0.01,0.01);
  RooRealVar *p3 = new RooRealVar("p3","p3",-0.0001,-0.01,0.01);

  //RooPowerLawSum *pow1 = new RooPowerLawSum("pow","pow",*mass,RooArgList(*p1));
  //RooPowerLawSum *pow2 = new RooPowerLawSum("pow","pow",*mass,RooArgList(*p1,*p2));
  //RooPowerLawSum *pow3 = new RooPowerLawSum("pow","pow",*mass,RooArgList(*p1,*p2,*p3));

  RooExponentialSum *pow1 = new RooExponentialSum("pow1","pow1",*mass,RooArgList(*p1));
  RooExponentialSum *pow2 = new RooExponentialSum("pow2","pow2",*mass,RooArgList(*p1,*p2));
  RooExponentialSum *pow3 = new RooExponentialSum("pow3","pow3",*mass,RooArgList(*p1,*p2,*p3));

  TCanvas *canv = new TCanvas();
  RooPlot *frame = mass->frame();
  data->plotOn(frame);
  
  cout << "bus" << endl;
  pow1->fitTo(*data,PrintEvalErrors(-1),PrintLevel(-1),Warnings(-1),Verbose(-1));
  cout << "bus" << endl;
  pow1->plotOn(frame);
  pow2->fitTo(*data);
  pow2->plotOn(frame,LineColor(kRed),LineStyle(kDashed));
  pow3->fitTo(*data);
  pow3->plotOn(frame,LineColor(kGreen),LineStyle(7));
  frame->Draw();
  canv->Print("test.pdf");

  TFile *outFile = new TFile("test.root","RECREATE");

  TTree *tree = new TTree("tree","tree");

  vector<double> mu;
  mu.push_back(1.);
  mu.push_back(2.);
  mu.push_back(3.);
  mu.push_back(4.);
  vector<string> label;
  label.push_back("pol");
  label.push_back("pow");
  label.push_back("lau");
  label.push_back("exp");

  tree->Branch("mu",&mu);
  tree->Branch("label",&label);

  tree->Fill();
  outFile->cd();
  tree->Write();
  outFile->Close();
  return 0;
}

