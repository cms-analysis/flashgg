#include <iostream>
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"

#include "../interface/InitialFit.h"
#include "../interface/LinearInterp.h"
#include "../interface/FinalModelConstruction.h"

using namespace std;
using namespace RooFit;

int main(){

  TFile *inFile = TFile::Open("../AnalysisScripts/MK_signal_test/CMS-HGG_sig.root");
  RooWorkspace *inWS = (RooWorkspace*)inFile->Get("cms_hgg_workspace");

  RooRealVar *mass = (RooRealVar*)inWS->var("CMS_hgg_mass");
  RooRealVar *intLumi = (RooRealVar*)inWS->var("IntLumi");
  RooRealVar *MH = new RooRealVar("MH","MH",110,150);

	/*
  InitialFit initFitRV(mass,MH,110,150);
  initFitRV.setVerbosity(0);
  initFitRV.buildSumOfGaussians("ggh_cat0",3);

  InitialFit initFitWV(mass,MH,110,150);
  initFitWV.setVerbosity(0);
  initFitWV.buildSumOfGaussians("ggh_cat0",1);

  for (int mh=110; mh<=150; mh+=5){
    RooDataSet *dataRV = (RooDataSet*)inWS->data(Form("sig_ggh_mass_m%d_rv_cat0",mh));
    initFitRV.addDataset(mh,dataRV);
    RooDataSet *dataWV = (RooDataSet*)inWS->data(Form("sig_ggh_mass_m%d_wv_cat0",mh));
    initFitWV.addDataset(mh,dataWV);
  }
  initFitRV.runFits(1);
  initFitRV.saveParamsToFileAtMH("dat/in/ggh_cat0_rv.dat",125);
  initFitRV.loadPriorConstraints("dat/in/ggh_cat0_rv.dat",0.1);
  initFitRV.runFits(1);
  initFitRV.plotFits("plots/test/rv");
  initFitWV.runFits(1);
  initFitWV.saveParamsToFileAtMH("dat/in/ggh_cat0_wv.dat",125);
  initFitWV.loadPriorConstraints("dat/in/ggh_cat0_wv.dat",0.1);
  initFitWV.runFits(1);
  initFitWV.plotFits("plots/test/wv");

  map<int,map<string,RooRealVar*> > fitParamsRV = initFitRV.getFitParams();
  map<int,map<string,RooRealVar*> > fitParamsWV = initFitWV.getFitParams();


  LinearInterp linInterpRV(MH,110,150,fitParamsRV,false);
  linInterpRV.setVerbosity(0);
  linInterpRV.interpolate(3);
  map<string,RooSpline1D*> splinesRV = linInterpRV.getSplines();
  LinearInterp linInterpWV(MH,110,150,fitParamsWV,false);
  linInterpWV.setVerbosity(0);
  linInterpWV.interpolate(1);
  map<string,RooSpline1D*> splinesWV = linInterpWV.getSplines();
	*/

	vector<int> skipMasses;
  FinalModelConstruction finalModel(mass,MH,intLumi,110,150,"ggh",0,false,"dat/photon_systs_massfac_legacy_8TeV_v2.dat",skipMasses,1,false);
	finalModel.printSignalSystematics();
  /*
	finalModel.setRVsplines(splinesRV);
  finalModel.setWVsplines(splinesWV);
  map<int,RooDataSet*> datasetsRV;
  map<int,RooDataSet*> datasetsWV;
  map<int,RooDataSet*> datasetsSTD;
  for (int mh=110; mh<=150; mh+=5){
    RooDataSet *dataRV = (RooDataSet*)inWS->data(Form("sig_ggh_mass_m%d_rv_cat0",mh));
    RooDataSet *dataWV = (RooDataSet*)inWS->data(Form("sig_ggh_mass_m%d_wv_cat0",mh));
    RooDataSet *dataSTD = (RooDataSet*)inWS->data(Form("sig_ggh_mass_m%d_cat0",mh));
    datasetsRV.insert(pair<int,RooDataSet*>(mh,dataRV));
    datasetsWV.insert(pair<int,RooDataSet*>(mh,dataWV));
    datasetsSTD.insert(pair<int,RooDataSet*>(mh,dataSTD));
  }
  finalModel.setRVdatasets(datasetsRV);
  finalModel.setWVdatasets(datasetsWV);
  finalModel.setSTDdatasets(datasetsSTD);
  finalModel.buildRvWvPdf("hggpdfsmrel",3,1,false);
  finalModel.getNormalization();
  finalModel.plotPdf("plots/test/final");
  
  TFile *outFile = new TFile("tempFile.root","RECREATE");
  RooWorkspace *outWS = new RooWorkspace("wsig");
  finalModel.save(outWS);
  outFile->cd();
  outWS->Write();
  outFile->Close();
  inFile->Close();
	*/
  return 0;
}
