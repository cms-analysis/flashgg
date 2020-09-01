#include <iostream>
#include "TDirectory.h"

using namespace std;

//int main(){
void RenameWorkspace_SignalTagger(){
  TString fn("copy_output_ggF_X250_WWgg_qqlnugg.root");
  //TString dsetName("ggF_X250_WWgg_qqlnugg_13TeV_HHWWggTag_0"); 
  TString dsetName("ggF_X250_WWgg_qqlnugg_13TeV_HHWWggTag_0");
  TString newdsetName("ggF_125_13TeV_SL");
  TString wsName("cms_hgg_13TeV");
  TString newwsName("cms_hgg_13TeV");
  TString direcName("tagsDumper");
  TFile *_file0 = TFile::Open(fn,"UPDATE");
  //cout << "_file0 = " << _file0 << endl;
  //TDirectory *direc = _file0->GetDirectory("HHWWggCandidateDumper");
  TDirectory *direc = _file0->GetDirectory(direcName);
  //RooWorkspace *ws = (RooWorkspace*)direc->Get("cms_HHWWgg_13TeV");
  RooWorkspace *ws = (RooWorkspace*)direc->Get(wsName);
  //RooWorkspace *ws_clone = ws->Clone();
  //std::cout << "ws = " << ws << std::endl; 
  //direc->cd();
  //TList *l = direc->GetListofKeys();
  //cout << "l = " << l << endl;
  //RooWorkspace *ws = cms_HHWWgg_13TeV;
  //RooAbsData *d = ws->data("ggF_X250_WWgg_qqlnugg_13TeV_SL");
  RooAbsData *d = ws->data(dsetName);
  d->SetName(newdsetName);
  direc->cd();
  //ws->Write();
  //ws->writeToFile("UpdatedwsName.root");
  ws->SetName(newwsName);
  
  // Create new file 
  TFile *newFile = TFile::Open("UpdatedwsName.root","UPDATE");
  newFile->mkdir(direcName);
  TDirectory *newDirec = newFile->GetDirectory(direcName);
  newDirec->cd();
  
  ws->Write();
}

