#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <typeinfo>
#ifndef __CINT__
#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"
#endif

#include "TFile.h"
#include "TMacro.h"
#include "TObjString.h"
#include "TTree.h"
#include "TH1F.h"

#include "RooDataSet.h"

#include "../interface/FMTPlots.h"
#include "../interface/FMTBase.h"
#include "../interface/FMTRebin.h"
#include "../interface/FMTFit.h"
#include "../interface/FMTSetup.h"
#include "../interface/FMTSigInterp.h"

using namespace std;
namespace po = boost::program_options;

string getFileName(int argc, char* argv[]){
  string filename="0";
  for (int i=0; i<argc; i++){
    if (string(argv[i])=="--filename" || string(argv[i])=="-i") filename=string(argv[i+1]);
  }
  return filename;
}

bool runTrees(int argc, char* argv[]){
	bool runtree=false;
	for (int i=0; i<argc; i++){
		if (string(argv[i])=="-T" || string(argv[i])=="--histosFromTrees") runtree=true;
	}
	return runtree;
}

int main(int argc, char* argv[]){

  string filename = getFileName(argc,argv);

	if (runTrees(argc,argv)) {
  /*    
    string path = filename.substr(0,filename.find_last_of("/"));
		string opfilename = filename.substr(filename.find("CMS-HGG"),string::npos);
    opfilename = path+'/'+opfilename;
    
		TFile *theFile = new TFile(filename.c_str());
    TFile *optFile = TFile::Open(opfilename.c_str());
		RooWorkspace *work = (RooWorkspace*)optFile->Get("cms_hgg_workspace");

    RooDataSet *data;
    TH1F *h = new TH1F("temp","temp",80,100,180);
    TTree *tree = (TTree*)theFile->Get("full_mva_trees/Data");
    for (int cat=0; cat<9; cat++){
      data = (RooDataSet*)work->data(Form("data_mass_cat%d",cat));
      if (!tree || !data){
        cout << "Problem" << endl;
        exit(1);
      }
      h->Reset();
      tree->Draw("mass>>temp",Form("category==%d",cat),"goff");
      cout << "cat: " << cat << " " << data->numEntries() << " " << h->GetSumOfWeights() << endl;
    }
    delete h;

		theFile->Close();
		optFile->Close();

		delete theFile;
*/
	}
	
  FMTSetup *runner = new FMTSetup(filename);
	runner->OptionParser(argc,argv);
	runner->CheckRunOptions();

	runner->runHistosFromTrees();
	runner->printRunOptions("before.txt");
	runner->runRebinning();
	runner->printRunOptions("after.txt");
	runner->runFitting();
	runner->createCorrBkgModel();
	runner->interpolateBDT();
	runner->writeDataCards();
  	runner->makePlots();
	runner->publishToWeb();
	runner->runCombine();

	delete runner;
	
  return 0;
}

