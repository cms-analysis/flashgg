#include <iostream>
#include <vector>
#include <string>

#include "boost/program_options.hpp"

#include "TStopwatch.h"

#include "../interface/SimultaneousFit.h"

using namespace std;
using namespace RooFit;
using namespace boost;
namespace po = boost::program_options;

string filename_;
string outfilename_;
int mhLow_=110;
int mhHigh_=150;
string proc_;
int cat_;
int nInclusiveCats_;
int nExclusiveCats_;
int nGaussians_;
int dmOrder_;
int sigmaOrder_;
int fracOrder_;
bool spin_=false;
bool splitVH_=false;
bool doSMHiggsAsBackground_=true;
bool doSecondHiggs_=true;
bool doNaturalWidth_=true;
bool skipSecondaryModels_=false;
bool recursive_=false;
bool forceFracUnity_=true;
int verbose_=0;
bool initialFit_=true;
bool onlyInitialFit_=false;
bool linearInterp_=false;
bool loadPriorConstraints_=false;
float constraintValue_;
bool setToFitValues_=false;
bool simultaneousFit_=true;
bool mhDependentFit_=false;
bool dumpVars_=false;
bool clean_=false;
bool saveExtra_=false;
string extrafilename_="0";
bool fork_=true;
int forkN_;
bool web_=false;
string webdir_;

void OptionParser(int argc, char *argv[]){

  po::options_description desc("Allowed options");

  desc.add_options()
    ("help,h",                                                                                "Show help")
    ("infilename,i", po::value<string>(&filename_),                                           "Input file name")
    ("outfilename,o", po::value<string>(&outfilename_)->default_value("CMS-HGG_sigfit.root"), "Output file name")
    ("mhLow,L", po::value<int>(&mhLow_)->default_value(110),                                  "Low mass point")
    ("mhHigh,H", po::value<int>(&mhHigh_)->default_value(150),                                "High mass point")
    ("proc,p", po::value<string>(&proc_)->default_value("ggh"),                               "Signal Process")
    ("cat,c", po::value<int>(&cat_)->default_value(0),                                        "Category")
    ("nInclusiveCats", po::value<int>(&nInclusiveCats_)->default_value(4),                    "Number of inclusive categories")
    ("nExclusiveCats", po::value<int>(&nExclusiveCats_)->default_value(5),                    "Number of exclusive categories")
    ("gauss,g", po::value<int>(&nGaussians_)->default_value(3),                               "Number of Gaussians")
    ("dmOrder", po::value<int>(&dmOrder_)->default_value(1),                                  "Order of dm polynomial")
    ("sigmaOrder", po::value<int>(&sigmaOrder_)->default_value(2),                            "Order of sigma polynomial")
    ("fracOrder", po::value<int>(&fracOrder_)->default_value(1),                              "Order of frac polynomial")
    ("spin",                                                                                  "For spin analysis")
    ("splitVH",                                                                               "Split VH production modes in WH and ZH")
    ("skipSecondaryModels",                                                                   "Turn off creation of all additional models")
    ("noSMHiggsAsBackground",                                                                 "Turn off creation of additional model for SM Higgs as background")
    ("noSecondHiggs",                                                                         "Turn off creation of additional model for a second Higgs")
    ("noNaturalWidth",                                                                        "Turn off creation of additional model for natural width of the Higgs")
    ("recursive",                                                                             "Recursively calculate gaussian fractions")
    ("fracUnity",                                                                             "Relax fraction unity")
    ("noInitialFit",                                                                          "Do not run initial fit")
    ("onlyInitialFit",                                                                        "Only run initial fit")
    ("linearInterp",                                                                          "Run the traditional method of directly interpolating parameters linearly")
    ("loadPriorConstraints",                                                                  "Load prior constraints from dat file")
    ("constraintValue", po::value<float>(&constraintValue_)->default_value(0.1),              "Constraining factor (default is 0.1 = 10%)")
    ("setToFitValues",                                                                        "If running the traditional method this massively helps the interpolation (although it cheats slightly)")
    ("mhFit",                                                                                 "Run mh dependent fit instead of simultaneous fit (NOTE: still in development)")
    ("dumpVars,d",                                                                            "Dump variables into .dat file")
    ("fork", po::value<int>(&forkN_)->default_value(8),                                       "Fork NLL calculations over multiple CPU (runs quicker)")
    ("clean,C",                                                                               "Clean plots before running")
    ("save,s",                                                                                "Save extra root file")
    ("saveFileName,S", po::value<string>(&extrafilename_),                                    "Give extra root file this name")
    ("html,w", po::value<string>(&webdir_),                                                   "Make html in this directory")
    ("verbose,v", po::value<int>(&verbose_)->default_value(0),                                "Verbosity level: 0 (lowest) - 2 (highest)")

  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);
  if (vm.count("help")){ cout << desc << endl; exit(1);}
  if (vm.count("spin"))                     spin_=true;
  if (vm.count("splitVH"))                  splitVH_=true;
  if (vm.count("skipSecondaryModels"))      skipSecondaryModels_=true;
  if (vm.count("noSMHiggsAsBackground"))    doSMHiggsAsBackground_=false;
  if (vm.count("noSecondHiggs"))            doSecondHiggs_=false;
  if (vm.count("noNaturalWidth"))           doNaturalWidth_=false;
  if (vm.count("recursive"))                recursive_=true;
  if (vm.count("fracUnity"))                forceFracUnity_=false;
  if (vm.count("noInitialFit"))             initialFit_=false;
  if (vm.count("onlyInitialFit"))           onlyInitialFit_=true;
  if (vm.count("linearInterp"))             linearInterp_=true;
  if (vm.count("loadPriorConstraints"))     loadPriorConstraints_=true;
  if (vm.count("setToFitValues"))           setToFitValues_=true;
  if (vm.count("mhFit")){           
                                            mhDependentFit_=true;
                                            simultaneousFit_=false;
  }
  if (vm.count("dumpVars"))                 dumpVars_=true;
  if (vm.count("clean"))                    clean_=true;
  if (vm.count("save"))                     saveExtra_=true;
  if (vm.count("html"))                     web_=true;
  if (forkN_<2)                             fork_=false;
}

int main (int argc, char *argv[]){
  
  OptionParser(argc,argv);
  
  cout << "This code has been updated to a new package which can be run by executing:" << endl;
  cout << "\t ./bin/SignalFit -i <infile> -o <outfile> -d dat/newConfig.dat [--splitVH]" << endl;
  string message;
  cout << "Are you sure you want to continue? [y/n]" << endl;
  cin >> message;
  if (message=="n") exit(1);
  TStopwatch sw;
  sw.Start();
  
  if (clean_){
    cout << "Cleaning up first..." << endl;
    system("rm -rf plots/*");
  }
  if (skipSecondaryModels_){
    doSMHiggsAsBackground_=false;
    doSecondHiggs_=false;
    doNaturalWidth_=false;
  }
  SimultaneousFit *simultaneousFit = new SimultaneousFit(filename_,outfilename_,mhLow_,mhHigh_,verbose_,nInclusiveCats_,nExclusiveCats_,doSMHiggsAsBackground_,doSecondHiggs_,doNaturalWidth_,spin_,splitVH_);
  simultaneousFit->setInitialFit(initialFit_);
  simultaneousFit->setSimultaneousFit(simultaneousFit_);
  simultaneousFit->setMHDependentFit(mhDependentFit_);
  simultaneousFit->setLoadPriorConstraints(loadPriorConstraints_,constraintValue_);
  if (linearInterp_) {
    simultaneousFit->setLinearInterp(true);
    onlyInitialFit_=true;
  }
  if (onlyInitialFit_) {
    simultaneousFit->setSimultaneousFit(false);
    simultaneousFit->setMHDependentFit(false);
  }
  simultaneousFit->setForceFracUnity(forceFracUnity_);
  if (fork_) simultaneousFit->setFork(forkN_);
  if (saveExtra_) {
    extrafilename_=="0" ?
      simultaneousFit->saveExtra(outfilename_.substr(0,outfilename_.find_last_of(".root"))+"_extra.root") :   
      simultaneousFit->saveExtra(extrafilename_.c_str())  
    ;
  }
  simultaneousFit->runFit(proc_,cat_,nGaussians_,dmOrder_,sigmaOrder_,fracOrder_,recursive_,setToFitValues_);
  if (dumpVars_) simultaneousFit->dumpPolParams(Form("dat/out/pols_%s_cat%d.dat",proc_.c_str(),cat_),proc_);
  delete simultaneousFit;
  
  cout << "Done." << endl;
  cout << "Whole process took..." << endl;
  cout << "\t "; sw.Print();
 
  if (web_){
    string sitename = webdir_.substr(webdir_.find("www")+4,string::npos);
    sitename = "www.cern.ch/mkenzie/"+sitename;
    cout << "Publishing to web directory " << webdir_ << endl;
    system(Form("rm -rf %s",webdir_.c_str()));
    system(Form("cp -r plots %s",webdir_.c_str()));
    system(Form("make_html.py %s -c -l -n --title \"Simultaneous Signal Fitting\"",webdir_.c_str()));
    cout << "\t" << sitename << endl;
  }

  return 0;
}
