// Script to be used Post - StatAnalysis to combine workpsaces / perform Fits
// Original Author -- Nicholas Wardle

#include "TROOT.h"
#include "TSystem.h"
#include "../RooContainer.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "TH1F.h"
#include "TFile.h"
#include "string.h"
#include <memory>

using namespace RooFit;

class workspaceMerger {
   public:	
	workspaceMerger(std::string);
	~workspaceMerger(){};
	void combineWorkspaces();
	void addFile(std::string);
	void cleanUp();

  private:
	void setupContainer();
	void saveContainer();
        std::string outputfilename;
	TFile *outPutFile;
	std::vector<std::string> combineFileNames;
	RooContainer *rooContainer;

};

workspaceMerger::workspaceMerger(std::string outputFileName){
  outputfilename=outputFileName;
  outPutFile =  TFile::Open(outputFileName.c_str(),"RECREATE");
  outPutFile->cd();
  rooContainer = new RooContainer();
  setupContainer();
}

void workspaceMerger::cleanUp(){
  delete rooContainer;
  gROOT->Reset();
}
	
void workspaceMerger::combineWorkspaces(){
	
	// Get names of objects inside RooContainer
	std::vector<std::string> histogramNames = rooContainer->GetTH1FNames();
	std::vector<std::string> datasetNames = rooContainer->GetDataSetNames();
		
	// Loop Over the files and get the relevant pieces to Merge:
	for (std::vector<std::string>::iterator it_comb=combineFileNames.begin()
	    ;it_comb!=combineFileNames.end()
	    ;it_comb++){

		TFile *tmpFile = TFile::Open((*it_comb).c_str());
		tmpFile->cd();
		std::cout << "Combining Current File - " << (*it_comb) << std::endl;

		for (std::vector<std::string>::iterator it_hist=histogramNames.begin()
		    ;it_hist!=histogramNames.end()
		    ;it_hist++) {
			
			TH1F *histExtra = (TH1F*) tmpFile->Get(Form("th1f_%s",it_hist->c_str()));
			rooContainer->AppendTH1F(*it_hist,histExtra);	
		}
		
		RooWorkspace *work = (RooWorkspace*) tmpFile->Get("cms_hgg_workspace");

		for (std::vector<std::string>::iterator it_data=datasetNames.begin()
		    ;it_data!=datasetNames.end()
		    ;it_data++) {

			RooDataSet *dataExtra = (RooDataSet*) work->data(Form("%s",it_data->c_str()));
			rooContainer->AppendDataSet(*it_data,dataExtra);	
		}

		std::cout << "Finished Combining File - " << (*it_comb) << std::endl;
		
		tmpFile->Close();
		//delete tmpFile;		
		
        }

	outPutFile->cd();
	saveContainer();	

}
void workspaceMerger::addFile(std::string newFileName){
  combineFileNames.push_back(newFileName);
}
// FIXME - All of the following should be The same as in StatAnalysis (and the dat file statanalysis.dat)
// Should make this function available so that the same is used by StatAnalysis and this class
void workspaceMerger::setupContainer(){


    double intlumi_=1092.;
    int nDataBins=50;
    double massMin = 100.;
    double massMax = 150.;

    int nCategories_=8;
    int nSystSteps=1;
    int systRange=3;

    rooContainer->SetNCategories(nCategories_);
    rooContainer->nsigmas = nSystSteps;
    rooContainer->sigmaRange = systRange;

    std::vector<std::string> sys;
    std::vector<int> sys_t(7,-1);	// Current analysis only has these constrained signal systematics
    sys.push_back("E_scale");
    sys.push_back("E_res");
    sys.push_back("idEff");
    sys.push_back("r9Eff");
    sys.push_back("kFactor");
    sys.push_back("vtxEff");
    sys.push_back("triggerEff");
    rooContainer->MakeSystematicStudy(sys,sys_t);
    // ----------------------------------------------------
    // ----------------------------------------------------
    // Global systematics - Lumi
    rooContainer->AddGlobalSystematic("lumi",1.06,1.00);
    // ----------------------------------------------------

    // Create observables for shape-analysis with ranges
    rooContainer->AddObservable("CMS_hgg_mass" ,massMin,massMax);

    rooContainer->AddConstant("IntLumi",intlumi_);

    // SM Model
    rooContainer->AddConstant("XSBR_110",0.0390848+0.00275406+0.002902204);
    rooContainer->AddConstant("XSBR_115",0.0386169+0.00283716+0.002717667);
    rooContainer->AddConstant("XSBR_120",0.0374175+0.00285525+0.002286);
    rooContainer->AddConstant("XSBR_130",0.0319112+0.00260804+0.0019327068);
    rooContainer->AddConstant("XSBR_140",0.0235322+0.00204088+0.0012874228);

    // FF model	
    rooContainer->AddConstant("ff_XSBR_105",0.1514688+0.1608224);
    rooContainer->AddConstant("ff_XSBR_110",0.08323692+0.08023015);
    rooContainer->AddConstant("ff_XSBR_115",0.0481518+0.04212559);
    rooContainer->AddConstant("ff_XSBR_120",0.02927583+0.023436813);
    rooContainer->AddConstant("ff_XSBR_130",0.01224394+0.008260946);
    rooContainer->AddConstant("ff_XSBR_140",0.005656604+0.003241793);

    rooContainer->AddRealVar("pol0",-0.01,-1.5,1.5);
    rooContainer->AddRealVar("pol1",-0.01,-1.5,1.5);
    rooContainer->AddFormulaVar("modpol0","@0*@0","pol0");
    rooContainer->AddFormulaVar("modpol1","@0*@0","pol1");

    std::vector<std::string> data_pol_pars(2,"p");	 
    data_pol_pars[0] = "modpol0";
    data_pol_pars[1] = "modpol1";
    rooContainer->AddGenericPdf("data_pol_model",
	  "0","CMS_hgg_mass",data_pol_pars,72);	// >= 71 means RooBernstein of order >= 1
    
    // -----------------------------------------------------
    // Make some data sets from the observables to fill in the event loop		  
    // Binning is for histograms (will also produce unbinned data sets)
    rooContainer->CreateDataSet("CMS_hgg_mass","data_mass"    ,nDataBins); // (100,110,150) -> for a window, else full obs range is taken 
    rooContainer->CreateDataSet("CMS_hgg_mass","bkg_mass"     ,nDataBins);    	  	
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_m105",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_m110",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_m115",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_m120",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_m130",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_m140",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_rv_m105",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_rv_m110",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_rv_m115",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_rv_m120",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_rv_m130",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_rv_m140",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_wv_m105",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_wv_m110",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_wv_m115",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_wv_m120",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_wv_m130",nDataBins);    
    rooContainer->CreateDataSet("CMS_hgg_mass","sig_mass_wv_m140",nDataBins);    

    // Make more data sets to represent systematic shitfs , 
    rooContainer->MakeSystematics("CMS_hgg_mass","sig_mass_m105",-1);	
    rooContainer->MakeSystematics("CMS_hgg_mass","sig_mass_m110",-1);	
    rooContainer->MakeSystematics("CMS_hgg_mass","sig_mass_m115",-1);	
    rooContainer->MakeSystematics("CMS_hgg_mass","sig_mass_m120",-1);	
    rooContainer->MakeSystematics("CMS_hgg_mass","sig_mass_m130",-1);	
    rooContainer->MakeSystematics("CMS_hgg_mass","sig_mass_m140",-1);	
}

void workspaceMerger::saveContainer(){

    rooContainer->FitToData("data_pol_model","data_mass");  // Fit to full range of dataset
    rooContainer->WriteSpecificCategoryDataCards(outputfilename,"data_mass","sig_mass","data_pol_model");
    rooContainer->WriteDataCard(outputfilename,"data_mass","sig_mass","data_pol_model");
    rooContainer->Save();

}




