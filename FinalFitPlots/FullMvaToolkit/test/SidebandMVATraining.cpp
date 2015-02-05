#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "boost/program_options.hpp"

#include "Optimizations.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TCut.h"
#include "TMath.h"

#ifndef __CINT__
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#endif

using namespace std;
using namespace TMVA;
namespace po = boost::program_options;

// GLOBAL variables
string filename_;
string outfilename_;
string methodname_;
map<string,int> Methods_;
int nSidebands_;
double sidebandWidth_=0.02;
vector<string> allowedMethods_;
vector<string> chosenMethods_;
bool optimize_=false;
bool skipTraining_=false;
bool skipTesting_=false;
bool skipEvaluation_=false;
bool isCutBased_=false;
int trainingMass_=125;
float bdtCut_=-0.78;
float mggMin_=100.;
float mggMax_=180.;

double totalBackgroundInSignal=0;
TFile *input_;
TFile *outFile_;

template <class T>
vector<T> getVecFromString(string name){
  vector<T> result;

  if (typeid(T)!=typeid(float) && typeid(T)!=typeid(double) && typeid(T)!=typeid(int) && typeid(T)!=typeid(string)){
    cout << typeid(T).name() << " is not a valid type. Bailing out " << endl;
    exit(1);
  }

  while (name.size()>0){
    if (name.find(",")==string::npos && name.find("-")==string::npos) {
      result.push_back(boost::lexical_cast<T>(name)); //niether
      name = "";
    }
    else { // contains ","
      string sub = name.substr(0,name.find(","));
      result.push_back(boost::lexical_cast<T>(sub));
      name = name.substr(name.find(",")+1,string::npos);
    }
  }

  return result;
}


void MethodReader(){

  allowedMethods_.push_back("MLP"); allowedMethods_.push_back("BDTG"); allowedMethods_.push_back("FDA_GA"); allowedMethods_.push_back("PDEFoam");
  allowedMethods_.push_back("2DOpt");
  for (vector<string>::iterator itM=chosenMethods_.begin(); itM!=chosenMethods_.end(); itM++){
    bool found=false;
    for (vector<string>::iterator it=allowedMethods_.begin(); it!=allowedMethods_.end(); it++){
      if (*itM==*it) {
        found=true;
        Methods_[*itM]=1;
      }
    }
    if (found==false) cerr << "WARNING - invalid method: " << *itM << endl; 
  }

}

void OptionParser(int argc, char *argv[]){

  po::options_description desc("Allowed options");

  desc.add_options()
    ("help,h",                                                                                  "Show this message")
    ("filename,i", po::value<string>(&filename_),                                               "Input file name")
    ("outfile,o", po::value<string>(&outfilename_)->default_value("SidebandTrainingOut.root"),  "Output file name")
    ("method,m", po::value<string>(&methodname_)->default_value("BDTG"),                        "Training method")
    ("nSidebands,n", po::value<int>(&nSidebands_)->default_value(4),                            "Number of sidebands for training")
    ("optimize,O",                                                                              "Optimize training - takes time")
    ("skipTraining,s",                                                                          "Skip training")
    ("skipTesting,t",                                                                           "Skip testing")
    ("skipEvaluation,e",                                                                        "Skip evaluation")
    ("isCutBased",                                                                              "Train cut based style")
    ("trainingMass", po::value<int>(&trainingMass_)->default_value(trainingMass_),              "Training Mass")
    ("bdtMin", po::value<float>(&bdtCut_)->default_value(bdtCut_),                        "diphton BDT Cut")
    ("mggMin", po::value<float>(&mggMin_)->default_value(mggMin_),                        "Minimum for Mgg")
    ("mggMax", po::value<float>(&mggMax_)->default_value(mggMax_),                        "Maximum for Mgg")
    ;

  po::positional_options_description p;
  p.add("filename",-1);

  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::store(po::command_line_parser(argc,argv).options(desc).positional(p).run(),vm);
  po::notify(vm);

  if (vm.count("help")){ cout << desc << endl; exit(1);}
  if (!vm.count("filename")){ cerr << "WARNING -- A FILENAME MUST BE PROVIDED" << endl; exit(1);}
  if (vm.count("optimize")) optimize_=true;
  if (vm.count("skipTraining")) skipTraining_=true;
  if (vm.count("skipTesting")) skipTesting_=true;
  if (vm.count("skipEvaluation")) skipEvaluation_=true;
  if (vm.count("isCutBased")) isCutBased_=true;

  chosenMethods_ = getVecFromString<string>(methodname_);
  MethodReader();

}

vector<pair<string,string> > getSignalTreeNames(){

  vector<pair<string,string> > treeNames;
  treeNames.push_back(pair<string,string>(Form("full_mva_trees/ggh_m%d_8TeV",trainingMass_),"signal"));
  treeNames.push_back(pair<string,string>(Form("full_mva_trees/vbf_m%d_8TeV",trainingMass_),"signal"));
  treeNames.push_back(pair<string,string>(Form("full_mva_trees/wzh_m%d_8TeV",trainingMass_),"signal"));
  treeNames.push_back(pair<string,string>(Form("full_mva_trees/tth_m%d_8TeV",trainingMass_),"signal"));

/*
  std::vector<int>::iterator it = extraMasses_.begin();
  for (;it!=extraMasses.end();it++){
	treeNames.push_back(
  }
*/
  return treeNames;
}

vector<pair<string,string> > getBackgroundTreeNames(){

  // KEEP ONLY HIGH STATS SAMPLES SO SOME ARE DROPPED
  vector<pair<string,string> > treeNames;
  // fake-fake
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_30_8TeV_ff","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_40_8TeV_ff","background"));
  // prompt-fake
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_30_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_40_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/gjet_20_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/gjet_40_8TeV_pf","background"));
  // prompt-prompt
  //treeNames.push_back(pair<string,string>("full_mva_trees/diphojet_8TeV","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/dipho_Box_25_8TeV","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/dipho_Box_250_8TeV","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/diphojet_8TeV","background"));

  // 7TeV
  /*
  treeNames.push_back(pair<string,string>("full_mva_trees/dipho_Box_25_7TeV","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/dipho_Box_250_7TeV","background"));

  treeNames.push_back(pair<string,string>("full_mva_trees/diphojet_7TeV","background"));
  treeNames.push_back(pair<string,string>("full_mva_trees/gjet_20_7TeV_pf","background"));
  */
  // These for 8 TeV 
  treeNames.push_back(pair<string,string>("full_mva_trees/diphojet_sherpa_8TeV","background"));
  treeNames.push_back(pair<string,string>("full_mva_trees/gjet_20_8TeV_pf","background"));
  treeNames.push_back(pair<string,string>("full_mva_trees/gjet_40_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_30_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_30_8TeV_ff","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_40_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("full_mva_trees/qcd_40_8TeV_ff","background"));

  return treeNames;
}

void makeTrainingTree(TTree *tree, vector<pair<string,string> > treeNames, bool isSignal){
  
  float mass;
  float weight;
  float deltaMoverM;
  float bdtoutput;
  double lead_eta;
  double sublead_eta;
  double lead_r9;
  double sublead_r9;

  tree->Branch("deltaMoverM",&deltaMoverM,"deltaMoverM/F");
  tree->Branch("weight",&weight,"weight/F");
  if (isCutBased_){
    tree->Branch("lead_eta",&lead_eta,"lead_eta/D");
    tree->Branch("sublead_eta",&sublead_eta,"sublead_eta/D");
    tree->Branch("lead_r9",&lead_r9,"lead_r9/D");
    tree->Branch("sublead_r9",&sublead_r9,"sublead_r9/D");
  }
  else {
    tree->Branch("bdtoutput",&bdtoutput,"bdtoutput/F");
  }

  TH1F *check = new TH1F("check","check",100,0,100);
  for (vector<pair<string,string> >::iterator it=treeNames.begin(); it!=treeNames.end(); it++){
    TTree *thisTree = (TTree*)input_->Get(it->first.c_str());
    //thisTree->Draw("1>>check","weight*(weight>0)","goff");
    cout << Form("Using  tree %15s with entries %8d and sum of weights %4.4f",thisTree->GetName(),int(thisTree->GetEntries()),check->GetSumOfWeights()) << endl;
    check->Reset();
    thisTree->SetBranchAddress("mass",&mass);
    thisTree->SetBranchAddress("weight",&weight);
    if (isCutBased_){
      thisTree->SetBranchAddress("lead_eta",&lead_eta);
      thisTree->SetBranchAddress("sublead_eta",&sublead_eta);
      thisTree->SetBranchAddress("lead_r9",&lead_r9);
      thisTree->SetBranchAddress("sublead_r9",&sublead_r9);
    }
    else {
      thisTree->SetBranchAddress("bdtoutput",&bdtoutput);
    }
    for (int e=0; e<thisTree->GetEntries(); e++){
      thisTree->GetEntry(e);
      // signal is easy just fill signal region
      if (isSignal) {
        deltaMoverM = (mass-trainingMass_)/trainingMass_;
      }
      // for background want to sum up a few sidebands
      else {
        // if in signal region add and move on
        deltaMoverM = (mass-trainingMass_)/trainingMass_;
        if (bdtoutput>bdtCut_ and TMath::Abs(deltaMoverM) < sidebandWidth_) totalBackgroundInSignal+=(double)weight;
        if (TMath::Abs(deltaMoverM)>sidebandWidth_){
          // else loop sidebands
          for (int i=1; i<=nSidebands_; i++){
            double hypothesisModifierLow = (1.-sidebandWidth_)/(1.+sidebandWidth_);
            double sidebandCenterLow = trainingMass_*hypothesisModifierLow*TMath::Power(hypothesisModifierLow,i-1);
            double hypothesisModifierHigh = (1.+sidebandWidth_)/(1.-sidebandWidth_);
            double sidebandCenterHigh = trainingMass_*hypothesisModifierHigh*TMath::Power(hypothesisModifierHigh,i-1);
            deltaMoverM = (mass-sidebandCenterLow)/sidebandCenterLow;
            if (TMath::Abs(deltaMoverM)<=sidebandWidth_) break;
            deltaMoverM = (mass-sidebandCenterHigh)/sidebandCenterHigh;
            if (TMath::Abs(deltaMoverM)<=sidebandWidth_) break;
          }
        }
      }
      tree->Fill();
    }
  }
  tree->Draw("1>>check","weight*(weight>0)","goff");
  cout << Form("Filled tree %15s with entries %8d and sum of weights %4.4f",tree->GetName(),int(tree->GetEntries()),check->GetSumOfWeights()) << endl;
  check->Reset();
  delete check;
}

void fillOneDHists(TTree *tr, TH1F* h, TH2F *map){
  h->Sumw2();
  for (int i=0;i<h->GetNbinsX();i++){
	h->GetXaxis()->SetBinLabel(i+1,Form("Bin %d",i+1));
  }
  float dmom,bdt,weight;
  tr->SetBranchAddress("deltaMoverM",&dmom);
  tr->SetBranchAddress("bdtoutput",&bdt);
  tr->SetBranchAddress("weight",&weight);

  for (int j=0;j<tr->GetEntries();j++){
    tr->GetEntry(j);
    if (bdt < bdtCut_) continue;
    if (fabs(dmom)>sidebandWidth_) continue;
    int bin = map->FindBin(bdt,dmom);
    //std::cout << " Event - "<< bdt << ", " << dmom << " -> " << map->GetBinContent(bin) <<std::endl;
    h->Fill(map->GetBinContent(bin),weight);
  }
 
}

void fillTwoDHists(TTree *tr, TH2F* h){
  h->Sumw2(); 
  float dmom,bdt,weight;
  tr->SetBranchAddress("deltaMoverM",&dmom);
  tr->SetBranchAddress("bdtoutput",&bdt);
  tr->SetBranchAddress("weight",&weight);

  for (int j=0;j<tr->GetEntries();j++){
    tr->GetEntry(j);
    h->Fill(bdt,dmom,weight);
  }
}

void run2DOptimization(TFile *outFile_,TTree *signalTree_, TTree *backgroundTree_){

 signalTree_->Print("v");
 gROOT->SetBatch(0);
 int nBins_dmom = 50;
 int nBins_bdt  = 200;
 // Step 1, make Signal and Background 2D histograms
 TH2F *hsig =new TH2F("hsig","hsig",nBins_bdt,bdtCut_,1,nBins_dmom,-1*sidebandWidth_,sidebandWidth_);
 TH2F *hbkg =new TH2F("hbkg","hbkg",nBins_bdt,bdtCut_,1,nBins_dmom,-1*sidebandWidth_,sidebandWidth_);
 fillTwoDHists(signalTree_,hsig) ;
 fillTwoDHists(backgroundTree_,hbkg) ;

 // Make sure bkg is normalized to bkg yield in signal
 std::cout << "BKG Integral from all sidebands " << hbkg->Integral() <<std::endl; 
 std::cout << "BKG Integral in Signal Window " << totalBackgroundInSignal <<std::endl; 
 hbkg->Scale(totalBackgroundInSignal/hbkg->Integral());

// std::cout << "Producing 2D Histogram " << Form("deltaMoverM:bdtoutput>>hsig(%d,%f,1,%d,%.3f,%.3f)",nBins_bdt,bdtCut_,nBins_dmom,-1*sidebandWidth_,sidebandWidth_) << std::endl; 
// signalTree_->Draw("deltaMoverM:bdtoutput>>hsig","weight","");
// std::cout << "Producing 2D Histogram " << Form("deltaMoverM:bdtoutput>>hbkg(%d,%f,1,%d,%.3f,%.3f)",nBins_bdt,bdtCut_,nBins_dmom,-1*sidebandWidth_,sidebandWidth_) << std::endl; 
// backgroundTree_->Draw("deltaMoverM:bdtoutput>>hbkg","weight","");

// TH2F *hsig =(TH2F*) gROOT->FindObject("hsig")	;
// TH2F *hbkg =(TH2F*) gROOT->FindObject("hbkg")	;

 // Have to do this :(
 //hsig->Smooth(1,"k5b");
 //hbkg->Smooth(1,"k5b");

 // Clone the originl hists, saved to File
 TH1F *hsig_o = (TH1F*)hsig->Clone("hsig_raw") ;
 TH1F *hbkg_o = (TH1F*)hbkg->Clone("hbkg_raw") ;
	
 // Step 2, create an optimization class

 Optimizations *optimizer = new Optimizations(hsig,hbkg);
 optimizer->setMaxBins(12);
 optimizer->smoothHistograms(0.002,0.005,1);
 optimizer->runOptimization();
  
 // Thats it so nw get the outputs
 TH2F *categoryMap = (TH2F*) optimizer->getCategoryMap();
 TH2F *bkg = (TH2F*) optimizer->getBackgroundTarget();
 TH2F *signal = (TH2F*) optimizer->getSignalTarget();
 TGraph *optGraph = (TGraph*)optimizer->getOptimizationGraph();
 TH1F *starget_hist = (TH1F*)optimizer->getTargetS1D();
 TH1F *btarget_hist = (TH1F*)optimizer->getTargetB1D();


 int nFinalBins = optimizer->getFinalNumberOfBins();

 // Step 3, save output and print ranges also S/B
 double *boundaries = new double[nFinalBins+1] ;
 boundaries[0] = -1.;
 for (int b = 1 ; b < nFinalBins ; b++){
  boundaries[b] = (double)b/nFinalBins;
 }
 boundaries[nFinalBins]=1.;

 TH1F *binedgesMap = new TH1F("Bin_Edges","Bin Boundaries",nFinalBins,boundaries);
 for (int b = 1 ; b <= nFinalBins ; b++){
	binedgesMap->SetBinContent(b,b);
 }
 delete boundaries;
 
 std::cout << "Final Number Of Bins -- " << nFinalBins << std::endl;
 //std::cout << "-1";
 //for (int b = 1 ; b < nFinalBins ; b++){
	
//	std::cout << "," << (double)b/nFinalBins;
// }
// std::cout << ",1" <<std::endl;
	
 outFile_->cd();
 categoryMap->Write();	
 starget_hist->SetLineColor(2);
 starget_hist->Scale(1./starget_hist->Integral());
 btarget_hist->Scale(1./btarget_hist->Integral());
 starget_hist->Write(); 
 btarget_hist->Write(); 
 signal->Write();
 bkg->Write();
 hsig_o->SetLineColor(2);hsig_o->Write();
 hbkg_o->SetLineColor(2);hbkg_o->Write();
 // create ratio hists too
 TH1F* hsig_rat = (TH1F*)hsig->Clone();
 TH1F* hbkg_rat = (TH1F*)hbkg->Clone();
 TGraph *roc = (TGraph*)optimizer->getRocCurve();
 hsig_rat->SetName("hsig_ratio");
 hbkg_rat->SetName("hbkg_ratio");
 hsig_rat->Divide(hsig_o);
 hbkg_rat->Divide(hbkg_o);
 hsig_rat->Write();
 hbkg_rat->Write();
 binedgesMap->Write();
 optGraph->Write();
 roc->Write(); 

 // Finally make some S/B hists
 TH1F *sfinal_hist = new TH1F("s_raw_final","",nFinalBins,0,1);
 TH1F *bfinal_hist = new TH1F("b_raw_final","",nFinalBins,0,1);

 fillOneDHists(signalTree_,sfinal_hist,categoryMap);
 fillOneDHists(backgroundTree_,bfinal_hist,categoryMap);
 sfinal_hist->SetLineColor(2);
 bfinal_hist->SetLineColor(4);

 sfinal_hist->Write();
 bfinal_hist->Write();
}

  
void runTMVATraining(TFile *outFile_,TTree *signalTree, TTree *backgroundTree){


  TMVA::Tools::Instance();
  TMVA::Factory *factory = new TMVA::Factory("TMVA_SidebandMVA", outFile_, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
  
  factory->AddVariable("deltaMoverM",  "#DeltaM / M_{H}", 'F');
  if (isCutBased_){
    factory->AddVariable("lead_eta","#eta_{1}", 'D');
    factory->AddVariable("sublead_eta","#eta_{2}", 'D');
    factory->AddVariable("lead_r9","#r9_{1}", 'D');
    factory->AddVariable("sublead_r9","#r9_{2}", 'D');
    //factory->AddVariable("lead_phi","#phi_{1}", 'D');
    //factory->AddVariable("sublead_phi","#phi_{2}", 'D');
  }
  else {
    factory->AddVariable("bdtoutput", "Diphoton BDT", 'F');
  }


  factory->AddSignalTree(signalTree);
  factory->AddBackgroundTree(backgroundTree);

  /*
  for (vector<pair<string,string> >::iterator sig=signalTrees.begin(); sig!=signalTrees.end(); sig++){
    TTree *temp = (TTree*)input->Get((sig->first).c_str());
    if (temp!=NULL) {
      if (temp->GetEntries()==0) continue;
      temp->Draw("1>>check","weight*(weight>0)","goff");
      cout << Form("Using tree %15s with entries %8d and sum of weights %4.4f",temp->GetName(),int(temp->GetEntries()),check->GetSumOfWeights()) << endl;
      check->Reset();
      trees[sig->first] = temp;
      factory->AddSignalTree(trees[sig->first],signalWeight);
    }
    else { cerr << "WARNING -- invalid pointer -- " << sig->first << endl; exit(1); }
  }

  for (vector<pair<string,string> >::iterator bkg=backgroundTrees.begin(); bkg!=backgroundTrees.end(); bkg++){
    TTree *temp = (TTree*)input->Get((bkg->first).c_str());
    if (temp!=NULL) {
      if (temp->GetEntries()==0) continue;
      temp->Draw("1>>check","weight*(weight>0)","goff");
      cout << Form("Using tree %15s with entries %8d and sum of weights %4.4f",temp->GetName(),int(temp->GetEntries()),check->GetSumOfWeights()) << endl;
      check->Reset();
      trees[bkg->first] = temp;
      factory->AddBackgroundTree(trees[bkg->first],backgroundWeight);
    }
    else { cerr << "WARNING -- invalid pointer -- " << bkg->first << endl; exit(1); }
  }
  */
  
  // Set individual event weights
  factory->SetBackgroundWeightExpression("weight");
  factory->SetSignalWeightExpression("weight");

  // Apply cuts on samples
  /*
  TString sigCutString = Form("TMath::Abs((mass-%d)/%d)<=0.02",trainingMass_,trainingMass_);
  // figure out high and low sideband boundary
  double hypothesisModifierLow = (1.-sidebandWidth_)/(1.+sidebandWidth_);
  double hypothesisModifierHigh = (1.+sidebandWidth_)/(1.-sidebandWidth_);
  double sidebandCenterLow = trainingMass_*hypothesisModifierLow*TMath::Power(hypothesisModifierLow,nSidebands_);
  double sidebandCenterHigh = trainingMass_*hypothesisModifierHigh*TMath::Power(hypothesisModifierHigh,nSidebands_);
  double sidebandBoundaryLow = sidebandCenterLow*(1-sidebandWidth_);
  double sidebandBoundaryHigh = sidebandCenterHigh*(1+sidebandWidth_);
  TString bkgCutString = Form("mass>=%3.1f && mass<=%3.1f",sidebandBoundaryLow,sidebandBoundaryHigh);
  */
  
  TString sigCutString = Form("TMath::Abs(deltaMoverM)<=%f",sidebandWidth_);
  TString bkgCutString = Form("TMath::Abs(deltaMoverM)<=%f",sidebandWidth_);
  if (!isCutBased_) {
    sigCutString += Form(" && bdtoutput>=%f",bdtCut_);
    bkgCutString += Form(" && bdtoutput>=%f",bdtCut_);
  }
  TCut sigCut(sigCutString);
  TCut bkgCut(bkgCutString);

  factory->PrepareTrainingAndTestTree( sigCut, bkgCut, "SplitMode=Random:NormMode=NumEvents:!V" );

  if (Methods_["BDTG"]) // gradient boosted decision trees
    factory->BookMethod( TMVA::Types::kBDT, "BDTgradMIT", "!H:!V:NTrees=500:MaxDepth=3:BoostType=Grad:Shrinkage=1.0:UseBaggedGrad:GradBaggingFraction=0.50:nCuts=20:NNodesMax=8:IgnoreNegWeights"); 
  if (Methods_["MLP"]) // neural network
    factory->BookMethod( TMVA::Types::kMLP, "MLP", "!H:!V:NeuronType=tanh:NCycles=300:HiddenLayers=N+5,5:TestRate=5:EstimatorType=MSE");   
  if (Methods_["FDA_GA"]) // functional discriminant with GA minimizer
    factory->BookMethod( TMVA::Types::kFDA, "FDA_GA", "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );
  if (Methods_["PDEFoam"]) // PDE-Foam approach
    factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", "!H:!V:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );
  
  // To do multiple tests of one BDT use e.g
  if (Methods_["SCAN"]){
    int nTrees[3] = {50,100,200};
    int maxDepth[3] = {3,5,10};
    float shrinkage[3] = {0.1,0.5,1.};
    float bagFrac[2] = {0.6,1.};
    //float shrinkage[3] = {0.1,0.5,1.};
    //float bagFrac[3] = {0.1,0.5,1.};
    for (int n=0; n<3; n++){
      for (int d=0; d<3;d++){
        for (int s=0; s<3; s++){
          for (int b=0; b<2; b++){
            factory->BookMethod( TMVA::Types::kBDT, 
                    Form("BDTG_Test_nTrees%d_mDepth%d_shr%1.2f_bf%1.2f",nTrees[n],maxDepth[d],shrinkage[s],bagFrac[b]), 
                    Form("!H:!V:NTrees=%d:MaxDepth=%d:BoostType=Grad:Shrinkage=%1.2f:UseBaggedGrad:GradBaggingFraction=%1.2f:nCuts=20:NNodesMax=8:IgnoreNegWeights",nTrees[n],maxDepth[d],shrinkage[s],bagFrac[b])); 
          }
        }
      }
    }
  }

  if (optimize_) factory->OptimizeAllMethodsForClassification();
  // Train MVAs using the set of training events
  if (!skipTraining_) factory->TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events
  if (!skipTesting_) factory->TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  if (!skipEvaluation_) factory->EvaluateAllMethods();

  // Save the output
  cout << "==> Wrote root file: " << outFile_->GetName() << endl; 
  cout << "==> TMVAClassification is done!" << endl;
  cout << "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << endl;

}

int main (int argc, char *argv[]){

  OptionParser(argc,argv);

  bool doTMVA = true;

  std::cout << std::endl;
  std::cout << "Start Sideband Training ==>" << std::endl;
  std::cout << "Running methods: " << endl;
  for (map<string,int>::iterator it=Methods_.begin(); it!=Methods_.end(); it++){
    if (it->second==1) {
	cout << "\t" << it->first << endl;
	if (it->first == "2DOpt") doTMVA = false; 
    }
  }

  input_ = TFile::Open(filename_.c_str());
  if (!input_) {
    cerr << "ERROR: could not open data file" << endl;
    exit(1);
  }

  outFile_ = new TFile(outfilename_.c_str(),"RECREATE");
  // Get all the Trees and assign them to the factory by their type (signal,background)
  vector<pair<string,string> > signalTrees = getSignalTreeNames();
  vector<pair<string,string> > backgroundTrees = getBackgroundTreeNames();

  TTree *signalTree = new TTree("signalTree","signalTree");
  TTree *backgroundTree = new TTree("backgroundTree","backgroundTree");

  makeTrainingTree(signalTree,signalTrees,true);
  makeTrainingTree(backgroundTree,backgroundTrees,false);
 
  // TMVA training or 2D optimization training
  if (doTMVA){
	runTMVATraining(outFile_,signalTree,backgroundTree);
  } else {
	run2DOptimization(outFile_,signalTree,backgroundTree);
  }
 
  input_->Close();
  outFile_->Close();
  delete outFile_;
  delete input_;

  return 0;
}
