#include <typeinfo>
//#include <algorithm>

#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"

#include "TMacro.h"
#include "TObjString.h"

#include "../python/createCorrectedBackgroundModel.C"

#include "../interface/FMTTree.h"
#include "../interface/FMTPlots.h"
#include "../interface/FMTSigInterp.h"
#include "../interface/FMTSetup.h"

using namespace std;
namespace po = boost::program_options;

FMTSetup::FMTSetup(string filename):
	all_(false),
	fit_(false),
  catByHand_(false),
	histosFromTrees_(false),
	rebin_(false),
	skipRebin_(false),
	justRebin_(false),
	binEdges_(false),
  dumpDatFile_(false),
  bkgModel_(false),
	interp_(false),
	datacards_(false),
	diagnose_(false),
	web_(false),
	blinding_(true),
	runCombine_(false),
	checkHistos_(false),
	safeMode_(true),
  noPlot_(true),
	runSB_(false),
	cleaned(false),
  userLumi_(0.),
  histFromTreeMode_("all")
{
  //if (filename!="0") system(Form("cp %s %s_beforeFMT.root",filename.c_str(),filename.c_str()));
  intLumi_=0.;
  readInFits_=false;
  	
}

FMTSetup::~FMTSetup(){
	if (!cleaned) delete rebinner;
	cout << "Exiting..." << endl;
}

void FMTSetup::OptionParser(int argc, char *argv[]){

  //cout << "\033[1mFullMvaToolkit -- Developed by Matthew Kenzie and Nick Wardle \033[0m \n";
  //cout <<        "                  Imperial College London\n" << endl;
  //cout << "\033[1mRecommend running is with following arguments:\033\[0m \n\t\t./runIt.exe -i <filename> -b -I -d -D -w <web_dir> -c \n" << endl;

  po::options_description desc("Allowed options");

  desc.add_options()
    ("help,h",                                                          "Show this message")
    ("filename,i", po::value<string>(&filename_),                       "Input file name")
    ("outfilename,o", po::value<string>(&outfilename_),                 "Output file name")
    ("normfitfile", po::value<string>(&normFitsFile_),                       "Input file for norm fits")
    ("fit,f",      po::value<string>(&fitString_),                      "Fit invariant mass distribution: \n" 
                                                                        "  - \tCan take mulitiple arguments which can be split by comma (110,120), or by range (110-120), or both (110,112,115-120)\n"
                                                                        "  - \tDefault is to run all \n" 
                                                                        "  - \tWill accept any double in fit range \n"
                                                                        "  - \tNOTE: this option should not be run at a MC mass without calling rebinning. ")
    ("rebin,r",    po::value<string>(&rebinString_),                    "Run optimized rebinning: \n" 
                                                                        "  - \tCan take multiple arguments which can be split by comma (110,120), or by range (110-120), or both (110,120-150)\n" 
                                                                        "  - \tDefault is to run all \n" 
                                                                        "  - \tWill accept only integers in 5GeV steps \n" 
                                                                        "  - \tNOTE: this will re-run all fits and rebinnings around this mass. This is the recommended way of executing any refit or re-rebinning. You should opt to run on the nearest MC mass. E.g. to refit and rebin 112.5 use --rebin 115")
    ("catByHand,H",                                                     "Categorize events by hand")
    ("use2DcatMap",                                                     "Use the 2D map of dm/m, bdt -> category")
		("histosFromTrees,T",																								"Get histos from trees")
		("histFromTreeMode,M", po::value<string>(&histFromTreeMode_),				"Mode for hists from trees")
    ("skipRebin,N",  																										"Skip the rebinning stage")
		("justRebin,J",																											"Just extract bin edges don't do anything else")
    ("getBinEdges,B",																										"Use bin edges from mvaanalysis")
    ("dumpDatFile,F",po::value<string>(&dumpDatFil_),                   "Save a new .dat file. For example if you want to save the bin edges so they can be read in later.")
    ("bkgModel,b",  																										"Correct the background model")
    ("interp,I",    																										"Run signal interpolation")
    ("datacards,d", 																										"Produce datacards")
    ("diagnose,D",  																										"Run full diagnostics (makes plots) - increases running time")
    ("www,w",     	po::value<string>(&webDir_),												"Publish to web - increases running time")
    ("runCombine,C", 																										"Run Higgs combine tool (please make sure you have checked out HiggsAnalyis/CombinedLimit and compile it somewhere in your CMSSW area)")
    ("mHMin,l",			po::value<int>(&tempmHMin_),													"Set lower bound (GeV) for Higgs mH")
    ("mHMax,u",			po::value<int>(&tempmHMax_),													"Set upper bound (GeV) for Higgs mH")
    ("mHStep,s",		po::value<double>(&tempmHStep_),											"Set bin size (GeV) for Higgs mH")
		("unblind,E",	  																											"Unblind analysis - data will be plotted (default is off)")
		("checkHistos,c",  po::value<string>(&grepString_),            			"Run check on histograms in file")
    ("verbose,v",                                                       "Increase output level")
		("useDat,U", po::value<string>(&datFil_)->default_value("0"),				"Get options from .dat file not TFile")
    ("doPlot,P",                                                        "Remake plots")
    ("safeModeOff,S",                                                        "Do not run in safe mode (will delete plots)")
    ("setLumi,L",po::value<double>(&userLumi_),                         "Set luminosity (by default read from datafiles.dat)")
    ("is2011,7",                                                        "Is data run 2011 (7TeV) - default is 2012 (8TeV)")
		("runSB,R",																													"Make S/B plots - in development (please don\'t use yet)")
    ;
  
	po::positional_options_description p;
	p.add("filename",-1);

//	po::variables_map vm;
	po::store(po::parse_command_line(argc,argv,desc),vm);
	po::store(po::command_line_parser(argc,argv).options(desc).positional(p).run(),vm);
	po::notify(vm);

  if (vm.count("help")){
    cout << desc << endl;
    exit(1);
  }
  if (!vm.count("filename")){ cerr << "WARNING -- A FILENAME MUST BE PROVIDED" << endl; exit(1);}
  if (!vm.count("outfilename")) { outfilename_=filename_+"_processed.root"; }
  
  // configure options
	configureOptions(this);

	// open files
	inFile_ = TFile::Open(filename_.c_str());
  if (checkHistos_) {
    outFile_ = TFile::Open(outfilename_.c_str());
  }
  else {
    if (skipRebin_ && !histosFromTrees_) outFile_ = new TFile(outfilename_.c_str(),"UPDATE");
    else outFile_ = new TFile(outfilename_.c_str(),"RECREATE");
  }

  // read configuration
 	ReadRunConfig(this);
	if (checkHistos_) checkAllHistos();
	
  // sort out fit and rebin vectors
	fitMasses_ = getVecFromString<double>(fitString_);
  rebinMasses_ = getVecFromString<int>(rebinString_);
  organiseVectors(rebinMasses_,fitMasses_);
  if (fitMasses_.size()>0)        fit_=true;
  if (rebinMasses_.size()>0)      rebin_=true;

  if (!skipRebin_) {
    cout << "Creating rebinner" << endl;
    cout << "Passing outfile " << outFile_->GetName() << endl;
		rebinner = new FMTRebin(inFile_, outFile_);
		rebinner->setintLumi(intLumi_);
                rebinner->setVerbosity(verbose_);
                rebinner->fitter->setblind(blinding_);
		rebinner->fitter->setplot(!noPlot_);
		rebinner->setcatByHand(catByHand_);
		rebinner->setjustRebin(justRebin_);
		
		configureOptions(rebinner);
		ReadRunConfig(rebinner);
		configureOptions(rebinner->fitter);
		ReadRunConfig(rebinner->fitter);
		
        	if (readInFits_){
		 std::cout << "READING IN NORM FITS FROM FILE " << normFitsFile_.c_str()<<std::endl;
	  	 TFile *fits = TFile::Open(normFitsFile_.c_str());
	
		 std::cout << fits->GetName() <<std::endl;
		 std::cout << fits->IsOpen() <<std::endl;
		 rebinner->fitter->SetNormGraph(fits);
		}
	}
	else {
		cleaned=true;
	}

  
    system("mkdir -p plots/png");
    system("mkdir -p plots/pdf");
    system("mkdir -p plots/macro");
	printPassedOptions();
  if (dumpDatFile_) dumpDatFile(dumpDatFil_);
	
	if (verbose_) {
		cout << "Sig MC Masses:" << endl;
		cout << "MC: [";
		printVec(MCMasses_);
		cout << "] " << endl;
		cout << "MH Masses:" << endl;
		cout << "MH: [";
		printVec(MHMasses_);
		cout << "] " << endl;
	}

	// clean plots
	if (diagnose_ && !safeMode_) {
		cout << "WARNING! -- Removing...." << endl;
		system("rm -rf plots/pdf/*");
		system("cp plots/png/PhoPhoDraw.png plots/");
		system("cp plots/png/PhotPhotEvent.png plots/");
		system("rm -rf plots/png/*");
		system("mv plots/PhoPhoDraw.png plots/png");
		system("mv plots/PhotPhotEvent.png plots/png");
	}

}

void FMTSetup::checkAllHistos(string opt){

  if (opt=="analysis"){
    cout << "------------------------------------ CHECK --------------------------------- " << endl;
    cout << outFile_->GetName() << endl;
    TH1F *temp;
    for (vector<double>::iterator mIt=MHMasses_.begin(); mIt!=MHMasses_.end(); mIt++){
      temp = (TH1F*)outFile_->Get(Form("th1f_data_grad_%3.1f",*mIt));
      cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
      temp = (TH1F*)outFile_->Get(Form("th1f_bkg_mc_grad_%3.1f",*mIt));
      cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
      continue;
      pair<int,int> uandd = getNsidebandsUandD(*mIt);
      // low
      for (int sideband=numberOfSidebandGaps_+1; sideband<=numberOfSidebandGaps_+uandd.first; sideband++){
        temp = (TH1F*)outFile_->Get(Form("th1f_bkg_%dlow_grad_%3.1f",sideband,*mIt));
        cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
      }
      // high
      for (int sideband=numberOfSidebandGaps_+1; sideband<=numberOfSidebandGaps_+uandd.second; sideband++){
        temp = (TH1F*)outFile_->Get(Form("th1f_bkg_%dhigh_grad_%3.1f",sideband,*mIt));
        cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
      }
      for (vector<string>::iterator proc=getProcesses().begin(); proc!=getProcesses().end(); proc++){
        temp = (TH1F*)outFile_->Get(Form("th1f_sig_grad_%s_%3.1f",proc->c_str(),*mIt));
        cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
        for (vector<string>::iterator syst=systematics_.begin(); syst!=systematics_.end(); syst++){
          temp = (TH1F*)outFile_->Get(Form("th1f_sig_grad_%s_%3.1f_%sUp01_sigma",proc->c_str(),*mIt,syst->c_str()));
          temp = (TH1F*)outFile_->Get(Form("th1f_sig_grad_%s_%3.1f_%sDown01_sigma",proc->c_str(),*mIt,syst->c_str()));
          cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
          cout << Form("%50s   %5.0f   %5d   %5.2f",temp->GetName(),temp->GetEntries(),temp->GetNbinsX(),temp->Integral()) << endl;
        }
      }
    }
  }
	//cout << "Skipping BDT entries" << endl;
  /*
	TFile *tFile;
	for (int i=0; i<2; i++){
		if (i==0) tFile = inFile_;
		if (i==1) tFile = outFile_;
		TList *histList = tFile->GetListOfKeys();
		for (int j=0; j<histList->GetSize(); j++){
			string name = histList->At(j)->GetName(); 
			//if (name.find("BDT")!=string::npos) continue;
			if (name.find("th1f")!=string::npos && name.find(grepString_)!=string::npos){
				TH1F *temp = (TH1F*)tFile->Get(histList->At(j)->GetName());
				checkHisto(temp);
			}
		}
  }
  tFile->Close();
  */
}

void FMTSetup::configureOptions(FMTBase *base){
 
	if (vm.count("catByHand"))      catByHand_=true;
	if (vm.count("normfitfile")) readInFits_=true; 
	if (vm.count("histosFromTrees")) histosFromTrees_=true;
	if (vm.count("skipRebin")) 			skipRebin_=true;
	if (vm.count("justRebin")) 			justRebin_=true;
	if (vm.count("getBinEdges")) 		binEdges_=true;
	if (vm.count("dumpDatFile"))    dumpDatFile_=true;
	if (vm.count("bkgModel")) 			bkgModel_=true;
	if (vm.count("interp")) 				interp_=true;
	if (vm.count("datacards")) 			datacards_=true;
	if (vm.count("diagnose")) 			diagnose_=true;
	if (vm.count("unblind")) 				blinding_=false;
	if (vm.count("checkHistos")) 		checkHistos_=true;
	if (vm.count("safeModeOff")) 		safeMode_=false;
	if (vm.count("www"))            web_=true;
	if (vm.count("doPlot"))         noPlot_=false;
	if (vm.count("runSB"))					runSB_=true;
	if (vm.count("runCombine"))			runCombine_=true;
	if (vm.count("verbose"))        verbose_=true;
	else                            verbose_=false;
	if (vm.count("use2DcatMap"))    base->setuseSidebandBDT(false);
	if (vm.count("mHMin")) 					base->setmHMinimum(tempmHMin_);
	if (vm.count("mHMax")) 					base->setmHMaximum(tempmHMax_);
	if (vm.count("mHStep")) 				base->setmHStep(tempmHStep_);
	if (vm.count("getBinEdges"))		base->setrederiveOptimizedBinEdges(false);
	else 														base->setrederiveOptimizedBinEdges(true);
	if (vm.count("is2011"))        base->setis2011(true);
	else                            base->setis2011(false);
	if (vm.count("setLumi"))        base->setintLumi(userLumi_);
	if (fitMasses_.size()==0 && rebinMasses_.size()==0 && !skipRebin_) all_=true;

	if (!vm.count("useDat") && !useSidebandBDT_) {
	   std::cout << "Cannot use 2D category map without passing .dat file" << std::endl;
	   assert(0);
	}
        // Optimization is part of 2D category map so never rederive
        if (!useSidebandBDT_) base->setrederiveOptimizedBinEdges(false); 
	std::cout << " Configured Options" << std::endl;
}

void FMTSetup::ReadRunConfig(FMTBase *base){

	TMacro *mva;
	if (datFil_=="0"){ // read from tFile
		cout << "Reading run configuration from file..." << filename_ << endl;
		TFile *inFile = TFile::Open(filename_.c_str());
		mva = (TMacro*)inFile->Get("mvaanalysis");
		inFile->Close();
	}
	else { // read from dat
		cout << "Reading run configuration from file..." << datFil_ << endl;
		mva = new TMacro(datFil_.c_str(),"mvaanalysis");
	}
	TList *list = (TList*)mva->GetListOfLines();
	bool comment=false;
  for (int l=0; l<list->GetSize(); l++){
    TObjString *line = (TObjString*)list->At(l);
    string sline = Form("%s",line->GetName());
    if (sline.find("->")!=string::npos) {
      comment=!comment;
      continue;
    }
		if (sline.find("#")!=string::npos) continue;
    if (sline.find("isCutBased")!=string::npos)                   base->setisCutBased(boost::lexical_cast<bool>(getOptFromConfig<int>(sline)));
    if (sline.find("bdtname")!=string::npos)                      bdtname=getOptFromConfig<string>(sline);
    if (sline.find("weightsFile")!=string::npos)                  weightsFile=getOptFromConfig<string>(sline);
		if (sline.find("IntLumi=")!=string::npos)                    base->setintLumi(boost::lexical_cast<double>(getOptFromConfig<double>(sline)));
    if (sline.find("mHMinimum=")!=string::npos)										base->setmHMinimum(boost::lexical_cast<int>(getOptFromConfig<double>(sline)));
		if (sline.find("mHMaximum=")!=string::npos)										base->setmHMaximum(boost::lexical_cast<int>(getOptFromConfig<double>(sline)));
		if (sline.find("mHStep=")!=string::npos)										 	base->setmHStep(getOptFromConfig<double>(sline));
		if (sline.find("massMin=")!=string::npos)										 	base->setmassMin(getOptFromConfig<double>(sline));
		if (sline.find("massMax=")!=string::npos)										 	base->setmassMax(getOptFromConfig<double>(sline));
		if (sline.find("nDataBins=")!=string::npos)										base->setnDataBins(getOptFromConfig<int>(sline));
		if (sline.find("diphotonBdtCut=")!=string::npos)										base->setdiphotonBdtCut(getOptFromConfig<double>(sline));
		if (sline.find("signalRegionWidth=")!=string::npos)						base->setsignalRegionWidth(getOptFromConfig<double>(sline));
		if (sline.find("sidebandWidth=")!=string::npos)								base->setsidebandWidth(getOptFromConfig<double>(sline));
		if (sline.find("numberOfSidebands=")!=string::npos)						base->setnumberOfSidebands(getOptFromConfig<int>(sline));
		if (sline.find("numberOfSidebandsForAlgos=")!=string::npos)		base->setnumberOfSidebandsForAlgos(getOptFromConfig<int>(sline));
		if (sline.find("numberOfSidebandGaps=")!=string::npos)				base->setnumberOfSidebandGaps(getOptFromConfig<int>(sline));
		if (sline.find("massSidebandMin=")!=string::npos)							base->setmassSidebandMin(getOptFromConfig<double>(sline));
		if (sline.find("massSidebandMax=")!=string::npos)							base->setmassSidebandMax(getOptFromConfig<double>(sline));
    if (sline.find("nInclusiveCategories=")!=string::npos)        base->setnIncCateogies(getOptFromConfig<int>(sline));
    if (sline.find("nMassFacInclusiveCategories=")!=string::npos)        base->setnMassFacInclusiveCateogies(getOptFromConfig<int>(sline));
		if (sline.find("includeVBF=")!=string::npos)									base->setincludeVBF(getOptFromConfig<bool>(sline));
    if (sline.find("nVBFCategories=")!=string::npos)              base->setnVBFCategories(getOptFromConfig<int>(sline));
		if (sline.find("includeLEP=")!=string::npos)									base->setincludeLEP(getOptFromConfig<bool>(sline));
    if (sline.find("nLEPCategories=")!=string::npos)              base->setnLEPCategories(getOptFromConfig<int>(sline));
    if (sline.find("doEscaleSyst=")!=string::npos)                if (getOptFromConfig<bool>(sline)) base->setsystematic("E_scale"); 
    if (sline.find("doEresolSyst=")!=string::npos)                if (getOptFromConfig<bool>(sline)) base->setsystematic("E_res"); 
    if (sline.find("doEcorrectionSyst=")!=string::npos)           if (getOptFromConfig<bool>(sline)) base->setsystematic("E_corr"); 
    if (sline.find("doRegressionSyst=")!=string::npos)            if (getOptFromConfig<bool>(sline)) base->setsystematic("regSig"); 
    if (sline.find("doPhotonIdEffSyst=")!=string::npos)           if (getOptFromConfig<bool>(sline)) base->setsystematic("idEff"); 
    if (sline.find("doVtxEffSyst=")!=string::npos)                if (getOptFromConfig<bool>(sline)) base->setsystematic("vtxEff"); 
    if (sline.find("doTriggerEffSyst=")!=string::npos)            if (getOptFromConfig<bool>(sline)) base->setsystematic("triggerEff"); 
    if (sline.find("doPhotonMvaIdSyst=")!=string::npos)           if (getOptFromConfig<bool>(sline)) base->setsystematic("phoIdMva"); 
    if (sline.find("doR9Syst=")!=string::npos)                    if (getOptFromConfig<bool>(sline)) base->setsystematic("r9Eff"); 
    if (sline.find("doKFactorSyst=")!=string::npos)               if (getOptFromConfig<bool>(sline)) base->setsystematic("kFactor");
    if (sline.find("doPdfWeightSyst=")!=string::npos)             if (getOptFromConfig<bool>(sline)) { // eigenvector set and scale (need to uncorrelate scales later in card
	base->setsystematic("pdfWeight_QCDscale");
        for (int pdf_i=1;pdf_i<=26;pdf_i++){
	  base->setsystematic(Form("pdfWeight_pdfset%d",pdf_i));
	}
    }

    if (sline.find("rederiveOptimizedBinEdges=")!=string::npos)   base->setrederiveOptimizedBinEdges(getOptFromConfig<bool>(sline));
    
    for (int m=110; m<=150; m+=5){
      if (is2011_ && m==145) continue;
      if (sline.find(Form("GradBinEdges_%3d=",m))!=string::npos)  base->setBinEdges(m,getBinEdgesFromString(getOptFromConfig<string>(sline)));
      if (sline.find(Form("VbfBinEdges_%3d=",m))!=string::npos)   base->setVBFBinEdges(m,getBinEdgesFromString(getOptFromConfig<string>(sline)));
      if (sline.find(Form("LepBinEdges_%3d=",m))!=string::npos)   base->setLEPBinEdges(m,getBinEdgesFromString(getOptFromConfig<string>(sline)));
    }
  }
  // if using the cat map, then overwrite the egdes for inclusives
  if (!useSidebandBDT_){
  	for (int m=110; m<=150; m+=5){
        if (is2011_ && m==145) continue;
	base->setBinEdges(m,getBinEdgesFromCatMap());
      }
  }	
	base->updateBinEdges();
	MHMasses_ = getAllMH();
	MCMasses_ = getMCMasses();
	systematics_ = getsystematics();
  if (intLumi_<0.001 ) {
    userLumi_ = getLumiFromWorkspace();
    base->setintLumi(userLumi_);
  }
  /*
  if (!histosFromTrees_ && !checkHistos_) {
    //saveLumiToWorkspace();
    outFile_->cd();
    mva->Write();
  }
  */
	printRunOptions();
	std::cout << " Configured Options From .dat File " << std::endl;
}

vector<double> FMTSetup::getBinEdgesFromCatMap(){
  TFile *fic = TFile::Open(weightsFile.c_str());
  TH1F *binedgeMap  = (TH1F*)(fic->Get("Bin_Edges"));
	
  vector<double> categoryBinEdges;
  for (int b=1;b<=binedgeMap->GetNbinsX()+1;b++){
	categoryBinEdges.push_back(binedgeMap->GetBinLowEdge(b));
  }
  return categoryBinEdges;
}

void FMTSetup::saveLumiToWorkspace(){
  RooWorkspace *work = (RooWorkspace*)outFile_->Get("cms_hgg_worksapce");
  if (!work) work = new RooWorkspace("cms_hgg_workspace");
  RooRealVar *tempLumi = (RooRealVar*)work->var("IntLumi");
  if (!tempLumi) tempLumi = new RooRealVar("IntLumi","IntLumi",userLumi_);
  tempLumi->setVal(userLumi_);
  work->import(*tempLumi);
  outFile_->cd();
  work->Write();
}

double FMTSetup::getLumiFromWorkspace(){
  //TFile *inFile = TFile::Open(filename_.c_str());
  RooWorkspace *inWS = (RooWorkspace*)inFile_->Get("cms_hgg_workspace");
  if (!inWS) return 0.;
  RooRealVar *tempLumi = (RooRealVar*)inWS->var("IntLumi");
  if (!tempLumi) return 0.;
  return (tempLumi->getVal())/1000.;
}

vector<double> FMTSetup::getBinEdgesFromString(string name){
  vector<double> result;
  while (name.find(",")!=string::npos){
    result.push_back(boost::lexical_cast<double>(name.substr(0,name.find(","))));
    name = name.substr(name.find(",")+1,string::npos);
  }
  result.push_back(boost::lexical_cast<double>(name));
  return result;
}


void FMTSetup::organiseVectors(vector<int> &rebinVec, vector<double> &fitVec){
	
  if (fitVec.size()==0 && rebinVec.size()==0) return;
	else{
		for (vector<int>::iterator rebM = rebinVec.begin(); rebM != rebinVec.end(); rebM++){
			vector<double> mhMasses = getMHMasses(*rebM);
      if (fitVec.size()>0){
        for (vector<double>::iterator mhIt = mhMasses.begin(); mhIt != mhMasses.end(); mhIt++){
          if (find(fitVec.begin(), fitVec.end(), *mhIt) != fitVec.end()) fitVec.erase(find(fitVec.begin(),fitVec.end(),*mhIt)); 
        }
      }
		}
		sort(fitVec.begin(),fitVec.end());
		sort(rebinVec.begin(),rebinVec.end());
	}
}

void FMTSetup::printPassedOptions(){
  cout << "Running with following options:" << endl;
  cout << "\tFile:         " << filename_ << endl;
  cout << "\tFit masses:            [";
  if (fitMasses_.size()>0) {
    for (unsigned int i=0; i<fitMasses_.size()-1; i++) cout << fitMasses_[i] << ",";
    cout << fitMasses_[fitMasses_.size()-1];
  }
  cout << "]" << endl;
  cout << "\tRebin masses:          [";
  if (rebinMasses_.size()>0) {
    for (unsigned int i=0; i<rebinMasses_.size()-1; i++) cout << rebinMasses_[i] << ",";
    cout << rebinMasses_[rebinMasses_.size()-1];
  }
  cout << "]" << endl;
  if (!fit_ && !rebin_) cout << "\tNo specifics given so running all masses" << endl;
  cout << "\tFit:                    " << fit_ << endl;
  cout << "\tRebin:                  " << rebin_ << endl;
  cout << "\tSkipRebin:              " << skipRebin_ << endl;
  cout << "\tBinEdges:               " << binEdges_ << endl;
  cout << "\tBkgModel                " << bkgModel_ << endl;
  cout << "\tInterpolate:            " << interp_ << endl;
  cout << "\tDatacards:              " << datacards_ << endl;
  cout << "\tDiagnostics:            " << diagnose_ << endl;
  cout << "\tBlinding:               " << blinding_ << endl;
  cout << "\tIs 2011 data:           " << getis2011() << endl;
  cout << "\tWeb publish:            " << web_ << endl;
  cout << "\tCheck histos:           " << checkHistos_ << endl;
  cout << "\tDump dat file:          " << dumpDatFile_ << endl;
}

void FMTSetup::CheckRunOptions(){
	if (fitMasses_.size()>0){
		cerr << "WARNING -- FMTSetup::CheckRunOptions -- ARE YOU SURE ABOUT THESE RUNNING OPTIONS?" << endl;
		if (fitMasses_.size()==1) cerr << "You have requested an indivdual fit mass: " << fitMasses_[0] << endl;
		else if (fitMasses_.size()>1) {
			cerr << "You have requested individual fit masses: [";
			for (unsigned int i=0; i<fitMasses_.size()-1; i++) cerr << fitMasses_[i] << ",";
			cerr << fitMasses_[fitMasses_.size()-1] << "]" << endl;
		}
		cerr << "The fit at the MC masses has an effect on the binning algorithm so be careful!" << endl;
		cerr << "It is recommended that you run on the nearest binning mass instead." << endl;
		cerr << "YOU HAVE BEEN WARNED! \n";
		cerr << "Press RETURN to continue (q to quit)." << endl;
    string temp;
    temp = cin.get();
    if (temp=="q" || temp=="Q") exit(1);
	}
}

void FMTSetup::runHistosFromTrees(){
	if (histosFromTrees_){
    if (!cleaned) cleanUp();
    cout << "Running histos from trees...." << endl;
    //bool isCutBased_=false;
		//string bdtname = "BDTgradMIT";
		//string weightsFile = "../../AnalysisScripts/aux/sidebandMVA_weights_hcp/TMVAClassification_BDTgradMIT.weights.xml";
		//string weightsFile = "weights/TMVA_SidebandMVA_BDTgradMIT.weights.xml";
		FMTTree *fmtTree = new FMTTree(filename_, outfilename_);
		configureOptions(fmtTree);
		ReadRunConfig(fmtTree);
		fmtTree->Setup(bdtname,weightsFile);
		fmtTree->run(histFromTreeMode_);
		delete fmtTree;
    cout << "Histos from trees complete" << endl;
	}
}

void FMTSetup::runRebinning(){

	vector<int> theMasses;
	if (!skipRebin_ && rebin_) theMasses = rebinMasses_;
	else if (!skipRebin_ && all_)	 theMasses = getMCMasses();
	else return;
  printVec(theMasses); cout << endl;
	for (vector<int>::iterator rebM = theMasses.begin(); rebM != theMasses.end(); rebM++){
		cout << "Running rebinning for mass " << *rebM << endl;
		cout << "UandD: ["; printVec(getUandDMCMasses(*rebM)); cout << "]" << endl;
		cout << "mH:    ["; printVec(getMHMasses(*rebM)); cout << "]" << endl;
    rebinner->executeRebinning(*rebM);
		setAllBinEdges(rebinner->getAllBinEdges());
    if (dumpDatFile_) dumpDatFile(dumpDatFil_); 
		cout << "Done rebinning" << endl;
	}
	cout << "Done all requested rebinning" << endl;
}

void FMTSetup::runFitting(){

	if (!rebin_ && fit_){
		// First, get the fits file 

		for (vector<double>::iterator fitM = fitMasses_.begin(); fitM != fitMasses_.end(); fitM++){
			cout << "Running fitting for mass " << *fitM << endl;
			rebinner->fitter->redoFit(*fitM);
		}
	}
}

void FMTSetup::cleanUp(){
	// need to call FMTRebin destructor to free up file
  cout << "Cleaning...." << endl;
	delete rebinner;
	inFile_->Close();
	outFile_->Close();
	delete inFile_;
	delete outFile_;
	cleaned=true;
}

void FMTSetup::createCorrBkgModel(){
	if (!cleaned) cleanUp();
	if (bkgModel_){
		cout << "Running createCorrectedBackgroundModel...." << endl;
		cout << "Attempt on " << outfilename_ << endl;
		createCorrectedBackgroundModel(outfilename_,numberOfSidebands_,sidebandWidth_,signalRegionWidth_,numberOfSidebandGaps_,massSidebandMin_,massSidebandMax_,boost::lexical_cast<double>(mHMinimum_),boost::lexical_cast<double>(mHMaximum_),mHStep_,!noPlot_, blinding_);
		cout << "Finished correcting background model" << endl;
	}
}

void FMTSetup::interpolateBDT(){
	if (!cleaned) cleanUp();
	if (interp_){
		cout << "Running signal interpolation...." << endl;
		FMTSigInterp *interpolater = new FMTSigInterp(outfilename_);
		configureOptions(interpolater);
		ReadRunConfig(interpolater);
		interpolater->runInterpolation();
		delete interpolater;
	}
}

void FMTSetup::writeDataCards(){
  if (!cleaned) cleanUp();
  if (datacards_){
	cout << "Preparing to write datacards...." << endl;
	
	// can ignore the tags
	std::string tagsString = "";
	if (! includeVBF_) tagsString += " --noVbfTag ";
	if (! includeLEP_) tagsString += " --noVHTag ";
	std::string blindOpt = "";
	std::string yearOpt = "";
	if (blinding_) blindOpt= " --blind ";
	if (is2011_) yearOpt = " --is2011 ";
	system(Form("python python/writeBinnedMvaCard.py -i %s -p plots --makePlot --mhLow %3d.0 --mhHigh %3d.0 --mhStep %1.1f --intLumi %1.1f %s %s %s ",outfilename_.c_str(),mHMinimum_,mHMaximum_,mHStep_,intLumi_,blindOpt.c_str(),yearOpt.c_str(),tagsString.c_str()));
  }
}

void FMTSetup::makePlots(){
  if (!cleaned) cleanUp();
  if (!noPlot_){
		if (runSB_) {
			cout << "Running S/B stuff...." << endl;
			system(Form("python python/GetFakeShapeDatacards.py -i mva-datacards-grad -o fake-shape-cards -D -C -N --mhLow %3d.0 --mhHigh %3d.0 --mhStep %1.1f",getmHMinimum(),getmHMaximum(),getmHStep()));
    }
		cout << "Making plots..." << endl;
    FMTPlots *plotter = new FMTPlots(outfilename_);
    configureOptions(plotter);  
    ReadRunConfig(plotter);
    vector<double> theMasses = getAllMH();
    plotter->Setup();
    for (vector<double>::iterator mh = theMasses.begin(); mh != theMasses.end(); mh++){
      plotter->plotAll(*mh);
    }
    plotter->makeNormPlot();
    //plotter->makeSignalNormPlot(); // This also breaks now ??!?!?!
    delete plotter;
  }
}


void FMTSetup::publishToWeb(){
	if (!cleaned) cleanUp();
	if (web_){
		cout << "Publishing to web: " << webDir_ << "/plots/png/home.html" << endl;
		if (blinding_) system(Form("python python/publish_plots.py %s --blind",outfilename_.c_str()));
		else system(Form("python python/publish_plots.py %s --not",outfilename_.c_str()));
		system(Form("rm -r %s",webDir_.c_str()));
		system(Form("mkdir %s",webDir_.c_str()));
		system(Form("cp -r plots %s",webDir_.c_str()));
	}
}

void FMTSetup::runCombine(){
	if (!cleaned) cleanUp();
	if (runCombine_){
		cout << "Running combine tool..... " << endl;
		cout << Form("./python/limit.sh mva-datacards-grad/ grad $PWD") << endl;
		system(Form("./python/limit.sh mva-datacards-grad/ grad $PWD"));
	}
}
