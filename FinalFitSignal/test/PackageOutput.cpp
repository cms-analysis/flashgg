#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"

#include "TFile.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TGraph.h"

#include "RooWorkspace.h"
#include "RooExtendPdf.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooHistFunc.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooArgList.h"
#include "RooAddition.h"

#include "../interface/Normalization_8TeV.h"

using namespace std;
using namespace RooFit;
using namespace boost;
namespace po = boost::program_options;

string infilename_;
string outfilename_;
float lumi_;
string wsname_;
int mhLow_=110;
int mhHigh_=150;
int ncats_;
string webdir_;
bool web_;
bool spin_=false;
bool splitVH_=false;
bool makePlots_=false;
bool doSMHiggsAsBackground_=true;
bool doSecondHiggs_=true;
bool doNaturalWidth_=true;
bool skipSecondaryModels_=false;
vector<int> allMH_;

vector<int> getAllMH(){
  vector<int> result;
  for (int m=mhLow_; m<=mhHigh_; m+=5){
    cout << "Adding mass: " << m << endl;
    result.push_back(m);
  }
  return result;
}

void OptionParser(int argc, char *argv[]){

  po::options_description desc("Allowed options");

  desc.add_options()
    ("help,h",                                                                                "Show help")
    ("infilename,i", po::value<string>(&infilename_)->default_value("dat/filestocombine.dat"),"Input file name")
    ("outfilename,o", po::value<string>(&outfilename_)->default_value("CMS-HGG_sigfit.root"), "Output file name")
    ("lumi,l", po::value<float>(&lumi_)->default_value(19620.0),                              "Luminosity")
    ("wsname,W", po::value<string>(&wsname_)->default_value("wsig_8TeV"),                     "Output workspace name")
    ("mhLow,L", po::value<int>(&mhLow_)->default_value(110),                                  "Low mass point")
    ("mhHigh,H", po::value<int>(&mhHigh_)->default_value(150),                                "High mass point")
    ("ncats,n", po::value<int>(&ncats_)->default_value(9),                                    "Number of categories")
    ("html,w", po::value<string>(&webdir_),                                                   "Make html in this directory")
    ("spin,s",                                                                                "Also include the spin processes")
    ("splitVH",                                                                               "Split VH production mode into WH and ZH")
    ("makePlots,P",                                                                           "Make AN style signal model plots")
    ("skipSecondaryModels",                                                                   "Turn off creation of all additional models")
    ("noSMHiggsAsBackground",                                                                 "Turn off creation of additional model for SM Higgs as background")
    ("noSecondHiggs",                                                                         "Turn off creation of additional model for a second Higgs")
    ("noNaturalWidth",                                                                        "Turn off creation of additional model for natural width of the Higgs")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);
  if (vm.count("help")){ cout << desc << endl; exit(1);}
  if (vm.count("html"))                     web_=true;
  if (vm.count("makePlots"))                makePlots_=true;
  if (vm.count("spin"))                     spin_=true;
  if (vm.count("splitVH"))                  splitVH_=true;
  if (vm.count("skipSecondaryModels"))      skipSecondaryModels_=true;
  if (vm.count("noSMHiggsAsBackground"))    doSMHiggsAsBackground_=false;
  if (vm.count("noSecondHiggs"))            doSecondHiggs_=false;
  if (vm.count("noNaturalWidth"))           doNaturalWidth_=false;
  allMH_ = getAllMH();
}
 
int main (int argc, char *argv[]){
  
  OptionParser(argc,argv);
  
  if (skipSecondaryModels_){
    doSMHiggsAsBackground_=false;
    doSecondHiggs_=false;
    doNaturalWidth_=false;
  }

  vector<string> processes;
  processes.push_back("ggh");
  processes.push_back("vbf");
  if (splitVH_) {
    processes.push_back("wh");
    processes.push_back("zh");
  }
  else {
    processes.push_back("wzh");
  }
  processes.push_back("tth");
  if (spin_){
    processes.push_back("ggh_grav");
    processes.push_back("vbf_grav");
  }
  
  map<string,pair<string,int> > filestocombine;

  ifstream datfile;
  datfile.open(infilename_.c_str());
  if (datfile.fail()) {
    cerr << "datfile: " << infilename_ << " not found. Bailing out" << endl;
    exit(1);
  }
  while (datfile.good()){
    string line;
    getline(datfile,line);
    if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
    string fname = line.substr(0,line.find(" "));
    string info = line.substr(line.find(" ")+1,string::npos);
    string proc = info.substr(0,info.find("_cat"));
    string cat_string = info.substr(info.find("_cat")+4,string::npos);
    cout << fname << " : " << info << " : " << proc << " : " << cat_string << endl;
    int cat = boost::lexical_cast<int>(cat_string);
    filestocombine.insert(pair<string,pair<string,int> >(fname,pair<string,int>(proc,cat)));
  }
  datfile.close();

  TFile *outFile = new TFile(outfilename_.c_str(),"RECREATE");

  RooRealVar *intLumi = new RooRealVar("IntLumi","IntLumi",lumi_,0.,10.e5);
  // first loop files and import all pdfs and dataset into one workspace
  RooArgList *runningNormSum = new RooArgList();
  RooWorkspace *work = new RooWorkspace(wsname_.c_str(),wsname_.c_str());
  for (map<string,pair<string,int> >::iterator file=filestocombine.begin(); file!=filestocombine.end(); file++){
    string filename = file->first;
    string proc = file->second.first;
    int cat = file->second.second;

    TFile *thisFile = TFile::Open(file->first.c_str());

    RooWorkspace *tempWS = (RooWorkspace*)thisFile->Get("wsig");
    RooAddPdf *resultPdf = (RooAddPdf*)tempWS->pdf(Form("hggpdfrel_%s_cat%d",proc.c_str(),cat));
    RooHistFunc *resultNorm = (RooHistFunc*)tempWS->function(Form("hggpdfrel_%s_cat%d_norm",proc.c_str(),cat));
    work->import(*resultPdf,RecycleConflictNodes());
    work->import(*resultNorm,RecycleConflictNodes());
    runningNormSum->add(*resultNorm);
    if (doSMHiggsAsBackground_){
      RooAddPdf *resultPdf_SM = (RooAddPdf*)tempWS->pdf(Form("hggpdfrel_%s_cat%d_SM",proc.c_str(),cat));
      RooHistFunc *resultNorm_SM = (RooHistFunc*)tempWS->function(Form("hggpdfrel_%s_cat%d_SM_norm",proc.c_str(),cat));
      work->import(*resultPdf_SM,RecycleConflictNodes());
      work->import(*resultNorm_SM,RecycleConflictNodes());
    }
    if (doSecondHiggs_){
      RooAddPdf *resultPdf_2 = (RooAddPdf*)tempWS->pdf(Form("hggpdfrel_%s_cat%d_2",proc.c_str(),cat));
      RooSpline1D *resultNorm_2 = (RooSpline1D*)tempWS->function(Form("hggpdfrel_%s_cat%d_2_norm",proc.c_str(),cat));
      work->import(*resultPdf_2,RecycleConflictNodes());
      work->import(*resultNorm_2,RecycleConflictNodes());
    }
    if (doNaturalWidth_){
      RooAddPdf *resultPdf_NW = (RooAddPdf*)tempWS->pdf(Form("hggpdfrel_%s_cat%d_NW",proc.c_str(),cat));
      RooHistFunc *resultNorm_NW = (RooHistFunc*)tempWS->function(Form("hggpdfrel_%s_cat%d_NW_norm",proc.c_str(),cat));
      work->import(*resultPdf_NW,RecycleConflictNodes());
      work->import(*resultNorm_NW,RecycleConflictNodes());
    }
    for (unsigned int i=0; i<allMH_.size(); i++){
      int m = allMH_[i];
      RooDataSet *resultData = (RooDataSet*)tempWS->data(Form("sig_%s_mass_m%d_cat%d",proc.c_str(),m,cat));
      work->import(*resultData);
    }
  }
 
  // now we want to sum up some datasets and their respective pdfs for sig_eff calculations later
  vector<string> expectedObjectsNotFound;
  for (unsigned int i=0; i<allMH_.size(); i++){
    int m = allMH_[i];
    // make sums of SM datasets first
    RooDataSet *allDataThisMass = NULL;
    for (int cat=0; cat<ncats_; cat++){
      RooDataSet *allDataThisCat = NULL;
      for (vector<string>::iterator proc=processes.begin(); proc!=processes.end(); proc++){
        RooDataSet *tempData = (RooDataSet*)work->data(Form("sig_%s_mass_m%d_cat%d",proc->c_str(),m,cat));
        if (!tempData) {
          cerr << "WARNING -- dataset: " << Form("sig_%s_mass_m%d_cat%d",proc->c_str(),m,cat) << " not found. It will be skipped" << endl;
          expectedObjectsNotFound.push_back(Form("sig_%s_mass_m%d_cat%d",proc->c_str(),m,cat));
          continue;
        }
        if (cat==0) allDataThisMass = (RooDataSet*)tempData->Clone(Form("sig_mass_m%d_AllCats",m));
        else allDataThisMass->append(*tempData);
        if (*proc=="ggh") allDataThisCat = (RooDataSet*)tempData->Clone(Form("sig_mass_m%d_cat%d",m,cat));
        else allDataThisCat->append(*tempData);
      }
      if (!allDataThisCat) {
        cerr << "WARNING -- allData for cat " << cat << " is NULL. Probably because the relevant datasets couldn't be found. Skipping.. " << endl;
        continue;
      }
      work->import(*allDataThisCat);
    }
    if (!allDataThisMass) {
      cerr << "WARNING -- allData for mass " << m << " is NULL. Probably because the relevant datasets couldn't be found. Skipping.. " << endl;
      continue;
    }
    work->import(*allDataThisMass);
  }
  // make sums of SM pdfs
  RooArgList *sumPdfs = new RooArgList();
  for (int cat=0; cat<ncats_; cat++){
    RooArgList *sumPdfsThisCat = new RooArgList();
    for (vector<string>::iterator proc=processes.begin(); proc!=processes.end(); proc++){
      //RooExtendPdf *tempPdf = (RooExtendPdf*)work->pdf(Form("sigpdfrel_%s_cat%d",proc->c_str(),cat));
      RooHistFunc *norm = (RooHistFunc*)work->function(Form("hggpdfrel_%s_cat%d_norm",proc->c_str(),cat));
      RooAddPdf *pdf = (RooAddPdf*)work->pdf(Form("hggpdfrel_%s_cat%d",proc->c_str(),cat));
      RooFormulaVar *thisLumNorm = new RooFormulaVar(Form("hggpdfabs_%s_cat%d_norm",proc->c_str(),cat),Form("hggpdfabs_%s_cat%d_norm",proc->c_str(),cat),"@*@1",RooArgList(*norm,*intLumi));
      if (!norm && !pdf) cout << "AHHHH" << endl;
      RooExtendPdf *tempPdf = new RooExtendPdf(Form("sigpdfrel_%s_cat%d",proc->c_str(),cat),Form("sigpdfrel_%s_cat%d",proc->c_str(),cat),*pdf,*thisLumNorm);
      if (!tempPdf) {
        cerr << "WARNING -- pdf: " << Form("sigpdfrel_%s_cat%d",proc->c_str(),cat) << " not found. It will be skipped" << endl;
        expectedObjectsNotFound.push_back(Form("sigpdfrel_%s_cat%d",proc->c_str(),cat));
        continue;
      }
      sumPdfsThisCat->add(*tempPdf);
    }
    if (sumPdfsThisCat->getSize()==0){
        cerr << "WARNING -- sumPdfs for cat " << cat << " is EMPTY. Probably because the relevant pdfs couldn't be found. Skipping.. " << endl;
        continue;
    }
    RooAddPdf *sumPdfsPerCat = new RooAddPdf(Form("sigpdfrelcat%d_allProcs",cat),Form("sigpdfrelcat%d_allProcs",cat),*sumPdfsThisCat);
    sumPdfs->add(*sumPdfsPerCat);
  }
  if (sumPdfs->getSize()==0){
      cerr << "WARNING -- sumAllPdfs is EMPTY. Probably because the relevant pdfs couldn't be found. Skipping.. " << endl;
  }
  else {
    RooAddPdf *sumPdfsAllCats = new RooAddPdf("sigpdfrelAllCats_allProcs","sigpdfrelAllCats_allProcs",*sumPdfs);
    work->import(*sumPdfsAllCats,RecycleConflictNodes());
  }

  RooAddition *effAccSum = new RooAddition("effAccSum","effAccSum",*runningNormSum);
  work->import(*effAccSum,RecycleConflictNodes());
  work->import(*intLumi,RecycleConflictNodes());
  outFile->cd();
  work->Write();
  outFile->Close();
  delete outFile;

  // figure out effAcc
  RooAddition *eA = (RooAddition*)work->function("effAccSum");
  Normalization_8TeV *normalization = new Normalization_8TeV();
  TGraph *effAcc = new TGraph();
  RooRealVar *MH = (RooRealVar*)work->var("MH");
  int p=0;
  for (double mh=mhLow_; mh<mhHigh_+0.5; mh+=1) {
    MH->setVal(mh);
    //cout << mh << " " << eA->getVal() << " " << normalization->GetXsection(mh)*normalization->GetBR(mh) << endl;
    effAcc->SetPoint(p,mh,eA->getVal()/(normalization->GetXsection(mh)*normalization->GetBR(mh)));
    p++;
  }
  TCanvas *canv = new TCanvas();
  effAcc->SetLineWidth(3);
  effAcc->Draw("AL");
  canv->Print("effAccCheck.pdf");
  canv->Print("effAccCheck.png");
  delete effAccSum;
  delete effAcc;
  delete normalization;
  delete canv;

  if (expectedObjectsNotFound.size()==0) cout << "All expected objects found and packaged successfully." << endl;
  else {
    cout << "Some expected objects not found:" << endl;
    for (vector<string>::iterator it=expectedObjectsNotFound.begin(); it!=expectedObjectsNotFound.end(); it++) cout << "\t" << *it << endl;
  }
  cout << "Output written to: " << endl;
  cout << "\t" << outfilename_ << endl;

  if (web_){
    string sitename = webdir_.substr(webdir_.find("www")+4,string::npos);
    sitename = "www.cern.ch/mkenzie/"+sitename;
    cout << "Publishing to web directory " << webdir_ << endl;
    system(Form("rm -rf %s",webdir_.c_str()));
    system(Form("cp -r plots %s",webdir_.c_str()));
    system(Form("make_html.py %s -c -l -n --title \"Simultaneous Signal Fitting\"",webdir_.c_str()));
    cout << "\t" << sitename << endl;
  }

  if (makePlots_){
    cout << "Sorry making AN style plots is not yet implemented" << endl;
    /*
    ifstream tempfile("../Macros/makeParametricSignalModelPlots.C");
    if (!tempfile) {
      cout << "I'm looking for a file: ../Macros/makeParametricSignalModelPlots.C to make the AN plots but I can't find it. Bailing!" << endl;
      exit(1);
    }
    gROOT->ProcessLine(".L ../Macros/makeParametricSignalModelPlots.C+g");
    */
  }

  return 0;
}  

