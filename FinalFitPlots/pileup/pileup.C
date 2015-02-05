#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TTree.h"

void fillHists(TTree *tree, TH1D *pu1D, TH2D *pu2D){
  
  int pu_n;
  int run;

  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("pu_n", 1);
  tree->SetBranchStatus("run", 1);

  tree->SetBranchAddress("pu_n",&pu_n);
  tree->SetBranchAddress("run",&run);

  for (int i=0; i<tree->GetEntries(); i++){
    std::cout << Form("\t %4.1f%% \r",float(i)*100./tree->GetEntries()) << std::flush; 
    tree->GetEntry(i);
    pu1D->Fill(pu_n);
    pu2D->Fill(pu_n,run);
  }
}

void fillHists(TFile *file, TH1D *pu1D, TH2D *pu2D){
  
  TTree *tree = (TTree*)file->Get("event");
  fillHists(tree,pu1D,pu2D);

}
