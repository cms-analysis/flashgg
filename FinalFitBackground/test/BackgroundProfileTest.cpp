#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TMath.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooGenericPdf.h"
#include "RooBernstein.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooHistPdf.h"
#include "RooFitResult.h"
#include "RooProduct.h"
#include "RooMsgService.h"

#include "../interface/ProfileMultiplePdfs.h"
#include "../interface/PdfModelBuilder.h"

#include "boost/program_options.hpp"
#include "TStopwatch.h"

using namespace std;
using namespace RooFit;

pair<double,double> getGraphMin(TGraph *graph){
  int n =graph->GetN();
  double *x = graph->GetX();
  double *y = graph->GetY();
  int locmin = TMath::LocMin(n,y);
  return pair<double,double>(x[locmin],y[locmin]);
}

vector<double> getValsFromLikelihood(TGraph *graph){
 
  TGraph *grRot = new TGraph();
  TGraph *grRotUpp = new TGraph();
  TGraph *grRotLow = new TGraph();
  int pLow=0,pHigh=0;
  pair<double,double> graphMin = getGraphMin(graph);
  for (int p=0; p<graph->GetN(); p++){
    double x,y;
    graph->GetPoint(p,x,y);
    grRot->SetPoint(p,y,x);
    if (x<=graphMin.first) {
      grRotLow->SetPoint(pLow,y,x);
      pLow++;
    }
    if (x>=graphMin.first) {
      grRotUpp->SetPoint(pHigh,y,x);
      pHigh++;
    }
  }
  // return best fit and +/- 1/2 sigma errors
  vector<double> result;
  result.push_back(grRot->Eval(0.));
  result.push_back(grRotLow->Eval(1.0));
  result.push_back(grRotUpp->Eval(1.0));
  result.push_back(grRotLow->Eval(4.0));
  result.push_back(grRotUpp->Eval(4.0));
  return result; 
}

int main(){
  
  system("mkdir -p plots");
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  TFile *bkgFile = TFile::Open("comb_svn/hgg.inputbkgdata_8TeV_MVA.root");
  TFile *sigFile = TFile::Open("comb_svn/hgg.inputsig_8TeV_nosplitVH_MVA.root");
  RooWorkspace *bkgWS = (RooWorkspace*)bkgFile->Get("cms_hgg_workspace");
  RooWorkspace *sigWS = (RooWorkspace*)sigFile->Get("wsig_8TeV");

  RooRealVar *mass = (RooRealVar*)bkgWS->var("CMS_hgg_mass");
  RooRealVar *mu = new RooRealVar("mu","mu",-5.,5.);

  mass->setBins(320);
  cout << mass->getBins() << endl;
  RooDataSet *dataAll;
  
  int firstCat=1;
  int lastCat=1;
  float mu_low=-1.;
  float mu_high=3.;
  float mu_step=0.01;

  vector<pair<double,TGraph*> > minNlltrack;

  for (int cat=firstCat; cat<=lastCat; cat++){
    RooDataSet *data = (RooDataSet*)bkgWS->data(Form("data_mass_cat%d",cat));
    if (cat==firstCat) dataAll = (RooDataSet*)data->Clone("data_mass_all");
    else dataAll->append(*data);
    RooDataHist *dataBinned = new RooDataHist(Form("roohist_data_mass_cat%d",cat),Form("roohist_data_mass_cat%d",cat),RooArgSet(*mass),*data);
    RooDataSet *sigMC = (RooDataSet*)sigWS->data(Form("sig_mass_m125_cat%d",cat));

    if (!dataBinned || !sigMC){
      cerr << "ERROR -- one of data or signal is NULL" << endl;
      exit(1);
    }
    
    // Construct PDFs for this category using PdfModelBuilder
    PdfModelBuilder modelBuilder;
    modelBuilder.setObsVar(mass);
    modelBuilder.setSignalModifier(mu);
    // For Standard Analysis
    //if (cat>=0 && cat<=3) modelBuilder.addBkgPdf("Bernstein",5,Form("pol5_cat%d",cat));
    //if (cat>=4 && cat<=5) modelBuilder.addBkgPdf("Bernstein",4,Form("pol4_cat%d",cat));
    //if (cat>=6 && cat<=8) modelBuilder.addBkgPdf("Bernstein",3,Form("pol3_cat%d",cat));
    // To Profile Multiple PDFs
    if (cat==0 || cat==1 || cat==2 || cat==3){
      modelBuilder.addBkgPdf("Bernstein",4,Form("pol4_cat%d",cat));
      modelBuilder.addBkgPdf("Bernstein",5,Form("pol5_cat%d",cat));
      modelBuilder.addBkgPdf("Bernstein",6,Form("pol6_cat%d",cat));
      /*
      modelBuilder.addBkgPdf("PowerLaw",1,Form("pow1_cat%d",cat));
      modelBuilder.addBkgPdf("PowerLaw",3,Form("pow3_cat%d",cat));
      modelBuilder.addBkgPdf("PowerLaw",5,Form("pow5_cat%d",cat));
      modelBuilder.addBkgPdf("Exponential",1,Form("exp1_cat%d",cat));
      modelBuilder.addBkgPdf("Exponential",3,Form("exp3_cat%d",cat));
      modelBuilder.addBkgPdf("Exponential",5,Form("exp5_cat%d",cat));
      modelBuilder.addBkgPdf("Laurent",1,Form("lau1_cat%d",cat));
      modelBuilder.addBkgPdf("Laurent",3,Form("lau3_cat%d",cat));
      modelBuilder.addBkgPdf("Laurent",5,Form("lau5_cat%d",cat));
      */
    }
    if (cat==4 || cat==5 || cat==6 || cat==7 || cat==8) {
      modelBuilder.addBkgPdf("Bernstein",3,Form("pol3_cat%d",cat));
      modelBuilder.addBkgPdf("Bernstein",4,Form("pol4_cat%d",cat));
      /*
      modelBuilder.addBkgPdf("PowerLaw",1,Form("pow1_cat%d",cat));
      modelBuilder.addBkgPdf("PowerLaw",3,Form("pow3_cat%d",cat));
      modelBuilder.addBkgPdf("Exponential",1,Form("exp1_cat%d",cat));
      modelBuilder.addBkgPdf("Exponential",3,Form("exp3_cat%d",cat));
      modelBuilder.addBkgPdf("Laurent",1,Form("lau1_cat%d",cat));
      modelBuilder.addBkgPdf("Laurent",3,Form("lau3_cat%d",cat));
      */
    }
    map<string,RooAbsPdf*> bkgPdfs = modelBuilder.getBkgPdfs();
    modelBuilder.setSignalPdfFromMC(sigMC);
    modelBuilder.makeSBPdfs();
    map<string,RooAbsPdf*> sbPdfs = modelBuilder.getSBPdfs();

    modelBuilder.fitToData(dataBinned,true,true);
    modelBuilder.fitToData(dataBinned,false,true);

    modelBuilder.throwToy(Form("cat%d_toy0",cat),dataBinned->sumEntries(),true,true);

    // Profile this category using ProfileMultiplePdfs
    ProfileMultiplePdfs profiler;
    for (map<string,RooAbsPdf*>::iterator pdf=sbPdfs.begin(); pdf!=sbPdfs.end(); pdf++) {
      string bkgOnlyName = pdf->first.substr(pdf->first.find("sb_")+3,string::npos);
      if (bkgPdfs.find(bkgOnlyName)==bkgPdfs.end()){
        cerr << "ERROR -- couldn't find bkg only pdf " << bkgOnlyName << " for SB pdf " << pdf->first << endl;
        pdf->second->fitTo(*dataBinned);
        exit(1);
      }
      int nParams = bkgPdfs[bkgOnlyName]->getVariables()->getSize()-1;
      profiler.addPdf(pdf->second,2*nParams);
      //profiler.addPdf(pdf->second);
      cout << pdf->second->GetName() << " nParams=" << pdf->second->getVariables()->getSize() << " nBkgParams=" << nParams << endl;
    }
    profiler.printPdfs();
    //cout << "Continue?" << endl;
    //string bus; cin >> bus;
    profiler.plotNominalFits(dataBinned,mass,80,Form("cat%d",cat));
    pair<double,map<string,TGraph*> > minNlls = profiler.profileLikelihood(dataBinned,mass,mu,mu_low,mu_high,mu_step);
    pair<double,map<string,TGraph*> > correctedNlls = profiler.computeEnvelope(minNlls,Form("cat%d",cat),2.);
    minNlltrack.push_back(make_pair(correctedNlls.first,correctedNlls.second["envelope"]));
    //minNlls.second.insert(pair<string,TGraph*>("envelope",envelopeNll.second));
    //map<string,TGraph*> minNLLs = profiler.profileLikelihoodEnvelope(dataBinned,mu,mu_low,mu_high,mu_step);
    profiler.plot(correctedNlls.second,Form("cat%d_nlls",cat));
    //profiler.print(minNLLs,mu_low,mu_high,mu_step);
    /*
    if (minNLLs.find("envelope")==minNLLs.end()){
      cerr << "ERROR -- envelope TGraph not found in minNLLs" << endl;
      exit(1);
    }
    */
    //minNlltrack.push_back(make_pair(profiler.getGlobalMinNLL(),minNLLs["envelope"]));
  }
  //exit(1);
  TGraph *comb = new TGraph();
  for (vector<pair<double,TGraph*> >::iterator it=minNlltrack.begin(); it!=minNlltrack.end(); it++){
    if (it->second->GetN()!=minNlltrack.begin()->second->GetN()){
      cerr << "ERROR -- unequal number of points for TGraphs " << it->second->GetName() << " and " << minNlltrack.begin()->second->GetName() << endl;
      exit(1);
    }
  }
  for (int p=0; p<minNlltrack.begin()->second->GetN(); p++){
    double x,y,sumy=0;
    for (vector<pair<double,TGraph*> >::iterator it=minNlltrack.begin(); it!=minNlltrack.end(); it++){
      it->second->GetPoint(p,x,y);
      sumy += (y+it->first);
    }
    comb->SetPoint(p,x,sumy);
  }
  pair<double,double> globalMin = getGraphMin(comb);
  for (int p=0; p<comb->GetN(); p++){
    double x,y;
    comb->GetPoint(p,x,y);
    comb->SetPoint(p,x,y-globalMin.second);
  }
  vector<double> fitVal = getValsFromLikelihood(comb);

  cout << "Best fit.." << endl;
  cout << "\t mu = " << Form("%4.3f",fitVal[0]) << " +/- (1sig) = " << fitVal[2]-fitVal[0] << " / " << fitVal[0]-fitVal[1] << endl;
  cout << "\t      " << "    " << " +/- (2sig) = " << fitVal[4]-fitVal[0] << " / " << fitVal[0]-fitVal[3] << endl;

  cout << comb->Eval(fitVal[0]) << " " << comb->Eval(fitVal[1]) << " " << comb->Eval(fitVal[2]) << " " << comb->Eval(fitVal[3]) << " " << comb->Eval(fitVal[4]) << endl;

  double quadInterpVal = ProfileMultiplePdfs::quadInterpMinimum(comb);
  cout << "quadInterp: mu = " << quadInterpVal << endl;
  cout << "\t " << comb->Eval(quadInterpVal) << " " << comb->Eval(quadInterpVal-0.005) << " " << comb->Eval(quadInterpVal-0.01) << " " << comb->Eval(quadInterpVal+0.005) << " " << comb->Eval(quadInterpVal+0.01) << endl;
  
  comb->SetLineWidth(2);
  TCanvas *canv = new TCanvas();
  comb->Draw("ALP");
  canv->Print("plots/comb.pdf");
  TFile *tempOut = new TFile("tempOut.root","RECREATE");
  tempOut->cd();
  comb->SetName("comb");
  comb->Write();
  tempOut->Close();
  return 0;
}
