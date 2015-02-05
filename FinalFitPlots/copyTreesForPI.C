#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjString.h"

#include <iostream>
#include <algorithm>
#include <vector>

std::vector<std::string> checkNames(TFile* f, const char* s) {

  std::vector<std::string> results;
  char a[100];
  for (int i=0; i<100; i++) {
    sprintf(a, "%s;%d", s, i);
    if (f->GetObjectUnchecked(a) != 0)
      results.push_back(a);
  }

  return results;
}

bool checkName(TString s, std::vector<TString>& c) {
  
  for (unsigned int i=0; i<c.size(); i++)
    if (s == c[i])
      return true;
  
  c.push_back(s);

  return false;
}

void copyTreesForPI(const char* mergedfile = "", const char* plottreefile = "") {

  std::vector<TString> container;

  int nfiles, nindfiles, makeOutputTree;
  Float_t intlumi_;

  TClonesArray* tca_histfilename = new TClonesArray("TObjString",1);
  TClonesArray* tca_inshortnames = new TClonesArray("TObjString", 100);
  TClonesArray* tca_infilenames = new TClonesArray("TObjString", 100);

  Int_t Nvar;
  Int_t h2d[10000], typplot[10000], histoncat[10000], histoindfromfiles[10000];
  Int_t histoncatindtonames[10000];
  Int_t nbinsx[10000], nbinsy[10000];
  Float_t lowlim[10000], highlim[10000], lowlim2[10000], highlim2[10000];
  Int_t itype[10000], infoind[10000], histoplotit[10000];
  Int_t ntot[10000], nred[10000];
  Float_t lumi[10000], xsec[10000], kfactor[10000], scale[10000];
  Int_t typplotall = 0;
  Int_t plothistoplotitPI[10000];    
  TClonesArray* tca_xaxislabels = new TClonesArray("TObjString",200);
  TClonesArray* tca_yaxislabels = new TClonesArray("TObjString",200);
  TClonesArray* tca_plotvarnames = new TClonesArray("TObjString",200);

  std::cout << "Copying plotvariables..." << std::endl;

  TFile *merged = new TFile(mergedfile, "update");
  TFile *tree2copy = new TFile(plottreefile, "update");
  TTree *t = (TTree*)tree2copy->Get("plotvariables");
  
  t->SetBranchAddress("Nvar", &Nvar);
  t->SetBranchAddress("typplotall", &typplotall);
  t->SetBranchAddress("doplot", &plothistoplotitPI);
  t->SetBranchAddress("h2d", &h2d);
  t->SetBranchAddress("typplot", &typplot);
  t->SetBranchAddress("histoncat", &histoncat);
  t->SetBranchAddress("histoncatindtonames", &histoncatindtonames);
  t->SetBranchAddress("nbinsx", &nbinsx);
  t->SetBranchAddress("nbinsy", &nbinsy);
  t->SetBranchAddress("lowlim", &lowlim);
  t->SetBranchAddress("highlim", &highlim);
  t->SetBranchAddress("lowlim2", &lowlim2);
  t->SetBranchAddress("highlim2", &highlim2);
  t->SetBranchAddress("xaxislabels", &tca_xaxislabels);
  t->SetBranchAddress("yaxislabels", &tca_yaxislabels);
  t->SetBranchAddress("plotvarnames", &tca_plotvarnames);

  TTree *newtree = t->CloneTree(0);
  t->GetEntry(0);    
  newtree->Fill();

  merged->Write();
  merged->Close();

  std::cout << "Copying inputfiles..." << std::endl;
  merged = new TFile(mergedfile, "update");
  TTree* inputfiletree = (TTree*)tree2copy->Get("inputfiles");

  inputfiletree->SetBranchAddress("nfiles", &nfiles);
  inputfiletree->SetBranchAddress("nindfiles", &nindfiles);
  inputfiletree->SetBranchAddress("intlumi", &intlumi_);
  inputfiletree->SetBranchAddress("makeOutputTree", &makeOutputTree);
  inputfiletree->SetBranchAddress("histfilename", &tca_histfilename);
  inputfiletree->SetBranchAddress("itype", &itype);
  inputfiletree->SetBranchAddress("histoind", &histoindfromfiles);
  inputfiletree->SetBranchAddress("infoind", &infoind);
  inputfiletree->SetBranchAddress("histoplotit", &histoplotit);
  inputfiletree->SetBranchAddress("ntot", &ntot);
  inputfiletree->SetBranchAddress("nred", &nred);
  inputfiletree->SetBranchAddress("lumi", &lumi);
  inputfiletree->SetBranchAddress("xsec", &xsec);
  inputfiletree->SetBranchAddress("kfactor", &kfactor);
  inputfiletree->SetBranchAddress("scale", &scale);
  inputfiletree->SetBranchAddress("inshortnames", &tca_inshortnames);
  inputfiletree->SetBranchAddress("infilenames", &tca_infilenames);

  std::cout << "Processed samples: " << std::endl;
  TTree *newinputfiletree = inputfiletree->CloneTree(0);
  inputfiletree->GetEntry(0);    
  newinputfiletree->Fill();

  merged->Write();
  merged->Close();

  merged = new TFile(mergedfile, "update");
  std::cout << "Delete additional trees..." << std::endl;

  std::vector<std::string> temp1 = checkNames(merged, "plotvariables");
  for (unsigned int y=0; y<temp1.size()-1; y++) 
    merged->Delete(temp1[y].c_str());

  std::vector<std::string> temp2 = checkNames(merged, "inputfiles");
  for (unsigned int y=0; y<temp2.size()-1; y++) 
    merged->Delete(temp2[y].c_str());
    
  merged->Write();

  std::cout << "done." << std::endl;
  merged->Close();
}

