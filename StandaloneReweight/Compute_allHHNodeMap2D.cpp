#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#include "CfgManager.h"
#include "CfgManagerT.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TString.h"

using namespace std;

vector<string> * GetListOfTrees(const string &oldfilename);

int main(int argc, char** argv)
{
  gStyle->SetOptStat(0);
  CfgManager conf(argv[1]);
  vector<string> infilenames = conf.GetOpt<vector<string> >("Input.filename");
  
  //Get the name of the trees in the old file
  vector<string> *treenames = GetListOfTrees(infilenames.at(0));
  gDirectory->cd(0);
  
  //Create the input TChains
  map<string,TChain*> ingenchain_map;
  for(auto treename : *treenames)
  {
    ingenchain_map[treename]  = new TChain(("gen_"+treename).c_str(),"");
    for(auto filename : infilenames)
      ingenchain_map[treename] -> Add((filename+"/genDiphotonDumper/trees/"+treename).c_str());
  }

  //open the outfiles and define the outhisto
  TString oldfilename = conf.GetOpt<string> ("DoubleHReweight.weightsFile");
  TString updatingfilename(oldfilename);
  updatingfilename.ReplaceAll(".root","_UPDATED.root");
  system(Form("cp %s %s",oldfilename.Data(),updatingfilename.Data())); 

  TFile *updatingfile = new TFile(updatingfilename,"UPDATE");
  TH2F* hMap = (TH2F*) updatingfile->Get("allHHNodeMap2D");
  TH1F* hMap1D = (TH1F*) updatingfile->Get("allHHNodeMap1D");
  TH2F* hMapOriginal = new TH2F(*(hMap));
  TH1F* hMap1DOriginal = new TH1F(*(hMap1D));
  hMap->Reset();
  hMap1D->Reset();

  int NUMev=0;
  for(auto &ingenchain_element : ingenchain_map)
  {
    string treename = ingenchain_element.first;
    TChain *ingenchain = ingenchain_element.second;
    cout<<"Starting to run on "<<treename<<endl;

    //branch the gen tree
    cout<<"branch ingenchain"<<endl;
    float genmHH;
    float gencosthetaHH;
    ingenchain->SetBranchAddress("mhh",&genmHH);
    ingenchain->SetBranchAddress("absCosThetaStar_CS",&gencosthetaHH);

    //Precaution to avoid crashes
    if(ingenchain->GetEntries() == 0)
    {
      cout<<"[ERROR]: tree "<<treename<<" is empty! --> switch to next tree"<<endl;
      continue;
    }

    //loop over events
    cout<<"Reading tree "<<treename<<endl;
    Long64_t nentries = ingenchain->GetEntries();
    cout<<nentries<<" entries"<<endl;
    for(long ientry=0;ientry<nentries; ++ientry)
    {
      ingenchain->GetEntry(ientry);
      if(ientry%5000==0)
	cout<<"reading entry "<<ientry<<"\r"<<std::flush;
      ++NUMev;      
      //fill output histo 
      hMap->Fill(genmHH,gencosthetaHH);
      hMap1D->Fill(genmHH);
    }
    cout<<"done"<<endl;
    cout<<endl;
  }
  cout<<endl<<"run over "<<NUMev<<" events"<<endl;

  //just check differences
  hMapOriginal->Divide(hMap);
  hMap1DOriginal->Divide(hMap1D);
  TCanvas c1;
  hMapOriginal->GetXaxis()->SetRangeUser(250,1300);
  hMapOriginal->SetTitle("old/new hMap ratio");
  hMapOriginal->GetXaxis()->SetTitle("mHH");
  hMapOriginal->GetYaxis()->SetTitle("costhetastarHH");
  hMap1DOriginal->GetXaxis()->SetRangeUser(250,1300);
  hMap1DOriginal->GetYaxis()->SetRangeUser(0.8,1.2);
  hMap1DOriginal->GetXaxis()->SetTitle("mHH");
  hMap1DOriginal->GetYaxis()->SetTitle("old/new hMap1D ratio");
  hMapOriginal->Draw("COLZ");
  c1.Print("hMapRatio.png");
  c1.SetGridy();
  hMap1DOriginal->Draw();
  c1.Print("hMap1DRatio.png");

  //write output
  updatingfile->cd();
  hMap->Write(hMap->GetName(),TObject::kOverwrite);
  hMap1D->Write(hMap1D->GetName(),TObject::kOverwrite);
  cout<<"closing updated file"<<endl;
  updatingfile->Close();
  
  //delete and terminate
  cout<<"deleting ingenchains"<<endl;
  for(auto &ingenchain_element : ingenchain_map)
    delete ingenchain_element.second;

  return 0;
}

vector<string> * GetListOfTrees(const string &oldfilename)
{
  TFile *oldfile = new TFile(oldfilename.c_str(),"READ");
  oldfile->cd("genDiphotonDumper/trees");
  TIter next(gDirectory->GetListOfKeys());
  TKey *key;
  vector<string> *treenames = new vector<string>;
  while ((key = (TKey*)next())) 
  {
    key->Print();
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TTree")) continue;
    //branching old tree
    TTree *oldtree = (TTree*)key->ReadObj();
    treenames->push_back(oldtree->GetName());
  }
  oldfile->Close();
  gDirectory->cd(0);
  return treenames;

}
