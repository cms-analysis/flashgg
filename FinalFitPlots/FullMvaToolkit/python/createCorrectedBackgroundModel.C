// Script used to create Background model from BDT bin Count vs Mass

// Standard Includes

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>

// ROOT Includes
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TF1.h"
#include "TStyle.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrix.h"
#include "TVectorD.h"
#include "TObject.h"
#include "TMinuit.h"

// RooFit includes
#include "RooWorkspace.h"
#include "RooRealVar.h"

#include "logn.hh"

using namespace std;
using namespace RooFit;

// The following global variables should be the same as definde in PhotonAnalysis_scripts/mvaanalysis.dat
bool global_BLIND=false;
double global_SIDEBANDWIDTH=0.02;
double global_SIGNALREGION=0.02;
int global_NUMBEROFSIDEBANDGAPS=1;
double global_MASSSIDEBANDMIN=99;
double global_MASSSIDEBANDMAX=180;

// Following is additional fractional error which is added to each error term (squared in error matrix)
// E.g if each error is scaled by 20%, set this to 0.2 and each term in the error matrix is scaled by 1.44
#define global_SYSTEMATICBIAS 0.2

const int global_nMaxBdtBins(25);
const int global_nMaxMassBins(25);

const int maxPar(2*(global_nMaxBdtBins-1)+global_nMaxMassBins);

int global_nBdtBins;
int global_nMassBins;
double global_mH;
double global_nSignalRegion;
typedef struct global_STRUCT_GLOBALPARAMTERS{

	int fData[global_nMaxMassBins][global_nMaxBdtBins];
	double fMass[global_nMaxMassBins];
	double parAll[maxPar];
	double errAll[maxPar];
	double errMat[maxPar][maxPar];
	int nrm[global_nMaxMassBins];
	int npb[global_nMaxBdtBins];
	double ntot;

} STRUCT_GLOBALPARAMETERS;

STRUCT_GLOBALPARAMETERS global_parameters;

// MINUIT fit function
void fcnFit(Int_t &npar, Double_t *gin, Double_t &f, Double_t *p, Int_t iflag) {
	if(iflag!=4) std::cout << "IFLAG = " << iflag << std::endl;

	if(iflag==1) {
	}

	if(iflag==5) {
	}

	f=0.0;

	// Unpack linear fit parameters for each BDT bin
	double par[global_nMaxBdtBins][2];
	par[0][0]=1.0;
	par[0][1]=0.0;

	for(int j(1);j<global_nBdtBins;j++) {
		par[j][0]=fabs(p[2*j-2]);
		par[j][1]=p[2*j-1];

		par[0][0]-=par[j][0];
		par[0][1]-=par[j][1];
	}

	/*
		 for(int j(0);j<nBdtBins;j++) {
		 std::cout << "fcnFit: par[" << j << "][0] = " << par[j][0]
		 << ",  par[" << j << "][1] = " << par[j][1]
		 << ",  rat[" << j << "] = " << par[j][1]/par[j][0] << std::endl;
		 }
	 */

	// Unpack number of events for each mass bin
	double nd[global_nMaxMassBins];
	for(int i(0);i<global_nMassBins;i++) {
		nd[i]=p[i+2*(global_nMaxBdtBins-1)];

		//std::cout << "nd[" << i << "] = " << nd[i] << std::endl;
	}

	for(int i(0);i<global_nMassBins;i++) {
		//    if(fMInc[i]) {
		for(int j(0);j<global_nBdtBins;j++) {
			double nm(nd[i]*(par[j][0]+par[j][1]*(global_parameters.fMass[i]-global_mH)));
		//	if (global_parameters.fData[i][j] > 0){
				//double deltaf = global_parameters.fData[i][j]*logn(global_parameters.fData[i][j]/nm)+nm-global_parameters.fData[i][j];
				double deltaf = poissonChiSquared(nm,global_parameters.fData[i][j])/2.;
				if (iflag==5) std::cout << "nm[" << i << "][" << j << "] = " << nm << " data: " << global_parameters.fData[i][j] << ", delf: " << deltaf <<", logn: "<< logn(global_parameters.fData[i][j]/nm) <<  std::endl;
	
				f+=deltaf;
		//	} else {
		//		f+=nm;
		//	}
		}
		//    }
	}

	// Make it a chi-squared
	f*=2.0;


	/*
	//std::cout << "f = " << f << std::endl;

	if(iflag==1) {
	}

	if(iflag==5) {
	int nDof(0);
	for(int i(0);i<nMassBins;i++) {
	if(fMInc[i]) nDof+=nBdtBins-1;
	}
	nDof-=2*(nBdtBins-1);

	std::cout << "Chi-sq/NDoF = " << f << "/" << nDof
	<< ", Prob = " << TMath::Prob(f,nDof)
	<< std::endl << std::endl;
	}
	 */
}

void fillData(double mH,TFile *in, std::string type){

	int numberofremainingsidebands = global_nMassBins;
	// Count how many of each side was used
	int nLowerSidebands  = 0;	
	//int nHigherSidebands = 0;	

	// Calculate Lower sideband points. Since we only go to 150 (and we don't really care too much about higher masses)
	// The restriction on the mass will only apply to the lower sideband
	for (int i=global_NUMBEROFSIDEBANDGAPS+1;i<=global_NUMBEROFSIDEBANDGAPS+global_nMassBins/2;i++){

		double hypothesisModifier = (1.-global_SIDEBANDWIDTH)/(1+global_SIDEBANDWIDTH);
		double mhsb  = (mH*(1.-global_SIGNALREGION)/(1.+global_SIDEBANDWIDTH))*(TMath::Power(hypothesisModifier,i-1));
		//lowestMH = mhsb;
		if (mhsb*(1.-global_SIDEBANDWIDTH) < global_MASSSIDEBANDMIN) break; 
		nLowerSidebands++;
		numberofremainingsidebands--;	
	}

	std::cout << "Number of Lower Sidebands for mass " << mH <<  " " << nLowerSidebands << std::endl;
	int nHigherSidebands = numberofremainingsidebands;
	std::cout << "Number of Higher Sidebands for mass " << mH <<  " " << nHigherSidebands << std::endl;

	for (int i=global_NUMBEROFSIDEBANDGAPS+1;i<=global_NUMBEROFSIDEBANDGAPS+nHigherSidebands;i++){

		//double hypothesisModifier = (1.+global_SIDEBANDWIDTH)/(1-global_SIDEBANDWIDTH);
		//double mhsb  = (mH*(1.+global_SIGNALREGION)/(1.-global_SIDEBANDWIDTH))*(TMath::Power(hypothesisModifier,i-1));

	}	

	for (int i=global_NUMBEROFSIDEBANDGAPS+1;i<=global_NUMBEROFSIDEBANDGAPS+nLowerSidebands;i++){

		double hypothesisModifier = (1.-global_SIDEBANDWIDTH)/(1+global_SIDEBANDWIDTH);
		double mhsb  = (mH*(1.-global_SIGNALREGION)/(1.+global_SIDEBANDWIDTH))*(TMath::Power(hypothesisModifier,i-1));

		TH1F *SB = (TH1F*) in->Get(Form("th1f_bkg_%dlow_%s_%3.1f",i,type.c_str(),mH));
    //cout << Form("%50s  %5d  %5d  %5.2f",SB->GetName(),SB->GetEntries(),SB->GetNbinsX(),SB->Integral()) << endl;
		cout << "Name: " << SB->GetName() << " Entries: " << SB->GetEntries() << " Nbins: " << SB->GetNbinsX() << " Integral: " << SB->Integral() << endl;
		int mass_index = global_NUMBEROFSIDEBANDGAPS+nLowerSidebands-i;

		for (int bin_i=1;bin_i<=global_nBdtBins;bin_i++){
			global_parameters.fData[mass_index][bin_i-1]=(int)SB->GetBinContent(bin_i);
			std::cout << "Bin "<<bin_i <<", " << SB->GetBinContent(bin_i)<<std::endl;
		}

		global_parameters.fMass[mass_index]=mhsb;

	}
	for (int i=global_NUMBEROFSIDEBANDGAPS+1;i<=global_NUMBEROFSIDEBANDGAPS+nHigherSidebands;i++){


		double hypothesisModifier = (1.+global_SIDEBANDWIDTH)/(1-global_SIDEBANDWIDTH);
		double mhsb  = (mH*(1.+global_SIGNALREGION)/(1.-global_SIDEBANDWIDTH))*(TMath::Power(hypothesisModifier,i-1));

		TH1F *SB = (TH1F*) in->Get(Form("th1f_bkg_%dhigh_%s_%3.1f",i,type.c_str(),mH));
    //cout << Form("%50s  %5d  %5d  %5.2f",SB->GetName(),SB->GetEntries(),SB->GetNbinsX(),SB->Integral()) << endl;
		cout << "Name: " << SB->GetName() << " Entries: " << SB->GetEntries() << " Nbins: " << SB->GetNbinsX() << " Integral: " << SB->Integral() << endl;
		int mass_index = nLowerSidebands-1+i-global_NUMBEROFSIDEBANDGAPS ;

		for (int bin_i=1;bin_i<=global_nBdtBins;bin_i++){
			global_parameters.fData[mass_index][bin_i-1]=(int)SB->GetBinContent(bin_i);
			std::cout << "Bin "<<bin_i <<", " << SB->GetBinContent(bin_i)<<std::endl;
		}
		global_parameters.fMass[mass_index]=mhsb;

	}

	// Initialize counts to 0
	for(int i(0);i<global_nMaxMassBins;i++) global_parameters.nrm[i]=0;
	for(int i(0);i<global_nMaxBdtBins;i++) global_parameters.npb[i]=0;
	global_parameters.ntot=0;

	for(int i(0);i<global_nMassBins;i++) {
		for(int j(0);j<global_nBdtBins;j++) {
			global_parameters.nrm[i]+=global_parameters.fData[i][j];
			global_parameters.npb[j]+=global_parameters.fData[i][j];
			global_parameters.ntot+=global_parameters.fData[i][j];
		}
	}


}

void paulFit(TDirectory *mDir,TH1F* fMFitS,TH1F* hMFitS,TH2F* hFCovar, bool makePlots, string type){
	std::string label[global_nMaxMassBins]={"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","25"};

	TH1F *hMRaw[global_nMaxMassBins],*hMFit[global_nMaxMassBins];
	TGraphErrors *hBRaw[global_nMaxBdtBins],*hBFit[global_nMaxBdtBins];
	TH1F *fMRaw[global_nMaxMassBins],*fMFit[global_nMaxMassBins];
	TGraphErrors *fBRaw[global_nMaxBdtBins],*fBFit[global_nMaxBdtBins];
	TH2F *hFCorr;
	TH1F *fBPar,*fBErr;



	for(int i(0);i<global_nMassBins;i++) {
		hMRaw[i]=new TH1F((std::string("hMass")+label[i]+"Raw").c_str(),
				(std::string("Events Mass range ")+label[i]+" Raw ").c_str(),
				global_nBdtBins,0.0,global_nBdtBins);
		hMFit[i]=new TH1F((std::string("hMass")+label[i]+"Fit").c_str(),
				(std::string("Events Mass range ")+label[i]+" Fit ").c_str(),
				global_nBdtBins,0.0,global_nBdtBins);
		fMRaw[i]=new TH1F((std::string("fMass")+label[i]+"Raw").c_str(),
				(std::string("Fractions Mass range ")+label[i]+" Raw ").c_str(),
				global_nBdtBins,0.0,global_nBdtBins);
		fMFit[i]=new TH1F((std::string("fMass")+label[i]+"Fit").c_str(),
				(std::string("Fractions Mass range ")+label[i]+" Fit ").c_str(),
				global_nBdtBins,0.0,global_nBdtBins);

		for(int j(0);j<global_nBdtBins;j++) {
			hMRaw[i]->SetBinContent(j+1,global_parameters.fData[i][j]);
			hMRaw[i]->SetBinError(j+1,sqrt(global_parameters.fData[i][j]));
			fMRaw[i]->SetBinContent(j+1,double(global_parameters.fData[i][j])/global_parameters.nrm[i]);
			fMRaw[i]->SetBinError(j+1,sqrt(global_parameters.fData[i][j])/global_parameters.nrm[i]);
		}
	}

	for(int j(0);j<global_nBdtBins;j++) {
		hBRaw[j]=new TGraphErrors((std::string("hBDT")+label[j]+"Raw").c_str(),
				(std::string("Events BDT bin ")+label[j]+" Raw ").c_str());
		hBRaw[j]->SetName((std::string("hBDT")+label[j]+"Raw").c_str());
		hBFit[j]=new TGraphErrors((std::string("hBDT")+label[j]+"Fit").c_str(),
				(std::string("Events BDT bin ")+label[j]+" Fit ").c_str());
		hBFit[j]->SetName((std::string("hBDT")+label[j]+"Fit").c_str());
		fBRaw[j]=new TGraphErrors((std::string("fBDT")+label[j]+"Raw").c_str(),
				(std::string("Fractions BDT bin ")+label[j]+" Raw ").c_str());
		fBRaw[j]->SetName((std::string("fBDT")+label[j]+"Raw").c_str());
		fBFit[j]=new TGraphErrors((std::string("fBDT")+label[j]+"Fit").c_str(),
				(std::string("Fractions BDT bin ")+label[j]+" Fit ").c_str());
		fBFit[j]->SetName((std::string("fBDT")+label[j]+"Fit").c_str());

		for(int i(0);i<global_nMassBins;i++) {
			double xERR = global_parameters.fMass[i]*global_SIDEBANDWIDTH;
			hBRaw[j]->SetPoint(i,global_parameters.fMass[i],global_parameters.fData[i][j]);
			hBRaw[j]->SetPointError(i,xERR,sqrt(global_parameters.fData[i][j]));
			fBRaw[j]->SetPoint(i,global_parameters.fMass[i],double(global_parameters.fData[i][j])/global_parameters.nrm[i]);
			fBRaw[j]->SetPointError(i,xERR,sqrt(global_parameters.fData[i][j])/global_parameters.nrm[i]);
		}
	}

	hFCorr=new TH2F((std::string("fCorr")).c_str(),
			Form("Fraction correlation matrix m %3.1f",global_mH),
			global_nBdtBins,0.0,global_nBdtBins,global_nBdtBins,0.0,global_nBdtBins);

	fBPar=new TH1F((std::string("fBPar")).c_str(),
			(std::string("Relative fit slope ")).c_str(),
			global_nBdtBins,0.0,global_nBdtBins);      
	fBErr=new TH1F((std::string("fBErr")).c_str(),
			(std::string("Fraction error ")).c_str(),
			global_nBdtBins,0.0,global_nBdtBins);      

	// Function taken directly from P. Dauncey to Perform Fits

	TMinuit tMinuit(maxPar);
	tMinuit.mninit(5,6,7);
	tMinuit.SetFCN(fcnFit);
	tMinuit.SetPrintLevel(9);
	tMinuit.SetMaxIterations(20000);


	int ell[5]={0,0,0,0,0};

	//static Double_t p0=0;
	static Double_t p1=1;
	//static Double_t p3=3;
	static Double_t p5=5;


	for(int i(1);i<global_nMaxBdtBins;i++) {
		double initVal = global_parameters.npb[i] >  0 ? global_parameters.npb[i]:10.;
		tMinuit.DefineParameter(2*i-2,(std::string("Par")+label[i]+"0").c_str(),
				initVal/global_parameters.ntot,sqrt(initVal)/global_parameters.ntot + 0.001,0.0,0.0);
		tMinuit.DefineParameter(2*i-1,(std::string("Par")+label[i]+"1").c_str(),
			   0.0,0.01,-0.5,0.5);

		if(i>=global_nBdtBins) {
			tMinuit.FixParameter(2*i-2);
			tMinuit.FixParameter(2*i-1);
		}
	}


	for(int i(0);i<global_nMaxMassBins;i++) {
		tMinuit.DefineParameter(i+2*(global_nMaxBdtBins-1),(std::string("Nrm")+label[i]).c_str(),
				global_parameters.nrm[i],sqrt(global_parameters.nrm[i]),0.0,0.0);

		if(i>global_nMassBins-1) tMinuit.FixParameter(i+2*(global_nMaxBdtBins-1));
	}

	tMinuit.mnexcm("CALL FCN", &p1 ,1,ell[0]);

	tMinuit.mnexcm("SCAN"    , &p1 ,0,ell[1]);

	ell[3]=tMinuit.Migrad();
	if(ell[3]!=0) std::cout << "WARNING MINUIT ERROR FLAG = " << ell[3] << std::endl;

	/*
		 for(int i(1);i<nPar[fitFcn];i+=2) {
		 tMinuit.Release(i);
		 tMinuit.Release(i+1);
		 tMinuit.mnexcm("SCAN"    , &p1 ,0,ell[1]);
		 ell[3]=tMinuit.Migrad();
		 }
	 */

	tMinuit.mnexcm("CALL FCN", &p5 ,1,ell[4]);

	for(int i(0);i<maxPar;i++) {
		tMinuit.GetParameter(i,global_parameters.parAll[i],global_parameters.errAll[i]);
		std::cout << "AFTER MINUIT PAR " << i << " = "
			<< global_parameters.parAll[i] << " +/- " << global_parameters.errAll[i] << std::endl;
	}

	tMinuit.mnemat(&(global_parameters.errMat[0][0]),maxPar);
	// Repeat!

	//   for(int i(0);i<2*(global_nBdtBins-1)+nMassBins;i++) {
	//	std::cout << "AFTER MNEMAT ERR2 " << i << " = "
	//		  << global_parameters.errMat[i][i] << ", ERR " << sqrt(global_parameters.errMat[i][i]) << std::endl;
	//    }

	//      std::cout << "ERRMAT" << std::endl;
	//      for(int i(0);i<nBdtBins-1;i++) {
	//	for(int j(0);j<nBdtBins-1;j++) {
	//	  std::cout << std::setw(14) << global_parameters.errMat[2*i][2*j];
	//	}
	//	std::cout << std::endl;
	//    }

	std::cout << "DQ/DP" << std::endl;
	double dqdp[global_nBdtBins-1][global_nBdtBins];
	for(int i(0);i<global_nBdtBins-1;i++) {
		for(int j(0);j<global_nBdtBins;j++) {
			if(j==0) dqdp[i][j]=-1.0;
			else {
				if(i+1==j) dqdp[i][j]=1.0;
				else       dqdp[i][j]=0.0;
			}
			std::cout << std::setw(14) << dqdp[i][j];
		}
		std::cout << std::endl;
	}

	std::cout << "FRACERR" << std::endl;
	double fracErr[global_nBdtBins][global_nBdtBins];
	for(int i(0);i<global_nBdtBins;i++) {
		for(int j(0);j<global_nBdtBins;j++) {
			fracErr[i][j]=0.0;	
			for(int k(0);k<global_nBdtBins-1;k++) {
				for(int l(0);l<global_nBdtBins-1;l++) {
					fracErr[i][j]+=dqdp[k][i]*global_parameters.errMat[2*k][2*l]*dqdp[l][j];

				}
			}
			//std::cout << std::setw(14) << fracErr[i][j];
		}
		std::cout << std::endl;
	}

	// Add Global Error to error matrix -> Motivated from Bias studies of fits:
	for(int i(0);i<global_nBdtBins;i++) {
		for(int j(0);j<global_nBdtBins;j++) {
			fracErr[i][j]*=(1.+global_SYSTEMATICBIAS)*(1.+global_SYSTEMATICBIAS);	
			std::cout << std::setw(14) << fracErr[i][j];
		}
		std::cout << std::endl;
	}


	std::cout << "FRACCOR" << std::endl;
	double fracCor[global_nBdtBins][global_nBdtBins];
	for(int i(0);i<global_nBdtBins;i++) {
		for(int j(0);j<global_nBdtBins;j++) {
			// Turns Error Matrix into Correlation Matrix
			fracCor[i][j]=fracErr[i][j]/sqrt(fracErr[i][i]*fracErr[j][j]);
			// Covariance Matrix

			//std::cout << std::setw(14) << fracCor[i][j];

			if (fabs(fracErr[i][j])< 10E-16) fracErr[i][j]=0.;
			if (fabs(fracCor[i][j])< 10E-16) fracCor[i][j]=0.;
			if (i==j && i<global_nBdtBins-1 && fabs(fracErr[i][j])< 10E-16){
				 double relErr=fracErr[i+1][j+1]/global_parameters.parAll[2*(j+2)-2];
				 fracErr[i][j]=relErr*global_parameters.parAll[2*(j+1)-2];
			}
			hFCovar->SetBinContent(i+1,j+1,fracErr[i][j]);
			hFCorr->SetBinContent(i+1,j+1,fracCor[i][j]);
		}
		std::cout << std::endl;
	}

	for(int i(0);i<global_nBdtBins;i++) {
		std::cout << "AFTER DQ/DP  ERR2 " << i << " = "
			<< fracErr[i][i] << ", ERR " << sqrt(fracErr[i][i]) << std::endl;
	}

	for(int j(0);j<global_nBdtBins;j++) {
		fBErr->SetBinContent(j+1,sqrt(fracErr[j][j]));
	}

	double p00(1.0);
	double p01(0.0);

	// Extract fractions/yields for each mass and for each BDT bin
	for(int i(0);i<global_nMassBins;i++) {
		for(int j(1);j<global_nBdtBins;j++) {
			hMFit[i]->SetBinContent(j+1,global_parameters.parAll[i+2*(global_nMaxBdtBins-1)]*(global_parameters.parAll[2*j-2]+global_parameters.parAll[2*j-1]*(global_parameters.fMass[i]-global_mH)));
			fMFit[i]->SetBinContent(j+1,              global_parameters.parAll[2*j-2]+global_parameters.parAll[2*j-1]*(global_parameters.fMass[i]-global_mH) );
			//	  hBFit[j]->SetBinContent(i+1,global_parameters.parAll[i+2*(global_nMaxBdtBins-1)]*(global_parameters.parAll[2*j-2]+global_parameters.parAll[2*j-1]*(global_parameters.fMass[i]-global_mH)));
			//	  fBFit[j]->SetBinContent(i+1,              global_parameters.parAll[2*j-2]+global_parameters.parAll[2*j-1]*(global_parameters.fMass[i]-global_mH) );
			fBFit[j]->SetPoint(i,global_parameters.fMass[i],global_parameters.parAll[2*j-2]+global_parameters.parAll[2*j-1]*(global_parameters.fMass[i]-global_mH));
			hBFit[j]->SetPoint(i,global_parameters.fMass[i],global_parameters.parAll[i+2*(global_nMaxBdtBins-1)]*(global_parameters.parAll[2*j-2]+global_parameters.parAll[2*j-1]*(global_parameters.fMass[i]-global_mH)));


			if(i==0) {
				p00-=global_parameters.parAll[2*j-2];
				p01-=global_parameters.parAll[2*j-1];
			}
		}

		hMFit[i]->SetBinContent(  1,global_parameters.parAll[i+2*(global_nMaxBdtBins-1)]*(p00+p01*(global_parameters.fMass[i]-global_mH)));
		fMFit[i]->SetBinContent(  1,              p00+p01*(global_parameters.fMass[i]-global_mH) );
		hBFit[0]->SetPoint(i,global_parameters.fMass[i],global_parameters.parAll[i+2*(global_nMaxBdtBins-1)]*(p00+p01*(global_parameters.fMass[i]-global_mH)));
		fBFit[0]->SetPoint(i,global_parameters.fMass[i],p00+p01*(global_parameters.fMass[i]-global_mH) );
	}

	double pS00(1.0);
	// We are interested in the BDT bins in the signal region, so need those histograms ie setting mH=m0
	for(int j(1);j<global_nBdtBins;j++) {
		float binSFrac = global_parameters.parAll[2*(j-1)];
		fMFitS->SetBinContent(j+1,binSFrac);
		fMFitS->SetBinError(j+1,fBErr->GetBinContent(j+1));
		hMFitS->SetBinContent(j+1,binSFrac*global_nSignalRegion);
		hMFitS->SetBinError(j+1,global_nSignalRegion*fBErr->GetBinContent(j+1));
		pS00-=binSFrac;
	}

	fMFitS->SetBinContent(1,pS00);
	fMFitS->SetBinError(1,fBErr->GetBinContent(1));
	hMFitS->SetBinContent(1,pS00*global_nSignalRegion);
	hMFitS->SetBinError(1,global_nSignalRegion*fBErr->GetBinContent(1));

	fBPar->SetBinContent(1,p01/p00);
	fBPar->SetBinError(1,0.0);
	for(int j(1);j<global_nBdtBins;j++) {
		fBPar->SetBinContent(j+1,global_parameters.parAll[2*j-1]/global_parameters.parAll[2*j-2]);
		fBPar->SetBinError(j+1,(global_parameters.parAll[2*j-1]/global_parameters.parAll[2*j-2])*sqrt((global_parameters.errAll[2*j-1]*global_parameters.errAll[2*j-1]/(global_parameters.parAll[2*j-1]*global_parameters.parAll[2*j-1]))+(global_parameters.errAll[2*j-2]*global_parameters.errAll[2*j-2]/(global_parameters.parAll[2*j-2]*global_parameters.parAll[2*j-2]))));
	}

	// Write out the histos
	mDir->cd();

	for(int i(0);i<global_nMassBins;i++) {
		hMRaw[i]->Write();
		hMFit[i]->Write();
		fMRaw[i]->Write();
		fMFit[i]->Write();
	}	

	for(int j(0);j<global_nBdtBins;j++) {
		hBRaw[j]->Write();
		hBFit[j]->Write();
		fBRaw[j]->Write();
		fBFit[j]->Write();
	}
	// Make a plot of the points Fitted and the fitted graph
	gROOT->SetBatch(true);
	gROOT->SetStyle("Plain");
	for(int j(0);j<global_nBdtBins;j++) {
		TCanvas *can = new TCanvas(Form("Fit_BDT%d",j),"c1",305,95,800,550);
		//can->SetName(Form("Fit_BDT%d",j));
		can->SetBottomMargin(0.12);
		can->SetGrid(); 
		fBFit[j]->SetLineColor(4);
		fBFit[j]->SetLineWidth(3);
		fBRaw[j]->SetMarkerStyle(20);
		fBRaw[j]->SetMarkerSize(1.0);
		fBFit[j]->GetXaxis()->SetTitle("m_{H}^{sideband} (GeV)");
		fBFit[j]->GetXaxis()->SetTitleSize(0.055);
		fBFit[j]->GetXaxis()->SetLabelSize(0.045);
		fBFit[j]->GetYaxis()->SetTitle(Form("Fraction in Bin %d",j+1));
		fBFit[j]->GetYaxis()->SetTitleSize(0.05);
		fBFit[j]->GetYaxis()->SetLabelSize(0.045);
		fBFit[j]->GetYaxis()->SetNdivisions(508); 
		//fBRaw[j]->SetTitle(Form("Fit BDT Mass %3.1f Bin %d",global_mH,j));
		double FVal = fBFit[j]->Eval(global_mH);
		//fBRaw[j]->GetYaxis()->SetRangeUser(floor(FVal*0.75*100)/100,floor(FVal*1.25*100)/100);

		//TLine l(global_mH,floor(FVal*0.75*100)/100,global_mH,floor(FVal*1.25*100)/100);
		//fBRaw[j]->GetYaxis()->SetRangeUser(FVal*0.1,FVal*1.9);
		//fBFit[j]->GetYaxis()->SetRangeUser(FVal*0.1,FVal*1.9);
		//fBRaw[j]->GetYaxis()->SetRangeUser(FVal*0.1,FVal*1.9);
		fBFit[j]->GetYaxis()->SetRangeUser(FVal*0.05,FVal*1.95);
		TLine l(global_mH,FVal*0.1,global_mH,FVal*1.9);
		l.SetLineColor(46);
		l.SetLineStyle(7);
		l.SetLineWidth(3);
		fBFit[j]->SetTitle("");
		fBFit[j]->Draw("AL");
		if (!global_BLIND) fBRaw[j]->Draw("sameP");
		else {
			int tgcount = 0;
			TGraphErrors *blind = new TGraphErrors();
			blind->SetMarkerStyle(20);
			blind->SetMarkerSize(1.0);
			for (int pp = 0 ; pp< fBRaw[j]->GetN() ; pp++) {
				double mht,vt;
				fBRaw[j]->GetPoint(pp,mht,vt);
				if (mht*(1+global_SIDEBANDWIDTH) < 110 || mht*(1-global_SIDEBANDWIDTH) > 150){
					blind->SetPoint(tgcount,mht,vt);
					blind->SetPointError(tgcount,mht*(global_SIDEBANDWIDTH),fBRaw[j]->GetErrorY(pp));
					tgcount++;
				}
			}
			blind->Draw("sameP");
		}
		l.Draw();

		TText *text = new TText(0.6,0.8,"CMS Preliminary");
		text->SetNDC();
		text->Draw();
		text->SetTextSize(0.05); 
    TText *btext = new TText(0.6,0.7,Form("mH = %.1f GeV",global_mH));
    btext->SetNDC();
    btext->Draw();
    btext->SetTextSize(0.05);

		can->Write();
		if (makePlots){
			can->Print(Form("plots/png/fit_m%3.1f_bin%d.png",global_mH,j));
			can->Print(Form("plots/pdf/fit_m%3.1f_bin%d.pdf",global_mH,j));
			can->Print(Form("plots/macro/fit_m%3.1f_bin%d.C",global_mH,j));
		}
	}

	fMFitS->Write();
	hMFitS->Write();

	hFCorr->Write();
	hFCovar->Write();
	fBPar->Write();
	fBErr->Write();

	if (makePlots){
		TCanvas *canv = new TCanvas();
		gStyle->SetOptStat(0);
		gPad->SetRightMargin(5.);
		hFCorr->SetMarkerColor(kGray+2);
		hFCorr->Draw("colz text");
		canv->Print(Form("plots/png/fCorr_m%3.1f.png",global_mH));
		canv->Print(Form("plots/pdf/fCorr_m%3.1f.pdf",global_mH));
		canv->Print(Form("plots/macro/fCorr_m%3.1f.C",global_mH));
		hFCovar->SetMarkerColor(kGray+2);
		hFCovar->Draw("colz text");
		canv->Print(Form("plots/png/fCovar_m%3.1f.png",global_mH));
		canv->Print(Form("plots/pdf/fCovar_m%3.1f.pdf",global_mH));
		canv->Print(Form("plots/macro/fCovar_m%3.1f.C",global_mH));
	}

}
void diagonalizeMatrix(TH2F *th2f_covar,TH2F *th2f_out){

	int nb = global_nBdtBins;	
	TMatrixD covariance(nb,nb);

	for (int i=0;i<nb;i++){
		for (int j=0;j<nb;j++){
			covariance[i][j]=th2f_covar->GetBinContent(i+1,j+1);
		}
	}

	TVectorD eval;
	TMatrixD evec = covariance.EigenVectors(eval);
	//	double errB[nb][nb];

	for (int i=0; i<nb;i++){
		double err = TMath::Sqrt(eval[i]);
		for (int j=0;j<nb;j++){
			//                    errB[i][j]=evec[j][i]*err;
			th2f_out->SetBinContent(i+1,j+1,evec[j][i]*err);
		}
	}

}

void createCorrectedBackgroundModel(std::string fileName, int nsidebands=6, double sidebandWidth_=0.02, double signalRegionWidth_=0.02, int numberOfSidebandGaps_=1, double massSidebandMin_=99., double massSidebandMax_=180., double mHLow=110, double mHHigh=150, double mHStep=0.5, bool makePlots=false, bool blind=false, std::string defaultPrepend=".root" ){

  global_BLIND=blind;
	global_SIDEBANDWIDTH=sidebandWidth_;
	global_SIGNALREGION=signalRegionWidth_;
	global_NUMBEROFSIDEBANDGAPS=numberOfSidebandGaps_;
	global_MASSSIDEBANDMIN=massSidebandMin_;
	global_MASSSIDEBANDMAX=massSidebandMax_;

	if (makePlots){
		gStyle->SetPalette(1);
		gStyle->SetOptStat(0);
	}

	global_nMassBins=nsidebands;

	double massMin = mHLow;
	double massMax = mHHigh;
	double dM  = mHStep;

	std::cout<< "STARTING HERE" <<std::endl;
	// Open the original Workspace
	std::cout << "Filename -- " << fileName.c_str() <<std::endl;
	TFile *in = TFile::Open(fileName.c_str(),"UPDATE");
	RooWorkspace *work = (RooWorkspace*)in->Get("cms_hgg_workspace");

	std::string type = "grad";


	// Create An output file for the TF1 Sideband Fits
	//std::string pathToFile=fileName.substr(0,fileName.find(defaultPrepend.c_str()));
	std::string fName=fileName.substr(0,fileName.find(defaultPrepend.c_str()));
	TFile *out = new TFile(Form("%sbdtSidebandFits_%s.root",fName.c_str(),type.c_str()),"RECREATE");
	std::cout<< "STARTING LOOP OVER MH" <<std::endl;
	for (double mH=massMin;mH<=massMax;mH+=dM){

		//TH1F *originalHist      = (TH1F*) in->Get(Form("th1f_bkg_%s_%3.1f",type.c_str(),mH)); // This histogram is normalized to the inclusive fit (will not include VBF cat)
		RooRealVar *nSignalVar = (RooRealVar*)work->var(Form("NBkgInSignal_mH%3.1f",mH));
		std::cout<< "Got NBKG" <<std::endl;
		TH1F *dataHist  = (TH1F*) in->Get(Form("th1f_data_%s_%3.1f",type.c_str(),mH)); // Data Histogram, includes VBF category
		std::cout<< "Got data" <<std::endl;
		int nBins = dataHist->GetNbinsX();
		std::cout<< "Get Norm and datahist" <<std::endl;

		// Want to make a "corrected" histogram 
		TH1F *correctedHist = new TH1F(Form("th1f_bkg_%s_%3.1f_fitsb_biascorr",type.c_str(),mH),Form("th1f_bkg_%s_%3.1f_fitsb_biascorr",type.c_str(),mH),nBins,0,nBins);		     
		TH1F *correctedHistFR = new TH1F(Form("th1f_bkg_%s_%3.1f_fitsb_biascorr_frac",type.c_str(),mH),Form("th1f_bkg_%s_%3.1f_fitsb_biascorr_frac",type.c_str(),mH),nBins,0,nBins);		     
		TH2F *hFCovar=new TH2F(Form("fCovar_%3.1f",mH),
				Form("Fraction covariance matrix m %3.1f",mH),
				nBins,0.0,nBins,nBins,0.0,nBins);
		TH2F *uCorrErr = new TH2F(Form("fUncorrErr_%s_%3.1f",type.c_str(),mH),
				Form("Uncorrelated Errors m %3.1f",mH),
				nBins,0.0,nBins,nBins,0.0,nBins);

		TDirectory *mass_dir = out->mkdir(Form("mH_%3.1f",mH));

		// For a given mass point, need to set up globals
		global_nBdtBins=nBins;		
		global_mH=mH;
		global_nSignalRegion=nSignalVar->getVal();

		std::cout<< "Getting Sidebands" <<std::endl;
    //cout << Form("%50s  %5d  %5d  %5.2f",dataHist->GetName(),dataHist->GetEntries(),dataHist->GetNbinsX(),dataHist->Integral()) << endl;
		cout << "Name: " << dataHist->GetName() << " Entries: " << dataHist->GetEntries() << " Nbins: " << dataHist->GetNbinsX() << " Integral: " << dataHist->Integral() << endl;
		fillData(mH,in,type);
		std::cout<< "Run Paul Fit" <<std::endl;
		paulFit(mass_dir,correctedHistFR,correctedHist,hFCovar,makePlots,type);

		// Finally Get the uncorrleated errors from the covariance matrix
		diagonalizeMatrix(hFCovar,uCorrErr);

		std::cout << "Final Check of Normalizations at mH="<<mH <<std::endl;
		std::cout << "Original - "<< global_nSignalRegion <<  ", After - " <<correctedHist->Integral() <<std::endl;

		// Write out the Hists into the original File
		in->cd();
    in->Cd(Form("%s:/",fileName.c_str()));
    cout << "----!---- Write corrected hists into original file......" << endl;
		// Double check and make sure that the normalization is still taken from the fit
		correctedHist->Scale(nSignalVar->getVal()/correctedHist->Integral());
		correctedHist->Write(correctedHist->GetName(),TObject::kOverwrite);
		correctedHistFR->Write(correctedHistFR->GetName(),TObject::kOverwrite);
		//hFCovar->Write();
		uCorrErr->Write(uCorrErr->GetName(),TObject::kOverwrite);
		out->cd();
		mass_dir->cd();
		uCorrErr->Write(uCorrErr->GetName(),TObject::kOverwrite);

		if (makePlots){
			TCanvas *canv = new TCanvas();
			uCorrErr->SetMarkerColor(kGray);
			gPad->SetRightMargin(2.);
			uCorrErr->Draw("colz text");
			canv->Print(Form("plots/png/uncorrErr_m%3.1f.png",mH));
			canv->Print(Form("plots/pdf/uncorrErr_m%3.1f.pdf",mH));
			canv->Print(Form("plots/macro/uncorrErr_m%3.1f.C",mH));
		}
	}
	std::cout << "Saving Fits to file -> " << out->GetName() << std::endl;
	out->Close();

	std::cout << "Updated (with corrected background model) -> " << in->GetName() << std::endl;
	in->Close();
}

