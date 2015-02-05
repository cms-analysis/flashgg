#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"

#include "RooWorkspace.h"
#include "RooExponential.h"
#include "RooGenericPdf.h"
//#include "RooExponentialSum.h"
//#include "RooPowerLawSum.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"

using namespace std;
using namespace RooFit;

int stupid(){

  gROOT->SetBatch(false);
  TFile *inFile = TFile::Open("files/CMS-HGG_cutbased_fmt.root");

  RooWorkspace *work = (RooWorkspace*)inFile->Get("cms_hgg_workspace");

  RooRealVar *mass = (RooRealVar*)work->var("CMS_hgg_mass");
  mass->setRange("low",100,0.98*110);
  mass->setRange("high",1.02*110,180);
  RooDataSet *data = (RooDataSet*)work->data("data_mass_cat0");
 
  RooRealVar *p1 = new RooRealVar("p1","p1",-0.02,-20.,0.);
  RooRealVar *p2 = new RooRealVar("p2","p2",-0.02,-20.,0.);
  RooRealVar *f1 = new RooRealVar("f1","f1",0.6,0.,1.);

  RooRealVar *p3 = new RooRealVar("p3","p3",-0.02,-20.,0.);
  RooRealVar *p4 = new RooRealVar("p4","p4",-0.02,-20.,0.);
  RooRealVar *f2 = new RooRealVar("f2","f2",0.6,0.,1.);

  RooRealVar *p5 = new RooRealVar("p5","p5",-0.02,-2.,0.);
  RooRealVar *p6 = new RooRealVar("p6","p6",-0.02,-2.,0.);
  RooRealVar *f3 = new RooRealVar("f3","f3",0.6,0.,1.);

  //RooExponentialSum *exp = new RooExponentialSum("exp","exp",*mass,RooArgList(*p1,*f1,*p2));
  RooExponential *e1 = new RooExponential("e1","e1",*mass,*p1);
  RooExponential *e2 = new RooExponential("e2","e2",*mass,*p2);
  e1->Print(); e2->Print(); f1->Print(); mass->Print();
  //RooAddPdf *exp = new RooAddPdf("exp","exp",RooArgList(*e1,*e2),RooArgList(*f1));
  RooAddPdf *exp = new RooAddPdf("exp","exp",*e1,*e2,*f1);

  RooGenericPdf *pow = new RooGenericPdf("pow","pow","(1.-@2)*TMath::Power(@0,@1)+@2*TMath::Power(@0,@3)",RooArgList(*mass,*p3,*f2,*p4));
  RooGenericPdf *exp2 = new RooGenericPdf("exp2","exp2","(1.-@2)*TMath::Exp(@0*@1)+@2*TMath::Exp(@0*@3)",RooArgList(*mass,*p5,*f3,*p6));
  

  //exp->fitTo(*data,Range("low,high"));
  exp->fitTo(*data,Range("low,high"),NumCPU(8));
  pow->fitTo(*data,Range("low,high"),NumCPU(8));
  exp2->fitTo(*data,Range("low,high"),NumCPU(8));

  //RooPowerLawSum *pow = new RooPowerLawSum("pow","pow",*mass,RooArgList(*p1,*f1,*p2));
  //pow->fitTo(*data);

  TCanvas *canv = new TCanvas();
  RooPlot *plot = mass->frame();
  data->plotOn(plot,Binning(80));
  exp->plotOn(plot);
  pow->plotOn(plot,LineColor(kRed),LineStyle(kDashed));
  exp2->plotOn(plot,LineColor(kGreen+2),LineStyle(kDashed));
  plot->Draw();
  canv->Print("test.pdf");

  return 0;
}

