#include <fstream>
#include <limits>
#include <algorithm>

#include "TCanvas.h"
#include "TROOT.h"
#include "TF1.h"
#include "TPaveText.h"

#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"

#include "boost/lexical_cast.hpp"

#include "../interface/SimultaneousFit.h"

using namespace std;
using namespace RooFit;

SimultaneousFit::SimultaneousFit(string infilename, string outfilename, int mhLow, int mhHigh, int verbose, int nInclusiveCats, int nExclusiveCats, bool SMasBkg, bool SecHiggs, bool NatWidth, bool spin, bool splitVH):
  mhLow_(mhLow),
  mhHigh_(mhHigh),
  verbose_(verbose),
  nInclusiveCats_(nInclusiveCats),
  nExclusiveCats_(nExclusiveCats),
  initialFit_(true),
  linearInterp_(false),
  simultaneousFit_(true),
  mhDependentFit_(true),
  forceFracUnity_(true),
  systematicsSet_(false),
  loadPriorConstraints_(false),
  doSMHiggsAsBackground_(SMasBkg),
  doSecondHiggs_(SecHiggs),
  doNaturalWidth_(NatWidth),
  fork_(1),
  saveExtraFile_(false)
{
  allMH_ = getAllMH();
  weight = new RooRealVar("weight","weight",1.,0.,100.);
  mhCategory = new RooCategory("mhCat","mhCat");
  inFile = TFile::Open(infilename.c_str());
  outFile = new TFile(outfilename.c_str(),"RECREATE");
  outWS = new RooWorkspace("wsig");

  normalization = new Normalization_8TeV();
  xsecsGraph.insert(pair<string,TGraph*>("ggh",normalization->GetSigmaGraph("ggh")));
  xsecsGraph.insert(pair<string,TGraph*>("vbf",normalization->GetSigmaGraph("vbf")));
  if (splitVH) {
    xsecsGraph.insert(pair<string,TGraph*>("wh",normalization->GetSigmaGraph("wh")));
    xsecsGraph.insert(pair<string,TGraph*>("zh",normalization->GetSigmaGraph("zh")));
  }
  else {
    xsecsGraph.insert(pair<string,TGraph*>("wzh",normalization->GetSigmaGraph("wzh")));
  }
  xsecsGraph.insert(pair<string,TGraph*>("tth",normalization->GetSigmaGraph("tth")));
  if (spin) {
    xsecsGraph.insert(pair<string,TGraph*>("ggh_grav",normalization->GetSigmaGraph("ggh_grav")));
    xsecsGraph.insert(pair<string,TGraph*>("vbf_grav",normalization->GetSigmaGraph("vbf_grav")));
  }
  brGraph = normalization->GetBrGraph();
}

SimultaneousFit::~SimultaneousFit(){
  cout << "Cleaning..." << endl;
  inFile->Close();
  outFile->cd();
  outWS->Write();
  /// outFile->Write();
  outFile->Close();
  delete outWS;
  delete inFile;
  delete outFile;
  /// clean();
  if (saveExtraFile_) {
    extraFile_->Close();
    delete extraFile_;
  }
}

void SimultaneousFit::clean(){
  for (map<string,RooRealVar*>::iterator f=fitParams.begin(); f!=fitParams.end(); f++) delete f->second;
  for (map<string,RooFormulaVar*>::iterator f=meanParams.begin(); f!=meanParams.end(); f++) delete f->second;
  for (map<string,RooFormulaVar*>::iterator f=recfracParams.begin(); f!=recfracParams.end(); f++) delete f->second;
  for (map<string,RooGaussian*>::iterator f=initialGaussians.begin(); f!=initialGaussians.end(); f++) delete f->second;
  for (map<string,RooRealVar*>::iterator f=polParams.begin(); f!=polParams.end(); f++) delete f->second;
  for (map<string,RooFormulaVar*>::iterator f=formVars.begin(); f!=formVars.end(); f++) delete f->second;
  for (map<string,RooDataSet*>::iterator d=allData.begin(); d!=allData.end(); d++) delete d->second;
  for (map<string,RooHistFunc*>::iterator h=histFuncs.begin(); h!=histFuncs.end(); h++) delete h->second;
  for (map<string,RooDataHist*>::iterator h=dHists.begin(); h!=dHists.end(); h++) delete h->second;
  for (map<string,RooSpline1D*>::iterator h=spline1Ds.begin(); h!=spline1Ds.end(); h++) delete h->second;
  if (simultaneousFit_) {
    delete simFitCombData;
    delete simFitCombPdf;
    for (map<string,RooAbsPdf*>::iterator f=simFitPdfMap.begin(); f!=simFitPdfMap.end(); f++) delete f->second;
  }
  delete extendPdfRel;
  delete dHist_eA;
  delete dHist_norm;
  delete funcEffAcc;
  delete funcEffAccRel;
  delete weight;
  delete mhCategory;
  delete normalization;
  delete mass;
  delete MH;
}

void SimultaneousFit::setFork(int f){
  fork_=f;
}

void SimultaneousFit::setLoadPriorConstraints(bool p, float v){
  loadPriorConstraints_=p;
  constraintValue_=v;
}

void SimultaneousFit::dumpStartVals(){
  for (map<string,double>::iterator val=polStartVals.begin(); val!=polStartVals.end(); val++){
    cout << Form("%25s",val->first.c_str()) << " : " << Form("%10.4f",val->second) << "  " << Form("%10.4f",polParams[val->first]->getVal()) << endl;
  }
  //for (map<string,RooRealVar*>::iterator val=polParams.begin(); val!=polParams.end(); val++){
    //cout << Form("%12s",val->first.c_str()) << " : " << Form("%10.4f",val->second->getVal()) << endl;//<< "  " << Form("%10.4f",polParams[val->first]->getVal()) << endl;
  //}
}

void SimultaneousFit::dumpStartVals(string fname){
  ofstream datfile;
  datfile.open(Form("%s",fname.c_str()));
  for (map<string,double>::iterator val=startVals.begin(); val!=startVals.end(); val++) {
    datfile << Form("%s %1.5f",val->first.c_str(),val->second) << endl;
  }
  datfile.close();
}

void SimultaneousFit::freezePolParams(){
  for (map<string,RooRealVar*>::iterator pol=polParams.begin(); pol!=polParams.end(); pol++) {
    pol->second->setConstant(true);
  }
}

void SimultaneousFit::dumpFitParams(){
  for (map<string,RooRealVar*>::iterator fit=fitParams.begin(); fit!=fitParams.end(); fit++) {
    cout << "\t"; 
    fit->second->Print();
  }
}

void SimultaneousFit::dumpPolParams(){
  for (map<string,RooRealVar*>::iterator pol=polParams.begin(); pol!=polParams.end(); pol++) {
    cout << "\t"; 
    pol->second->Print();
  }
}

void SimultaneousFit::dumpPolParams(string fname, string proc){
  
  ofstream datfile;
  datfile.open(Form("dat/%s",fname.c_str()));
  for (map<string,RooRealVar*>::iterator pol=polParams.begin(); pol!=polParams.end(); pol++) {
    cout << "\t"; 
    datfile << Form("%s %1.5f %1.5f %1.5f",pol->first.substr(0,pol->first.find("_"+proc)).c_str(),pol->second->getVal(),pol->second->getMin(),pol->second->getMax()) << endl;
  }
  datfile.close();
}

void SimultaneousFit::dumpFormVars(){
  for (map<string,RooFormulaVar*>::iterator form=formVars.begin(); form!=formVars.end(); form++) {
    cout << "\t"; 
    form->second->Print();
  }
}

void SimultaneousFit::dumpConstVars(){
  for (map<string,RooConstVar*>::iterator cons=constParams.begin(); cons!=constParams.end(); cons++) {
    cout << "\t"; 
    cons->second->Print();
  }
}

void SimultaneousFit::dumpSimFitValues(string outfilename){
  
  ofstream datfile;
  datfile.open(outfilename.c_str());
  for (map<string,RooFormulaVar*>::iterator form=formVars.begin(); form!=formVars.end(); form++){
    if (form->first.find("mh")==string::npos){
      for (unsigned int i=0; i<allMH_.size(); i++){
        int mh = allMH_[i];
        MH->setVal(mh);
        string type = form->first.substr(form->first.find("func_")+5,form->first.find("_g")-(form->first.find("func")+5));
        string nG = form->first.substr(form->first.find("_g"),3);
        datfile << type << "_mh" << mh << nG << " " << form->second->getVal() << endl;
      }
    }
  }
  datfile.close();
}

void SimultaneousFit::loadSmearVals(){
  ifstream datfile;
  datfile.open("dat/smear_vals.dat");
  if (datfile.fail()) return;
  while (datfile.good()){
    string line;
    getline(datfile,line);
    if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
    categorySmears.insert(pair<string,double>(line.substr(0,line.find_first_of(" ")),boost::lexical_cast<double>(line.substr(line.find_first_of(" ")+1,string::npos))));
  }
  datfile.close();
}

void SimultaneousFit::loadVariablesMap(){
  ifstream datfile;
  datfile.open("dat/variables_map.dat");
  if (datfile.fail()) return;
  while (datfile.good()){
    string line;
    getline(datfile,line);
    if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
    variablesMap.insert(pair<string,string>(line.substr(0,line.find_first_of(" ")),line.substr(line.find_first_of(" ")+1,string::npos)));
  }
  datfile.close();
}

void SimultaneousFit::dumpVariablesMap(){
  for (map<string,string>::iterator it=variablesMap.begin(); it!=variablesMap.end(); it++) cout << Form("%12s -> %30s",it->first.c_str(),it->second.c_str()) << endl;
}

void SimultaneousFit::setInitialFit(bool fit){
  initialFit_=fit;
}

void SimultaneousFit::setLinearInterp(bool fit){
  linearInterp_=fit;
}

void SimultaneousFit::setSimultaneousFit(bool fit){
  simultaneousFit_=fit;
}

void SimultaneousFit::setMHDependentFit(bool fit){
  mhDependentFit_=fit;
}

void SimultaneousFit::setForceFracUnity(bool frac){
  forceFracUnity_=frac;
}

void SimultaneousFit::saveExtra(string name){
  saveExtraFile_=true;
  extraFile_ = new TFile(name.c_str(),"RECREATE");
}

vector<int> SimultaneousFit::getAllMH(){
  vector<int> result;
  for (int m=mhLow_; m<=mhHigh_; m+=5){
    cout << "Adding mass: " << m << endl;
    result.push_back(m);
  }
  return result;
}

void SimultaneousFit::setVerbose(int verbose){
  verbose_=verbose;
}

string SimultaneousFit::getDataSetName(string proc, int mh, int cat){
  return string(Form("sig_%s_mass_m%d_cat%d",proc.c_str(),mh,cat));
}

string SimultaneousFit::getWeightedDataSetName(string proc, int mh, int cat){
  return Form("wt_%s",getDataSetName(proc,mh,cat).c_str());
}

RooDataSet* SimultaneousFit::getMHDepData(string name, string proc, int cat){

  RooDataSet *data = NULL;
  for (unsigned int i=0; i<allMH_.size(); i++){
    int mh=allMH_[i];
    string searchname = "mh_"+getDataSetName(proc,mh,cat);
    assert(allData.find(searchname)!=allData.end());

    if (i==0) data = (RooDataSet*)allData[searchname]->Clone(name.c_str());
    else data->append(*allData[searchname]);
  }
  data->SetName(name.c_str());
  data->SetTitle(name.c_str());
  return data;
}

RooDataSet* SimultaneousFit::putWTinDataset(RooDataSet *data, int thisMH){
  
  RooDataSet *newDatSet = new RooDataSet(Form("wt_%s",data->GetName()),Form("wt_%s",data->GetName()),RooArgSet(*mass,*weight),WeightVar(*weight));
  for (int entry=0; entry<data->numEntries(); entry++){
    mass->setVal(data->get(entry)->getRealValue("CMS_hgg_mass"));
    weight->setVal(data->weight());
    newDatSet->add(RooArgSet(*mass,*weight),weight->getVal());
  }
  cout << "Old: " << data->sumEntries() << endl;
  cout << "New: " << newDatSet->sumEntries() << endl;
  assert(fabs(data->sumEntries()-newDatSet->sumEntries())<0.0001);
  return newDatSet;
}


RooDataSet* SimultaneousFit::putMHinDataset(RooDataSet *data, int thisMH){
  
  RooDataSet *newDatSet = new RooDataSet(Form("mh_%s",data->GetName()),Form("mh_%s",data->GetName()),RooArgSet(*mass,*MH,*weight),WeightVar(*weight));
  for (int entry=0; entry<data->numEntries(); entry++){
    mass->setVal(data->get(entry)->getRealValue("CMS_hgg_mass"));
    weight->setVal(data->weight());
    MH->setVal(double(thisMH));
    newDatSet->add(RooArgSet(*mass,*MH),weight->getVal());
  }
  cout << "Old: " << data->sumEntries() << endl;
  cout << "New: " << newDatSet->sumEntries() << endl;
  assert(fabs(data->sumEntries()-newDatSet->sumEntries())<0.0001);
  return newDatSet;
}

RooAddPdf* SimultaneousFit::getSumOfGaussiansMHDependent(string name, int nGaussians, bool recursive, string proc, int cat){
   
  RooArgList *gaussians = new RooArgList();
  RooArgList *coeffs = new RooArgList();
  
  for (int g=0; g<nGaussians; g++){
  
    RooFormulaVar *dm = findFormVar(Form("func_dm_g%d_%s_cat%d",g,proc.c_str(),cat));
    RooFormulaVar *mean = new RooFormulaVar(Form("mean_g%d_%s_cat%d",g,proc.c_str(),cat),Form("mean_g%d_%s_cat%d",g,proc.c_str(),cat),"@0+@1",RooArgList(*MH,*dm));
    RooFormulaVar *sigma = findFormVar(Form("func_sigma_g%d_%s_cat%d",g,proc.c_str(),cat));
    RooGaussian *gaus = new RooGaussian(Form("gaus_g%d_%s_cat%d",g,proc.c_str(),cat),Form("gaus_g%d_%s_cat%d",g,proc.c_str(),cat),*mass,*mean,*sigma);

    meanParams.insert(pair<string,RooFormulaVar*>(string(mean->GetName()),mean));
    initialGaussians.insert(pair<string,RooGaussian*>(string(gaus->GetName()),gaus));
    gaussians->add(*gaus);

    if (g<nGaussians-1) {
      RooFormulaVar *frac = findFormVar(Form("func_frac_g%d_%s_cat%d",g,proc.c_str(),cat));
      coeffs->add(*frac);
    }
    if (g==nGaussians-1 && forceFracUnity_) {
      string formula="1.";
      for (int i=0; i<nGaussians-1; i++) formula += Form("-@%d",i);
      RooFormulaVar *recFrac = new RooFormulaVar(Form("func_frac_g%d_%s_cat%d",g,proc.c_str(),cat),Form("func_frac_g%d_%s_cat%d",g,proc.c_str(),cat),formula.c_str(),*coeffs);
      recfracParams.insert(pair<string,RooFormulaVar*>(string(recFrac->GetName()),recFrac));
      coeffs->add(*recFrac);
    }
  }
  assert(gaussians->getSize()==nGaussians && coeffs->getSize()==nGaussians-(1*!forceFracUnity_));
  return new RooAddPdf(name.c_str(),name.c_str(),*gaussians,*coeffs,recursive);
}

RooAddPdf* SimultaneousFit::getSumOfGaussiansEachMH(int mh, int nGaussians, bool recursive, string proc, int cat){
  
  RooArgList *gaussians = new RooArgList();
  RooArgList *coeffs = new RooArgList();
  
  for (int g=0; g<nGaussians; g++){
  
    RooFormulaVar *dm = findFormVar(Form("func_mh%d_dm_g%d_%s_cat%d",mh,g,proc.c_str(),cat));
    RooFormulaVar *mean = new RooFormulaVar(Form("mean_mh%d_g%d_%s_cat%d",mh,g,proc.c_str(),cat),Form("mean_mh%d_g%d_%s_cat%d",mh,g,proc.c_str(),cat),Form("%d+@0",mh),RooArgList(*dm));
    RooFormulaVar *sigma = findFormVar(Form("func_mh%d_sigma_g%d_%s_cat%d",mh,g,proc.c_str(),cat));
    RooGaussian *gaus = new RooGaussian(Form("gaus_mh%d_g%d_%s_cat%d",mh,g,proc.c_str(),cat),Form("gaus_mh%d_g%d_%s_cat%d",mh,g,proc.c_str(),cat),*mass,*mean,*sigma);

    meanParams.insert(pair<string,RooFormulaVar*>(string(mean->GetName()),mean));
    initialGaussians.insert(pair<string,RooGaussian*>(string(gaus->GetName()),gaus));
    gaussians->add(*gaus);

    if (g<nGaussians-1) {
      RooFormulaVar *frac = findFormVar(Form("func_mh%d_frac_g%d_%s_cat%d",mh,g,proc.c_str(),cat));
      coeffs->add(*frac);
    }
    if (g==nGaussians-1 && forceFracUnity_) {
      string formula="1.";
      for (int i=0; i<nGaussians-1; i++) formula += Form("-@%d",i);
      RooFormulaVar *recFrac = new RooFormulaVar(Form("func_mh%d_frac_g%d_%s_cat%d",mh,g,proc.c_str(),cat),Form("func_mh%d_frac_g%d_%s_cat%d",mh,g,proc.c_str(),cat),formula.c_str(),*coeffs);
      recfracParams.insert(pair<string,RooFormulaVar*>(string(recFrac->GetName()),recFrac));
      coeffs->add(*recFrac);
    }
  }
  assert(gaussians->getSize()==nGaussians && coeffs->getSize()==nGaussians-(1*!forceFracUnity_));
  return new RooAddPdf(Form("sigModel_mh%d_%s_cat%d",mh,proc.c_str(),cat),Form("sigModel_mh%d_%s_cat%d",mh,proc.c_str(),cat),*gaussians,*coeffs,recursive);
}

void SimultaneousFit::setupSystematics(int cat){
  
  int mycat = cat;
  if (isExclusiveCat(cat)) mycat=nInclusiveCats_;

  loadSmearVals(); 
  globalScale = new RooRealVar("CMS_hgg_globalscale","CMS_hgg_globalscale",0.,-0.2,0.2); // 20%
  categoryScale = new RooRealVar(Form("CMS_hgg_nuissancedeltamcat%d",mycat),Form("CMS_hgg_nuissancedeltamcat%d",mycat),0.0,-0.2,0.2); // 20%
  categorySmear = new RooConstVar(Form("CMS_hgg_constsmearcat%d",cat),Form("CMS_hgg_constsmearcat%d",cat),categorySmears[Form("CMS_hgg_constsmearcat%d",cat)]);
  categoryResolution = new RooRealVar(Form("CMS_hgg_nuissancedeltasmearcat%d",mycat),Form("CMS_hgg_nuissancedeltasmearcat%d",mycat),0.0,-0.2,0.2); // 20%
  systematicsSet_=true;
}

RooAddPdf *SimultaneousFit::buildFinalPdf(string name, int nGaussians, bool recursive, string proc, int cat){
 
  if (!systematicsSet_) setupSystematics(cat);

  RooArgList *gaussians = new RooArgList();
  RooArgList *coeffs = new RooArgList();
 
  // for SM Higgs as Background
  RooArgList *gaussians_SM = new RooArgList();
  RooArgList *coeffs_SM = new RooArgList();
  
  // for Second Higgs
  RooArgList *gaussians_2 = new RooArgList();
  RooArgList *coeffs_2 = new RooArgList();

  // for Natural Width
  RooArgList *voigtians_NW = new RooArgList();
  RooArgList *coeffs_NW = new RooArgList();
  
  for (int g=0; g<nGaussians; g++){
   
    // Gaussian mean
    RooAbsReal *dm = findFormVar(Form("const_func_dm_g%d_%s_cat%d",g,proc.c_str(),cat));
    RooAbsReal *dm_SM=NULL; 
    RooAbsReal *dm_2=NULL; 
    if (linearInterp_) {
      dm = findHistFunc(Form("hist_func_dm_g%d_%s_cat%d",g,proc.c_str(),cat));
      if (doSMHiggsAsBackground_) dm_SM = findHistFunc(Form("hist_func_dm_g%d_%s_cat%d_SM",g,proc.c_str(),cat));
      if (doSecondHiggs_) dm_2 = findSpline1D(Form("hist_func_dm_g%d_%s_cat%d_2",g,proc.c_str(),cat));
    }
    RooFormulaVar *mean = new RooFormulaVar(Form("hgg_mean_g%d_%s_cat%d",g,proc.c_str(),cat),Form("mean_g%d_%s_cat%d",g,proc.c_str(),cat),"@0+@1+@0*(@2+@3)",RooArgList(*MH,*dm,*globalScale,*categoryScale));
    RooFormulaVar *mean_SM=NULL;
    RooFormulaVar *mean_2=NULL;
    if (doSMHiggsAsBackground_) mean_SM = new RooFormulaVar(Form("hgg_mean_g%d_%s_cat%d_SM",g,proc.c_str(),cat),Form("mean_g%d_%s_cat%d_SM",g,proc.c_str(),cat),"@0+@1+@0*(@2+@3)",RooArgList(*MH_SM,*dm_SM,*globalScale,*categoryScale));
    if (doSecondHiggs_) mean_2 = new RooFormulaVar(Form("hgg_mean_g%d_%s_cat%d_2",g,proc.c_str(),cat),Form("mean_g%d_%s_cat%d_2",g,proc.c_str(),cat),"@0+@1+@0*(@2+@3)",RooArgList(*MH_2,*dm_2,*globalScale,*categoryScale));

    // Gaussian sigma
    RooAbsReal *sig_fit = findFormVar(Form("const_func_sigma_g%d_%s_cat%d",g,proc.c_str(),cat));
    RooAbsReal *sig_fit_SM=NULL;
    RooAbsReal *sig_fit_2=NULL;
    if (linearInterp_) {
      sig_fit = findHistFunc(Form("hist_func_sigma_g%d_%s_cat%d",g,proc.c_str(),cat)); 
      if (doSMHiggsAsBackground_) sig_fit_SM = findHistFunc(Form("hist_func_sigma_g%d_%s_cat%d_SM",g,proc.c_str(),cat)); 
      if (doSecondHiggs_) sig_fit_2 = findSpline1D(Form("hist_func_sigma_g%d_%s_cat%d_2",g,proc.c_str(),cat)); 
    }
    // Have to ensure what's in side the sqrt is not negative
    RooFormulaVar *sigma = new RooFormulaVar(Form("hgg_sigma_g%d_%s_cat%d",g,proc.c_str(),cat),Form("hgg_sigma_g%d_%s_cat%d",g,proc.c_str(),cat),"(TMath::Power(@0,2)-TMath::Power(@1*@2,2)+TMath::Power(@1*(@2+@3),2))>0. ? TMath::Sqrt(TMath::Power(@0,2)-TMath::Power(@1*@2,2)+TMath::Power(@1*(@2+@3),2)) : 0.",RooArgList(*sig_fit,*MH,*categorySmear,*categoryResolution));
    RooFormulaVar *sigma_SM=NULL;
    RooFormulaVar *sigma_2=NULL;
    if (doSMHiggsAsBackground_) sigma_SM = new RooFormulaVar(Form("hgg_sigma_g%d_%s_cat%d_SM",g,proc.c_str(),cat),Form("hgg_sigma_g%d_%s_cat%d_SM",g,proc.c_str(),cat),"(TMath::Power(@0,2)-TMath::Power(@1*@2,2)+TMath::Power(@1*(@2+@3),2))>0. ? TMath::Sqrt(TMath::Power(@0,2)-TMath::Power(@1*@2,2)+TMath::Power(@1*(@2+@3),2)) : 0.",RooArgList(*sig_fit_SM,*MH_SM,*categorySmear,*categoryResolution));
    if (doSecondHiggs_) sigma_2 = new RooFormulaVar(Form("hgg_sigma_g%d_%s_cat%d_2",g,proc.c_str(),cat),Form("hgg_sigma_g%d_%s_cat%d_2",g,proc.c_str(),cat),"(TMath::Power(@0,2)-TMath::Power(@1*@2,2)+TMath::Power(@1*(@2+@3),2))>0. ? TMath::Sqrt(TMath::Power(@0,2)-TMath::Power(@1*@2,2)+TMath::Power(@1*(@2+@3),2)) : 0.",RooArgList(*sig_fit_2,*MH_2,*categorySmear,*categoryResolution));
    
    // make Gaussians
    RooGaussian *gaus = new RooGaussian(Form("hgg_gaus_g%d_%s_cat%d",g,proc.c_str(),cat),Form("hgg_gaus_g%d_%s_cat%d",g,proc.c_str(),cat),*mass,*mean,*sigma);
    RooGaussian *gaus_SM=NULL;
    RooGaussian *gaus_2=NULL;
    RooVoigtian *voig_NW=NULL;
    if (doSMHiggsAsBackground_) gaus_SM = new RooGaussian(Form("hgg_gaus_g%d_%s_cat%d_SM",g,proc.c_str(),cat),Form("hgg_gaus_g%d_%s_cat%d_SM",g,proc.c_str(),cat),*mass,*mean_SM,*sigma_SM);
    if (doSecondHiggs_) gaus_2 = new RooGaussian(Form("hgg_gaus_g%d_%s_cat%d_2",g,proc.c_str(),cat),Form("hgg_gaus_g%d_%s_cat%d_2",g,proc.c_str(),cat),*mass,*mean_2,*sigma_2);
    if (doNaturalWidth_) voig_NW = new RooVoigtian(Form("hgg_voig_g%d_%s_cat%d_NW",g,proc.c_str(),cat),Form("hgg_voig_g%d_%s_cat%d_NW",g,proc.c_str(),cat),*mass,*mean,*higgsDecayWidth,*sigma);
    
    meanParams.insert(pair<string,RooFormulaVar*>(string(mean->GetName()),mean));
    finalGaussians.insert(pair<string,RooGaussian*>(string(gaus->GetName()),gaus));
    if (doSMHiggsAsBackground_){
      meanParams.insert(pair<string,RooFormulaVar*>(string(mean_SM->GetName()),mean_SM));
      finalGaussians.insert(pair<string,RooGaussian*>(string(gaus_SM->GetName()),gaus_SM));
    }
    if (doSMHiggsAsBackground_){
      meanParams.insert(pair<string,RooFormulaVar*>(string(mean_SM->GetName()),mean_SM));
      finalGaussians.insert(pair<string,RooGaussian*>(string(gaus_SM->GetName()),gaus_SM));
    }
    if (doNaturalWidth_){
      finalVoigtians.insert(pair<string,RooVoigtian*>(string(voig_NW->GetName()),voig_NW));
    }

    gaussians->add(*gaus);
    if (doSMHiggsAsBackground_) gaussians_SM->add(*gaus_SM);
    if (doSecondHiggs_) gaussians_2->add(*gaus_2);
    if (doNaturalWidth_) voigtians_NW->add(*voig_NW); 

    // do fracs
    if (g<nGaussians-1) {
      RooAbsReal *frac = findFormVar(Form("const_func_frac_g%d_%s_cat%d",g,proc.c_str(),cat));
      RooAbsReal *frac_SM=NULL;
      RooAbsReal *frac_2=NULL;
      if (linearInterp_) {
        frac = findHistFunc(Form("hist_func_frac_g%d_%s_cat%d",g,proc.c_str(),cat));
        if (doSMHiggsAsBackground_) frac_SM = findHistFunc(Form("hist_func_frac_g%d_%s_cat%d_SM",g,proc.c_str(),cat));
        if (doSecondHiggs_) frac_2 = findSpline1D(Form("hist_func_frac_g%d_%s_cat%d_2",g,proc.c_str(),cat));
      }
      coeffs->add(*frac);
      if (doSMHiggsAsBackground_) coeffs_SM->add(*frac_SM);
      if (doSecondHiggs_) coeffs_2->add(*frac_2);
      if (doNaturalWidth_) coeffs_NW->add(*frac);
    }
    if (g==nGaussians-1 && forceFracUnity_) {
      string formula="1.";
      for (int i=0; i<nGaussians-1; i++) formula += Form("-@%d",i);
      if (verbose_) cout << "Forcing frac unity with formula " << formula << endl;

      RooFormulaVar *recFrac = new RooFormulaVar(Form("const_func_frac_g%d_%s_cat%d",g,proc.c_str(),cat),Form("const_func_frac_g%d_%s_cat%d",g,proc.c_str(),cat),formula.c_str(),*coeffs);
      recfracParams.insert(pair<string,RooFormulaVar*>(string(recFrac->GetName()),recFrac));
      coeffs->add(*recFrac);
      
      RooFormulaVar *recFrac_SM, *recFrac_2;
      if (doSMHiggsAsBackground_) {
        recFrac_SM = new RooFormulaVar(Form("const_func_frac_g%d_%s_cat%d_SM",g,proc.c_str(),cat),Form("const_func_frac_g%d_%s_cat%d_SM",g,proc.c_str(),cat),formula.c_str(),*coeffs_SM);
        recfracParams.insert(pair<string,RooFormulaVar*>(string(recFrac_SM->GetName()),recFrac_SM));
        coeffs_SM->add(*recFrac_SM);
      }
      if (doSecondHiggs_) { 
        recFrac_2 = new RooFormulaVar(Form("const_func_frac_g%d_%s_cat%d_2",g,proc.c_str(),cat),Form("const_func_frac_g%d_%s_cat%d_2",g,proc.c_str(),cat),formula.c_str(),*coeffs_2);
        recfracParams.insert(pair<string,RooFormulaVar*>(string(recFrac_2->GetName()),recFrac_2));
        coeffs_2->add(*recFrac_2);
      }
      if (doNaturalWidth_) coeffs_NW->add(*recFrac);
    }
  }
  assert(gaussians->getSize()==nGaussians && coeffs->getSize()==nGaussians-(1*!forceFracUnity_));
  if (doSMHiggsAsBackground_) {
    assert(gaussians_SM->getSize()==nGaussians && coeffs_SM->getSize()==nGaussians-(1*!forceFracUnity_));
    smHiggsBkgPdf = new RooAddPdf(Form("%s_SM",name.c_str()),Form("%s_SM",name.c_str()),*gaussians_SM,*coeffs_SM,recursive);
  }
  if (doSecondHiggs_) {
    assert(gaussians_2->getSize()==nGaussians && coeffs_2->getSize()==nGaussians-(1*!forceFracUnity_));
    secondHiggsPdf = new RooAddPdf(Form("%s_2",name.c_str()),Form("%s_2",name.c_str()),*gaussians_2,*coeffs_2,recursive);
  }
  if (doNaturalWidth_) {
    assert(voigtians_NW->getSize()==nGaussians && coeffs_NW->getSize()==nGaussians-(1*!forceFracUnity_));
    naturalWidthPdf = new RooAddPdf(Form("%s_NW",name.c_str()),Form("%s_NW",name.c_str()),*voigtians_NW,*coeffs_NW,recursive);
  }
  return new RooAddPdf(name.c_str(),name.c_str(),*gaussians,*coeffs,recursive);

}

RooAddPdf* SimultaneousFit::getSumOfGaussians(int nGaussians, bool recursive){
   
  RooArgList *gaussians = new RooArgList();
  RooArgList *coeffs = new RooArgList();
  
  for (int g=0; g<nGaussians; g++){
    
    RooRealVar *dm = new RooRealVar(Form("dm_g%d",g),Form("dm_g%d",g),0.1,-10.,10.);
    RooFormulaVar *mean = new RooFormulaVar(Form("mean_g%d",g),Form("mean_g%d",g),"@0+@1",RooArgList(*MH,*dm));
    RooRealVar *sigma = new RooRealVar(Form("sigma_g%d",g),Form("sigma_g%d",g),2.,0.7,10.);
    RooGaussian *gaus = new RooGaussian(Form("gaus_g%d",g),Form("gaus_g%d",g),*mass,*mean,*sigma);

    fitParams.insert(pair<string,RooRealVar*>(string(dm->GetName()),dm));
    fitParams.insert(pair<string,RooRealVar*>(string(sigma->GetName()),sigma));
    meanParams.insert(pair<string,RooFormulaVar*>(string(mean->GetName()),mean));
    initialGaussians.insert(pair<string,RooGaussian*>(string(gaus->GetName()),gaus));
    gaussians->add(*gaus);

    TH1F *dmTH = new TH1F(Form("th_%s",dm->GetName()),Form("th_%s",dm->GetName()),9,107.5,152.5);
    TH1F *sigmaTH = new TH1F(Form("th_%s",sigma->GetName()),Form("th_%s",sigma->GetName()),9,107.5,152.5);
    th1fs.insert(pair<string,TH1F*>(string(dmTH->GetName()),dmTH));    
    th1fs.insert(pair<string,TH1F*>(string(sigmaTH->GetName()),sigmaTH));  

    if (g<nGaussians-1) {
      RooRealVar *frac = new RooRealVar(Form("frac_g%d",g),Form("frac_g%d",g),0.01,0.,1.);
      fitParams.insert(pair<string,RooRealVar*>(string(frac->GetName()),frac));
      coeffs->add(*frac);
      
      TH1F *fracTH = new TH1F(Form("th_%s",frac->GetName()),Form("th_%s",frac->GetName()),9,107.5,152.5);
      th1fs.insert(pair<string,TH1F*>(string(fracTH->GetName()),fracTH));
    }
    if (g==nGaussians-1 && forceFracUnity_) {
      string formula="1.";
      for (int i=0; i<nGaussians-1; i++) formula += Form("-@%d",i);
      RooFormulaVar *recFrac = new RooFormulaVar(Form("frac_g%d",g),Form("frac_g%d",g),formula.c_str(),*coeffs);
      recfracParams.insert(pair<string,RooFormulaVar*>(string(recFrac->GetName()),recFrac));
      coeffs->add(*recFrac);
    }
  }
  assert(gaussians->getSize()==nGaussians && coeffs->getSize()==nGaussians-(1*!forceFracUnity_));
  RooAddPdf *temp = new RooAddPdf("sigModel","sigModel",*gaussians,*coeffs,recursive);
  return temp;
}

void SimultaneousFit::setParamAttributes(RooRealVar *param){
  if (verbose_>=2) cout << "Setting parameter attributes..." << endl;
  string paramName=param->GetName();
  for (map<string,string>::iterator att=variablesMap.begin(); att!=variablesMap.end(); att++){
    if (paramName.find(att->first)!=string::npos){
      if (verbose_>=2) cout << "Match found: " << paramName << " " << att->first << " " << att->second << endl;
      string attributes = att->second;
      string val = attributes.substr(0,attributes.find_first_of(" "));
      attributes=attributes.substr(attributes.find_first_of(" ")+1,string::npos);
      string min = attributes.substr(0,attributes.find_first_of(" "));
      attributes=attributes.substr(attributes.find_first_of(" ")+1,string::npos);
      string max = attributes.substr(0,string::npos);
      if (val!="-") param->setVal(boost::lexical_cast<double>(val));
      if (min!="-") param->setMin(boost::lexical_cast<double>(min));
      if (max!="-") param->setMax(boost::lexical_cast<double>(max));
    }
  }
}

RooConstVar* SimultaneousFit::getConstPolParam(string name){
  assert(polParams.find(name)!=polParams.end());
  if (constParams.find(name)==constParams.end()) {
    constParams.insert(pair<string,RooConstVar*>(name, new RooConstVar(Form("const_%s",name.c_str()),Form("const_%s",name.c_str()),polParams[name]->getVal())));
  }
  return constParams[name];
}

RooRealVar* SimultaneousFit::getPolParam(string name){
  
  // if not in map then add it
  if (polParams.find(name)==polParams.end()){
    RooRealVar *p = new RooRealVar(name.c_str(),name.c_str(),0.05,-20.,20.);
    polParams.insert(pair<string,RooRealVar*>(name,p));
  }
  // if it has a starting value set this value
  if (polStartVals.find(name)!=polStartVals.end()){
    polParams[name]->setVal(polStartVals[name]);
  }
  // also can load values from map
  setParamAttributes(polParams[name]);

  return polParams[name];
}

RooRealVar* SimultaneousFit::findPolParam(string name){
  assert(polParams.find(name)!=polParams.end());
  return polParams[name];
}

RooFormulaVar* SimultaneousFit::findFormVar(string name){
 
  assert(formVars.find(name)!=formVars.end());
  return formVars[name];
}

RooHistFunc* SimultaneousFit::findHistFunc(string name){
  assert(histFuncs.find(name)!=histFuncs.end());
  return histFuncs[name];
}

RooSpline1D* SimultaneousFit::findSpline1D(string name){
  assert(spline1Ds.find(name)!=spline1Ds.end());
  return spline1Ds[name];
}

// with const params
void SimultaneousFit::constructConstFormulaVar(string prefix, int order){

  RooArgList *dependents = new RooArgList();
  dependents->add(*MH);
  string formula="";
  for (int i=0; i<=order; i++){
    RooConstVar *p = getConstPolParam(Form("p%d_%s",i,prefix.c_str()));
    dependents->add(*p);
    if (i>0) formula+=" + ";
    formula += Form("@%d*TMath::Power(@0,%d)",i+1,i);
  }
  RooFormulaVar *formV = new RooFormulaVar(Form("const_func_%s",prefix.c_str()),Form("const_func_%s",prefix.c_str()),formula.c_str(),*dependents);
  formVars.insert(pair<string,RooFormulaVar*>(string(formV->GetName()),formV));
}

void SimultaneousFit::constructConstFormulaVars(int nGaussians, int dmOrder, int sigmaOrder, int fracOrder, string proc, int cat){
  
  for (int g=0; g<nGaussians; g++){
    constructConstFormulaVar(Form("dm_g%d_%s_cat%d",g,proc.c_str(),cat),dmOrder);
    constructConstFormulaVar(Form("sigma_g%d_%s_cat%d",g,proc.c_str(),cat),sigmaOrder);
    if (g<nGaussians-1) constructConstFormulaVar(Form("frac_g%d_%s_cat%d",g,proc.c_str(),cat),fracOrder);
  }
}


// mh has floating dependence
void SimultaneousFit::constructFormulaVar(string prefix, int order){

  RooArgList *dependents = new RooArgList();
  dependents->add(*MH);
  string formula="";
  for (int i=0; i<=order; i++){
    RooRealVar *p = getPolParam(Form("p%d_%s",i,prefix.c_str()));
    dependents->add(*p);
    if (i>0) formula+=" + ";
    formula += Form("@%d*TMath::Power(@0,%d)",i+1,i);
  }
  RooFormulaVar *formV = new RooFormulaVar(Form("func_%s",prefix.c_str()),Form("func_%s",prefix.c_str()),formula.c_str(),*dependents);
  formVars.insert(pair<string,RooFormulaVar*>(string(formV->GetName()),formV));
}

// mh dependence hard wired
void SimultaneousFit::constructFormulaVar(string prefix, int order, int mh){
  
  RooArgList *dependents = new RooArgList();
  string formula="";
  for (int i=0; i<=order; i++){
    RooRealVar *p = getPolParam(Form("p%d_%s",i,prefix.c_str()));
    dependents->add(*p);
    if (i>0) formula+=" + ";
    formula += Form("@%d*TMath::Power(%d,%d)",i,mh,i);
  }
  RooFormulaVar *formV = new RooFormulaVar(Form("func_mh%d_%s",mh,prefix.c_str()),Form("func_mh%d_%s",mh,prefix.c_str()),formula.c_str(),*dependents);
  formVars.insert(pair<string,RooFormulaVar*>(string(formV->GetName()),formV));
}

void SimultaneousFit::constructFormulaVars(int nGaussians, int dmOrder, int sigmaOrder, int fracOrder, string proc, int cat){
  
  for (int g=0; g<nGaussians; g++){
    constructFormulaVar(Form("dm_g%d_%s_cat%d",g,proc.c_str(),cat),dmOrder);
    constructFormulaVar(Form("sigma_g%d_%s_cat%d",g,proc.c_str(),cat),sigmaOrder);
    if (g<nGaussians-1) constructFormulaVar(Form("frac_g%d_%s_cat%d",g,proc.c_str(),cat),fracOrder);
    
    for (vector<int>::iterator mh=allMH_.begin(); mh!=allMH_.end(); mh++){
      constructFormulaVar(Form("dm_g%d_%s_cat%d",g,proc.c_str(),cat),dmOrder,*mh);
      constructFormulaVar(Form("sigma_g%d_%s_cat%d",g,proc.c_str(),cat),sigmaOrder,*mh);
      if (g<nGaussians-1) constructFormulaVar(Form("frac_g%d_%s_cat%d",g,proc.c_str(),cat),fracOrder,*mh);
    }
  }
}

void SimultaneousFit::plotPolParam(string name, int order, string proc, int cat){

  string formula="";
  string text="";
  for (int i=0; i<=order; i++){
    RooRealVar *p = findPolParam(Form("p%d_%s",i,name.c_str()));
    double value = p->getVal();
    if (i>0) {
      formula+=" + ";
      text+=", ";
    }
    text += Form("p%d = %6.4f",i,value);
    formula += Form("%10.6f*TMath::Power(x,%d)",value,i);
  }
  TFormula *tf = new TFormula(Form("tf_%s",name.c_str()),formula.c_str());
  
  TGraph *tg = new TGraph();
  tg->SetName(Form("tg_%s",name.c_str()));
  int i=0;
  for (double mh=floor(double(mhLow_)+0.5); mh<floor(double(mhHigh_)+0.5)+0.1; mh+=0.1){
    tg->SetPoint(i,mh,tf->Eval(mh));
    i++;
  }
  delete tf;

  TPaveText pave(0.1,0.9,0.9,0.99,"NDC");
  pave.SetFillColor(0);
  pave.AddText(text.c_str());
  pave.SetTextSize(0.03);
  TCanvas *canv = new TCanvas();
  tg->SetLineColor(kRed);
  tg->SetLineWidth(2);
  tg->GetXaxis()->SetTitle("m_{H} (GeV)");
  tg->GetYaxis()->SetTitle(name.c_str());
  tg->Draw("AL");
  pave.Draw("same");
  canv->Print(Form("plots/finalFit/%s_cat%d/%s.pdf",proc.c_str(),cat,tg->GetName()));
  canv->Print(Form("plots/finalFit/%s_cat%d/%s.png",proc.c_str(),cat,tg->GetName()));
  delete canv;
  
  if (saveExtraFile_){
    extraFile_->cd();
    tg->Write();
  }
  delete tg;
}

void SimultaneousFit::plotPolParams(int nGaussians, int dmOrder, int sigmaOrder, int fracOrder, string proc, int cat){
 
  for (int g=0; g<nGaussians; g++){
    plotPolParam(Form("dm_g%d_%s_cat%d",g,proc.c_str(),cat),dmOrder,proc,cat);
    plotPolParam(Form("sigma_g%d_%s_cat%d",g,proc.c_str(),cat),sigmaOrder,proc,cat);
    if (g<nGaussians-1) plotPolParam(Form("frac_g%d_%s_cat%d",g,proc.c_str(),cat),fracOrder,proc,cat);
  }
}

void SimultaneousFit::addFitResultToMap(int mh){
 
  for (map<string,RooRealVar*>::iterator param=fitParams.begin(); param!=fitParams.end(); param++){
    string name = param->first;
    if (name.find("dm")==string::npos && name.find("sigma")==string::npos && name.find("frac")==string::npos) continue; 
    name.replace(name.find("_g"),2,Form("_mh%d_g",mh));
    startVals.insert(pair<string,double>(name,param->second->getVal()));
  }
}

void SimultaneousFit::revertStartingVals(int mh){
  
  for (map<string,RooRealVar*>::iterator param=fitParams.begin(); param!=fitParams.end(); param++){
    string name = param->first;
    if (name.find("dm")==string::npos && name.find("sigma")==string::npos && name.find("frac")==string::npos) continue; 
    name.replace(name.find("_g"),2,Form("_mh%d_g",mh));
    if (startVals.find(name)!=startVals.end()){
      param->second->setVal(startVals.find(name)->second);
    }
  }
}

bool orderer(pair<double,pair<double,double> > i, pair<double,pair<double,double> > j){
  return i.first<j.first;
}


// ordering type = 0 by mass 1 by sigma 2 by frac
void SimultaneousFit::orderGaussians(int nGaussians, bool recursive, int orderingType){
  
  if (nGaussians<2) return;
  for (unsigned int i=0; i<allMH_.size(); i++){
    int mh = allMH_[i];
    vector<pair<double,pair<double,double> > > params;
    double sumFrac=0.;
    for (int g=0; g<nGaussians; g++){
      double mean=th1fs[Form("th_dm_g%d",g)]->GetBinContent(th1fs[Form("th_dm_g%d",g)]->FindBin(mh));
      double sigma=th1fs[Form("th_sigma_g%d",g)]->GetBinContent(th1fs[Form("th_sigma_g%d",g)]->FindBin(mh));
      double frac;
      if (g<nGaussians-1) {
        frac =th1fs[Form("th_frac_g%d",g)]->GetBinContent(th1fs[Form("th_frac_g%d",g)]->FindBin(mh));
        sumFrac += frac;
      }
      else {
        if (recursive) {
          cout << "Recursive ordering not supported" << endl;
          exit(1);
        }
        else {
          frac = 1.-sumFrac;
        }
      }
      double orderVal;
      pair<double,double> otherVals;
      // order by mean
      if (orderingType==0){
        orderVal=mean,
        otherVals = pair<double,double>(sigma,frac);
      }
      // order by sigma
      else if (orderingType==1){
        orderVal=sigma,
        otherVals = pair<double,double>(mean,frac);
      }
      // order by frac
      else if (orderingType==2){
        orderVal=frac,
        otherVals = pair<double,double>(mean,sigma);
      }
      else {
        cout << "ordering type " << orderingType << " not recognised" << endl;
        exit(1);
      }
      params.push_back(pair<double,pair<double,double> >(orderVal,otherVals));
    }
    sort(params.begin(), params.end(), orderer);
    for (int g=0; g<nGaussians; g++){
      double mean=0.;
      double sigma=0.;
      double frac=0.;
      // order by mean
      if (orderingType==0){
        mean = params[g].first;
        sigma = params[g].second.first;
        frac = params[g].second.second;
      }
      // order by sigma
      else if (orderingType==1){
        mean = params[g].second.first;
        sigma = params[g].first;
        frac = params[g].second.second;
      }
      // order by frac
      else if (orderingType==2){
        mean = params[g].second.first;
        sigma = params[g].second.second;
        frac = params[g].first;
      }
      th1fs[Form("th_dm_g%d",g)]->SetBinContent(th1fs[Form("th_dm_g%d",g)]->FindBin(mh),mean);
      th1fs[Form("th_sigma_g%d",g)]->SetBinContent(th1fs[Form("th_sigma_g%d",g)]->FindBin(mh),sigma);
      if (g<nGaussians-1) th1fs[Form("th_frac_g%d",g)]->SetBinContent(th1fs[Form("th_frac_g%d",g)]->FindBin(mh),frac);
    }
  }

}

void SimultaneousFit::fitTH1F(string name, int order, string proc, int cat, bool setToFitValues){

  string thname = "th_"+name;
  TF1 *pol = new TF1(Form("pol%d_%s",order,name.c_str()),Form("pol%d",order),mhLow_,mhHigh_);
  map<string,TH1F*>::iterator th1f = th1fs.find(thname);
  if (th1f!=th1fs.end()){
    verbose_ >=2 ?
      th1f->second->Fit(pol,"EMFEX0") :
      th1f->second->Fit(pol,"QEMFEX0")
    ;
    for (int i=0; i<=order; i++){
      polStartVals.insert(pair<string,double>(Form("p%d_%s_%s_cat%d",i,name.c_str(),proc.c_str(),cat),pol->GetParameter(i)));
    }
    TCanvas *canv = new TCanvas("TH1F","TH1F");
    th1f->second->Draw("LEP");
    pol->Draw("same");
    canv->Print(Form("plots/initialFit/%s_cat%d/%s.pdf",proc.c_str(),cat,thname.c_str()));
    canv->Print(Form("plots/initialFit/%s_cat%d/%s.png",proc.c_str(),cat,thname.c_str()));
    delete canv;
    if (setToFitValues){
      for (unsigned int i=0; i<allMH_.size(); i++){
        int mh=allMH_[i];
        th1f->second->SetBinContent(th1f->second->FindBin(mh),pol->Eval(mh));
      }
    }
    delete pol;
    if (saveExtraFile_){
      extraFile_->cd();
      th1f->second->Write();
    }
  }
  else cerr << "WARNING -- TH1F - " << name << " not found" << endl;
}

void SimultaneousFit::fitTH1Fs(int nGaussians, int dmOrder, int sigmaOrder, int fracOrder, string proc, int cat, bool setToFitValues){
  
  for (int g=0; g<nGaussians; g++){
    fitTH1F(Form("dm_g%d",g),dmOrder,proc,cat,setToFitValues);
    fitTH1F(Form("sigma_g%d",g),sigmaOrder,proc,cat,setToFitValues);
    if (g<nGaussians-1) fitTH1F(Form("frac_g%d",g),fracOrder,proc,cat,setToFitValues);
  }
}

void SimultaneousFit::loadPriorConstraints(string filename, int mh){
  
  dumpFitParams();
  ifstream datfile;
  datfile.open(filename.c_str());
  if (datfile.fail()) return;
  while (datfile.good()) {
    string line;
    getline(datfile,line);
    if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
    string name = line.substr(0,line.find_first_of(" "));
    double val = boost::lexical_cast<double>(line.substr(line.find_first_of(" ")+1,string::npos));
    string var = name.substr(0,name.find("_mh"));
    string gausN = name.substr(name.find("_g"),string::npos);
    string paramName = var+gausN;
    int mhS = boost::lexical_cast<int>(name.substr(name.find("_mh")+3,name.find("_g")-name.find("_mh")-3));
    if (verbose_>=2) cout << paramName << " " << mhS << " " << val << endl;
    if (mhS==mh) {
      fitParams[paramName]->setVal(val);
      if (val>0.) fitParams[paramName]->setRange((1.-constraintValue_)*val,(1.+constraintValue_)*val);
      else fitParams[paramName]->setRange((1.+constraintValue_)*val,(1.-constraintValue_)*val);
    }
  }
  datfile.close();
  dumpFitParams();

}

void SimultaneousFit::makeHistFunc(string name, int order, string proc, int cat){
  
  string thname = "th_"+name;
  map<string,TH1F*>::iterator th1f = th1fs.find(thname);
  if (th1f!=th1fs.end()){
    // store the values in vectors for the RooSpline1D
    vector<double> xvalues, yvalues;
    for (int bin=1; bin<=th1f->second->GetNbinsX(); bin++){
      xvalues.push_back(th1f->second->GetBinCenter(bin));
      yvalues.push_back(th1f->second->GetBinContent(bin));
    }

    RooDataHist *dHist = new RooDataHist(Form("dHist_%s_%s_cat%d",name.c_str(),proc.c_str(),cat),Form("dHist_%s_%s_cat%d",name.c_str(),proc.c_str(),cat),RooArgList(*MH),th1f->second);
    dHists.insert(pair<string,RooDataHist*>(dHist->GetName(),dHist));
    RooHistFunc *hFunc = new RooHistFunc(Form("hist_func_%s_%s_cat%d",name.c_str(),proc.c_str(),cat),Form("hist_func_%s_%s_cat%d",name.c_str(),proc.c_str(),cat),RooArgSet(*MH),*dHist,1);
    histFuncs.insert(pair<string,RooHistFunc*>(hFunc->GetName(),hFunc));
    if (doSMHiggsAsBackground_){
      RooDataHist *dHistSM = new RooDataHist(Form("dHist_%s_%s_cat%d_SM",name.c_str(),proc.c_str(),cat),Form("dHist_%s_%s_cat%d_SM",name.c_str(),proc.c_str(),cat),RooArgList(*MH_SM),th1f->second);
      RooHistFunc *hFuncSM = new RooHistFunc(Form("hist_func_%s_%s_cat%d_SM",name.c_str(),proc.c_str(),cat),Form("hist_func_%s_%s_cat%d_SM",name.c_str(),proc.c_str(),cat),RooArgSet(*MH_SM),*dHistSM,1);
      histFuncs.insert(pair<string,RooHistFunc*>(hFuncSM->GetName(),hFuncSM));
    }
    // as this depends on a RooFormulaVar MH_2 = (MH+DeltaM) we cannot use a RooHistFunc
    // instead use RooSpline1D implementation from HiggsAnalysis/CombinedLimit
    if (doSecondHiggs_){
      RooSpline1D *spline2 = new RooSpline1D(Form("hist_func_%s_%s_cat%d_2",name.c_str(),proc.c_str(),cat),Form("hist_func_%s_%s_cat%d_2",name.c_str(),proc.c_str(),cat),*MH_2,xvalues.size(),&(xvalues[0]),&(yvalues[0]));
      spline1Ds.insert(pair<string,RooSpline1D*>(spline2->GetName(),spline2)); 
    }
    
    TCanvas *canv = new TCanvas("hFunc","hFunc");
    RooPlot *plot = MH->frame(Range(110,150));
    hFunc->plotOn(plot);
    plot->Draw();
    canv->Print(Form("plots/initialFit/%s_cat%d/%s.pdf",proc.c_str(),cat,dHist->GetName()));
    canv->Print(Form("plots/initialFit/%s_cat%d/%s.png",proc.c_str(),cat,dHist->GetName()));
  }
  else {
    cout << "TH1F - " << thname << " not found" << endl;
    exit(1);
  }
}

void SimultaneousFit::makeHistFuncs(int nGaussians, int dmOrder, int sigmaOrder, int fracOrder, string proc, int cat){
  
  for (int g=0; g<nGaussians; g++){
    makeHistFunc(Form("dm_g%d",g),dmOrder,proc,cat);
    makeHistFunc(Form("sigma_g%d",g),sigmaOrder,proc,cat);
    if (g<nGaussians-1) makeHistFunc(Form("frac_g%d",g),fracOrder,proc,cat);
  }

}

void SimultaneousFit::runFit(string proc, int cat, int nGaussians, int dmOrder, int sigmaOrder, int fracOrder, bool recursive, bool setToFitValues){
  
  gROOT->SetBatch();

  if (recursive && forceFracUnity_){
    cerr << "ERROR -- Cannot run with forceFracUnity and recursive both being true -- Please change options" << endl;
    exit(1);
  }
  
  loadVariablesMap();
  if (verbose_>=2) dumpVariablesMap();

  RooWorkspace *work = (RooWorkspace*)inFile->Get("cms_hgg_workspace");
  mass = (RooRealVar*)work->var("CMS_hgg_mass");
  mass->setBins(120);
  mass->SetTitle("m_{#gamma#gamma}");
  mass->setUnit("GeV");
  intLumi = (RooRealVar*)work->var("IntLumi");
  system(Form("mkdir -p plots/initialFit/%s_cat%d",proc.c_str(),cat));
  system(Form("mkdir -p plots/simFit/%s_cat%d",proc.c_str(),cat));
  system(Form("mkdir -p plots/mhFit/%s_cat%d",proc.c_str(),cat));
  system(Form("mkdir -p plots/finalFit/%s_cat%d",proc.c_str(),cat));
  system("mkdir -p dat/in");
  system("mkdir -p dat/out");
  //TCanvas *canv = new TCanvas("GenFit","GenFit");
  
  // mh for FormulaVars
  MH = new RooRealVar("MH","m_{H}",mhLow_-2.5,mhHigh_+2.5);
  MH->setUnit("GeV");

  // model extensions
  if ((simultaneousFit_ || mhDependentFit_) && (doSMHiggsAsBackground_ || doSecondHiggs_ || doNaturalWidth_)) {
    cout << "The model extensions are not yet implemented for the simultaneousFit and mhDependentFit methods" << endl;
    exit(1);
  }
  if (doSMHiggsAsBackground_) {
    MH_SM = new RooRealVar("MH_SM","m_{H} (SM)",125.,mhLow_=-2.5,mhHigh_+2.5);
    MH_SM->setUnit("GeV");
  }
  if (doSecondHiggs_) {
    DeltaM = new RooRealVar("DeltaMH","#Delta m_{H}",0.,-10.,10.);
    //MH_2 = new RooFormulaVar("MH_2","m_{H} (2)","@0+@1",RooArgList(*MH,*DeltaM));
    MH_2 = new RooAddition("MH_2","m_{H} (2)",RooArgList(*MH,*DeltaM));
  }
  if (doNaturalWidth_){
    higgsDecayWidth = new RooRealVar("HiggsDecayWidth","#Gamma_{H}",0.,0.,10.);
  }

  RooAddPdf *sigModel = getSumOfGaussians(nGaussians,recursive);

  // loop the masses once over and fit independently to get nice starting values
  // also sets some things up in this loop
  cout << "----------------------------------------" << endl;
  cout << "--------- RUNNING INITIAL FIT ----------" << endl;
  cout << "----------------------------------------" << endl;

  for (unsigned int i=0; i<allMH_.size(); i++) {
   
    int mh = allMH_[i];
    cout << "----------- mh " << mh << " ----------" << endl;
    MH->setConstant(false);
    MH->setVal(mh);
    MH->setConstant(true);
    
    RooDataSet *data = (RooDataSet*)work->data(getDataSetName(proc,mh,cat).c_str());
    RooDataSet *dataWithMH = putMHinDataset(data,mh);
    RooDataSet *dataWithWT = putWTinDataset(data,mh); 
    allData.insert(pair<string,RooDataSet*>(string(data->GetName()),data));
    allData.insert(pair<string,RooDataSet*>(string(dataWithMH->GetName()),dataWithMH));
    allData.insert(pair<string,RooDataSet*>(string(dataWithWT->GetName()),dataWithWT));
   
    if (initialFit_){
      cout << "------ fitting ------- " << endl;
      if (loadPriorConstraints_) loadPriorConstraints(Form("dat/in/initFit_%s_cat%d.dat",proc.c_str(),cat),mh);
      RooFitResult *fitRes;
      verbose_ >=2 ?
        fitRes = sigModel->fitTo(*data,NumCPU(fork_),SumW2Error(true),Save(true)) :
        verbose_ >=1 ?
          fitRes = sigModel->fitTo(*data,NumCPU(fork_),SumW2Error(true),Save(true),PrintLevel(-1)) :
          fitRes = sigModel->fitTo(*data,NumCPU(fork_),SumW2Error(true),Save(true),PrintLevel(-1),PrintEvalErrors(-1))
      ;
      
      fitRes->floatParsFinal().Print("v");
      addFitResultToMap(mh);
      delete fitRes;
      
      cout << "------ plotting ------- " << endl;
      //plot
      TCanvas *canv = new TCanvas();
      RooPlot *plot = mass->frame(Range(100,160));
      data->plotOn(plot);
      sigModel->plotOn(plot);
      plot->Draw();
      canv->Print(Form("plots/initialFit/%s_cat%d/%s_cat%d_m%d.pdf",proc.c_str(),cat,proc.c_str(),cat,mh));
      canv->Print(Form("plots/initialFit/%s_cat%d/%s_cat%d_m%d.png",proc.c_str(),cat,proc.c_str(),cat,mh));
      delete plot;
      delete canv;
    }
    // save results in TH1Fs
    for (map<string,RooRealVar*>::iterator param=fitParams.begin(); param!=fitParams.end(); param++){
      string thname = "th_"+param->first;
      if (th1fs.find(thname)!=th1fs.end()) {
        th1fs[thname]->SetBinContent(th1fs[thname]->FindBin(mh),param->second->getVal());
        th1fs[thname]->SetBinError(i+1,param->second->getError());
      }
      else {
        cout << "WARNING -- TH1F " << thname << " not found" << endl;
      }
    }
  }
 
  // Gaussian ordering
  // ordering type = 0 by mass 1 by sigma 2 by frac
  if (!loadPriorConstraints_) orderGaussians(nGaussians,recursive,0);

  // Fit TH1F to find starting values
  fitTH1Fs(nGaussians,dmOrder,sigmaOrder,fracOrder,proc,cat,setToFitValues);
  
  // make the RooHistFuncs for the parameters
  makeHistFuncs(nGaussians,dmOrder,sigmaOrder,fracOrder,proc,cat);
  dumpStartVals(Form("dat/out/initFit_%s_cat%d.dat",proc.c_str(),cat));

  // Set up the simultaneous fit
  constructFormulaVars(nGaussians,dmOrder,sigmaOrder,fracOrder,proc,cat);

  if (verbose_){
    cout << "Set starting values..." << endl;
    dumpStartVals();
    cout << "Constructed polynomial parameters...." << endl;
    dumpPolParams();
    cout << "Constructed polynomials...." << endl;
    dumpFormVars();
  }
  
  assert(polParams.size()==unsigned((dmOrder+1)*nGaussians+(sigmaOrder+1)*nGaussians+(fracOrder+1)*(nGaussians-1)));
  assert(formVars.size()==(3*nGaussians-1)*(allMH_.size()+1));

  if (simultaneousFit_ || mhDependentFit_){
    cout << "------- Setting up..... -------" << endl;
    for (unsigned int i=0; i<allMH_.size(); i++) {
     
      int mh = allMH_[i];
      MH->setConstant(false);
      MH->setVal(mh);
      MH->setConstant(true);
     
      string simFitCatName = string(Form("mh%d",mh));
      mhCategory->defineType(simFitCatName.c_str());
    
      //simFitDataMap.insert(pair<string,RooDataSet*>(simFitCatName,allData[getDataSetName(proc,mh,cat)]));
      allData[getWeightedDataSetName(proc,mh,cat)]->Print();
      simFitDataMap.insert(pair<string,RooDataSet*>(simFitCatName,allData[getWeightedDataSetName(proc,mh,cat)]));

      simFitPdfMap.insert(pair<string,RooAbsPdf*>(simFitCatName,getSumOfGaussiansEachMH(mh,nGaussians,recursive,proc,cat)));
    }
  }
    
  if (simultaneousFit_){
   
    simFitCombData = new RooDataSet(Form("simFitCombData_sig_%s_mass_cat%d",proc.c_str(),cat),Form("simFitCombData_sig_%s_mass_cat%d",proc.c_str(),cat),RooArgList(*mass),Index(*mhCategory),Import(simFitDataMap),WeightVar("weight"));
    simFitCombPdf = new RooSimultaneous(Form("simFitCombPdf_sig_%s_mass_cat%d",proc.c_str(),cat),Form("simFitCombPdf_sig_%s_mass_cat%d",proc.c_str(),cat),simFitPdfMap,*mhCategory);
    
    if (verbose_>0){
      cout << "----------------------------------------" << endl;
      cout << "------- Simultaneous Fit Info.... ------" << endl;
      cout << "----------------------------------------" << endl;
      cout << "\t"; mhCategory->Print("v");
      cout << "\t"; simFitCombData->Print(); cout << endl;
      cout << "\t"; simFitCombPdf->Print(); cout << endl;
      if (verbose_>1){
        for (map<string,RooDataSet*>::iterator dat=simFitDataMap.begin(); dat!=simFitDataMap.end(); dat++){
          cout << "\t"; dat->second->Print("v");
        }
        for (map<string,RooAbsPdf*>::iterator pdf=simFitPdfMap.begin(); pdf!=simFitPdfMap.end(); pdf++){
          cout << "\t"; pdf->second->Print("v");
        }
      }
    }
    //cout << "Run Simultaneous Fit?" << endl;
    //std::cin.ignore( std::numeric_limits<int>::max(), '\n' );
    
    cout << "----------------------------------------" << endl;
    cout << "------- RUNNING SIMULTANEOUS FIT -------" << endl;
    cout << "----------------------------------------" << endl;
    RooFitResult *simFitRes;
    verbose_ >=2 ?
      simFitRes = simFitCombPdf->fitTo(*simFitCombData,NumCPU(fork_),SumW2Error(true),Save(true)) :
      verbose_ >=1 ?
        simFitRes = simFitCombPdf->fitTo(*simFitCombData,NumCPU(fork_),SumW2Error(true),Save(true),PrintLevel(-1)) :
        simFitRes = simFitCombPdf->fitTo(*simFitCombData,NumCPU(fork_),SumW2Error(true),Save(true),PrintLevel(-1),PrintEvalErrors(-1)) 
    ;
      
    simFitRes->floatParsFinal().Print("v");
    delete simFitRes;
    dumpSimFitValues(Form("dat/out/simFit_%s_cat%d.dat",proc.c_str(),cat));

    assert(simFitDataMap.size()==simFitPdfMap.size());
    for (unsigned int i=0; i<allMH_.size(); i++) {
      int mh = allMH_[i];
      TCanvas *canv = new TCanvas();
      RooPlot *plot = mass->frame(Range(100,160));
      simFitCombData->plotOn(plot,Cut(Form("mhCat==mhCat::mh%d",mh)));
      simFitCombPdf->plotOn(plot,Slice(*mhCategory,Form("mh%d",mh)),ProjWData(*mhCategory,*simFitCombData));
      plot->Draw();
      canv->Print(Form("plots/simFit/%s_cat%d/%s_cat%d_m%d.pdf",proc.c_str(),cat,proc.c_str(),cat,mh));
      canv->Print(Form("plots/simFit/%s_cat%d/%s_cat%d_m%d.png",proc.c_str(),cat,proc.c_str(),cat,mh));
      delete plot;
      delete canv;
    }
  }

  if (mhDependentFit_){
    // set up mh dependent fit (most of it already done)
    mhFitCombData = getMHDepData(Form("mhFitCombData_sig_%s_mass_cat%d",proc.c_str(),cat),proc,cat);
    mhFitCombPdf = getSumOfGaussiansMHDependent(Form("mhFitCombPdf_sig_%s_mass_cat%d",proc.c_str(),cat),nGaussians,recursive,proc,cat);

    if (verbose_>0){
      cout << "----------------------------------------" << endl;
      cout << "------- mhDependent Fit Info.... -------" << endl;
      cout << "----------------------------------------" << endl;
      cout << "\t"; mhFitCombData->Print(); cout << endl;
      cout << "\t"; mhFitCombPdf->Print(); cout << endl;
    }
    //cout << "Run MH Dependent Fit?" << endl;
    //std::cin.ignore( std::numeric_limits<int>::max(), '\n' );

    cout << "----------------------------------------" << endl;
    cout << "------- RUNNING MH DEPENDENT FIT -------" << endl;
    cout << "----------------------------------------" << endl;
    MH->setConstant(false);
    RooFitResult *mhFitRes;
    verbose_ >=2 ?
    mhFitRes = mhFitCombPdf->fitTo(*mhFitCombData,NumCPU(fork_),SumW2Error(true),Save(true)) :
    verbose_ >=1 ?
      mhFitRes = mhFitCombPdf->fitTo(*mhFitCombData,NumCPU(fork_),SumW2Error(true),Save(true),PrintLevel(-1)) :
      mhFitRes = mhFitCombPdf->fitTo(*mhFitCombData,NumCPU(fork_),SumW2Error(true),Save(true),PrintLevel(-1),PrintEvalErrors(-1)) 
    ;
      
    mhFitRes->floatParsFinal().Print("v");
    delete mhFitRes;
    TCanvas *canv = new TCanvas();
    RooPlot *plot = mass->frame(Range(100,160));
    mhFitCombData->plotOn(plot,SumW2Error(true));
    mhFitCombPdf->plotOn(plot);
    plot->Draw();
    canv->Print(Form("plots/mhFit/%s_cat%d/%s_cat%d.pdf",proc.c_str(),cat,proc.c_str(),cat));
    canv->Print(Form("plots/mhFit/%s_cat%d/%s_cat%d.png",proc.c_str(),cat,proc.c_str(),cat));
    delete plot;
    RooPlot *mhplot = MH->frame(Range(100,160));
    mhFitCombData->plotOn(mhplot,SumW2Error(true));
    mhFitCombPdf->plotOn(mhplot);
    mhplot->Draw();
    canv->Print(Form("plots/mhFit/%s_cat%d/mh_%s_cat%d.pdf",proc.c_str(),cat,proc.c_str(),cat));
    canv->Print(Form("plots/mhFit/%s_cat%d/mh_%s_cat%d.png",proc.c_str(),cat,proc.c_str(),cat));
    delete mhplot;
    delete canv;
  }
  
  plotPolParams(nGaussians,dmOrder,sigmaOrder,fracOrder,proc,cat);
  
  //finalPdf = getSumOfGaussiansMHDependent(Form("finalPdf_sig_%s_mass_cat%d",proc.c_str(),cat),nGaussians,recursive,proc,cat);
  constructConstFormulaVars(nGaussians,dmOrder,sigmaOrder,fracOrder,proc,cat);
  
  finalPdf = buildFinalPdf(Form("hggpdfrel_%s_cat%d",proc.c_str(),cat),nGaussians,recursive,proc,cat);
  
  if ((simultaneousFit_ || mhDependentFit_) && verbose_>0) {
    cout << "Dump of pol params....." << endl;
    dumpPolParams();
    cout << "Dump of const vars....." << endl;
    dumpConstVars();
    cout << "Dump of form vars......" << endl;
    dumpFormVars();
    cout << "Polynomial fit results...." << endl;
    for (map<string,RooFormulaVar*>::iterator form=formVars.begin(); form!=formVars.end(); form++) {
      cout << "\t"; 
      form->second->Print();
    }
  }
 
  cout << "----------------------------------------" << endl;
  cout << "------- MAKING CROSS CHECK PLOTS -------" << endl;
  cout << "----------------------------------------" << endl;
  RooPlot *allPlot = mass->frame(Range(100,160));
  for (unsigned int i=0; i<allMH_.size(); i++) {
   
    int mh = allMH_[i];
    MH->setConstant(false);
    MH->setVal(mh);
    MH->setConstant(true);
    
    RooDataSet *data = allData[getDataSetName(proc,mh,cat)];
     
    //plot
    TCanvas *canv = new TCanvas();
    RooPlot *plot = mass->frame(Range(100,160));
    data->plotOn(plot);
    finalPdf->plotOn(plot);
    data->plotOn(allPlot);
    finalPdf->plotOn(allPlot);
    plot->Draw();
    canv->Print(Form("plots/finalFit/%s_cat%d/%s_cat%d_m%d.pdf",proc.c_str(),cat,proc.c_str(),cat,mh));
    canv->Print(Form("plots/finalFit/%s_cat%d/%s_cat%d_m%d.png",proc.c_str(),cat,proc.c_str(),cat,mh));
    delete plot;
    delete canv;
  }
  TCanvas *canv = new TCanvas();
  allPlot->Draw();
  canv->Print(Form("plots/%s_cat%d_allmh.pdf",proc.c_str(),cat));
  canv->Print(Form("plots/%s_cat%d_allmh.png",proc.c_str(),cat));
  delete allPlot;
  delete canv;
  
  freezePolParams();
  makeEffAccFunc(work,proc,cat);
  makeExtendPdf(proc,cat);
  //outWS->import(*extendPdfRel);
  outWS->import(*finalPdf);
  outWS->import(*funcEffAccRel);
  //outWS->import(*funcEffAcc);
  if (doSMHiggsAsBackground_){
    outWS->import(*smHiggsBkgPdf);
    outWS->import(*funcEffAccRel_SM);
  }
  if (doSecondHiggs_){
    outWS->import(*secondHiggsPdf);
    outWS->import(*funcEffAccRel_2,RecycleConflictNodes());
  }
  if (doNaturalWidth_){
    outWS->import(*naturalWidthPdf,RecycleConflictNodes());
    outWS->import(*funcEffAccRel_NW);
  }
  putDataInWS(work,proc,cat);
  plotPDF(proc,cat);
  cout << "Fit complete. Output written to " << outFile->GetName() << " : " << outWS->GetName() << endl;
}

void SimultaneousFit::putDataInWS(RooWorkspace *work, string proc, int cat){
  
  for (unsigned int i=0; i<allMH_.size(); i++) {
    int mh = allMH_[i];
    //RooDataSet *data = (RooDataSet*)work->data(getDataSetName(proc,mh,cat).c_str());
    RooDataSet *data = allData[getDataSetName(proc,mh,cat)];
    outWS->import(*data);
  }
}

void SimultaneousFit::makeExtendPdf(string proc, int cat){
  extendPdfRel = new RooExtendPdf(Form("sigpdfrel_%s_cat%d",proc.c_str(),cat),Form("sigpdfrel_%s_cat%d",proc.c_str(),cat),*finalPdf,*funcEffAccRel);
}

void SimultaneousFit::makeEffAccFunc(RooWorkspace *inWS, string proc, int cat){
  
  plotXS();
  plotBR();
  assert(xsecsGraph.find(proc)!=xsecsGraph.end());
  TGraph *eA = makeEffAccGraph(inWS,proc,cat);

  TH1F *th1f_eA = new TH1F("eAcc","eAcc",400,110,150);
  TH1F *th1f_norm = new TH1F("norm","norm",400,110,150);
  vector<double> xvalues, yvalues;
  
  for (int i=1; i<=th1f_eA->GetNbinsX(); i++) {
    double mh = th1f_eA->GetBinCenter(i);
    double effAcc = eA->Eval(mh);
    double norm = effAcc*xsecsGraph[proc]->Eval(mh)*brGraph->Eval(mh);

    th1f_eA->SetBinContent(i,effAcc);
    th1f_norm->SetBinContent(i,norm);
    xvalues.push_back(th1f_norm->GetBinCenter(i));
    yvalues.push_back(th1f_norm->GetBinContent(i));
  }
  dHist_eA = new RooDataHist("dHist_eA","dHist_eA",RooArgList(*MH),th1f_eA);
  dHist_norm = new RooDataHist("dHist_norm","dHist_norm",RooArgList(*MH),th1f_norm);
  
  funcEffAcc = new RooHistFunc(Form("hggpdfea_%s_cat%d_norm",proc.c_str(),cat),Form("hggpdfea_%s_cat%d_norm",proc.c_str(),cat),RooArgSet(*MH),*dHist_eA,2);
  funcEffAccRel = new RooHistFunc(Form("hggpdfrel_%s_cat%d_norm",proc.c_str(),cat),Form("hggpdfrel_%s_cat%d_norm",proc.c_str(),cat),RooArgSet(*MH),*dHist_norm,2);

  // model extensions
  if (doSMHiggsAsBackground_) {
    RooDataHist *dHist_SM_norm = new RooDataHist("dHist_SM_norm","dHist_SM_norm",RooArgList(*MH_SM),th1f_norm);
    funcEffAccRel_SM = new RooHistFunc(Form("hggpdfrel_%s_cat%d_SM_norm",proc.c_str(),cat),Form("hggpdfrel_%s_cat%d_SM_norm",proc.c_str(),cat),RooArgSet(*MH_SM),*dHist_SM_norm,2);
  }
  if (doSecondHiggs_) {
    funcEffAccRel_2 = new RooSpline1D(Form("hggpdfrel_%s_cat%d_2_norm",proc.c_str(),cat),Form("hggpdfrel_%s_cat%d_2_norm",proc.c_str(),cat),*MH_2,xvalues.size(),&(xvalues[0]),&(yvalues[0]));
  }
  if (doNaturalWidth_) funcEffAccRel_NW = new RooHistFunc(Form("hggpdfrel_%s_cat%d_NW_norm",proc.c_str(),cat),Form("hggpdfrel_%s_cat%d_NW_norm",proc.c_str(),cat),RooArgSet(*MH),*dHist_norm,2);
  
  TCanvas *canv = new TCanvas("EffAccFunc","EffAccFunc");
  RooPlot *plot = MH->frame(Range(110,150),Title("#epsilon#times#alpha"));
  funcEffAcc->plotOn(plot);
  plot->Draw();
  canv->Print(Form("plots/finalFit/%s_cat%d/effAccFunc_%s_cat%d.pdf",proc.c_str(),cat,proc.c_str(),cat));
  canv->Print(Form("plots/finalFit/%s_cat%d/effAccFunc_%s_cat%d.png",proc.c_str(),cat,proc.c_str(),cat));
  plot = MH->frame(Range(110,150),Title("Expected Events per 1pb^{-1}"));
  funcEffAccRel->plotOn(plot);
  plot->Draw();
  canv->Print(Form("plots/finalFit/%s_cat%d/nEntriesRel_%s_cat%d.pdf",proc.c_str(),cat,proc.c_str(),cat));
  canv->Print(Form("plots/finalFit/%s_cat%d/nEntriesRel_%s_cat%d.png",proc.c_str(),cat,proc.c_str(),cat));
  delete plot;
  delete canv;

}

TGraph* SimultaneousFit::makeEffAccGraph(RooWorkspace *work, string proc, int cat){

  TGraph *temp = new TGraph();
  TF1 *pol2 = new TF1("pol","pol2",110,150);
  for (unsigned int i=0; i<allMH_.size(); i++){
    double mh = double(allMH_[i]);
    RooDataSet *data = allData[getDataSetName(proc,mh,cat)];
    // calcu eA as sumEntries / totalxs * totalbr * intL
    double effAcc = (data->sumEntries()/(intLumi->getVal()*normalization->GetXsection(mh,proc)*normalization->GetBR(mh)));
    temp->SetPoint(i,mh,effAcc);
  }
  verbose_ >=2 ?
    temp->Fit(pol2,"EMFEX0") :
    temp->Fit(pol2,"QEMFEX0")
  ;
  TGraph *graph = new TGraph(pol2);
  graph->SetName(Form("tg_effAcc_%s_cat%d",proc.c_str(),cat));

  TCanvas *canv = new TCanvas("EffAccGraph","EffAccGraph");
  temp->SetMarkerStyle(kFullCircle);
  temp->SetMarkerSize(2);
  temp->Draw("AP");
  graph->Draw("L");
  canv->Print(Form("plots/finalFit/%s_cat%d/effAcc_%s_cat%d.png",proc.c_str(),cat,proc.c_str(),cat));
  canv->Print(Form("plots/finalFit/%s_cat%d/effAcc_%s_cat%d.pdf",proc.c_str(),cat,proc.c_str(),cat));
  delete canv;
  delete temp;
  return graph;
}

void SimultaneousFit::plotXS(string proc){
  
  TCanvas *canv = new TCanvas("XS","XS");
  canv->SetLogy();
  if (proc!=""){
    assert(xsecsGraph.find(proc)!=xsecsGraph.end());
    xsecsGraph.find(proc)->second->SetLineWidth(3);
    xsecsGraph.find(proc)->second->GetXaxis()->SetTitle("m_{H} (GeV)");
    xsecsGraph.find(proc)->second->GetXaxis()->SetRangeUser(110,150);
    xsecsGraph.find(proc)->second->GetYaxis()->SetTitle(Form("#sigma(%s) (pb)",proc.c_str()));
    xsecsGraph.find(proc)->second->Draw("ALP");
    canv->Print(Form("plots/xs_%s.pdf",proc.c_str()));
    canv->Print(Form("plots/xs_%s.png",proc.c_str()));
  }
  else {
    int colors[4] = {kRed,kBlue,kGreen,kMagenta};
    int col=0;
    for (map<string,TGraph*>::iterator it=xsecsGraph.begin(); it!=xsecsGraph.end(); it++){
      it->second->SetLineWidth(3);
      it->second->SetLineColor(colors[col]);
      it->second->GetXaxis()->SetRangeUser(110,150);
      it->second->GetXaxis()->SetTitle("m_{H} (GeV)");
      it->second->GetYaxis()->SetTitle("#sigma(pp #rightarrow H) (pb)");
      if (col==0) it->second->Draw("ALP");
      else it->second->Draw("LPSAME");
      TLegend *leg = new TLegend(0.1+col*0.2,0.91,0.1+(col+1)*0.2,0.99);
      leg->SetLineColor(0);
      leg->SetFillColor(0);
      leg->AddEntry(it->second,it->first.c_str(),"L");
      leg->SetTextSize(0.04);
      leg->Draw("same");
      canv->Modified();
      col++;
    }
    canv->Print("plots/xs_log.pdf");
    canv->Print("plots/xs_log.png");
  }
  delete canv;
}

void SimultaneousFit::plotBR(){
  
  TCanvas *canv = new TCanvas("BR","BR");
  brGraph->SetLineWidth(3);
  brGraph->GetXaxis()->SetTitle("m_{H} (GeV)");
  brGraph->GetXaxis()->SetRangeUser(110,150);
  brGraph->GetYaxis()->SetTitle("H #rightarrow #gamma#gamma BR");
  brGraph->Draw("ALP");
  canv->Print("plots/br.pdf");
  canv->Print("plots/br.png");
  delete canv;
}

void SimultaneousFit::plotPDF(string proc, int cat){
  
  TCanvas *canv = new TCanvas("PDF","PDF");
  RooPlot *plot = mass->frame(Range(105,155));
  for (int m=110; m<=150; m++){
    MH->setVal(m);
    extendPdfRel->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected));
  }
  plot->Draw();
  canv->Print(Form("plots/%s_cat%d.pdf",proc.c_str(),cat));
  canv->Print(Form("plots/%s_cat%d.png",proc.c_str(),cat));
  //delete MH;
  //delete extendPdfRel;
  //delete plot;
  //delete canv;
  //delete extendPdfRel;
}

bool SimultaneousFit::isInclusiveCat(int cat){
  if (cat>=0 && cat<nInclusiveCats_) return true;
  else return false;
}

bool SimultaneousFit::isExclusiveCat(int cat){
  if (cat>=nInclusiveCats_ && cat<nInclusiveCats_+nExclusiveCats_) return true;
  else return false;
}
