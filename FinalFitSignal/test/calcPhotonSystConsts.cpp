#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStopwatch.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TF1.h"
#include "TMatrixD.h"

#include "boost/program_options.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

using namespace std;
using namespace boost;
namespace po = boost::program_options;

string infilenamesStr_;
vector<string> infilenames_;
string outfilename_;

string photonCatScalesStr_;
vector<string> photonCatScales_;
string photonCatScalesCorrStr_;
vector<string> photonCatScalesCorr_;

string photonCatSmearsStr_;
vector<string> photonCatSmears_;
string photonCatSmearsCorrStr_;
vector<string> photonCatSmearsCorr_;

string globalScalesStr_;
vector<string> globalScales_;
string globalScalesCorrStr_;
vector<string> globalScalesCorr_;

string procStr_;
vector<string> procs_;
string plotDir_;
bool doPlots_;
int mh_;
int nCats_;
string sqrtS_;
int quadInterpolate_;

void OptionParser(int argc, char *argv[]){

	po::options_description general_opts("General options");
	general_opts.add_options()
    ("help,h",                                                                                					"Show help")
    ("infilenames,i", po::value<string>(&infilenamesStr_),                                           		"Input file names (comma sep)")
    ("outfilename,o", po::value<string>(&outfilename_)->default_value("dat/photonCatSyst.dat"), 				"Output file name")
    ("mh,m", po::value<int>(&mh_)->default_value(125),                                  								"Mass point")
    ("nCats,n", po::value<int>(&nCats_)->default_value(9),                                    					"Number of total categories")
		("sqrtS", po::value<string>(&sqrtS_)->default_value("8"),																								"CoM energy")
		("procs,p",po::value<string>(&procStr_)->default_value("ggh,vbf,wh,zh,tth"),												"Processes (comma sep)")
		("plotDir,D", po::value<string>(&plotDir_)->default_value("plots"),																	"Out directory for plots")
		("doPlots,P", po::value<bool>(&doPlots_)->default_value(true),																	"Plot variations")
		("quadInterpolate",	po::value<int>(&quadInterpolate_)->default_value(0),														"Do a quadratic interpolation from this amount of sigma")
  ;                                                                                             		
	
	po::options_description syst_opts("Systematics options");
	syst_opts.add_options()
		("photonCatScales,s", 		po::value<string>(&photonCatScalesStr_)->default_value("EBlowR9,EBhighR9,EElowR9,EEhighR9"),												"Photon category scales (comma sep) which get correlated across diphoton categories but NOT different years.")
		("photonCatScalesCorr,S", po::value<string>(&photonCatScalesCorrStr_)->default_value("MaterialEBCentral,MaterialEBOuterEE"),									"Photon category scales (comma sep) which get correlated across diphoton categories AND across years.")
		("photonCatSmears,r", 		po::value<string>(&photonCatSmearsStr_)->default_value("EBlowR9,EBhighR9,EBlowR9Phi,EBhighR9Phi,EElowR9,EEhighR9"),	"Photon category smears (comma sep) which get correlated across diphoton categories but NOT different years.")
		("photonCatSmearsCorr,R", po::value<string>(&photonCatSmearsCorrStr_)->default_value(""),																											"Photon category smears (comma sep) which get correlated across diphoton categories AND years.")
		("globalScales,g", 				po::value<string>(&globalScalesStr_)->default_value("NonLinearity"),																						"Global scales (comma sep) which get correlated across diphoton categories but NOT different years. Can add additional options with a \':\' to insist that a particular category get a bigger or smaller effect. E.g. passing \'NonLinearity:0:2\' will create a systematics called \'NonLinearity\' and make its effect in category 0 twice as large")
		("globalScalesCorr,G", 		po::value<string>(&globalScalesCorrStr_)->default_value(""),																												"As above but scales ARE correlated across diphoton categories AND years.")
	;

	po::options_description all("Allowed options");
	all.add(general_opts).add(syst_opts);

  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,all),vm);
  po::notify(vm);
  if (vm.count("help")){ cout << all << endl; exit(1);}

	// make vectors of strings from passed strings
	split(infilenames_,infilenamesStr_,boost::is_any_of(","));
	split(procs_,procStr_,boost::is_any_of(","));
	split(photonCatScales_,photonCatScalesStr_,boost::is_any_of(","));
	split(photonCatScalesCorr_,photonCatScalesCorrStr_,boost::is_any_of(","));
	split(photonCatSmears_,photonCatSmearsStr_,boost::is_any_of(","));
	split(photonCatSmearsCorr_,photonCatSmearsCorrStr_,boost::is_any_of(","));
	split(globalScales_,globalScalesStr_,boost::is_any_of(","));
	split(globalScalesCorr_,globalScalesCorrStr_,boost::is_any_of(","));
}

// quadInterpolate function from Nick
double quadInterpolate(double C, double X1,double X2,double X3,double Y1,double Y2,double Y3){

        gROOT->SetStyle("Plain");
        gROOT->SetBatch(true);
        gStyle->SetOptStat(0);
        // Use the 3 points to determine a,b,c
        TF1 func("f1","[0]*x*x+[1]*x+[2]",-5,5);

        double entries[9];
        entries[0]=X1*X1; entries[1]=X1; entries[2]=1;
        entries[3]=X2*X2; entries[4]=X2; entries[5]=1;
        entries[6]=X3*X3; entries[7]=X3; entries[8]=1;

        //create the Matrix;
        TMatrixD M(3,3);
        M.SetMatrixArray(entries);
        M.Invert();

        double a = M(0,0)*Y1+M(0,1)*Y2+M(0,2)*Y3;
        double b = M(1,0)*Y1+M(1,1)*Y2+M(1,2)*Y3;
        double c = M(2,0)*Y1+M(2,1)*Y2+M(2,2)*Y3;

        func.SetParameter(0,a);
        func.SetParameter(1,b);
        func.SetParameter(2,c);

	return func.Eval(C);
}

//effsigma function from Chris
Double_t effSigma(TH1 * hist)
{

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    cout << "effsigma: Not a valid histo. nbins = " << nb << endl;
    return 0.;
  }
 
  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    cout << "effsigma: Not a valid histo. bwid = " << bwid << endl;
    return 0.;
  }
  //Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
//   if(total < 100.) {
//     cout << "effsigma: Too few entries " << total << endl;
//     return 0.;
//   }
  Int_t ierr=0;
  Int_t ismin=999;
 
  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }  
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) cout << "effsigma: Error of type " << ierr << endl;

  std::cout << hist->GetName() << " " << widmin << std::endl;

  return widmin;
 
}

void plotVariation(TH1F *nom, TH1F *up, TH1F *down, string phoCat, string name){

	TCanvas *canv = new TCanvas();
	canv->SetWindowSize(750,750);
	canv->SetCanvasSize(750,750);
	gStyle->SetOptStat(0);
	nom->SetLineWidth(3);
	nom->SetLineColor(kBlack);
	up->SetLineWidth(2);
	up->SetLineColor(kBlue);
	down->SetLineWidth(2);
	down->SetLineColor(kRed);

	double max=0.;
	max = nom->Integral()/(sqrt(2*TMath::Pi())*0.7)*nom->GetBinWidth(1);
	/// max = TMath::Max(max,nom->GetMaximum());
	/// max = TMath::Max(max,up->GetMaximum());
	/// max = TMath::Max(max,down->GetMaximum());

	nom->GetYaxis()->SetRangeUser(0,max*1.1);
	up->GetYaxis()->SetRangeUser(0,max*1.1);
	down->GetYaxis()->SetRangeUser(0,max*1.1);

	nom->GetXaxis()->SetRangeUser(mh_-10,mh_+10);
	up->GetXaxis()->SetRangeUser(mh_-10,mh_+10);
	down->GetXaxis()->SetRangeUser(mh_-10,mh_+10);

	nom->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");
	up->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");
	down->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");

	nom->SetTitle(Form("%s_%s",name.c_str(),phoCat.c_str()));
	up->SetTitle(Form("%s_%s",name.c_str(),phoCat.c_str()));
	down->SetTitle(Form("%s_%s",name.c_str(),phoCat.c_str()));

	TLegend *leg = new TLegend(0.6,0.6,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetLineColor(0);
	leg->AddEntry(nom,"Nominal","L");
	if (quadInterpolate_!=0) {
		leg->AddEntry(up,Form("+%d#sigma",quadInterpolate_),"L");
		leg->AddEntry(down,Form("-%d#sigma",quadInterpolate_),"L");
	}
	else {
		leg->AddEntry(up,"+1#sigma","L");
		leg->AddEntry(down,"-1#sigma","L");
	}

	nom->Draw("HIST");
	up->Draw("HISTsame");
	down->Draw("HISTsame");
	nom->Draw("HISTsame");
	leg->Draw();
	
	canv->Print(Form("%s/systematics/%s_%s.pdf",plotDir_.c_str(),name.c_str(),phoCat.c_str()));
	canv->Print(Form("%s/systematics/%s_%s.png",plotDir_.c_str(),name.c_str(),phoCat.c_str()));
}

double getMeanVar(TH1F* nom, TH1F *up, TH1F* down){
	double meanNom = nom->GetMean();
	double meanUp = up->GetMean();
	double meanDown = down->GetMean();
	double u = (meanUp-meanNom)/meanNom;
	double d = (meanNom-meanDown)/meanNom;
	if (quadInterpolate_!=0) {
		u = ( quadInterpolate(1.,-1.*quadInterpolate_,0.,1.*quadInterpolate_,meanDown,meanNom,meanUp) - meanNom ) / meanNom;
		d = ( meanNom - quadInterpolate(-1.,-1.*quadInterpolate_,0.,1.*quadInterpolate_,meanDown,meanNom,meanUp) ) / meanNom;
	}
	double val = (TMath::Abs(u)+TMath::Abs(d))/2.;
	if (val!=val) val=0.;
	return val;
}

double getSigmaVar(TH1F* nom, TH1F *up, TH1F* down){
	double effSigNom = effSigma(nom);
	double effSigUp = effSigma(up);
	double effSigDown = effSigma(down);
	double u = (effSigUp-effSigNom)/effSigNom;
	double d = (effSigNom-effSigDown)/effSigNom;
	if (quadInterpolate_!=0) {
		u = ( quadInterpolate(1.,-1.*quadInterpolate_,0.,1.*quadInterpolate_,effSigDown,effSigNom,effSigUp) - effSigNom ) / effSigNom;
		d = ( effSigNom - quadInterpolate(-1.,-1.*quadInterpolate_,0.,1.*quadInterpolate_,effSigDown,effSigNom,effSigUp) ) / effSigNom;
	}
	double val = (TMath::Abs(u)+TMath::Abs(d))/2.;
	if (val!=val) val=0.;
	return val;
}

double getRateVar(TH1F* nom, TH1F *up, TH1F* down){
	double rateNom = nom->Integral();
	double rateUp = up->Integral();
	double rateDown = down->Integral();
	double u = (rateUp-rateNom)/rateNom;
	double d = (rateNom-rateDown)/rateNom;
	if (quadInterpolate_!=0) {
		u = ( quadInterpolate(1.,-1.*quadInterpolate_,0.,1.*quadInterpolate_,rateDown,rateNom,rateUp) - rateNom ) / rateNom;
		d = ( rateNom - quadInterpolate(-1.,-1.*quadInterpolate_,0.,1.*quadInterpolate_,rateDown,rateNom,rateUp) ) / rateNom;
	}
	double val = (TMath::Abs(u)+TMath::Abs(d))/2.;
	if (val!=val) val=0.;
	return val;
}

vector<TH1F*> getHistograms(vector<TFile*> files, string name, string syst){

	vector<TH1F*> ret_hists;
	for (unsigned int i=0; i<files.size(); i++){
		
		files[i]->cd();
		TH1F *up = (TH1F*)files[i]->Get(Form("%s_%sUp01_sigma",name.c_str(),syst.c_str()));
		TH1F *down = (TH1F*)files[i]->Get(Form("%s_%sDown01_sigma",name.c_str(),syst.c_str()));
		TH1F *nominal = (TH1F*)files[i]->Get(name.c_str());
		if (up && down && nominal) {
			ret_hists.push_back(nominal);
			ret_hists.push_back(up);
			ret_hists.push_back(down);
			return ret_hists;
		}
	}
	cout << "ERROR - at least one of histograms " << name << ", " << name+"_"+syst+"Up01_sigma, " << name+"_"+syst+"Down01_sigma not found in any file" << endl;
	return vector<TH1F*>(3,NULL);
}

TH1F *getHistogram(vector<TFile*> files, string name){
	
	for (unsigned int i=0; i<files.size(); i++){
		files[i]->cd();
		TH1F *h = (TH1F*)files[i]->Get(name.c_str());
		if (h) return h;
	}
	cout << "ERROR - histogram " << name << " not found in any file" << endl;
	return 0;
}

void printInfo(ofstream &outfile, string name, vector<string> systs, string ext){
	
	outfile << name;
	for (unsigned int i=0; i<systs.size(); i++) {
		string pre = systs[i];
		string post = "";
		if (systs[i].find(":")!=string::npos){
			pre = systs[i].substr(0,systs[i].find(":"));
			post = systs[i].substr(systs[i].find(":"),string::npos);
		}
		outfile << pre+ext+post;
		if (i<systs.size()-1) outfile << ",";
	}
	outfile << endl;
}

int main(int argc, char *argv[]){
 
  OptionParser(argc,argv);

  TStopwatch sw;
  sw.Start();

	system(Form("mkdir -p %s/systematics",plotDir_.c_str()));

	vector<TFile*> inFiles;
	for (unsigned int i=0; i<infilenames_.size(); i++){
		inFiles.push_back(TFile::Open(infilenames_[i].c_str()));
		cout << "Opened file " << infilenames_[i] << endl;
		inFiles[i]->Print();
	}

	ofstream outfile;
	outfile.open(outfilename_.c_str());
	cout << "Writing to datfile " << outfilename_ << endl;

	outfile << "# this file has been autogenerated by calcPhotonSystConsts.cpp" << endl;
	outfile << endl;

	if (!photonCatScalesStr_.empty()) printInfo(outfile,"photonCatScales=",photonCatScales_,"_"+sqrtS_+"TeVscale");
	if (!photonCatScalesCorrStr_.empty()) printInfo(outfile,"photonCatScalesCorr=",photonCatScalesCorr_,"_scale");
	if (!photonCatSmearsStr_.empty()) printInfo(outfile,"photonCatSmears=",photonCatSmears_,"_"+sqrtS_+"TeVsmear");
	if (!photonCatSmearsCorrStr_.empty()) printInfo(outfile,"photonCatSmearsCorr=",photonCatSmearsCorr_,"_smear");
	if (!globalScalesStr_.empty()) printInfo(outfile,"globalScales=",globalScales_,"_"+sqrtS_+"TeVscale");
	if (!globalScalesCorrStr_.empty()) printInfo(outfile,"globalScalesCorr=",globalScalesCorr_,"_scale");
	outfile << endl;
	outfile << "# photonCat                   mean_change    sigma_change    rate_change" << endl;

	for (int cat=0; cat<nCats_; cat++){
		for (vector<string>::iterator proc=procs_.begin(); proc!=procs_.end(); proc++){
	
			cout << *proc << " - cat " << cat << endl;
			
			outfile << Form("diphotonCat=%d",cat) << endl;
			outfile << Form("proc=%s",proc->c_str()) << endl;

			// photon scales not correlated ....
			if (photonCatScalesStr_.size()!=0){
				for (vector<string>::iterator phoCat=photonCatScales_.begin(); phoCat!=photonCatScales_.end(); phoCat++){
				
					// this is to ensure nominal comes from the right file
					vector<TH1F*> hists = getHistograms(inFiles,Form("th1f_sig_%s_mass_m%d_cat%d",proc->c_str(),mh_,cat),Form("E_scale_%s",phoCat->c_str()));
					TH1F *nominal = hists[0];
					TH1F *scaleUp = hists[1];
					TH1F *scaleDown = hists[2];

					outfile << Form("%-30s",(*phoCat+"_"+sqrtS_+"TeVscale").c_str());
					if( scaleUp != 0 && scaleDown != 0 && nominal != 0) {
						if( doPlots_ ) { plotVariation(nominal,scaleUp,scaleDown,*phoCat,Form("%s_cat%d_scale",proc->c_str(),cat)); }
						outfile << Form("%1.4g     %1.4g     %1.4g    ",getMeanVar(nominal,scaleUp,scaleDown),getSigmaVar(nominal,scaleUp,scaleDown),getRateVar(nominal,scaleUp,scaleDown)) << endl;
					} else {
						outfile << Form("%1.4g     %1.4g     %1.4g    ",0.,0.,0.) << endl;
					}
				}
			}

			// photon smears not correlated
			if (photonCatSmearsStr_.size()!=0){
				for (vector<string>::iterator phoCat=photonCatSmears_.begin(); phoCat!=photonCatSmears_.end(); phoCat++){
				
					// this is to ensure nominal comes from the right file
					vector<TH1F*> hists = getHistograms(inFiles,Form("th1f_sig_%s_mass_m%d_cat%d",proc->c_str(),mh_,cat),Form("E_res_%s",phoCat->c_str()));
					TH1F *nominal = hists[0];
					TH1F *smearUp = hists[1];
					TH1F *smearDown = hists[2];

					outfile << Form("%-30s",(*phoCat+"_"+sqrtS_+"TeVsmear").c_str());
					if( smearUp != 0 && smearDown != 0 && nominal != 0) {
						if( doPlots_ ) { plotVariation(nominal,smearUp,smearDown,*phoCat,Form("%s_cat%d_smear",proc->c_str(),cat)); }
						outfile << Form("%1.4g     %1.4g     %1.4g    ",getMeanVar(nominal,smearUp,smearDown),getSigmaVar(nominal,smearUp,smearDown),getRateVar(nominal,smearUp,smearDown)) << endl;
					} else {
						outfile << Form("%1.4g     %1.4g     %1.4g    ",0.,0.,0.) << endl;
					}
				}
			}
			
			// photon scales correlated
			if (photonCatScalesCorrStr_.size()!=0){
				for (vector<string>::iterator phoCat=photonCatScalesCorr_.begin(); phoCat!=photonCatScalesCorr_.end(); phoCat++){
				
					// this is to ensure nominal comes from the right file
					vector<TH1F*> hists = getHistograms(inFiles,Form("th1f_sig_%s_mass_m%d_cat%d",proc->c_str(),mh_,cat),Form("E_scale_%s",phoCat->c_str()));
					TH1F *nominal = hists[0];
					TH1F *scaleUp = hists[1];
					TH1F *scaleDown = hists[2];
					
					outfile << Form("%-30s",(*phoCat+"_scale").c_str());
					if( scaleUp != 0 && scaleDown != 0 && nominal != 0) {
						if( doPlots_ ) { plotVariation(nominal,scaleUp,scaleDown,*phoCat,Form("%s_cat%d_scale",proc->c_str(),cat)); }
						outfile << Form("%1.4g     %1.4g     %1.4g    ",getMeanVar(nominal,scaleUp,scaleDown),getSigmaVar(nominal,scaleUp,scaleDown),getRateVar(nominal,scaleUp,scaleDown)) << endl;
					} else {
						outfile << Form("%1.4g     %1.4g     %1.4g    ",0.,0.,0.) << endl;
					}
				}
			}

			// photon smears correlated
			if (photonCatSmearsCorrStr_.size()!=0){
				for (vector<string>::iterator phoCat=photonCatSmearsCorr_.begin(); phoCat!=photonCatSmearsCorr_.end(); phoCat++){
				
					// this is to ensure nominal comes from the right file
					vector<TH1F*> hists = getHistograms(inFiles,Form("th1f_sig_%s_mass_m%d_cat%d",proc->c_str(),mh_,cat),Form("E_res_%s",phoCat->c_str()));
					TH1F *nominal = hists[0];
					TH1F *smearUp = hists[1];
					TH1F *smearDown = hists[2];
					
					outfile << Form("%-30s",(*phoCat+"_smear").c_str());
					if( smearUp != 0 && smearDown != 0 && nominal != 0) {
						if( doPlots_ ) { plotVariation(nominal,smearUp,smearDown,*phoCat,Form("%s_cat%d_smear",proc->c_str(),cat)); }
						outfile << Form("%1.4g     %1.4g     %1.4g    ",getMeanVar(nominal,smearUp,smearDown),getSigmaVar(nominal,smearUp,smearDown),getRateVar(nominal,smearUp,smearDown)) << endl;
					} else {
						outfile << Form("%1.4g     %1.4g     %1.4g    ",0.,0.,0.) << endl;
					}
				}
			}
			outfile << endl;	
		} // end process loop
		outfile << endl;
	} // end category loop

	for (unsigned int i=0; i<inFiles.size(); i++){
		cout << "Closed file " << inFiles[i]->GetName() << endl;
		inFiles[i]->cd();
		inFiles[i]->Close();
	}
	outfile.close();
	
	sw.Stop();
	cout << "Took ..." << endl;
	cout << "\t";
	sw.Print();
	return 0;
}
