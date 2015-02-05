#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;

void writeHists(TFile *outFile, map<string,TH1F*> hists){
  
  outFile->cd();
  for (map<string,TH1F*>::iterator it=hists.begin(); it!=hists.end(); it++){
    it->second->Write();
  }
}

void write2DHists(TFile *outFile, map<string,TH2F*> hists){
  
  outFile->cd();
  for (map<string,TH2F*>::iterator it=hists.begin(); it!=hists.end(); it++){
    it->second->Write();
  }
}

void fillHist(string truth, string name, double val, map<string,TH1F*> &hists, int bins=30, double low=-3., double high=3.){
  
  if (hists.find(truth)==hists.end()) {
    hists.insert(pair<string,TH1F*>(truth,new TH1F(Form("%s_mu%s",truth.c_str(),name.c_str()),Form("%s_mu%s",truth.c_str(),name.c_str()),bins,low,high)));
  }
  if (val<999) hists[truth]->Fill(val);
}

void fillPullHist(string truth, string name, double expmean, double mu, double muerr, map<string,TH1F*> &hists){
  
  if (hists.find(truth)==hists.end()){
    hists.insert(pair<string,TH1F*>(truth, new TH1F(Form("%s_mu%s",truth.c_str(),name.c_str()),Form("%s_mu%s",truth.c_str(),name.c_str()),50,-5.,5.)));
  }
  if (mu<999 && muerr<999) hists[truth]->Fill((mu-expmean)/muerr);
}

void fill2DHist(string truth, string name, double xval, double yval, map<string,TH2F*> &hists, int xbins=40, double xlow=0.5, double xhigh=1., int ybins=40, double ylow=0.5, double yhigh=1.){
  
  if (hists.find(truth)==hists.end()) {
    hists.insert(pair<string,TH2F*>(truth,new TH2F(Form("%s_mu%s",truth.c_str(),name.c_str()),Form("%s_mu%s",truth.c_str(),name.c_str()),xbins,xlow,xhigh,ybins,ylow,yhigh)));
  }
  if (xval<999 && yval<999) hists[truth]->Fill(xval,yval);
}

void make_bias_hists(string filename, string outfilename, int cat, float expmean=0., int meanBins=30, float meanL=-3., float meanH=3., int resBins=50, float resL=0.5, float resH=1.){
  
  string treename = Form("muTree_cat%d",cat);

  TFile *inFile = TFile::Open(filename.c_str());
  TChain *chain = (TChain*)inFile->Get(treename.c_str());

  TFile *outFile = new TFile(outfilename.c_str(),"RECREATE");

  map<string,TH1F*> muFabMap;
  map<string,TH1F*> muPaulMap;
  map<string,TH1F*> muChi2Map;
  map<string,TH1F*> muAICMap;
  map<string,TH1F*> muFabErrMap;
  map<string,TH1F*> muPaulErrMap;
  map<string,TH1F*> muChi2ErrMap;
  map<string,TH1F*> muAICErrMap;
  map<string,TH1F*> muFabPullMap;
  map<string,TH1F*> muPaulPullMap;
  map<string,TH1F*> muChi2PullMap;
  map<string,TH1F*> muAICPullMap;
  map<string,TH2F*> muFabPaulErrMap;
  map<string,TH2F*> muFabChi2ErrMap;
  map<string,TH2F*> muFabAICErrMap;
  
  int jobn;
  int toyn;
  vector<string> *truthModel=0;
  vector<double> *muFab=0;
  vector<double> *muPaul=0;
  vector<double> *muChi2=0;
  vector<double> *muAIC=0;
  vector<double> *muFabErr=0;
  vector<double> *muPaulErr=0;
  vector<double> *muChi2Err=0;
  vector<double> *muAICErr=0;
  chain->SetBranchAddress("jobn",&jobn);
  chain->SetBranchAddress("toyn",&toyn);
  chain->SetBranchAddress("truthModel",&truthModel);
  chain->SetBranchAddress("muFab",&muFab);
  chain->SetBranchAddress("muPaul",&muPaul);
  chain->SetBranchAddress("muChi2",&muChi2);
  chain->SetBranchAddress("muAIC",&muAIC);
  chain->SetBranchAddress("muFabErr",&muFabErr);
  chain->SetBranchAddress("muPaulErr",&muPaulErr);
  chain->SetBranchAddress("muChi2Err",&muChi2Err);
  chain->SetBranchAddress("muAICErr",&muAICErr);

  for (int e=0; e<chain->GetEntries(); e++){
    chain->GetEntry(e);
    //cout << jobn << " " << toyn << " " << truthModel->size() << " " << muFab->size() << " " << muPaul->size() << endl;
    
    for (unsigned int el=0; el<truthModel->size(); el++){
      string truth = truthModel->at(el);
      truth = truth.substr(0,truth.find("_job"));
      cout << truth << endl;
      fillHist(truth,"Fab",muFab->at(el),muFabMap,meanBins,meanL,meanH);
      fillHist(truth,"Paul",muPaul->at(el),muPaulMap,meanBins,meanL,meanH);
      fillHist(truth,"Chi2",muChi2->at(el),muChi2Map,meanBins,meanL,meanH);
      fillHist(truth,"AIC",muAIC->at(el),muAICMap,meanBins,meanL,meanH);

      fillHist(truth,"FabErr",muFabErr->at(el),muFabErrMap,resBins,resL,resH);
      fillHist(truth,"PaulErr",muPaulErr->at(el),muPaulErrMap,resBins,resL,resH);
      fillHist(truth,"Chi2Err",muChi2Err->at(el),muChi2ErrMap,resBins,resL,resH);
      fillHist(truth,"AICErr",muAICErr->at(el),muAICErrMap,resBins,resL,resH);

      fillPullHist(truth,"FabPull",expmean,muFab->at(el),muFabErr->at(el),muFabPullMap);
      fillPullHist(truth,"PaulPull",expmean,muPaul->at(el),muPaulErr->at(el),muPaulPullMap);
      fillPullHist(truth,"Chi2Pull",expmean,muChi2->at(el),muChi2Err->at(el),muChi2PullMap);
      fillPullHist(truth,"AICPull",expmean,muAIC->at(el),muAICErr->at(el),muAICPullMap);

      fill2DHist(truth,"FabPaulErr",muFabErr->at(el),muPaulErr->at(el),muFabPaulErrMap,resBins,resL,resH,resBins,resL,resH);
      fill2DHist(truth,"FabChi2Err",muFabErr->at(el),muChi2Err->at(el),muFabChi2ErrMap,resBins,resL,resH,resBins,resL,resH);
      fill2DHist(truth,"FabAICErr",muFabErr->at(el),muAICErr->at(el),muFabAICErrMap,resBins,resL,resH,resBins,resL,resH);
    }
  }

  writeHists(outFile,muFabMap);
  writeHists(outFile,muPaulMap);
  writeHists(outFile,muChi2Map);
  writeHists(outFile,muAICMap);
  writeHists(outFile,muFabErrMap);
  writeHists(outFile,muPaulErrMap);
  writeHists(outFile,muChi2ErrMap);
  writeHists(outFile,muAICErrMap);
  writeHists(outFile,muFabPullMap);
  writeHists(outFile,muPaulPullMap);
  writeHists(outFile,muChi2PullMap);
  writeHists(outFile,muAICPullMap);
  write2DHists(outFile,muFabPaulErrMap);
  write2DHists(outFile,muFabChi2ErrMap);
  write2DHists(outFile,muFabAICErrMap);

  inFile->Close();
  outFile->Close();

}
