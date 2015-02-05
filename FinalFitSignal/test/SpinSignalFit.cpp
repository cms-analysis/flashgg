#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "TKey.h"
#include "TMacro.h"

#include "../interface/InitialFit.h"
#include "../interface/SpinModelConstruction.h"
#include "../interface/SpinPackager.h"

#include "boost/program_options.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/predicate.hpp"

using namespace std;
using namespace RooFit;
using namespace boost;
namespace po = boost::program_options;

string filename_;
string outfilename_;
string datfilename_;
string systfilename_;
string plotDir_;
int nCats_;
string procSMStr_;
vector<string> procsSM_;
string procGravStr_;
vector<string> procsGrav_;
double scaleGravGG_;
double scaleGravQQ_;
string highR9cats_;
string lowR9cats_;
int mh_;
bool is2011_=false;
bool runInitialFitsOnly_=false;
bool doQuadraticSigmaSum_=false;
bool recursive_=true;
int verbose_=0;
int sqrts_;

void OptionParser(int argc, char *argv[]){
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h",                                                                                			"Show help")
    ("infilename,i", po::value<string>(&filename_),                                           			"Input file name")
    ("outfilename,o", po::value<string>(&outfilename_)->default_value("CMS-HGG_sigfit.root"), 			"Output file name")
    ("datfilename,d", po::value<string>(&datfilename_)->default_value("dat/config.dat"),      			"Configuration file")
    ("systfilename,s", po::value<string>(&systfilename_)->default_value("dat/photon_systs_spin.dat"),"Configuration file")
    ("nCats,n", po::value<int>(&nCats_)->default_value(20),                                    			"Number of total categories")
    ("plotDir,p", po::value<string>(&plotDir_)->default_value("plots"),															"Put plots in this directory")
		("procsSM", po::value<string>(&procSMStr_)->default_value("ggh,vbf,wh,zh,tth"),									"Processes for SM (comma sep)")
		("procsGRAV", po::value<string>(&procGravStr_)->default_value("gg_grav,qq_grav"),								"Processes for graviton (comma sep)")
		("scaleGravGG", po::value<double>(&scaleGravGG_)->default_value(1.),														"Scale up graviton gg xs")
		("scaleGravQQ", po::value<double>(&scaleGravQQ_)->default_value(1.),														"Scale up graviton qq xs")
		("mh,m", po::value<int>(&mh_)->default_value(125),																							"Fit this MH")
    ("is2011",                                                                         							"Is this the 7TeV analysis")
    ("runInitialFitsOnly",                                                                          "Just fit gaussians - no interpolation, no systematics - useful for testing nGaussians")
    ("doQuadraticSigmaSum",  										        																						"Add sigma systematic terms in quadrature")
    ("nonRecursive",                                                                             		"Do not recursively calculate gaussian fractions")
    ("highR9cats", po::value<string>(&highR9cats_)->default_value("0,1,2,3,4,10,11,12,13,14"),					"For cut based only - pass over which categories are inclusive high R9 cats (comma sep string)")
    ("lowR9cats", po::value<string>(&lowR9cats_)->default_value("5,6,7,8,9,15,16,17,18,19"),              			"For cut based only - pass over which categories are inclusive low R9 cats (comma sep string)")
    ("verbose,v", po::value<int>(&verbose_)->default_value(0),                                			"Verbosity level: 0 (lowest) - 3 (highest)")
	;
  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);
  if (vm.count("help")){ cout << desc << endl; exit(1);}
  if (vm.count("is2011"))               		is2011_=true;
  if (vm.count("runInitialFitsOnly"))       runInitialFitsOnly_=true;
  if (vm.count("doQuadraticSigmaSum"))			doQuadraticSigmaSum_=true;

	split(procsSM_,procSMStr_,boost::is_any_of(","));
	split(procsGrav_,procGravStr_,boost::is_any_of(","));
	if (is2011_) sqrts_=7;
	else sqrts_=8;
}

void transferMacros(TFile *inFile, TDirectory *outFile){
  
  TIter next(inFile->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())){
    if (string(key->ReadObj()->ClassName())=="TMacro") {
      //cout << key->ReadObj()->ClassName() << " : " << key->GetName() << endl;
      TMacro *macro = (TMacro*)inFile->Get(key->GetName());
      outFile->cd();
      macro->Write();
    }
  }
}

int main(int argc, char *argv[]) {

	OptionParser(argc,argv);

  TFile *inFile = TFile::Open(filename_.c_str());
  RooWorkspace *inWS = (RooWorkspace*)inFile->Get("cms_hgg_workspace");
  
  RooRealVar *mass = (RooRealVar*)inWS->var("CMS_hgg_mass");
  mass->SetTitle("m_{#gamma#gamma}");
  mass->setUnit("GeV");
  RooRealVar *intLumi = (RooRealVar*)inWS->var("IntLumi");
  RooRealVar *MH = new RooRealVar("MH","m_{H}",mh_-5.,mh_+5.);
  MH->setUnit("GeV");
  
	TFile *outFile = new TFile(outfilename_.c_str(),"RECREATE");
  RooWorkspace *outWS;
  if (is2011_) outWS = new RooWorkspace("wsig_7TeV");
  else outWS = new RooWorkspace("wsig_8TeV");
  
	transferMacros(inFile,outFile);
  system(Form("mkdir -p %s/initialFits",plotDir_.c_str()));
  system("mkdir -p dat/in");

  // run fits for each line in datfile
  ifstream datfile;
  datfile.open(datfilename_.c_str());
  if (datfile.fail()) {
	  std::cerr << "Could not open " << datfilename_ <<std::endl;
	  exit(1);
  }
  while (datfile.good()){
    string line;
    getline(datfile,line);
    if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
    vector<string> els;
    split(els,line,boost::is_any_of(" "));
    if( els.size()!=4 ) {
	    cerr << line << " " << els.size() <<endl;
	    assert(0);
    }
    string proc = els[0];
    int cat = boost::lexical_cast<int>(els[1]);
    int nGaussiansRV = boost::lexical_cast<int>(els[2]);
    int nGaussiansWV = boost::lexical_cast<int>(els[3]);
    
		cout << "-----------------------------------------------------------------" << endl;
    cout << Form("Running fits for proc:%s - cat:%d with nGausRV:%d nGausWV:%d",proc.c_str(),cat,nGaussiansRV,nGaussiansWV) << endl;
    cout << "-----------------------------------------------------------------" << endl;
    
		// get datasets for each MH here
    map<int,RooDataSet*> datasetsRV;
    map<int,RooDataSet*> datasetsWV;
    map<int,RooDataSet*> datasets;
		RooDataSet* dataRV = (RooDataSet*)inWS->data(Form("sig_%s_mass_m%d_rv_cat%d",proc.c_str(),mh_,cat));
		RooDataSet* dataWV = (RooDataSet*)inWS->data(Form("sig_%s_mass_m%d_wv_cat%d",proc.c_str(),mh_,cat));
		RooDataSet* data = (RooDataSet*)inWS->data(Form("sig_%s_mass_m%d_cat%d",proc.c_str(),mh_,cat));
		datasetsRV.insert(pair<int,RooDataSet*>(mh_,dataRV));
		datasetsWV.insert(pair<int,RooDataSet*>(mh_,dataWV));
		datasets.insert(pair<int,RooDataSet*>(mh_,data));

		vector<int> dummy;
		InitialFit initFitRV(mass,MH,mh_,mh_,dummy);
		initFitRV.setVerbosity(verbose_);
    initFitRV.buildSumOfGaussians(Form("%s_cat%d",proc.c_str(),cat),nGaussiansRV,recursive_);
		initFitRV.setDatasets(datasetsRV);
		initFitRV.runFits(4);
		initFitRV.plotFits(Form("%s/initialFits/%s_cat%d_rv",plotDir_.c_str(),proc.c_str(),cat));
		map<int,map<string,RooRealVar*> > fitParamsRV = initFitRV.getFitParams();
		
		InitialFit initFitWV(mass,MH,mh_,mh_,dummy);
		initFitWV.setVerbosity(verbose_);
    initFitWV.buildSumOfGaussians(Form("%s_cat%d",proc.c_str(),cat),nGaussiansWV,recursive_);
		initFitWV.setDatasets(datasetsWV);
		initFitWV.runFits(4);
		initFitWV.plotFits(Form("%s/initialFits/%s_cat%d_wv",plotDir_.c_str(),proc.c_str(),cat));
		map<int,map<string,RooRealVar*> > fitParamsWV = initFitWV.getFitParams();

		SpinModelConstruction spinModel(mass,MH,intLumi,mh_,proc,cat,sqrts_,systfilename_,scaleGravGG_,scaleGravQQ_,verbose_,doQuadraticSigmaSum_);
		spinModel.setHighR9cats(highR9cats_);
		spinModel.setLowR9cats(lowR9cats_);
		spinModel.setRVfitParams(fitParamsRV);
		spinModel.setWVfitParams(fitParamsWV);
		spinModel.setRVdatasets(datasetsRV);
		spinModel.setWVdatasets(datasetsWV);
   	spinModel.makeSTDdatasets();
		spinModel.buildRvWvPdf(Form("hggpdfsmrel_%dTeV",sqrts_),nGaussiansRV,nGaussiansWV,recursive_);
		spinModel.getNormalization();
		spinModel.plotPdf(plotDir_,120,130);
		spinModel.save(outWS);

	}
	SpinPackager spinPackage(outWS,procsSM_,procsGrav_,nCats_,mh_,sqrts_);
	spinPackage.packageOutput();

  cout << "Writing to file..." << endl;
	outFile->cd();
	outWS->Write();
  outFile->Close();
  inFile->Close();
  cout << "Done." << endl;

}
