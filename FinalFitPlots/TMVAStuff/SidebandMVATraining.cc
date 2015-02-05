#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "boost/program_options.hpp"

#include "TDirectory.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TCut.h"

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
vector<string> allowedMethods_;
vector<string> chosenMethods_;
bool optimize_=false;
bool skipTraining_=false;
bool skipTesting_=false;
bool skipEvaluation_=false;

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
    ("help,h",                                                                              "Show this message")
    ("filename,i", po::value<string>(&filename_),                                           "Input file name")
    ("outfile,o", po::value<string>(&outfilename_)->default_value("MKMulticlassOut.root"),  "Output file name")
    ("method,m", po::value<string>(&methodname_)->default_value("BDTG"),                    "Training method")
    ("optimize,O",                                                                          "Optimize training - takes time")
    ("skipTraining,s",                                                                      "Skip training")
    ("skipTesting,t",                                                                       "Skip testing")
    ("skipEvaluation,e",                                                                    "Skip evaluation")
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

  chosenMethods_ = getVecFromString<string>(methodname_);
  MethodReader();

}

vector<pair<string,string> > getSignalTreeNames(){

  vector<pair<string,string> > treeNames;
  treeNames.push_back(pair<string,string>("ggh_m124_pu2012","signal"));
  treeNames.push_back(pair<string,string>("vbf_m124_pu2012","signal"));
  treeNames.push_back(pair<string,string>("wzh_m124_pu2012","signal"));
  treeNames.push_back(pair<string,string>("tth_m124_pu2012","signal"));
  return treeNames;
}

vector<pair<string,string> > getBackgroundTreeNames(){

  vector<pair<string,string> > treeNames;
  //treeNames.push_back(pair<string,string>("qcd_30_8TeV_ff","background"));
  //treeNames.push_back(pair<string,string>("qcd_30_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("qcd_30_8TeV_pp","background"));
  //treeNames.push_back(pair<string,string>("qcd_40_8TeV_ff","background"));
  //treeNames.push_back(pair<string,string>("qcd_40_8TeV_pf","background"));
  //treeNames.push_back(pair<string,string>("qcd_40_8TeV_pp","background"));
  treeNames.push_back(pair<string,string>("gjet_20_8TeV_ff","background"));
  treeNames.push_back(pair<string,string>("gjet_20_8TeV_pf","background"));
  treeNames.push_back(pair<string,string>("gjet_20_8TeV_pp","background"));
  treeNames.push_back(pair<string,string>("gjet_40_8TeV_ff","background"));
  treeNames.push_back(pair<string,string>("gjet_40_8TeV_pf","background"));
  treeNames.push_back(pair<string,string>("gjet_40_8TeV_pp","background"));
  treeNames.push_back(pair<string,string>("diphojet_8TeV","background"));
  treeNames.push_back(pair<string,string>("dipho_Box_10_8TeV","background"));
  treeNames.push_back(pair<string,string>("dipho_Box_25_8TeV","background"));
  treeNames.push_back(pair<string,string>("dipho_Box_250_8TeV","background"));
  //treeNames.push_back(pair<string,string>("dipho_Born_10_8TeV","background"));
  //treeNames.push_back(pair<string,string>("dipho_Born_25_8TeV","background"));
  //treeNames.push_back(pair<string,string>("dipho_Born_250_8TeV","background"));
  //treeNames.push_back(pair<string,string>("dyjetsll_50_8TeV","background"));

  return treeNames;
}

int main (int argc, char *argv[]){

  OptionParser(argc,argv);

  TMVA::Tools::Instance();

  std::cout << std::endl;
  std::cout << "==> Start TMVAMulticlass" << std::endl;
  std::cout << "Running methods: " << endl;
  for (map<string,int>::iterator it=Methods_.begin(); it!=Methods_.end(); it++){
    if (it->second==1) cout << "\t" << it->first << endl;
  }

  TFile *input = TFile::Open(filename_.c_str());
  if (!input) {
    cerr << "ERROR: could not open data file" << endl;
    exit(1);
  }

  vector<string> trackOfFilesWritten;

  TFile *outFile = new TFile(outfilename_.c_str(),"RECREATE");

  TMVA::Factory *factory = new TMVA::Factory("TMVA_SidebandMVA", outFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
      
  factory->AddVariable("bdtoutput", "Diphoton BDT", 'F');
  factory->AddVariable("deltaMoverM",  "#DeltaM / M_{H}", 'F');

  // Get all the Trees and assign them to the factory by their type (signal,background)
  vector<pair<string,string> > signalTrees = getSignalTreeNames();
  vector<pair<string,string> > backgroundTrees = getBackgroundTreeNames();
  map<string,TTree*> trees;
  // set global weight for signal and background
  float signalWeight=1.0;
  float backgroundWeight=1.0;

  for (vector<pair<string,string> >::iterator sig=signalTrees.begin(); sig!=signalTrees.end(); sig++){
    TTree *temp = (TTree*)input->Get((sig->first).c_str());
    if (temp!=NULL) {
      if (temp->GetEntries()==0) continue;
      trees[sig->first] = temp;
      factory->AddSignalTree(trees[sig->first],signalWeight);
    }
    else { cerr << "WARNING -- invalid pointer -- " << sig->first << endl; exit(1); }
  }

  for (vector<pair<string,string> >::iterator bkg=backgroundTrees.begin(); bkg!=backgroundTrees.end(); bkg++){
    TTree *temp = (TTree*)input->Get((bkg->first).c_str());
    if (temp!=NULL) {
      if (temp->GetEntries()==0) continue;
      trees[bkg->first] = temp;
      factory->AddBackgroundTree(trees[bkg->first],backgroundWeight);
    }
    else { cerr << "WARNING -- invalid pointer -- " << bkg->first << endl; exit(1); }
  }
  
  // Set individual event weights
  factory->SetBackgroundWeightExpression("weight");
  factory->SetSignalWeightExpression("weight");

  // Apply cuts on samples
  TCut cuts("TMath::Abs(deltaMoverM)<=0.02 && bdtoutput >= -0.05");

  factory->PrepareTrainingAndTestTree( cuts, cuts, "SplitMode=Alternate:NormMode=NumEvents:!V" );

  if (Methods_["BDTG"]) // gradient boosted decision trees
    factory->BookMethod( TMVA::Types::kBDT, "BDTgradMIT", "!H:!V:NTrees=500:MaxDepth=3:BoostType=Grad:Shrinkage=1.0:UseBaggedGrad:GradBaggingFraction=0.50:nCuts=20:NNodesMax=8:IgnoreNegWeights"); 
//  if (Methods_["MLP"]) // neural network
//    factory->BookMethod( TMVA::Types::kMLP, "MLP", "!H:!V:NeuronType=tanh:NCycles=300:HiddenLayers=N+5,5:TestRate=5:EstimatorType=MSE");   
//  if (Methods_["FDA_GA"]) // functional discriminant with GA minimizer
//    factory->BookMethod( TMVA::Types::kFDA, "FDA_GA", "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );
//  if (Methods_["PDEFoam"]) // PDE-Foam approach
//    factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", "!H:!V:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );
  // To do multiple tests of one BDT use e.g
/*
  int nTrees[3] = {100,200,500};
  int maxDepth[4] = {3,5,10,50};
  float shrinkage[3] = {0.1,0.5,1.};
  float bagFrac[3] = {0.1,0.5,1.};
  for (int n=0; n<3; n++){
    for (int d=0; d<4;d++){
      for (int s=0; s<3; s++){
        for (int b=0; b<3; b++){
          factory->BookMethod( TMVA::Types::kBDT, 
                  Form("BDTG_Test_nTrees%d_mDepth%d_shr%1.2f_bf%1.2f",nTrees[n],maxDepth[d],shrinkage[s],bagFrac[b]), 
                  Form("!H:!V:NTrees=%d:MaxDepth=%d:BoostType=Grad:Shrinkage=%1.2f:UseBaggedGrad:GradBaggingFraction=%1.2f:nCuts=20:NNodesMax=8",nTrees[n],maxDepth[d],shrinkage[s],bagFrac[b])); 
        }
      }
    }
  }
*/

  if (optimize_) factory->OptimizeAllMethodsForClassification();
  // Train MVAs using the set of training events
  if (!skipTraining_) factory->TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events
  if (!skipTesting_) factory->TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  if (!skipEvaluation_) factory->EvaluateAllMethods();

  // Save the output
  cout << "==> Wrote root file: " << outFile->GetName() << endl; 
  cout << "==> TMVAClassification is done!" << endl;
  cout << "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << endl;

  input->Close();
  outFile->Close();
  delete outFile;
  delete input;

  return 0;
}

